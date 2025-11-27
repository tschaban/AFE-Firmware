# AFE Firmware Build Script - PowerShell Version
# Main script to organize and copy firmware builds

param(
    [string]$type = "1",
    [string]$version = "3.5.0",
    [string]$language = "pl",
    [bool]$development = $true
)

# Load the T1 configuration
. "$PSScriptRoot\T1.ps1"

# Helper function to create index.html file
function New-IndexFile {
    param([string]$path)
    New-Item -Path "$path\index.html" -ItemType File -Force | Out-Null
}

# Helper function to copy folder recursively
function Copy-FolderRecursive {
    param(
        [string]$source,
        [string]$destination
    )
    
    if (-not (Test-Path $destination)) {
        New-Item -Path $destination -ItemType Directory -Force | Out-Null
    }
    
    Get-ChildItem -Path $source -Recurse | ForEach-Object {
        $destPath = $_.FullName.Replace($source, $destination)
        if ($_.PSIsContainer) {
            if (-not (Test-Path $destPath)) {
                New-Item -Path $destPath -ItemType Directory -Force | Out-Null
            }
        } else {
            Copy-Item -Path $_.FullName -Destination $destPath -Force
        }
    }
}

Write-Host "`nCreating folders structure"

# Create main target folder
if (-not (Test-Path $targetFolder)) {
    New-Item -Path $targetFolder -ItemType Directory -Force | Out-Null
    New-IndexFile -path $targetFolder
    
    Write-Host "Copying bootloaders"
    $bootloaderSource = Join-Path $PSScriptRoot "esp32.bootloaders"
    $bootloaderDest = Join-Path $targetFolder "esp32.bootloaders"
    
    if (Test-Path $bootloaderSource) {
        Copy-FolderRecursive -source $bootloaderSource -destination $bootloaderDest
    }
}

# Create folder structure for languages, APIs, and hardware
$finalFolder = @{}

foreach ($lang in $targetLanguage) {
    $langPath = Join-Path $targetFolder $lang
    if (-not (Test-Path $langPath)) {
        New-Item -Path $langPath -ItemType Directory -Force | Out-Null
        New-IndexFile -path $langPath
    }
    
    $finalFolder[$lang] = @{}
    
    foreach ($api in $targetAPI) {
        $apiPath = Join-Path $langPath $api
        if (-not (Test-Path $apiPath)) {
            New-Item -Path $apiPath -ItemType Directory -Force | Out-Null
            New-IndexFile -path $apiPath
        }
        
        $finalFolder[$lang][$api] = @{}
        
        foreach ($hw in $targetHardware) {
            $hwFolder = $hw[0]
            $hwPath = Join-Path $apiPath $hwFolder
            if (-not (Test-Path $hwPath)) {
                New-Item -Path $hwPath -ItemType Directory -Force | Out-Null
                New-IndexFile -path $hwPath
            }
            $finalFolder[$lang][$api][$hwFolder] = $hwPath
        }
    }
}

# Create SQL file
$sqlFile = Join-Path $targetFolder "script-$language.sql"

# Does not generate update for development version
if (-not $development) {
    "UPDATE afe_firmwares set current_version = 0 WHERE type = $type AND language = '$language';" | Out-File -FilePath $sqlFile -Encoding UTF8
} else {
    if (Test-Path $sqlFile) {
        Remove-Item $sqlFile -Force
    }
}

Write-Host "Copying firmwares"

foreach ($source in $sourceFolder) {
    # Normalize path to Windows format
    $sourceToCopy = $source.file -replace '/', '\\'
    
    # Build filename
    $hwPart = if ($targetHardware[$source.hardware][2]) { 
        "$($targetHardware[$source.hardware][2])." 
    } else { 
        "" 
    }
    
    $debugPart = if ($source.debug) { ".debug" } else { "" }
    
    $fileName = "afe.firmware.t$type.$version.$language.esp$($source.chip).$hwPart$($source.size)mb$debugPart.bin"
    
    $hwFolder = $targetHardware[$source.hardware][0]
    $copyTo = Join-Path $finalFolder[$language][$source.api][$hwFolder] $fileName
    
    if (Test-Path $sourceToCopy) {
        Copy-Item -Path $sourceToCopy -Destination $copyTo -Force
        Write-Host "SUCCESS: $fileName"
        
        # Generate SQL insert
        $apiCode = switch ($source.api) {
            $targetAPI[0] { "D" }  # Domoticz
            $targetAPI[1] { "S" }  # Standard
            $targetAPI[2] { "H" }  # Home Assistant
        }
        
        $currentVersion = if ($development) { 2 } else { 1 }
        $debugFlag = if ($source.debug) { 1 } else { 0 }
        # Normalize paths and create relative path with forward slashes for SQL
        $normalizedCopyTo = $copyTo -replace '\\', '/'
        $normalizedRootPath = $rootPath -replace '\\', '/'
        $relativePath = $normalizedCopyTo.Replace($normalizedRootPath, '')
        
        $sqlInsert = "INSERT INTO afe_firmwares (type,version,chip,language,api,hardware,flash_size,current_version,downloaded,debug,path) VALUES ($type, '$version', $($source.chip), '$language', '$apiCode', $($targetHardware[$source.hardware][1]), $($source.size), $currentVersion, 0, $debugFlag, '$relativePath');"
        
        $sqlInsert | Out-File -FilePath $sqlFile -Encoding UTF8 -Append
    } else {
        Write-Host "ERROR: File doesn't exist: $sourceToCopy" -ForegroundColor Red
    }
}

Write-Host "`nDone!" -ForegroundColor Green

#ifndef _AFE_HTTP_COMMAND_Structure_h
#define _AFE_HTTP_COMMAND_Structure_h

struct HTTPCOMMAND {
  char device[32];
  char name[33];
  char command[32];
  char source[9];
};

#endif

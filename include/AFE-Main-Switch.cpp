/* AFE Firmware for smarthome devices, More info: https://afe.smartnydom.pl/ */

#ifdef AFE_CONFIG_HARDWARE_SWITCH

/* ---------Headers ---------*/

void processSwitchEvents(void);
void switchEventsListener(void);

/* --------- Body -----------*/

/**
 * @brief Method processes Switch related events
 *
 */
void processSwitchEvents(void)
{

  if (Firmware->Device->getMode() == AFE_MODE_NORMAL)
  {
    for (uint8_t i = 0; i < Firmware->Device->configuration.noOfSwitches; i++)
    {
      /**
       * @brief One of the switches has been shortly pressed and there is
       * assigned functionaity to it
       *
       */

      if (Hardware->Switch[i]->isPressed() &&
          Hardware->Switch[i]->configuration->functionality !=
              AFE_SWITCH_FUNCTIONALITY_NONE)
      {

#ifdef AFE_CONFIG_HARDWARE_RELAY
        if (Hardware->Switch[i]->configuration->relayID != AFE_HARDWARE_ITEM_NOT_EXIST &&
            Hardware->Switch[i]->configuration->relayID + 1 <=
                Firmware->Device->configuration.noOfRelays)
        {

#ifdef DEBUG
          Firmware->Debugger->printInformation(F("Pressed with assigned Relay: "), F("SWITCH"));
          Firmware->Debugger->printValue(Hardware->Switch[i]->configuration->relayID);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
          /**
           * @brief The code here is only appilcable for a Switch that controlls
           * a
           * Gate
           *
           */
          if (Hardware->Relay[Hardware->Switch[i]->configuration->relayID]->gateId !=
              AFE_HARDWARE_ITEM_NOT_EXIST)
          {
            if (Hardware->Relay[Hardware->Switch[i]->configuration->relayID]->gateId <=
                Firmware->Device->configuration.noOfGates)
            {

#ifdef DEBUG
              Firmware->Debugger->printInformation(F("is assigned to a gate: "), F("RELAY"));
              Firmware->Debugger->printValue(Relay[Hardware->Switch[i]->configuration->relayID]->gateId);
#endif
            }
          }
          else
          {
#endif

            Hardware->Relay[Hardware->Switch[i]->configuration->relayID]->toggle();
            MqttAPI->publishRelayState(Hardware->Switch[i]->configuration->relayID);

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
            HttpDomoticzAPI->publishRelayState(
                Hardware->Switch[i]->configuration->relayID);
#endif

#ifdef AFE_CONFIG_HARDWARE_GATE
            /**
             * @brief The code here is only appilcable for a Switch that
             * controlls
             * a Gate
             *
             */
          }
#endif
        }
#endif // AFE_CONFIG_HARDWARE_RELAY

#ifdef AFE_CONFIG_HARDWARE_CLED
        /**
         * @brief Processing switch short press assigned to RGB LED
         *
         */
        if (Hardware->Switch[i]->configuration->rgbLedID != AFE_HARDWARE_ITEM_NOT_EXIST &&
            Hardware->Switch[i]->configuration->rgbLedID + 1 <=
                Firmware->Device->configuration.noOfCLEDs)
        {

#ifdef DEBUG
          Firmware->Debugger->printInformation(F(" pressed with assigned RGB LED: "), F("SWITCH"));
          Firmware->Debugger->printValue(Hardware->Switch[i]->configuration->rgbLedID);

#endif

          Hardware->RGBLEDStrip->toggle(Hardware->Switch[i]->configuration->rgbLedID, true);
          MqttAPI->publishCLEDState(Hardware->Switch[i]->configuration->rgbLedID);

#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
/**
 * @brief HTTP API for Domoticz is nos supported in T7 version for RGB LED
 *

HttpDomoticzAPI->publishCLEDState(Hardware->Switch[i]->configuration->rgbLedID);
   */
#endif
        }

#endif // AFE_CONFIG_HARDWARE_CLED

      } // Switch is pressed

      if (Hardware->Switch[i]->isPressed(true))
      {
        MqttAPI->publishSwitchState(i);
#if AFE_FIRMWARE_API == AFE_FIRMWARE_API_DOMOTICZ
        HttpDomoticzAPI->publishSwitchState(i);
#endif
      }
    }
  }
}

void switchEventsListener(void)
{

  /**
   * @brief Listens for switch events
   *
   */

  for (uint8_t i = 0; i < Firmware->Device->configuration.noOfSwitches; i++)
  {
    Hardware->Switch[i]->listener();
    /**
     * @brief One of the Multifunction switches pressed for 10 seconds
     *
     */
    if (Hardware->Switch[i]->configuration->functionality ==
        AFE_SWITCH_FUNCTIONALITY_MULTI)
    {
      if (Hardware->Switch[i]->is10s())
      {
        Firmware->Device->getMode() == AFE_MODE_NORMAL
            ? Firmware->Device->reboot(AFE_MODE_ACCESS_POINT)
            : Firmware->Device->reboot(AFE_MODE_NORMAL);
      }
      else if (Hardware->Switch[i]->is5s())
      {
        Firmware->Device->getMode() == AFE_MODE_NORMAL
            ? Firmware->Device->reboot(AFE_MODE_CONFIGURATION)
            : Firmware->Device->reboot(AFE_MODE_NORMAL);
      }
      else if (Hardware->Switch[i]->is30s())
      {
        Firmware->API->Flash->setDefaultConfiguration();
        Firmware->Device->reboot(AFE_MODE_ACCESS_POINT);
      }
    }
  }
}

#endif // AFE_CONFIG_HARDWARE_SWITCH
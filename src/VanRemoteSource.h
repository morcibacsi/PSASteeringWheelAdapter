#pragma once

#ifndef _VanRemoteSource_h
    #define _VanRemoteSource_h

#include "AbstractRemoteSource.h"
#include <esp32_arduino_rmt_van_rx.h>

class VanRemoteSource : public AbstractRemoteSource
{
    ESP32_RMT_VAN_RX* VAN_RX;

    uint8_t vanMessageLength;
    uint8_t vanMessage[34];

  public:
      VanRemoteSource(uint8_t rmtChannel, uint8_t rxPin, uint8_t ledPin) {
          VAN_RX = new ESP32_RMT_VAN_RX();
          VAN_RX->Init(rmtChannel, rxPin, ledPin, VAN_LINE_LEVEL_HIGH, VAN_NETWORK_TYPE_COMFORT);
      }

      bool GetRemoteStatus(uint8_t* buttonData, uint8_t* scrollData) override
      {
          VAN_RX->Receive(&vanMessageLength, vanMessage);

          if (VAN_RX->IsCrcOk(vanMessage, vanMessageLength))
          {
              if (vanMessage[1] == 0x9C && vanMessage[2] == 0x4C)
              {
                  *buttonData = vanMessage[3];
                  *scrollData = vanMessage[4];
              }
              return true;
          }

          return false;
      }
};

#endif

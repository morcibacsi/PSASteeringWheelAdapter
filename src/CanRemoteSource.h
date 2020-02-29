#pragma once

#ifndef _CanRemoteSource_h
    #define _CanRemoteSource_h

#include "AbstractRemoteSource.h"
#include "AbstractCanMessageSender.h"

class CanRemoteSource : public AbstractRemoteSource
{
    const uint16_t CAN_ID_RADIO_REMOTE = 0x21F;

    AbstractCanMessageSender* _canInterface;

    uint16_t canId;
    uint8_t canMessageLength;
    uint8_t canMessage[8];

  public:
      CanRemoteSource(AbstractCanMessageSender *canInterface) {
          _canInterface = canInterface;
      }

      bool GetRemoteStatus(uint8_t* buttonData, uint8_t* scrollData) override 
      {
          _canInterface->ReadMessage(&canId, &canMessageLength, canMessage);

          if (canId == CAN_ID_RADIO_REMOTE)
          {
              *buttonData = canMessage[0];
              *scrollData = canMessage[1];
              return true;
          }

          return false;
      }
};

#endif

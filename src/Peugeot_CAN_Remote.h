// Peugeot_CAN_Remote.h

/*
 * This class converts the VAN bus remote control command to Peugeot CAN bus remote control command. Actually there is no much processing needed as
 * Peugeot used the same message structure on the CAN bus as they used on the VAN bus. So basically we just need to forward the bytes on the CAN bus.
 * This implementation is good for the one who bought a headunit which is only compatible with Peugeot CAN bus not with VAN bus. So if one builds
 * a little PCB with an ESP32 and with two CAN transceiver like this: https://github.com/morcibacsi/PSAVanCanBridgeHW/tree/v1.1
 * Then it can be connected to the headunits CAN port and by setting up a Peugeot 207 or 307 or 407 in the headunit the remote will work.
 */
#pragma once

#ifndef _Peugeot_CAN_Remote_h
    #define _Peugeot_CAN_Remote_h

#include "AbstractRemote.h"
#include "AbstractCanMessageSender.h"

class Peugeot_CAN_Remote : public AbstractRemote  {
    const uint16_t CAN_ID_RADIO_REMOTE = 0x21F;

    AbstractCanMessageSender* _canInterface;

    public:
     /// <summary> Initialize a remote controller which "translates" the VAN bus remote codes to Peugeot CAN bus remote codes</summary>
     /// <param name="canInterface"> A CAN bus sender implementation configured to 125kbs </param>
    Peugeot_CAN_Remote(AbstractCanMessageSender* canInterface)
    {
        _canInterface = canInterface;
    }

    void VolumeUp() override
    {
    }

    void VolumeDown() override
    {
    }

    void SeekUp() override
    {
    }

    void SeekDown() override
    {
    }

    void Source() override
    {
    }

    void Mute() override
    {
    }

    void WheelPosition(uint8_t wheelUpBit, uint8_t wheelDownBit, uint8_t currentPosition) override
    {
    }

    void ProcessRawData(uint8_t buttonData, uint8_t scrollData) override
    {
        uint8_t data[] = { buttonData, scrollData, 0x00 };

        _canInterface->SendMessage(CAN_ID_RADIO_REMOTE, 0, 3, data);
    }
 };

#endif


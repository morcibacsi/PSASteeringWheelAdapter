// Alpine_CNY173_Remote.h
#pragma once

#ifndef _Alpine_CNY173_Remote_h
    #define _Alpine_CNY173_Remote_h

#include "AbstractRemote.h"

class Alpine_CNY173_Remote : public AbstractRemote  {
    //const uint16_t PULSEWIDTH = 555;
    //const uint8_t  ADDRESS    = 0x47;

    const uint8_t  POWERONOFF = 0x09;
    const uint8_t  VOLUP      = 0x14;
    const uint8_t  VOLDOWN    = 0x15;
    const uint8_t  SOURCE     = 0x0A;
    //const uint8_t  EQUALIZER  = 0x0D;
    const uint8_t  MUTE       = 0x16;
    const uint8_t  NEXTTRACK  = 0x0E;
    const uint8_t  PREVTRACK  = 0x0F;
    //const uint8_t  FOLDERFORW = 0x14;
    //const uint8_t  FOLDERBACK = 0x15;

    uint8_t _outPin;
    uint8_t _prevWheelPosition;
    uint8_t _prevWheelUpBit;
    uint8_t _prevWheelDownBit;

    // Send a byte (NEC-Clarion protocol)
    void SendByte(byte data)
    {
        for (int i = 0; i < 8; i++)
        {
            digitalWrite(_outPin, HIGH);
            delayMicroseconds(560); // Pause between bits
            digitalWrite(_outPin, LOW);

            if (data & 1) // The last bit is multiplied by 1
            {
                delayMicroseconds(1680);  // logical 1
            }
            else
            {
                delayMicroseconds(560);  // logical 0
            }

            data >>= 1; //shifted data to the right (1 bit)
        }
    }

    // Send a command to the radio, including the header, start bit, address and stop bits
    void SendCommand(byte command)
    {
        // Something like a greeting, feature of the modified NEC protocol
        digitalWrite(_outPin, HIGH);
        delayMicroseconds(9000);
        digitalWrite(_outPin, LOW);
        delayMicroseconds(4500);

        // Protocol address (public reference info)
        SendByte(0x86);
        SendByte(0x72);

        // send command
        SendByte(command);
        SendByte(~command);

        // To determine the last bit (end of sending)
        digitalWrite(_outPin, HIGH);
        delayMicroseconds(560);
        digitalWrite(_outPin, LOW);

        delay(50);  // Technical pause
    }

    public:
    // 
     /// <summary> Initialize the CNY173 based Alpine remote controller </summary>
     /// <param name="outPin"> CNY173 optocoupler input should be connected through a 1k resistor to this pin </param>
     /// <param name="ledPin"> An output pin for a led indicator </param>
    Alpine_CNY173_Remote(uint8_t outPin)
    {
        _outPin = outPin;
    }

    void VolumeUp() override
    {
        SendCommand(VOLUP);
    }

    void VolumeDown() override
    {
        SendCommand(VOLDOWN);
    }

    void SeekUp() override
    {
        SendCommand(NEXTTRACK);
    }

    void SeekDown() override
    {
        SendCommand(PREVTRACK);
    }

    void Source() override
    {
        SendCommand(SOURCE);
    }

    void Mute() override
    {
        SendCommand(MUTE);
    }

    void WheelPosition(uint8_t wheelUpBit, uint8_t wheelDownBit, uint8_t currentPosition) override
    {
        if (_prevWheelPosition > currentPosition || _prevWheelDownBit != wheelDownBit)
        {
            _prevWheelPosition = currentPosition;
            _prevWheelDownBit = wheelDownBit;
            //SendCommand(FOLDERFORW);
        }
        if (_prevWheelPosition < currentPosition || _prevWheelUpBit != wheelUpBit)
        {
            _prevWheelPosition = currentPosition;
            _prevWheelUpBit = wheelUpBit;
            //SendCommand(FOLDERBACK);
        }
    }

    void ProcessRawData(uint8_t buttonData, uint8_t scrollData) override
    {
    }
 };

#endif


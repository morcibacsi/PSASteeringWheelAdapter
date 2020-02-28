// JVC_CNY173_Remote.h
#pragma once

#ifndef _JVC_CNY173_Remote_h
    #define _JVC_CNY173_Remote_h

#include "AbstractRemote.h"

class JVC_CNY173_Remote : public AbstractRemote  {
    //JVC KD-330BT

    const uint16_t PULSEWIDTH = 555;
    const uint8_t  ADDRESS    = 0x47;

    const uint8_t  POWERONOFF = 0x0A;
    const uint8_t  VOLUP      = 0x04;
    const uint8_t  VOLDOWN    = 0x05;
    const uint8_t  SOURCE     = 0x08;
    const uint8_t  EQUALIZER  = 0x0D;
    const uint8_t  MUTE       = 0x0E;
    const uint8_t  NEXTTRACK  = 0x12;
    const uint8_t  PREVTRACK  = 0x13;
    const uint8_t  FOLDERFORW = 0x14;
    const uint8_t  FOLDERBACK = 0x15;

    uint8_t _outPin;
    uint8_t _ledPin;
    uint8_t _prevWheelPosition;
    uint8_t _prevWheelUpBit;
    uint8_t _prevWheelDownBit;

    bool isPaused = false;

    // Send a value (7 bits, LSB is sent first, value can be an address or command)
    void SendValue(uint8_t value) {
        unsigned char i, tmp = 1;
        for (i = 0; i < sizeof(value) * 8 - 1; i++) {
            if (value & tmp)  // Do a bitwise AND on the value and tmp
                SendOne();
            else
                SendZero();
            tmp = tmp << 1; // Bitshift left by 1
        }
    }

    // Send a command to the radio, including the header, start bit, address and stop bits
    void SendCommand(uint8_t value) {
        unsigned char i;
        Preamble();
        for (i = 0; i < 1; i++) {             // Repeat address, command and stop bits three times so radio will pick them up properly
            SendValue(ADDRESS);               // Send the address
            SendValue((unsigned char)value);  // Send the command
            Postamble();                      // Send signals to follow a command to the radio
        }
    }

    // Signals to transmit a '0' bit
    void SendZero() {
        digitalWrite(_outPin, HIGH);         // Output HIGH for 1 pulse width
        digitalWrite(_ledPin, HIGH);         // Turn on on-board LED
        delayMicroseconds(PULSEWIDTH);
        digitalWrite(_outPin, LOW);          // Output LOW for 1 pulse width
        digitalWrite(_ledPin, LOW);          // Turn off on-board LED
        delayMicroseconds(PULSEWIDTH);
    }

    // Signals to transmit a '1' bit
    void SendOne() {
        digitalWrite(_outPin, HIGH);         // Output HIGH for 1 pulse width
        digitalWrite(_ledPin, HIGH);         // Turn on on-board LED
        delayMicroseconds(PULSEWIDTH);
        digitalWrite(_outPin, LOW);          // Output LOW for 3 pulse widths
        digitalWrite(_ledPin, LOW);          // Turn off on-board LED
        delayMicroseconds(PULSEWIDTH * 3);
    }

    // Signals to precede a command to the radio
    void Preamble() {
        // HEADER: always LOW (1 pulse width), HIGH (16 pulse widths), LOW (8 pulse widths)
        digitalWrite(_outPin, LOW);          // Make sure output is LOW for 1 pulse width, so the header starts with a rising edge
        digitalWrite(_ledPin, LOW);          // Turn off on-board LED
        delayMicroseconds(PULSEWIDTH * 1);
        digitalWrite(_outPin, HIGH);         // Start of header, output HIGH for 16 pulse widths
        digitalWrite(_ledPin, HIGH);         // Turn on on-board LED
        delayMicroseconds(PULSEWIDTH * 16);
        digitalWrite(_outPin, LOW);          // Second part of header, output LOW 8 pulse widths
        digitalWrite(_ledPin, LOW);          // Turn off on-board LED
        delayMicroseconds(PULSEWIDTH * 8);

        // START BIT: always 1
        SendOne();
    }

    // Signals to follow a command to the radio
    void Postamble() {
        // STOP BITS: always 1
        SendOne();
        SendOne();
    }

    public:
    // 
     /// <summary> Initialize the CNY173 based JVC remote controller </summary>
     /// <param name="outPin"> CNY173 optocoupler input should be connected through a 1k resistor to this pin </param>
     /// <param name="ledPin"> An output pin for a led indicator </param>
    JVC_CNY173_Remote(uint8_t outPin, uint8_t ledPin)
    {
        _outPin = outPin;
        _ledPin = ledPin;
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
            SendCommand(FOLDERFORW);
        }
        if (_prevWheelPosition < currentPosition || _prevWheelUpBit != wheelUpBit)
        {
            _prevWheelPosition = currentPosition;
            _prevWheelUpBit = wheelUpBit;
            SendCommand(FOLDERBACK);
        }
    }

    void ProcessRawData(uint8_t buttonData, uint8_t scrollData) override
    {
    }
 };

#endif


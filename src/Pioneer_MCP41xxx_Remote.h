// JVC_CNY173_Remote.h
#pragma once

#ifndef _Pioneer_MCP4100_Remote_h
    #define _Pioneer_MCP4100_Remote_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include <SPI.h>
#else
    #include "WProgram.h"
#endif

#include "AbstractRemote.h"

// Datasheet: http://ww1.microchip.com/downloads/en/devicedoc/11195c.pdf

// Overall resistance of MCP41xxx
enum RAB
{
    RAB_10 = 10,
    RAB_50 = 50,
    RAB_100 = 100
};

class Pioneer_MCP41xxx_Remote : public AbstractRemote  {
    #define SLAVE_SELECT()   digitalWrite(_ss_pin, LOW)
    #define SLAVE_UNSELECT() digitalWrite(_ss_pin, HIGH)
    #define _delay_ms(ms) delayMicroseconds((ms) * 1000)

    // Pioneer resistance values

    const float VOLUP      = 16.00;
    const float VOLDOWN    = 24.00;
    const float SOURCE     = 1.20;
    const float MUTE       = 3.50;
    const float NEXTTRACK  = 8.00;
    const float PREVTRACK  = 11.25;

    const uint8_t NOCMD       = 0x00;
    const uint8_t CMD_WAIT_MS = 50;

    SPIClass* _spi;
    uint8_t _ss_pin;
    RAB _rab;
    uint8_t _rw;

    uint8_t _outPin;
    uint8_t _ledPin;
    uint8_t _prevWheelPosition;
    uint8_t _prevWheelUpBit;
    uint8_t _prevWheelDownBit;

    // See figure 4-9 on page 16 in datasheet for formula
    uint8_t GetValueForResistance(float kOhm)
    {
        return 256 - (((kOhm - _rw) * 256) / 100);
    }

    void SendCommand(float kOhm)
    {
        // it might be possible that you need to fiddle with the SPI settings

        uint8_t cmd = GetValueForResistance(kOhm);

        SLAVE_SELECT();
        _spi->beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE3));
        _spi->transfer(0x11);
        _spi->transfer(cmd);
        _spi->endTransaction();
        SLAVE_UNSELECT();

        _delay_ms(CMD_WAIT_MS);

        SLAVE_SELECT();
        _spi->beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE3));
        _spi->transfer(0x11);
        _spi->transfer(NOCMD);
        _spi->endTransaction();
        SLAVE_UNSELECT();
    }

    public:
     /// <summary> Initialize the MCP41xxx based Pioneer remote controller </summary>
     /// <param name="sck_pin"> SCK pin of the MCP41xxx </param>
     /// <param name="miso_pin"> MISO pin of the MCP41xxx </param>
     /// <param name="mosi_pin"> MOSI pin of the MCP41xxx </param>
     /// <param name="ss_pin"> CS or SS pin of the MCP41xxx </param>
     /// <param name="overall_resistance"> Overall resistance of the MCP41xxx (R_AB) </param>
     /// <param name="wiper_resistance"> Value of wiper resistance (R_W) </param>
    Pioneer_MCP41xxx_Remote(uint8_t sck_pin, uint8_t miso_pin, uint8_t mosi_pin, uint8_t ss_pin, RAB overall_resistance, uint8_t wiper_resistance)
    {
        _rw = wiper_resistance;
        _rab = overall_resistance;
        _ss_pin = ss_pin;
        _spi = new SPIClass();
        _spi->begin(sck_pin, miso_pin, mosi_pin, ss_pin);
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


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

class Pioneer_MCP4100_Remote : public AbstractRemote  {
    #define SLAVE_SELECT()   digitalWrite(_ss_pin, LOW)
    #define SLAVE_UNSELECT() digitalWrite(_ss_pin, HIGH)
    #define _delay_ms(ms) delayMicroseconds((ms) * 1000)

    //Pioneer

    const uint8_t  POWERONOFF = 0x0A;
    const uint8_t  VOLUP      = 0xd5;      // 16.00 k
    const uint8_t  VOLDOWN    = 0xbe;      // 24.00 k
    const uint8_t  SOURCE     = 0xfd;      //  1.20 k
    //const uint8_t  EQUALIZER  = 0x0D;
    const uint8_t  MUTE       = 0xf8;      //  3.50 k
    const uint8_t  NEXTTRACK  = 0xeb;      //  8.00 k
    const uint8_t  PREVTRACK  = 0xe2;      // 11.25 k
    //const uint8_t  FOLDERFORW = 0x14;
    //const uint8_t  FOLDERBACK = 0x15;
    const uint8_t NOCMD = 0x00;      // max
    const uint8_t CMD_WAIT_MS = 50;

    SPIClass* _spi;
    uint8_t _ss_pin;


    uint8_t _outPin;
    uint8_t _ledPin;
    uint8_t _prevWheelPosition;
    uint8_t _prevWheelUpBit;
    uint8_t _prevWheelDownBit;

    bool isPaused = false;

    void SendCommand(uint8_t cmd)
    {
        // it might be possible that you need to fiddle with the SPI settings

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
    // 
     /// <summary> Initialize the MCP4100 based Pioneer remote controller </summary>
     /// <param name="sck_pin"> SCK pin of the MCP4100 </param>
     /// <param name="miso_pin"> MISO pin of the MCP4100 </param>
     /// <param name="mosi_pin"> MOSI pin of the MCP4100 </param>
     /// <param name="ss_pin"> CS or SS pin of the MCP4100 </param>
    Pioneer_MCP4100_Remote(uint8_t sck_pin, uint8_t miso_pin, uint8_t mosi_pin, uint8_t ss_pin)
    {
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
 };

#endif


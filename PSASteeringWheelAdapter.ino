#include <esp32_arduino_rmt_van_rx.h>

#include "src/AbstractRemote.h"
#include "src/RemoteStalkStruct.h"

#include "src/JVC_CNY173_Remote.h"
#include "src/Pioneer_MCP41xxx_Remote.h"
#include "src/Alpine_CNY173_Remote.h"
#include "src/Peugeot_CAN_Remote.h"

#include "src/CanMessageSenderEsp32Arduino.h"

#include "src/AbstractRemoteSource.h"
#include "src/VanRemoteSource.h"
#include "src/CanRemoteSource.h"

const uint8_t VAN_DATA_RX_RMT_CHANNEL = 0;
const uint8_t VAN_DATA_RX_PIN = 21;
const uint8_t VAN_DATA_RX_LED_INDICATOR_PIN = 2;

const uint8_t CAN_RX_PIN = 33;
const uint8_t CAN_TX_PIN = 32;

const uint8_t LED_PIN = 2; 
const uint8_t OUT_PIN = 19;

uint32_t currentTime = 0;
uint32_t prevTime = 0;

AbstractRemote *remoteControl;
RemoteButton remoteButton;
AbstractCanMessageSender* canInterface;
AbstractRemoteSource* remoteSource;

void InitPioneer()
{
    const uint8_t SCK_PIN = 25;
    const uint8_t MISO_PIN = 5;
    const uint8_t MOSI_PIN = 33;
    const uint8_t SS_PIN = 32;

    remoteControl = new Pioneer_MCP41xxx_Remote(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN, RAB_100, 125);
}

void InitAlpine()
{
    remoteControl = new Alpine_CNY173_Remote(OUT_PIN);
}

void InitJVC()
{
    remoteControl = remoteControl = new JVC_CNY173_Remote(OUT_PIN, LED_PIN);
}

void InitPeugeotCAN()
{
    remoteControl = new Peugeot_CAN_Remote(canInterface);
}

void setup()
{
    Serial.begin(500000);
    Serial.println("ESP32 Arduino VAN bus steering wheel adapter");

    // Use this if you have a car with VAN bus
    remoteSource = new VanRemoteSource(VAN_DATA_RX_RMT_CHANNEL, VAN_DATA_RX_PIN, VAN_DATA_RX_LED_INDICATOR_PIN);

    // Use this if you have a car with CAN bus
    //canInterface = new CanMessageSenderEsp32Arduino(CAN_RX_PIN, CAN_TX_PIN);
    //remoteSource = new CanRemoteSource(canInterface);

    // JVC remote control
    //InitJVC();

    // Pioneer remote control
    //InitPioneer();

    // Alpine remote control
    InitAlpine();

    // Peugeot CAN remote control (only makes sense if the remoteSource is not a CanRemoteSource), make sure you initialize the CanInterface above
    //InitPeugeotCAN;
}

void loop()
{
    currentTime = millis();
    if (currentTime - prevTime > 10)
    {
        prevTime = currentTime;

        uint8_t buttonData;
        uint8_t scrollData;

        if (remoteSource->GetRemoteStatus(&buttonData, &scrollData))
        {
            remoteButton.value = buttonData;

            remoteControl->WheelPosition(remoteButton.buttons.wheel_scrolled_up, remoteButton.buttons.wheel_scrolled_down, scrollData);

            if (remoteButton.buttons.seek_down_pressed)
            {
                remoteControl->SeekDown();
            }

            if (remoteButton.buttons.seek_up_pressed)
            {
                remoteControl->SeekUp();
            }

            if (remoteButton.buttons.source_pressed)
            {
                remoteControl->Source();
            }

            if (remoteButton.buttons.volume_down_pressed && !remoteButton.buttons.volume_up_pressed)
            {
                remoteControl->VolumeDown();
            }

            if (remoteButton.buttons.volume_up_pressed && !remoteButton.buttons.volume_down_pressed)
            {
                remoteControl->VolumeUp();
            }

            if (remoteButton.buttons.volume_down_pressed && remoteButton.buttons.volume_up_pressed)
            {
                remoteControl->Mute();
            }

            remoteControl->ProcessRawData(buttonData, scrollData);
        }
    }
}
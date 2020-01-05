#include <esp32_arduino_rmt_van_rx.h>

#include "src/AbstractRemote.h"
#include "src/JVC_CNY173_Remote.h"
#include "src/RemoteStalkStruct.h"

ESP32_RMT_VAN_RX VAN_RX;

const uint8_t VAN_DATA_RX_RMT_CHANNEL = 0;
const uint8_t VAN_DATA_RX_PIN = 21;
const uint8_t VAN_DATA_RX_LED_INDICATOR_PIN = 2;

const uint8_t LED_PIN = 2;
const uint8_t OUT_PIN = 19;

uint8_t vanMessageLength;
uint8_t vanMessage[34];

uint32_t currentTime = 0;
uint32_t prevTime = 0;

AbstractRemote *remoteControl;
RemoteButton remoteButton;

void setup()
{
    Serial.begin(500000);
    Serial.println("ESP32 Arduino VAN bus steering wheel adapter");

    VAN_RX.Init(VAN_DATA_RX_RMT_CHANNEL, VAN_DATA_RX_PIN, VAN_DATA_RX_LED_INDICATOR_PIN, VAN_LINE_LEVEL_HIGH, VAN_NETWORK_TYPE_COMFORT);

    remoteControl = new JVC_CNY173_Remote(OUT_PIN, LED_PIN);
}

void loop()
{
    currentTime = millis();
    if (currentTime - prevTime > 10)
    {
        prevTime = currentTime;
        VAN_RX.Receive(&vanMessageLength, vanMessage);

        if (vanMessageLength > 0)
        {
            if (VAN_RX.IsCrcOk(vanMessage, vanMessageLength))
            {
                if (vanMessage[1] == 0x9C && vanMessage[2] == 0x4C)
                {
                    remoteControl->WheelPosition(vanMessage[4]);

                    remoteButton.value = vanMessage[3];

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
                }
            }
        }
    }
}
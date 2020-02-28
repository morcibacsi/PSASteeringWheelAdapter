#pragma once

#ifndef _AbstractCanMessageSender_h
    #define _AbstractCanMessageSender_h

class AbstractCanMessageSender {
  public:
    virtual void Init() = 0;
    virtual void SendMessage(uint16_t canId, byte ext, byte sizeOfByteArray, unsigned char *byteArray) = 0;
    virtual void ReadMessage(uint16_t* canId, uint8_t* len, uint8_t* buf) = 0;
 };

#endif

// AbstractRemote.h
#pragma once

#ifndef _AbstractRemote_h
    #define _AbstractRemote_h

class AbstractRemote {
  public:
    virtual void VolumeUp()=0;
    virtual void VolumeDown() = 0;
    virtual void SeekUp() = 0;
    virtual void SeekDown() = 0;
    virtual void Source() = 0;
    virtual void Mute() = 0;
    virtual void WheelPosition(uint8_t currentPosition) = 0;
 };

#endif

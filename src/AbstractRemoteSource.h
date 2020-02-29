#pragma once

#ifndef _AbstractRemoteSource_h
    #define _AbstractRemoteSource_h

class AbstractRemoteSource {
  public:
    virtual bool GetRemoteStatus(uint8_t* buttonData, uint8_t* scrollData) = 0;
 };

#endif

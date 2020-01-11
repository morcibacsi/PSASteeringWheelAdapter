// RemoteStalkStruct.h
#pragma once

#ifndef _RemoteStalkStruct_h
    #define _RemoteStalkStruct_h

typedef struct {
    uint8_t unknown0            : 1; // bit 0
    uint8_t source_pressed      : 1; // bit 1
    uint8_t volume_down_pressed : 1; // bit 2
    uint8_t volume_up_pressed   : 1; // bit 3
    uint8_t wheel_scrolled_down : 1; // bit 4
    uint8_t wheel_scrolled_up   : 1; // bit 5
    uint8_t seek_down_pressed   : 1; // bit 6
    uint8_t seek_up_pressed     : 1; // bit 7
} RemoteStalkStruct;

typedef union {
    RemoteStalkStruct buttons;
    uint8_t value;
} RemoteButton;

#endif

#pragma once
#include <Arduino.h>

namespace PS4
{
    void poll();

    namespace DATA
    {
        void init();
        bool receiveData();

        // data structure to receive

        // https://github.com/pablomarquez76/PS4_Controller_Host/raw/refs/heads/main/examples/PS4ReceiveData/PS4ReceiveData.ino

        struct __attribute__((packed)) STRUCT
        {
            bool PSButton;
            bool Share;
            bool Options;
            bool L1;
            bool L2;
            bool L3;
            bool R1;
            bool R2;
            bool R3;
            bool Touchpad;

            bool Up;
            bool Down;
            bool Left;
            bool Right;

            bool Cross;
            bool Circle;
            bool Square;
            bool Triangle;

            uint16_t LStickX;
            uint16_t LStickY;
            uint16_t RStickX;
            uint16_t RStickY;
            uint16_t L2Value;
            uint16_t R2Value;
        };
        extern STRUCT inputStruct;
    }


    // abstractions
    extern bool *PSButton;
    extern bool *Share;
    extern bool *Options;
    extern bool *L1;
    extern bool *L2;
    extern bool *L3;
    extern bool *R1;
    extern bool *R2;
    extern bool *R3;
    extern bool *Touchpad;
    extern bool *Up;
    extern bool *Down;
    extern bool *Left;
    extern bool *Right;
    extern bool *Cross;
    extern bool *Circle;
    extern bool *Square;
    extern bool *Triangle;
    extern uint16_t *LStickX;
    extern uint16_t *LStickY;
    extern uint16_t *RStickX;
    extern uint16_t *RStickY;
    extern uint16_t *L2Value;
    extern uint16_t *R2Value;
}

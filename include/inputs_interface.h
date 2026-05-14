#pragma once
#include <Arduino.h>

namespace PS4
{
    bool poll();

    namespace DATA
    {
        void init();
        bool receiveData();

        // https://github.com/pablomarquez76/PS4_Controller_Host/raw/refs/heads/main/examples/PS4ReceiveData/PS4ReceiveData.ino

        /*

        Below are all possible inputs.

        Those suffixed with ': 1' are called bitfields;
        they are integers that take up 1 bit of space.
        They can only store two states in that bit (on and off)
        so they're basically booleans. When put into a
        struct, they share the same byte in memory.
        Optimizations like these are very important
        when transferring latency critical data,
        especially since we're doing all of this on
        a single wire.

        Please note the variables that are not bitfields.
        I'm pretty sure the only ones worth noting are the
        joysticks, and the battery. I believe the battery
        reports a max of 100, but I'm not sure so please
        reference the library
        https://github.com/pablomarquez76/PS4_Controller_Host/blob/main/examples/PS4ReceiveData/PS4ReceiveData.ino

        https://github.com/pablomarquez76/PS4_Controller_Host/

    */
   
        // data structure to receive

        struct __attribute__((packed)) STRUCT
        {
            /*********************/
            /*   B U T T O N S   */
            /*********************/

            uint8_t Cross : 1;
            uint8_t Circle : 1;
            uint8_t Square : 1;
            uint8_t Triangle : 1;

            uint8_t Up : 1;
            uint8_t Down : 1;
            uint8_t Left : 1;
            uint8_t Right : 1;

            uint8_t PSButton : 1;
            uint8_t Touchpad : 1;
            uint8_t Share : 1;
            uint8_t Options : 1;

            uint8_t L1 : 1;
            uint8_t L2 : 1;

            uint8_t R1 : 1;
            uint8_t R2 : 1;

            uint8_t R3 : 1;
            uint8_t L3 : 1;

            /********************/
            /*    A N A L O G   */
            /********************/

            int8_t LStickX;
            int8_t LStickY;
            int8_t RStickX;
            int8_t RStickY;

            /*******************************/
            /*   S T A T U S   F L A G S   */
            /*******************************/

            uint8_t battery;
            uint8_t charging : 1;
            uint8_t audio : 1;
            uint8_t mic : 1;

            /********************/
            /*   S E N S O R S  */
            /********************/

            int16_t gyro_x;
            int16_t gyro_y;
            int16_t gyro_z;

            int16_t accel_x;
            int16_t accel_y;
            int16_t accel_z;
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
    extern int8_t *LStickX;
    extern int8_t *LStickY;
    extern int8_t *RStickX;
    extern int8_t *RStickY;
    extern uint8_t *battery;
    extern bool *charging;
    extern bool *audio;
    extern bool *mic;
    extern int16_t *gyro_x;
    extern int16_t *gyro_y;
    extern int16_t *gyro_z;
    extern int16_t *accel_x;
    extern int16_t *accel_y;
    extern int16_t *accel_z;
}

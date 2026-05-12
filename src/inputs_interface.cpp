#include "inputs_interface.h"
#include "SerialTransfer.h"

namespace PS4
{
    void poll()
    {
        // recieve new data, and process if available
        DATA::receiveData();
        // process/abstract new data
    }

    namespace DATA
    {
        SerialTransfer serialTransfer;
        STRUCT inputStruct;

        void init()
        {
            Serial1.begin(115200);
            serialTransfer.begin(Serial1);
        }
        bool receiveData()
        {
            if (serialTransfer.available())
            {
                serialTransfer.rxObj(inputStruct);
                return true;
            }
            return false;
        }
    }
    bool *PSButton = &DATA::inputStruct.PSButton;
    bool *Share = &DATA::inputStruct.Share;
    bool *Options = &DATA::inputStruct.Options;
    bool *L1 = &DATA::inputStruct.L1;
    bool *L2 = &DATA::inputStruct.L2;
    bool *L3 = &DATA::inputStruct.L3;
    bool *R1 = &DATA::inputStruct.R1;
    bool *R2 = &DATA::inputStruct.R2;
    bool *R3 = &DATA::inputStruct.R3;
    bool *Touchpad = &DATA::inputStruct.Touchpad;
    bool *Up = &DATA::inputStruct.Up;
    bool *Down = &DATA::inputStruct.Down;
    bool *Left = &DATA::inputStruct.Left;
    bool *Right = &DATA::inputStruct.Right;
    bool *Cross = &DATA::inputStruct.Cross;
    bool *Circle = &DATA::inputStruct.Circle;
    bool *Square = &DATA::inputStruct.Square;
    bool *Triangle = &DATA::inputStruct.Triangle;
    uint16_t *LStickX = &DATA::inputStruct.LStickX;
    uint16_t *LStickY = &DATA::inputStruct.LStickY;
    uint16_t *RStickX = &DATA::inputStruct.RStickX;
    uint16_t *RStickY = &DATA::inputStruct.RStickY;
    uint16_t *L2Value = &DATA::inputStruct.L2Value;
    uint16_t *R2Value = &DATA::inputStruct.R2Value;
}
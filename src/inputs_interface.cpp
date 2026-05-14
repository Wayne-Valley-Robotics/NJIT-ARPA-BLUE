#include "inputs_interface.h"
#include "SerialTransfer.h"

namespace PS4
{
    // Button getters
    bool getCross() { return DATA::inputStruct.Cross; }
    bool getCircle() { return DATA::inputStruct.Circle; }
    bool getSquare() { return DATA::inputStruct.Square; }
    bool getTriangle() { return DATA::inputStruct.Triangle; }
    bool getUp() { return DATA::inputStruct.Up; }
    bool getDown() { return DATA::inputStruct.Down; }
    bool getLeft() { return DATA::inputStruct.Left; }
    bool getRight() { return DATA::inputStruct.Right; }
    bool getPSButton() { return DATA::inputStruct.PSButton; }
    bool getTouchpad() { return DATA::inputStruct.Touchpad; }
    bool getShare() { return DATA::inputStruct.Share; }
    bool getOptions() { return DATA::inputStruct.Options; }
    bool getL1() { return DATA::inputStruct.L1; }
    bool getL2() { return DATA::inputStruct.L2; }
    bool getR1() { return DATA::inputStruct.R1; }
    bool getR2() { return DATA::inputStruct.R2; }
    bool getR3() { return DATA::inputStruct.R3; }
    bool getL3() { return DATA::inputStruct.L3; }

    // Analog stick getters
    int8_t getLStickX() { return DATA::inputStruct.LStickX; }
    int8_t getLStickY() { return DATA::inputStruct.LStickY; }
    int8_t getRStickX() { return DATA::inputStruct.RStickX; }
    int8_t getRStickY() { return DATA::inputStruct.RStickY; }

    // Battery and status getters
    uint8_t getBattery() { return DATA::inputStruct.battery; }
    bool getCharging() { return DATA::inputStruct.charging; }
    bool getAudio() { return DATA::inputStruct.audio; }
    bool getMic() { return DATA::inputStruct.mic; }

    // Sensor getters
    int16_t getGyroX() { return DATA::inputStruct.gyro_x; }
    int16_t getGyroY() { return DATA::inputStruct.gyro_y; }
    int16_t getGyroZ() { return DATA::inputStruct.gyro_z; }
    int16_t getAccelX() { return DATA::inputStruct.accel_x; }
    int16_t getAccelY() { return DATA::inputStruct.accel_y; }
    int16_t getAccelZ() { return DATA::inputStruct.accel_z; }

    // Poll function
    bool poll()
    {
        // receive new data, and process if available
        return DATA::receiveData();
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
}

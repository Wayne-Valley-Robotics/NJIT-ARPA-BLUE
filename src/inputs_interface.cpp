#include "inputs_interface.h"
#include "SerialTransfer.h"

#define DEADZONE 25

namespace PS4
{

    // abstracted for ease of access

    /*********************/
    /*   B U T T O N S   */
    /*********************/

    bool Cross() { return DATA::inputStruct.Cross; }
    bool Circle() { return DATA::inputStruct.Circle; }
    bool Square() { return DATA::inputStruct.Square; }
    bool Triangle() { return DATA::inputStruct.Triangle; }

    bool Up() { return DATA::inputStruct.Up; }
    bool Down() { return DATA::inputStruct.Down; }
    bool Left() { return DATA::inputStruct.Left; }
    bool Right() { return DATA::inputStruct.Right; }

    bool PSButton() { return DATA::inputStruct.PSButton; }
    bool Touchpad() { return DATA::inputStruct.Touchpad; }
    bool Share() { return DATA::inputStruct.Share; }
    bool Options() { return DATA::inputStruct.Options; }

    bool L1() { return DATA::inputStruct.L1; }
    int8_t L2() { return DATA::inputStruct.L2; }

    bool R1() { return DATA::inputStruct.R1; }
    int8_t R2() { return DATA::inputStruct.R2; }

    bool R3() { return DATA::inputStruct.R3; }
    bool L3() { return DATA::inputStruct.L3; }

    /********************/
    /*    A N A L O G   */
    /********************/

    int8_t LStickX()
    {
        return DATA::filterDeadzone(DATA::inputStruct.LStickX, DEADZONE);
    }

    int8_t LStickY()
    {
        return DATA::filterDeadzone(DATA::inputStruct.LStickY, DEADZONE);
    }

    int8_t RStickX()
    {
        return DATA::filterDeadzone(DATA::inputStruct.RStickX, DEADZONE);
    }

    int8_t RStickY()
    {
        return DATA::filterDeadzone(DATA::inputStruct.RStickY, DEADZONE);
    }

    /*******************************/
    /*   S T A T U S   F L A G S   */
    /*******************************/

    uint8_t battery() { return DATA::inputStruct.battery; }
    bool isCharging() { return DATA::inputStruct.charging; }
    bool hasAudio() { return DATA::inputStruct.audio; }
    bool hasMic() { return DATA::inputStruct.mic; }

    /********************/
    /*   S E N S O R S  */
    /********************/

    int16_t gyro_x() { return DATA::inputStruct.gyro_x; }
    int16_t gyro_y() { return DATA::inputStruct.gyro_y; }
    int16_t gyro_z() { return DATA::inputStruct.gyro_z; }
    int16_t accel_x() { return DATA::inputStruct.accel_x; }
    int16_t accel_y() { return DATA::inputStruct.accel_y; }
    int16_t accel_z() { return DATA::inputStruct.accel_z; }

    bool poll()
    {
        // receive new data, and process if available
        return DATA::receiveData();
        // process/abstract new data
    }

    namespace DATA
    {
        SerialTransfer serialTransfer;
        INPUT_STRUCT inputStruct;

        void init()
        {
            Serial2.begin(115200);
            serialTransfer.begin(Serial2);
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
        int filterDeadzone(int _input, int _deadZone)
        {
            if (_input > _deadZone || _input < -_deadZone)
            {
                return _input;
            }
            return 0;
        }
    }
}

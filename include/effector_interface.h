#pragma once

namespace effector_interface
{
    void init();
    // Call as often as possible.
    void proc();
    void moveUp(int speed);
    void moveDown(int speed);
    void moveLeft(int speed);
    void moveRight(int speed);
}
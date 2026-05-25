#include "Arduino.h"
// #include "battery.h"
#include "inputs_interface.h"
#include "drive_interface.h"
#include "effector_interface.h"

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  PS4::DATA::init();

  // TODO: implement some kind of battery check to make sure the battery is charged. there are no safe-guards for this
  // and in the worst case scenario everything could look fine, until you try to drive all the motors at once and the
  // whole system browns out. maybe it's also a good idea to implement some kind of connectivity check with the controller?
  // the inputs shouldn't update (calling PS4::poll() will return false) if the controller isn't connected. then stop motors or smth idk
  // ^^ THIS IS NOT TRUE. it will return false if the data transmission failed. this can be used to only run input processing when inputs are available,
  // but it will NOT tell you if the controller is connected. thats a feature that needs to be implemented in the esp32 project. i dont have time...

  drive_interface::init();
  effector_interface::init();

  if (!PS4::poll())
  {
    Serial.println("Waiting for controller...");
    // wait for first input from esp32.
    // this only tells you if the controller
    // is connected because the esp32 will
    // not transmit data unless it is.
    // this is a stupid idea.
    bool state = false;
    while (!PS4::poll())
    {
      // ts lowk cool it will logically only start blinking after 800ms
      delay(400);
      digitalWrite(LED_BUILTIN, state);
      state = !state;
    }
  }
  Serial.println("Controller Connected!");
}

void loop()
{
  PS4::poll();
  using namespace drive_interface;
  triDrive(PS4::LStickY() * 2, PS4::LStickX() * 2, PS4::RStickX() * 2);

  // ima js slap the gantry controls here ts od work
  int effector_speed = 150;

  effector_interface::moveUp(PS4::Up() * effector_speed);
  effector_interface::moveDown(PS4::Down() * effector_speed);
  effector_interface::moveLeft(PS4::Left() * effector_speed);
  effector_interface::moveRight(PS4::Right() * effector_speed);

  effector_interface::proc();

  digitalWrite(LED_BUILTIN, PS4::PSButton());
}
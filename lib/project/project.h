// **** Project code definition here ...
#include <buttons.h>
// taken from https://randomnerdtutorials.com/esp8266-nodemcu-stepper-motor-28byj-48-uln2003/
#include <AccelStepper.h>

const int StepsPerRevolution = 4096;        // change this to fit the number of steps per revolution
const int RevolutionsPerTurn = 7;           // change this to fit the number of revolutions per Plate turn
float MAXVelocity = 1024;                   // Max Velocuty value supported
int Angle = 0;                              // Turntable position measured in angle
int LastAngle = 0;
bool Moving = false;
const unsigned long ZeroTimeOut = 60000;    // Timeout to return to Angle Zero  

// IR Sensor Pin
#define IR_Out 13

// ULN2003 Motor Driver Pins
#define IN1 5
#define IN2 4
#define IN3 14
#define IN4 12

// initialize the stepper library
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

void print_angle() {
            telnet_println("Angle: " + String(Angle), true);
            //telnet_print("\tStepPosition: " + String(stepper.currentPosition()));
            telnet_println("\tIR Satus: " + String(A_STATUS));
}

void MoveTo() {
    if(Angle < config.MINAngle || Angle > config.MAXAngle) {
        telnet_println ("ERROR: Value to wide! Allowed range from" + String(config.MINAngle) + " to " + String(config.MAXAngle) + ".", true);
        Angle = LastAngle;
    }
    else {
      stepper.moveTo(StepsPerRevolution * RevolutionsPerTurn * Angle / 360);
      Moving = true;
    }
    
}

void ReturnToZero() {
    unsigned long last_zerotime = millis();
  telnet_println("ReturnToZero A_Satus: " + String(A_STATUS));
  if (A_STATUS)
      while (A_STATUS && millis() - last_zerotime < ZeroTimeOut) {
          stepper.setSpeed(-config.Velocity);
          stepper.run();
          yield();
      }
  else 
      while (!A_STATUS && millis() - last_zerotime < ZeroTimeOut) {
          stepper.setSpeed(config.Velocity);
          stepper.run();
          yield();
      }
  if (millis() - last_zerotime >= ZeroTimeOut)  telnet_println("Return To Zero TimeOut!", true);
  stepper.disableOutputs();
  stepper.setCurrentPosition(0);
  Angle = 0;
  LastAngle = 0;
  Moving = false;
  print_angle();
}

void project_hw() {
 // Output GPIOs


 // Input GPIOs

}


void project_setup() {
    buttons_setup();
  // set the speed and acceleration
    stepper.setMaxSpeed(config.Velocity);
    stepper.setAcceleration(256);
    ReturnToZero();
    stepper.setCurrentPosition(0);
}

void project_loop() {
  // move the stepper motor (one step at a time)
    if (Moving) {
        if (stepper.distanceToGo() != 0) stepper.run();
        else {
            stepper.disableOutputs();
            Moving = false;
            telnet_println("Mechanical movement done.", true);
            print_angle();
            console_prompt();
        }
    }
    
  // check Angle Change to move the Step Motor
    if (Angle != LastAngle) {
        MoveTo();
        LastAngle = Angle;
    }


    if (A_COUNT == 2 && (millis() - Last_A > Butt_Interval)) {
        A_COUNT = 0;
    }
    if (A_COUNT == 3 && (millis() - Last_A > Butt_Interval)) {
        ESPRestart();
        A_COUNT = 0;
    }
    if (A_COUNT == 4 && (millis() - Last_A > Butt_Interval)) {
        A_COUNT = 0;
    }
    if (A_COUNT == 5 && (millis() - Last_A > Butt_Interval)) {
        storage_reset();
        ESPRestart();
        A_COUNT = 0;
    }
}

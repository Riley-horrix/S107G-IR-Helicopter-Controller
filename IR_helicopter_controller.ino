#include "IR_helicopter_utils.h"

#define TRIM 52

#define IR_OUTPUT_PIN D4 

#define THROTTLE_PIN A0
#define INCREASE_THROTTLE_PIN D2
#define DECREASE_THROTTLE_PIN D3

#define CONTROLLER_IN_0 D0
#define CONTROLLER_IN_1 D1
#define CONTROLLER_IN_2 D5

// const int fifty_throttle = 0x3c3fc834;  // Test Instruction


void setup(void) {

  // Set IR analog pulse frequency
  analogWriteFreq(FREQUENCY);

  // Set IR write range to some defined number so that 50% duty cycle is well defined
  analogWriteRange(DUTY_RANGE);

  // Initialise the output pin
  pinMode(IR_OUTPUT_PIN, OUTPUT);

  // Initialise 3 controller inputs
  pinMode(CONTROLLER_IN_0, INPUT);
  pinMode(CONTROLLER_IN_1, INPUT);
  pinMode(CONTROLLER_IN_2, INPUT);

  // Initialise 2 digital throttle adjustment inputs
  pinMode(INCREASE_THROTTLE_PIN, INPUT_PULLUP);
  pinMode(DECREASE_THROTTLE_PIN, INPUT_PULLUP);

  // Initialise throttle analogue pin
  pinMode(THROTTLE_PIN, INPUT);

}


void loop(void) {

  static uint32_t instruction = 0;

  if (should_send(instruction)) {
    send_instr(instruction);
  }

  instruction = construct_instruction();

  delayMicroseconds(TIME_TILL_NEXT_INSTR);

}

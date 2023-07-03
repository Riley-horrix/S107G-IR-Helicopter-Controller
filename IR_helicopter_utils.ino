#include "IR_helicopter_utils.h"


/** Returns bits 23 - 16 of the given 32bit input instruction to isolate throttle byte. */
int get_throttle(uint32_t instruction) {

  return (instruction & (BYTE_MASK << THROTTLE_SHIFT)) >> THROTTLE_SHIFT;

}


/** 
 * Used so that low power instructions are not sent to the helicopter, 
 * as ~ 60 % throttle is needed to takeoff. 
 */
bool should_send(uint32_t instruction) {

  return get_throttle(instruction) > 50;

}


/** 
 * Decodes the breadboard signals according to the README, returns a 3 bit number 
 * wrapped in 8 bits, corresponding to which inputs are set. 
 */
uint8_t decode_3bit_input(void) {

  uint8_t result = 0;

  if (digitalRead(CONTROLLER_IN_0) != 0) {
    result |= BIT_0;
  }

  if (digitalRead(CONTROLLER_IN_1) != 0) {
    result |= BIT_1;
  }

  if (digitalRead(CONTROLLER_IN_2) != 0) {
    result |= BIT_2;
  }

  return result;

}


/** Returns a bool, corresponding to (bit2 XOR bit1). */
bool is_yaw(uint8_t input) {

  return (input & BIT_1) != (input & BIT_2);

}


/** 
 * Pre -- yaw byte must be in the least significant byte of 'base'.
 * Will offset the base by the YAW_OFFSET if the input parameter includes a yaw configuration.
 */
uint32_t get_3bit_yaw_byte(uint32_t base, uint8_t input) {

  if (is_yaw(input)) {
    if (input & IS_RIGHT) {

      base -= YAW_OFFSET;

    } else {

      base += YAW_OFFSET;

    }
  }

  return base;

}


/** 
 * Pre -- pitch byte must be in the least significant byte of parameter 'base'.
 * Will offset the base by the PITCH_OFFSET if the 'input' parameter includes a pitch configuration.
 */
uint32_t get_3bit_pitch_byte(uint32_t base, uint8_t input) {

  if (input & IS_FORWARD) {

    base -= PITCH_OFFSET;

    if (input == EXTRA_PITCH) {
      base -= PITCH_OFFSET;
    }


  } else if (input == IS_BACKWARD) {

    base += PITCH_OFFSET;

  }

  return base;

}


/** 
 * Sends a 38 khz, set at 50% duty cycle signal to designated output 
 * pin for parameter 'duration' number of microseconds. 
 */
void send_38khz_pulse(unsigned int duration) {
 
  analogWrite(IR_OUTPUT_PIN, DUTY);
  delayMicroseconds(duration);

}


/** 
 * Sends a '1' according to the S107G IR protocol. 
 * Pre -- output pin must be set to LOW. 
 */
void send_one(void) {

  delayMicroseconds(ONE_LOW_DURATION);
  send_38khz_pulse(HIGH_DURATION);
  digitalWrite(IR_OUTPUT_PIN, LOW);

}

/** 
 * Sends a '0' according to the S107G IR protocol. 
 * Pre --  output pin must be set to LOW. 
 */
void send_zero(void) {

  delayMicroseconds(ZERO_LOW_DURATION);
  send_38khz_pulse(HIGH_DURATION);
  digitalWrite(IR_OUTPUT_PIN, LOW);

}


/** Sends the instruction header according to the S107G IR protocol. */
void send_header(void) {

  send_38khz_pulse(HEADER_LENGTH);
  digitalWrite(IR_OUTPUT_PIN, HIGH);
  delayMicroseconds(HEADER_LENGTH);
  send_38khz_pulse(HIGH_DURATION);
  digitalWrite(IR_OUTPUT_PIN, LOW);

}


/** 
 * Takes input from two breadboard buttons, and offsets throttle by 
 * THROTTLE_OFFSET if either one is pressed, if both are pressed then 
 * it will decrease the throttle as a safety measure.
 */
uint32_t adjust_throttle(uint32_t base) {

  if (digitalRead(DECREASE_THROTTLE_PIN) == 0) {

    // Make sure to not underflow the byte
    if (base >= THROTTLE_OFFSET) {

      base -= THROTTLE_OFFSET;

    } else { 

      base = 0;

    }

  } else if (digitalRead(INCREASE_THROTTLE_PIN) == 0) {

    // Make sure to not overflow the byte
    if (base <= THROTTLE_MAX - THROTTLE_OFFSET) {

      base += THROTTLE_OFFSET;
      
    } else { 

      base = THROTTLE_MAX;

    }

  }

  return base;
}


void send_instr(uint32_t instruction) {

  uint32_t bit_mask = 1;
  bit_mask <<= 31;

  send_header();

  for (int i = 0; i  < INSTR_LENGTH; i++) {

    if ((bit_mask & instruction) == 0) {
      send_zero();
    } else {
      send_one();
    }

    bit_mask >>= 1;

  }

}


uint32_t construct_instruction(void) {

  // Read the hover throttle from analog input pin
  // Then translate it to the 0-255 constraint
  // Digitally read from buttons to adjust it
  // Then shift it into the right position
  uint32_t throttle_byte = analogRead(THROTTLE_PIN);
  throttle_byte /= 4;
  throttle_byte = adjust_throttle(throttle_byte);
  throttle_byte <<= THROTTLE_SHIFT;

  // Gather accelerometer input
  uint8_t user_input = decode_3bit_input();

  // Check whether an input from the controller requires a change of pitch
  uint32_t pitch_byte = PITCH_BASE;
  pitch_byte = get_3bit_pitch_byte(pitch_byte, user_input);
  pitch_byte <<= PITCH_SHIFT;

  // Check whether an input from the controller requires a change of yaw
  uint32_t yaw_byte = YAW_BASE;
  yaw_byte = get_3bit_yaw_byte(yaw_byte, user_input);
  yaw_byte <<= YAW_SHIFT;

  // Hardcoded trim for now
  uint32_t trim_byte = TRIM;

  return throttle_byte | pitch_byte | yaw_byte | trim_byte;

}

#define THROTTLE_SHIFT 8
#define PITCH_SHIFT 16
#define YAW_SHIFT 24

#define THROTTLE_MAX UINT8_MAX
#define THROTTLE_OFFSET 20

#define BIT_0 1
#define BIT_1 2
#define BIT_2 4

#define BYTE_MASK (0xff)

#define YAW_BASE 60
#define IS_YAW 6      // Bit mask for bits 1 & 2
#define IS_RIGHT 1    // Bit mask for bit 0

/** Value the base is offset by when input detected. */
#define YAW_OFFSET 40

#define PITCH_BASE 58
#define IS_FORWARD 6          // Bit mask for bits 1 & 2
#define IS_BACKWARD 1         // Bit mask for bit 0
#define EXTRA_PITCH 7         // Bit mask for bits 0, 1 ,2

/** Value the base is offset by when input detected. */
#define PITCH_OFFSET 30

// All delay times are given in microseconds
#define HEADER_LENGTH 2000
#define HIGH_DURATION 380
#define ONE_LOW_DURATION 600
#define ZERO_LOW_DURATION 220

#define INSTR_LENGTH 32

#define FREQUENCY 38000
#define DUTY_RANGE 255
#define DUTY 127  // 50% duty

#define INSTR_DURATION 180000


/** 
 * This function returns a 32 bit instruction, that is dynamically controlled
 * by breadboard inputs, into the ESP8266. 
 */
extern uint32_t construct_instruction(void);


/** 
 * This function takes as parameter an unsigned 32 bit instruction to be sent, 
 * and will send it after sending the header. The bits in the actual instruction
 * correlate to whether a '1' or a '0' will be sent according to the protocol outlined
 * in the README.
 */
extern void send_instr(uint32_t instruction);

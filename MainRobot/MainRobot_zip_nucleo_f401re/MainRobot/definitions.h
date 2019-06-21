#ifndef PCA9685_LIBRARY_DEFINITIONS_H
#define PCA9685_LIBRARY_DEFINITIONS_H

#define ADDR 0x80

#define MODE1 0x0
#define MODE2 0x1
#define PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

#define OSC_CLOCK 25e6

#define PWM_SCALER 0.90425 //set by manual calibration - ratio of FREQUENCY_SET:ACTUAL_OUTPUT

#endif
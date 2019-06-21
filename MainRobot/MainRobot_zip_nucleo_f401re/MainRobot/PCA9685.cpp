#ifndef PCA9685_LIBRARY_CPP
#define PCA9685_LIBRARY_CPP


#include "mbed.h"
#include "PCA9685.h"
#include "definitions.h"


PCA9685::PCA9685(uint8_t i2c_address, I2C i2c_object, float frequency) :

    i2c_addr(i2c_address),
    freq(frequency),
    i2c(i2c_object)

{

}

void PCA9685::init(void)
{

    reset();

    uint8_t prescale = (uint8_t) (OSC_CLOCK / (4096 * PWM_SCALER * freq)) - 1;

    write_8(MODE1, 0x21); //0010 0001 : AI ENABLED
    write_8(MODE2, 0x07); //0000 0111 : NOT INVRT, CHANGE ON STOP, TOTEM POLE, \OE = 1, LEDn = HIGH IMP

    set_prescale(prescale);

}

void PCA9685::reset(void)
{

    write_8(MODE1,0x00);

}

void PCA9685::set_prescale(uint8_t prescale)
{

    uint8_t oldmode = read_8(MODE1);
    uint8_t newmode = (oldmode&0x7F) | 0x10; // set the sleep bit

    write_8(MODE1, newmode); // send the device to sleep
    wait_ms(5);
    write_8(PRESCALE, prescale); // set the prescaler
    write_8(MODE1, oldmode);
    wait_ms(5);
    write_8(MODE1, oldmode | 0xa1); // wake up the device

}


//NB REQUIRES AUTO-INCREMENT MODE ENABLED
//0 <= pwm_output <= 15
//0 <= (count_on || count_off) <= 4095
void PCA9685::set_pwm_output(int pwm_output, uint16_t count_on, uint16_t count_off)
{

    char msg[5];

    msg[0] = LED0_ON_L + (4 * pwm_output);
    msg[1] = count_on;
    msg[2] = count_on >> 8;
    msg[3] = count_off;
    msg[4] = count_off >> 8;

    i2c.write(i2c_addr, msg, 5);

}

void PCA9685::set_pwm_output_on_0(int pwm_output, uint16_t count_off)
{

    char msg[3];

    msg[0] = LED0_ON_L + 2 + (4 * pwm_output);
    msg[1] = count_off;
    msg[2] = count_off >> 8;

    i2c.write(i2c_addr,msg,3);

}


//NB REQUIRES AUTO-INCREMENT MODE ENABLED
//0 <= pwm_output <= 15
void PCA9685::set_pwm_duty(int pwm_output, float duty_cycle)
{

    if (duty_cycle > 1.0) {
        duty_cycle = 1.0;
    }
    if (duty_cycle < 0.0) {
        duty_cycle = 0.0;
    }

    uint16_t count_off = (uint16_t) (duty_cycle * 4095);
    uint16_t count_on = 0x0000;

    set_pwm_output(pwm_output, count_on, count_off);

}


//NB REQUIRES AUTO-INCREMENT MODE ENABLED
//0 <= pwm_output <= 15
uint8_t PCA9685::set_pwm_pw(int pwm_output, float pulse_width_us)
{

    
    float period_us = (1e6/freq);

    float duty = pulse_width_us/period_us;

    set_pwm_duty(pwm_output, duty);
   
}


void PCA9685::update(void)
{

    i2c.stop();

}



void PCA9685::write_8(uint8_t reg, uint8_t msg)
{

    char send[2]; //Store the address and data in an array
    send[0] = reg;
    send[1] = msg;
    i2c.write(i2c_addr, send, 2);

}

uint8_t PCA9685::read_8(uint8_t reg)
{

    char send[1] ;
    send[0] = reg;
    i2c.write(i2c_addr, send, 1);
    char recieve[1];
    i2c.read(i2c_addr, recieve, 1);
    return recieve[0];

}

int PCA9685::convert_pwm_value(float pulse_width_us, float period_us)
{

    int result;
    float interim;

    interim = ((pulse_width_us / period_us) * 4095); //scale the pulse width to a 12-bit scale
    result = (int) interim; //round the value to the nearest integer

    return result;

}

#endif
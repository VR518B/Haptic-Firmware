#ifndef MOTOR_H
#define MOTOR_H

#include <config.h>
#include <stdint.h>
#include <stddef.h>
#include <packet.h>

#include <PCA9685.h>

enum AccessType 
{
  I2C_DRIVER,
  DATA_PIN
};

struct i2c_Address{
  uint8_t driver;
  uint8_t port;
};

struct motorMapEntry
{
  uint8_t motorAccessType;
  union 
  {
    struct i2c_Address i2c;
    uint8_t DataPin;
  };
};

// Initalize the Motormap structure according to config.h
void InitMotorMap();

// Takes a motorID assigned in the PWM_PINS array in config.h.
// Takes a Strength value from 0-255.
// Updates PWM signals to motors and
// calculates attenuation if the Strength is
// unchanging for a long time.
void WriteToMotor(size_t motorID, uint8_t Str);

// Updates the attenuation values of motors when it is called
void CheckStrAttenuation();

void UpdateMotorStrength(uint8_t *StrArray, size_t length);

void UpdateAttenuationFunc(struct Attenuation_Control control);

#endif
#include <Wire.h>
#include <config.h>
#include <motor.h>

uint8_t currentStrInput[NUM_PINS];

uint8_t attenuationSpeed = 20;
uint32_t attenuationDelay = 10000;

const size_t NUM_EXT_MOTORS = NUM_DRIVERS * MOTORS_PER_DRIVER;
const size_t motorMapLen = NUM_EXT_MOTORS + NUM_PINS;
PCA9685 pwm[NUM_DRIVERS] = {0}; 
struct motorMapEntry motorMap[motorMapLen];


// initialize all the pwm controllers to the addresses in
// in the i2c addrs arry in config.h
void InitPWM(PCA9685 *pwm_list) 
{
  Wire.setClock(400000);

  for (size_t i = 0; i < NUM_DRIVERS; i++) 
  {
    pwm_list[i] = PCA9685(i2c_ADDRS[i]);

    // Bring all servo drivers to a clean state
    pwm_list[i].resetDevices();

    // Initialize PWM pins
    pwm_list[i].init();
    pwm_list[i].setPWMFrequency(1600);
  }
}

void InitMotorMap() 
{
  InitPWM(pwm);

  for (size_t i = 0; i < NUM_PINS; i++) 
  {
    motorMap[i].motorAccessType = DATA_PIN;
    motorMap[i].DataPin = PWM_PINS[i];
  }

  for (size_t i = 0; i < NUM_DRIVERS; i++) 
  {
    for (size_t j = 0; j < MOTORS_PER_DRIVER; j++) 
    {
      size_t cur_index = (i * MOTORS_PER_DRIVER) + j;

      motorMap[cur_index].motorAccessType = I2C_DRIVER;
      motorMap[cur_index].i2c.driver = i;
      motorMap[cur_index].i2c.port = j;
    }
  }
}

uint8_t CalculateAttenuation(uint8_t currentStrength, uint lastUpdate)
{
  static unsigned long LastAttenuation = 0;
  unsigned long sinceLastUpdate = millis() - lastUpdate;
  
  // update according to the refresh rate.
  if ((millis() - LastAttenuation) < (1000 / FALLOFF_REFRESH_RATE)) 
  {
    return currentStrength;
  }

  if (currentStrength == 0) {return 0;}

  if (sinceLastUpdate < attenuationDelay)
  {
    return currentStrength;
  }

  // Calculate how much to decrease the strength this loop
  // the amount of time that has elapsed since we last attenuated
  float timeElapsed = (float)(millis() - LastAttenuation) * 0.001;
  // speed * delta_time
  uint8_t attenuationValue = floor((float)attenuationSpeed * timeElapsed);

  // Integer overflow! Yay!
  if (attenuationValue > currentStrength)
  {
    LastAttenuation = millis();
    return 0;
  }

  LastAttenuation = millis();
  return currentStrength - attenuationValue;
}

void WriteToMap(size_t motorID, uint8_t Str) 
{
  struct motorMapEntry motor_entry = motorMap[motorID];

  switch(motor_entry.motorAccessType) 
  {
    case DATA_PIN:
    analogWrite(motor_entry.DataPin, Str);
    break;

    case I2C_DRIVER:
    PCA9685 driver = pwm[motor_entry.i2c.driver];
    driver.setChannelPWM(motor_entry.i2c.port, Str << 4);
    break;
  }
}

void WriteToMotor(size_t motorID, uint8_t Str)
{
  static unsigned long lastTimeUpdated[motorMapLen];
  static uint8_t currentMotorOutput[motorMapLen];

  if (currentStrInput[motorID] != Str)
  {
    // if the input we recieve is new, update
    WriteToMap(motorID, Str);
    currentStrInput[motorID] = Str;
    lastTimeUpdated[motorID] = millis();
    currentMotorOutput[motorID] = Str;
  }
  else
  {
    // if the input we recieve is still the same, apply attenuation
    currentMotorOutput[motorID] = CalculateAttenuation(currentMotorOutput[motorID],
                                                       lastTimeUpdated[motorID]);

    WriteToMap(motorID, Str);
  }
}

void UpdateMotorStrength(uint8_t *StrArray, size_t length)
{
  Serial.print("Received data: ");
  for (size_t i = 0; i < length; i++)
  {
    Serial.print(StrArray[i]);
    Serial.print(" ");

    // Set PWM pins based of of recieved strength values
    if (i < motorMapLen)
    {
      WriteToMotor(i, StrArray[i]);
    }
  }
  Serial.print(" | ");
}

void UpdateAttenuationFunc(struct Attenuation_Control control)
{
  attenuationDelay = control.AttenuationTime;
  attenuationSpeed = control.AttenuationStrength;
}

void CheckStrAttenuation()
{
  for (size_t i = 0; i < motorMapLen; i++)
  {
    WriteToMotor(i, currentStrInput[i]);
  }
}
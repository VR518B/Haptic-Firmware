#include <Arduino.h>
#include <WebSocketsServer.h>
#include <config.h>
#include <motor.h>
#include <network.h>
#include <Wire.h>

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  delay(200);
  Serial.println(" ");
  Serial.print("Connecting to WiFi");

  InitNetwork(WIFI_HOSTNAME, SSID, PASSWORD);

  InitMotorMap();

  size_t num_motors = (NUM_DRIVERS * MOTORS_PER_DRIVER) + NUM_PINS;
  for (size_t i = 0; i < num_motors; i++)
  {
    // pinMode(PWM_PINS[i], OUTPUT);
    WriteToMotor(i, 128);
  }

}

void loop()
{
  bool updated_packet = LoopSocket();

  // if we didn't recieve a packet this loop,
  // check if haptic strength needs to be lowered
  if (!updated_packet)
  {
    CheckStrAttenuation();
  }

#ifdef ESP8266
  //yield(); // take a breather, required for ESP8266
#endif
}
# Haptic Firmware

## Aims

Provide high-quality full-body haptic feedback for Virtual Reality using low-cost hardware in a system simple enough for the hobbyist to assemble with minimal tooling. Our system should be flexible enough to keep up with many different hardware configurations in order to allow tinkerers to come up with a setup that caters to their needs.
## Hardware Overview

The firmware is primarily designed to run on an ESP8266. When it comes to generating haptic feedback, the firmware has the ability to use the Data Pins on the microcontroller to send PWM signals to vibration motors. Since there are a very limited amount of data pins, we augment them with i2c controlled servo drivers: [Adafruit PCA9684](https://learn.adafruit.com/16-channel-pwm-servo-driver/chaining-drivers). 

```
┌────────────┐                   
│Firmware    │                   
└┬──────────┬┘                   
┌▽────────┐┌▽───────┐            
│Data Pins││I2C Bus │            
└┬────────┘└───────┬┘            
┌▽───────────────┐┌▽────────────┐
│Vibration Motors││Servo Drivers│
└────────────────┘└┬────────────┘
┌──────────────────▽──┐          
│More Vibration Motors│          
└─────────────────────┘          
```

## Server

Haptic signals are received by a server running on the user's PC.  This server listens to data from games (eg. Colliders connected to VRChat's OSC), and provides the firmware with the correct motors to trigger. The server also stores user configuration that would be impractical to flash to a microcontroller each time it is changed. There is a prototype implementation of a server available at [VR518B/Python-Haptic-Server](https://github.com/VR518B/Python-Haptic-Server).

## Contributing

 Pull requests are welcome, but it would be appreciated if you were to open an issue first for bigger changes.

## License

[GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html)

#ifndef CONFIG_H
#define CONFIG_H

// Network Information
const char SSID[] = "VR-Gehirnwaescheeinheit24";
const char PASSWORD[] = "LidBd21J";
// ESP-L-Arm / ESP-F-Body / ESP-R-Body / ESP-R-Arm / ESP-Head / ESP-L-Leg / ESP-R-Leg / ESP-Extra
const char WIFI_HOSTNAME[] = "ESP-L-Arm";
const int WEB_SOCKET_PORT = 8080;

// Motor Configuration
const uint FALLOFF_REFRESH_RATE = 10; // how often, in HZ, should we update the falloff of haptic sensors. 

const int PWM_PIN_MAPPING[] = {12,13,14,15};
const size_t NUM_MOTORS = sizeof(PWM_PIN_MAPPING) / sizeof(PWM_PIN_MAPPING[0]);
#endif
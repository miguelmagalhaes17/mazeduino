#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM303.h> // Accelerometer utils
#include <Adafruit_GFX.h> // Auxilary Library for LCD
#include <Adafruit_PCD8544.h> // LCD utils
#include <PCP.hpp>
#include <Utils.hpp>

// Pin definitions
// Accelerometer Pins
#define ACCEL1_SDA_PIN 12
#define ACCEL1_SCL_PIN 13
#define ACCEL2_SDA_PIN 14
#define ACCEL2_SCL_PIN 15

// Button Pins
#define SELBUTTON_PIN 16
#define CYCLEBUTTON_PIN 17

// LCD Pins
#define LCD1_CLK_PIN 11
#define LCD1_DIN_PIN 10
#define LCD1_DC_PIN 9
#define LCD1_CS_PIN 8
#define LCD1_RST_PIN 7
#define LCD2_CLK_PIN 6
#define LCD2_DIN_PIN 5
#define LCD2_DC_PIN 4
#define LCD2_CS_PIN 3
#define LCD2_RST_PIN 2

// Accelerometer objects
extern Adafruit_LSM303 accel1;
extern Adafruit_LSM303 accel2;

// LCD objects
extern Adafruit_PCD8544 lcd1;										
extern Adafruit_PCD8544 lcd2;	

// Button related variables
extern bool readSelectButtonState;
extern bool readCycleButtonState;
extern bool selectButtonState;
extern bool cycleButtonState;
extern bool prevSelectButtonState;
extern bool prevCycleButtonState;

// Task declaration                                         
void TaskReadButtons(void*);
void TaskReadAccel1(void*);
void TaskReadAccel2(void*);
void TaskDisplayLCD(void*);
void TaskGameLogic(void*);
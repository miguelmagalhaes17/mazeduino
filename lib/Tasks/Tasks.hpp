#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM303_Accel.h> // Accelerometer utils
#include <Adafruit_GFX.h> // Auxilary Library for LCD
#include <Adafruit_PCD8544.h> // LCD utils
#include <PCP.hpp>
#include <Utils.hpp>
#include <game.hpp>
#include <Accel_Utils.hpp>

// Pin definitions
// Accelerometer Pins
#define ACCEL1_SDA_PIN 12 // cabo azul
#define ACCEL1_SCL_PIN 13 // cabo laranja
#define ACCEL2_SDA_PIN 14 // cabo azul
#define ACCEL2_SCL_PIN 15 // cabo laranja

// Button Pins
#define SELBUTTON_PIN 16 // cabo laranja
#define CYCLEBUTTON_PIN 17 // cabo azul

// LCD Pins
#define LCD1_CLK_PIN 11 // cabo branco
#define LCD1_DIN_PIN 10 // cabo verde
#define LCD1_DC_PIN 9   // cabo azul
#define LCD1_CS_PIN 8   // cabo castanho
#define LCD1_RST_PIN 7  // cabo laranja
#define LCD2_CLK_PIN 6  // cabo branco
#define LCD2_DIN_PIN 5  // cabo verde
#define LCD2_DC_PIN 4   // cabo azul
#define LCD2_CS_PIN 3   // cabo castanho
#define LCD2_RST_PIN 2  // cabo laranja

// Accelerometer objects
//extern Adafruit_LSM303_Accel_Unified accel1;
//extern Adafruit_LSM303_Accel_Unified accel2;

// Accel events
//extern sensors_event_t eventAccel1;
//extern sensors_event_t eventAccel2;

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

extern TaskTiming ttButtons, ttAccel1, ttAccel2, ttLCD, ttGamePhysics, ttGameLogic;

// Task declaration                                         
void TaskReadButtons(void*);
void TaskReadAccel1(void*);
void TaskReadAccel2(void*);
void TaskUpdateGamePhysics(void*);
void TaskGameLogic(void*);
void TaskRenderLCD1(void*);
void TaskRenderLCD2(void*);
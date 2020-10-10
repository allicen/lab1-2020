#include <Arduino.h>
#include <MD_TCS230.h>
#include "pitches.h"
#include "button.h"
#include "buzzer.h"

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5

#define R_OUT 6
#define G_OUT 7
#define B_OUT 8

// назначить кнопки пинам
Button buttonR(S0_OUT);
Button buttonG(S1_OUT);
Button buttonB(S2_OUT);


MD_TCS230 colorSensor(S2_OUT, S3_OUT, S0_OUT, S1_OUT);

//Компоненты: макетная плата + 3 кнопки + RGB светодиод.
//Описание: через Serial приходят компоненты цвета для светодиода. Каждая кнопка включает/выключает одну из компонент цвета.

void setup()
{
    Serial.begin(115200);
    Serial.println("Started!");

    sensorData whiteCalibration;
    whiteCalibration.value[TCS230_RGB_R] = 83440;
    whiteCalibration.value[TCS230_RGB_G] = 78370;
    whiteCalibration.value[TCS230_RGB_B] = 271630;

    sensorData blackCalibration;
    blackCalibration.value[TCS230_RGB_R] = 10820;
    blackCalibration.value[TCS230_RGB_G] = 9750;
    blackCalibration.value[TCS230_RGB_B] = 35910;

    colorSensor.begin();
    colorSensor.setDarkCal(&blackCalibration);
    colorSensor.setWhiteCal(&whiteCalibration);

    pinMode(R_OUT, OUTPUT);
    pinMode(G_OUT, OUTPUT);
    pinMode(B_OUT, OUTPUT);
}

void loop() 
{
    colorData rgb;
    colorSensor.read();

    while (!colorSensor.available());

    colorSensor.getRGB(&rgb);
    print_rgb(rgb);
    set_rgb_led(rgb);

    if (buttonR.wasPressed()) {
      
    }
    if (buttonG.wasPressed()) {
      
    }
    if (buttonB.wasPressed()) {
      
    }
}

void print_rgb(colorData rgb)
{
  Serial.print(rgb.value[TCS230_RGB_R]);
  Serial.print(" ");
  Serial.print(rgb.value[TCS230_RGB_G]);
  Serial.print(" ");
  Serial.print(rgb.value[TCS230_RGB_B]);
  Serial.println();
}

void set_rgb_led(colorData rgb)
{
    analogWrite(R_OUT, 255 - rgb.value[TCS230_RGB_R]);
    analogWrite(G_OUT, 255 - rgb.value[TCS230_RGB_G]);
    analogWrite(B_OUT, 255 - rgb.value[TCS230_RGB_B]);
}

#include <Arduino.h>
#include <MD_TCS230.h>
#include "pitches.h"
#include "button.h"

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

bool redPush = true, bluePush = true, greenPush = true;
int redValue = 0, greenValue = 0, blueValue = 0; 

//Компоненты: макетная плата + 3 кнопки + RGB светодиод.
//Описание: через Serial приходят компоненты цвета для светодиода. Каждая кнопка включает/выключает одну из компонент цвета.

void setup()
{
    Serial.begin(9600);
    Serial.println("Started!");
    Serial.println("Введите значения RGB через пробел (три числа от 0 до 255): ");

    pinMode(R_OUT, OUTPUT);
    pinMode(G_OUT, OUTPUT);
    pinMode(B_OUT, OUTPUT);
}

void loop() 
{
    
    while(true) {
       String command = readUserInput(); // получение строки
            
       if (command.length() > 0) {
          bool success = true; 
          int colors[3];

          // строку в массив символов
          int command_len = command.length() + 1; 
          char command_char_array[command_len];
          command.toCharArray(command_char_array, command_len);
          
          char *inputNumber;
          char *comand_remainder = command_char_array;
          int indexNumber = 0; // индекс текущего элемента
          while ((inputNumber = strtok_r(comand_remainder, " ", &comand_remainder)) != NULL) {
            
             if (indexNumber > 2) {
                Serial.println("Было введено больше 3-х чисел!"); 
                success = false; 
                break; 
             }

             int number = atoi (inputNumber); // массив символов в int      

             if (number < 0 || number > 255) {
                Serial.println("Число должно быть в пределах 0-255"); 
                success = false; 
                break; 
             }

             colors[indexNumber] = number; 
             indexNumber++; 
           }

          if (success) {
              char colorHide[10];

              redValue = colors[0]; 
              greenValue = colors[1]; 
              blueValue = colors[2]; 

              if (buttonR.wasPressed()) {
                strcpy(colorHide, "green");
              }
              if (buttonG.wasPressed()) {
                strcpy(colorHide, "red");
              }
              if (buttonB.wasPressed()) {
                strcpy(colorHide, "blue");
              }
              
              setColorPushButton(colorHide);
          } else {
              Serial.println("Введена строка не по формату. Нужно ввесли 3 числа от 0 до 255 через пробел.");
          }
       }
    }
}


// Распечатать цифры цвета 
void printRgb()
{
  Serial.print("Вы ввели: ");
  Serial.print(redValue);
  Serial.print(" ");
  Serial.print(greenValue);
  Serial.print(" ");
  Serial.print(blueValue);
  Serial.println();
}


// Включить/выключить цвет по нажатию кнопки
void setColorPushButton(char* colorHide)
{
    if (colorHide == "green" && greenPush) {
        greenPush = false;
        greenValue = 0; 
    } 

    if (colorHide == "green" && !greenPush) {
        greenPush = true;
    } 

    if (colorHide == "blue" && bluePush) {
        bluePush = false; 
        blueValue = 0; 
    } 

    if (colorHide == "blue" && !bluePush) {
        bluePush = true; 
    } 

    if (colorHide == "red" && redPush) {
        redPush = false; 
        redValue = 0; 
    } 

    if (colorHide == "red" && !redPush) {
        redPush = true; 
    }

    analogWrite(R_OUT, 255 - redValue);
    analogWrite(G_OUT, 255 - greenValue);
    analogWrite(B_OUT, 255 - blueValue);

    printRgb(); 
}


// Строка, которую ввел пользователь
String readUserInput() {
    while (!Serial.available()); // дожидаемся команды пользователя (нажатие Enter)
    String command = Serial.readStringUntil('\n');
    return command;
}

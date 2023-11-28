/*!
 *  @file Morse.h
 *
 *  @mainpage  Morse Simulator Library.
 *
 *  @section intro_sec Introduction
 *
 *  This is a Library for Morse Code Simulation.
 *  For details on using this library see the guide at:
 *  https://itmaster.biz.ua/electronics/arduino.html
 *
 *  @section author Author
 *
 *  Written by Serhiy Matviyenko.
 *
 *  @section license License
 *
 *  MIT license, all text above must be included in any redistribution
 */
 
#ifndef Morse_h
#define Morse_h
 
#include "Arduino.h"
 
#define DEBUG                     0
#define LOW                       0
#define HIGH                      1
#define CYRILLIC                  0
#define ERROR                     -1
#define DOT_TIME_DEFAULT          250
#define DASH_TIME_DEFAULT         3*DOT_TIME_DEFAULT
#define SPACE_TIME_DEFAULT        DOT_TIME_DEFAULT
#define SPACE_LETTER_TIME_DEFAULT 3*DOT_TIME_DEFAULT
#define SPACE_WORD_TIME_DEFAULT   7*DOT_TIME_DEFAULT

#define MIN_VELOCITY_MINUTES      5   // letters per minute
#define MAX_VELOCITY_MINUTES      300 // letters per minute
#define DEFAULT_VELOCITY_MINUTES  40  // letters per minute
#define MIN_DURATION_MSEC_DIV     3000
 
class Morse
{
  public:
    Morse(int pin, int polarity = LOW);
    int setVelocity(int velocity);
    void dot();
    void dash();
    int sendSymbolToPin(char* letter);
    int sendWordToPin(char* letter, int lenght);
  private:
    int _pin, _polarity;
    int _dotDuration;
    int _dashDuration;
    int _spaceDuration;
    int _spaceLetterDuration;
    int _spaceWordDuration; 

    int getCyrillicSymbolIndex(char* letter);
    int getLatinSymbolIndex(char* letter);
    int getSymbolIndex(char* letter);
};
 
#endif
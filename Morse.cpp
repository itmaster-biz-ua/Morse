/*!
 *  @file Morse.cpp
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
#include "Morse.h"
  
#define SYMBOL_COUNT_MAX          57
  
// Morse code table. 
// The nth element of the code corresponds to the nth symbol of the layout.
char* code[] = {
  "*-","-***","*--","--*","-**","*","***-","--**","**","*---",
  "-*-","*-**","--","-*","---","*--*","*-*","***","-","**-",
  "**-*","****","-*-*","---*","----","--*-","-*--","-**-","**-**","**--",
  "*-*-",
  "*----","**---","***--","****-","*****","-****","--***","---**","----*","-----",
  "......","*-*-*-","---***","-*-*-","-*--*-","*----*","*-**-*","-****-","-**-*","**--**","--**--",
  "-***-","********","*--*-*","**-*-",
  ""
};

// Cyrillic layout
char* layoutCyrillic[] = {
  'а','б','в','г','д','е','ж','з','и','й',
  'к','л','м','н','о','п','р','с','т','у',
  'ф','х','ц','ч','ш','щ','ы','ь','э','ю',
  'я',
  '1','2','3','4','5','6','7','8','9','0',
  '.',',',':',';','(','\'','\'','-','/','?','!',
  ' *DELIMITER* ',' *ERR* ','@',' *END* '
 };
// Латинська розкладка.
char* layoutLatin[] = {
  'a','b','w','g','d','e','v','z','i','j',
  'k','l','m','n','o','p','r','s','t','u',
  'f','h','c','ö','ch','q','y','x','é','ü',
  'ä',
  '1','2','3','4','5','6','7','8','9','0',
  '.',',',':',';','(','\'','\'','-','/','?','!',
  ' *DELIMITER* ',' *ERR* ','@',' *END* '
};

 
/*!
 *  @brief  Instantiates a new Morse class
 *  @param  pin
 *          pin number that the buzzer is connected
 *  @param  polarity (optional)
 *          the buzzer input polarity
 *          LOW - the buzzer active high signal
 *          HIGH - the buzzer active low signal
 */
Morse::Morse(int pin, int polarity = LOW)
{
  // initialize the digital pin as an output
  pinMode(pin, OUTPUT);
  if ((polarity != LOW) && (polarity != HIGH)) polarity = HIGH; // set when wrong
  digitalWrite(pin, polarity);

  _pin = pin;
  _polarity = polarity;
  _dotDuration = DOT_TIME_DEFAULT;
  _dashDuration = DASH_TIME_DEFAULT;
  _spaceDuration = SPACE_TIME_DEFAULT;
  _spaceLetterDuration = SPACE_LETTER_TIME_DEFAULT;
  _spaceWordDuration = SPACE_WORD_TIME_DEFAULT;
}

/*!
 *  @brief  Sets velocity 
 *  @param  velocity
 *          the velocity in letters per minute
 *  @return 0 if there is no error and -1 if there is error
 */
int Morse::setVelocity(int velocity) {
  int result = 0;

  if ((velocity < MIN_VELOCITY_MINUTES) || (velocity > MAX_VELOCITY_MINUTES)) {
    result = ERROR;
    return result;
  }

  // Set durations in ms
  _dotDuration = MIN_DURATION_MSEC_DIV/velocity;
  _dashDuration = 3 * _dotDuration;
  _spaceDuration = _dotDuration;
  _spaceLetterDuration = 3 * _dotDuration;
  _spaceWordDuration = 7 * _dotDuration;
  
  return result;
}

/*!
 *  @brief  Sends a dot on a pin
 */
void Morse::dot()
{
  digitalWrite(_pin, HIGH^_polarity);
  delay(_dotDuration);
  digitalWrite(_pin, LOW^_polarity);
  delay(_spaceDuration);  
}

/*!
 *  @brief  Sends a dash on a pin
 */
void Morse::dash()
{
  digitalWrite(_pin, HIGH^_polarity);
  delay(_dashDuration);
  digitalWrite(_pin, LOW^_polarity);
  delay(_spaceDuration);
}

/*!
 *  @brief  Gets the Cyrillic symbol index
 *  @param  letter
 *          the letter
 *  @return the symbol index
 */
int Morse::getCyrillicSymbolIndex(char* letter) {
  int i;
  int result = ERROR;

  for (i = 0; (i < SYMBOL_COUNT_MAX); i++) {
    if (layoutCyrillic[i] == letter) {
      result = i;
      break;
    }
  }

  return result;
}

/*!
 *  @brief  Gets the Latin symbol index
 *  @param  letter
 *          the letter
 *  @return the symbol index
 */
int Morse::getLatinSymbolIndex(char* letter) {
  int i;
  int result = ERROR;

  for (i = 0; (i < SYMBOL_COUNT_MAX); i++) {
    if ((char)layoutLatin[i] == letter[0]) {
      result = i;
      break;
    }
  }

  return result;
}

/*!
 *  @brief  Gets the symbol index
 *  @param  letter
 *          the letter
 *  @return the symbol index or -1 in case of an error
 */
int Morse::getSymbolIndex(char* letter) {
  int index = Morse::getLatinSymbolIndex(letter);
  #if CYRILLIC
    if (index == ERROR) {
        index = Morse::getCyrillicSymbolIndex(letter);
        if (index == ERROR) {
          #if DEBUG
          Serial.print(" getCyrillicSymbolIndex error \n");
          #endif
        }
    }
  #endif
  
  return index;
}

/*!
 *  @brief  Sends a symbol to a pin
 *  @param  letter
 *          the letter
 *  @return 0 if there is no error and -1 if there is error
 */
int Morse::sendSymbolToPin(char* letter) {
  int status = 0;
  int index;
  char* symbol;

  index = Morse::getSymbolIndex(letter);
  if (ERROR == index) {
    #if DEBUG  
    Serial.print(" getSymbolIndex error \n");
    #endif
    return status;
  }

  symbol = code[index];

  for (int i = 0; i < strlen(symbol); i++) {
    if (symbol[i] == '*') {
      dot();
      #if DEBUG
      Serial.print(".");
      #endif
    }
    else if (symbol[i] == '-') {
      dash();
      #if DEBUG
      Serial.print("-");
      #endif
    }
    else {
      status = ERROR;
      #if DEBUG
      Serial.print(" Decoding error \n");
      #endif
      return status;
    }
  }
  
  #if DEBUG
  Serial.print("\n");
  #endif
  delay(_spaceLetterDuration);

  return status;
}

/*!
 *  @brief  Sends a word to a pin
 *  @param  letter
 *          the letter
 *  @return 0 if there is no error and -1 if there is error
 */
int Morse::sendWordToPin(char* letter, int lenght) {
  int status = 0;

  for (int i = 0; i < lenght; i++) {
    status = Morse::sendSymbolToPin(&letter[i]);
    if (ERROR == status) {
      #if DEBUG 
      Serial.print(" sendSymbolToPin error \n");
      #endif
      break;
    }
  }

  delay(_spaceWordDuration);

  return status;
}

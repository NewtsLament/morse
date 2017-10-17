// Turns Morse key into USB keyboard

// Macro stuff detecting chip.
#if defined(__AVR_ATmega328P__)
#define __SERIAL__ // Assume UNO or similar
#else
#include <Keyboard.h>
#endif

#include <Bounce2.h> // include de-bounce library

const int led = 13; // led is connected to pin 13
const int audioOut = 12;
const int audioSelect = 11;
const int keyPin = 2;  // morse key is connected to pin 7
Bounce morseKey = Bounce();

const unsigned long dot = 100;
const unsigned long dashThresh = 3 * dot; // time threshold in ms to differentiate dots from dashes
const unsigned long letterThresh = dashThresh; // time threshold in ms to differentiate letter gaps
const unsigned long wordThresh = 7 * dot; // time threshold in ms to differentiate word gaps

String inputString = ""; // initialise input string
String outString = "";

unsigned long downTime = 0; // records the start time of state change
unsigned long upTime = 0; // records the end time of state change
unsigned long timeNow = 0; // records the current time
unsigned long changeDuration = 0; // records the duration of state change
unsigned long pauseDuration = 0; // records the duration of the last pause

int pauseFlag = 0; // initilise the flag to indicate whether a pause has already been evaluated
int lastLetterSpace = 0;


void setup()
{
  pinMode(led, OUTPUT); // configure the pin connected to the led as an output
  pinMode(audioOut, OUTPUT);
  pinMode(audioSelect, INPUT_PULLUP);
  pinMode(keyPin, INPUT_PULLUP); // configure the pin connected to the morse key as a pullup
  morseKey.attach(keyPin);
  morseKey.interval(10); // 10 ms debounce
#ifndef __SERIAL__
  Keyboard.begin();
#endif
  Serial.begin(57600);
  while (!Serial)
  {
    ;
  }

} // end of setup

void loop()
{
  checkPause();
  // start of IF loop
  if (morseKey.update()) {
    if (morseKey.risingEdge()) { // if input from key has gone to 1 and model is still 0, update model
      noTone(audioOut);
      keyUp();
    } else if (morseKey.fallingEdge()) { // if input from key has gone to 0 and model is still 1, update model
      if (digitalRead(audioSelect) == 0)
        tone(audioOut, 700);
      keyDown();
    }
  } // end of if update loop

} // end of loop

void keyDown()
{
  downTime = millis();
  digitalWrite(led, HIGH); // switch LED on

  pauseFlag = 0;
}

void keyUp()
{
  upTime = millis();
  changeDuration = upTime - downTime;
  digitalWrite(led, LOW); // switch LED off

  if (changeDuration > 0 and changeDuration < dashThresh) {
    inputString = inputString + ".";
#ifndef __SERIAL__
    Serial.println("DOT");
#endif
  } else if (changeDuration >= dashThresh) {
    inputString = inputString + "-";
#ifndef __SERIAL__
    Serial.println("DASH");
#endif
  }

  pauseFlag = 1;

}

void checkPause()
{
  if (pauseFlag)
  {
    timeNow = millis();
    pauseDuration = timeNow - upTime;

    if (pauseDuration >= letterThresh and pauseDuration < wordThresh and pauseFlag == 1)
    { // if the preceding pause was long enough AND a pause hasn't just been evaluated, evaluate the previous inputs as a single letter.
      evaluateLetter();
      if (lastLetterSpace)
      {
        pauseFlag = 0;
      } else
      {
        pauseFlag = 2;
      }
    } else if (pauseDuration >= wordThresh) {
      evaluateLetter();
      newWord();
      pauseFlag = 0;
    }
  }
}

void newWord()
{
#ifdef __SERIAL__
  Serial.print(" ");
#else
  Keyboard.press(' ');
#endif
  lastLetterSpace = 1;
}

void evaluateLetter()
{
  // Letters a-z
  if (inputString == ".-") {
    outString = "a";
  } else if (inputString == "-...") {
    outString = "b";
  } else if (inputString == "-.-.") {
    outString = "c";
  } else if (inputString == "-..") {
    outString = "d";
  } else if (inputString == ".") {
    outString = "e";
  } else if (inputString == "..-.") {
    outString = "f";
  } else if (inputString == "--.") {
    outString = "g";
  } else if (inputString == "....") {
    outString = "h";
  } else if (inputString == "..") {
    outString = "i";
  } else if (inputString == ".---") {
    outString = "j";
  } else if (inputString == "-.-") {
    outString = "k";
  } else if (inputString == ".-..") {
    outString = "l";
  } else if (inputString == "--") {
    outString = "m";
  } else if (inputString == "-.") {
    outString = "n";
  } else if (inputString == "---") {
    outString = "o";
  } else if (inputString == ".--.") {
    outString = "p";
  } else if (inputString == "--.-") {
    outString = "q";
  } else if (inputString == ".-.") {
    outString = "r";
  } else if (inputString == "...") {
    outString = "s";
  } else if (inputString == "-") {
    outString = "t";
  } else if (inputString == "..-") {
    outString = "u";
  } else if (inputString == "...-") {
    outString = "v";
  } else if (inputString == ".--") {
    outString = "w";
  } else if (inputString == "-..-") {
    outString = "x";
  } else if (inputString == "-.--") {
    outString = "y";
  } else if (inputString == "--..") {
    outString = "z";
  } else if (inputString == ".-.-") {
    // Letters æøå
    outString = "æ";
  } else if (inputString == "---.") {
    outString = "ø";
  } else if (inputString == ".--.-") {
    outString = "å";
    // Numbers
  } else if (inputString == ".----") {
    outString = "1";
  } else if (inputString == "..---") {
    outString = "2";
  } else if (inputString == "...--") {
    outString = "3";
  } else if (inputString == "....-") {
    outString = "4";
  } else if (inputString == ".....") {
    outString = "5";
  } else if (inputString == "-....") {
    outString = "6";
  } else if (inputString == "--...") {
    outString = "7";
  } else if (inputString == "---..") {
    outString = "8";
  } else if (inputString == "----.") {
    outString = "9";
  } else if (inputString == "-----") {
    outString = "0";
    // Special
  } else if (inputString == "...-.-") {
    outString = "\n";
  } else if (inputString == ".-.-.-") {
    outString = ".";
  } else if (inputString == "--..--") {
    outString = ",";
  } else if (inputString == "..--..") {
    outString = "?";
  } else if (inputString == ".----.") {
    outString = "'";
  } else if (inputString == "-.-.--") {
    outString = "!";
  } else if (inputString == "-..-.") {
    outString = "/";
  } else if (inputString == "-.--.") {
    outString = "(";
  } else if (inputString == "-.--.-") {
    outString = ")";
  } else if (inputString == ".-...") {
    outString = "&";
  } else if (inputString == "---...") {
    outString = ":";
  } else if (inputString == "-.-.-.") {
    outString = ";";
  } else if (inputString == "-...-") {
    outString = "=";
  } else if (inputString == ".-.-.") {
    outString = "+";
  } else if (inputString == "-....-") {
    outString = "-";
  } else if (inputString == "..--.-") {
    outString = "_";
  } else if (inputString == ".-..-.") {
    outString = "\"";
  } else if (inputString == "...-..-") {
    outString = "$";
  } else if (inputString == ".--.-.") {
    outString = "@";
  } else 
  {
    outString = "";
  }

  if (outString != "")
  {
    #ifdef __SERIAL__
    Serial.print(outString);
    #else
    Keyboard.press(outString);
    #endif
  }

  if (outString != "")
  {
    lastLetterSpace = 0;
  }
  
  inputString = ""; // re-initialise inputString ready for new letter
}

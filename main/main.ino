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
const unsigned long dashThresh = 3*dot; // time threshold in ms to differentiate dots from dashes
const unsigned long letterThresh = dashThresh; // time threshold in ms to differentiate letter gaps
const unsigned long wordThresh = 7*dot; // time threshold in ms to differentiate word gaps

String inputString = ""; // initialise input string

unsigned long downTime = 0; // records the start time of state change
unsigned long upTime = 0; // records the end time of state change
unsigned long timeNow = 0; // records the current time
unsigned long changeDuration = 0; // records the duration of state change
unsigned long pauseDuration = 0; // records the duration of the last pause

int pauseFlag = 0; // initilise the flag to indicate whether a pause has already been evaluated
int emptyLetter = 0;

void setup()
{
  pinMode(led, OUTPUT); // configure the pin connected to the led as an output
  pinMode(audioOut, OUTPUT);
  pinMode(audioSelect, INPUT_PULLUP);
  pinMode(keyPin, INPUT_PULLUP); // configure the pin connected to the morse key as a pullup
  morseKey.attach(keyPin);
  morseKey.interval(10); // 10 ms debounce
  #ifdef __SERIAL__
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
  if (morseKey.update()){
    if (morseKey.risingEdge()) { // if input from key has gone to 1 and model is still 0, update model
      noTone(audioOut);
      keyUp();
    } else if (morseKey.fallingEdge()) { // if input from key has gone to 0 and model is still 1, update model
      if (digitalRead(audioSelect) == 0)
        tone(audioOut,700);
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
    changeDuration = upTime-downTime;
    digitalWrite(led, LOW); // switch LED off

    if (changeDuration>0 and changeDuration<dashThresh) {
      inputString = inputString + ".";
      #ifndef __SERIAL__
      Serial.println("DOT");
      #endif
    } else if (changeDuration>=dashThresh) {
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
    pauseDuration = timeNow-upTime;

    if (pauseDuration>=letterThresh and pauseDuration<wordThresh and pauseFlag == 1)
    { // if the preceding pause was long enough AND a pause hasn't just been evaluated, evaluate the previous inputs as a single letter.
      evaluateLetter();
      pauseFlag = 2;
    } else if (pauseDuration >= wordThresh) {
      evaluateLetter();
      newWord();
      pauseFlag = 0; 
    }
  }
}

void newWord()
{
  if (!emptyLetter)
  {
    #ifdef __SERIAL__
    Serial.print(" ");
    #else
    Keyboard.press(' ');
    #endif
  }
}

void evaluateLetter()
{
  emptyLetter = 0;
  #ifdef __SERIAL__
  // Letters a-z
  if (inputString==".-") {
    Serial.print("a");
  } else if (inputString=="-..."){
    Serial.print("b");
  } else if (inputString == "-.-."){
    Serial.print("c");
  } else if (inputString=="-.."){
    Serial.print("d");
  } else if (inputString=="."){
    Serial.print("e");
  } else if (inputString=="..-."){
    Serial.print("f");
  } else if (inputString=="--."){
    Serial.print("g");
  } else if (inputString=="...."){
    Serial.print("h");
  } else if (inputString==".."){
    Serial.print("i");
  } else if (inputString==".---"){
    Serial.print("j");
  } else if (inputString=="-.-"){
    Serial.print("k");
  } else if (inputString==".-.."){
    Serial.print("l");
  } else if (inputString=="--"){
    Serial.print("m");
  } else if (inputString=="-."){
    Serial.print("n");
  } else if (inputString=="---"){
    Serial.print("o");
  } else if (inputString==".--."){
    Serial.print("p");
  } else if (inputString=="--.-"){
    Serial.print("q");
  } else if (inputString==".-."){
    Serial.print("r");
  } else if (inputString=="..."){
    Serial.print("s");
  } else if (inputString=="-"){
    Serial.print("t");
  } else if (inputString=="..-"){
    Serial.print("u");
  } else if (inputString=="...-"){
    Serial.print("v");
  } else if (inputString==".--"){
    Serial.print("w");
  } else if (inputString=="-..-"){
    Serial.print("x");
  } else if (inputString=="-.--"){
    Serial.print("y");
  } else if (inputString=="--.."){
    Serial.print("z");
  } else if (inputString==".-.-"){
    // Letters æøå
    Serial.print("æ");
  } else if (inputString=="---."){
    Serial.print("ø");
  } else if (inputString==".--.-"){
    Serial.print("å");
    // Numbers
  } else if (inputString==".----"){
    Serial.print("1");
  } else if (inputString=="..---"){
    Serial.print("2");
  } else if (inputString=="...--"){
    Serial.print("3");
  } else if (inputString=="....-"){
    Serial.print("4");
  } else if (inputString=="....."){
    Serial.print("5");
  } else if (inputString=="-...."){
    Serial.print("6");
  } else if (inputString=="--..."){
    Serial.print("7");
  } else if (inputString=="---.."){
    Serial.print("8");
  } else if (inputString=="----."){
    Serial.print("9");
  } else if (inputString=="-----"){
    Serial.print("0");
    // Special
  } else if (inputString=="...-.-"){
    Serial.print("\n");
  } else if (inputString==".-.-.-"){
    Serial.print(".");
  } else if (inputString=="--..--"){
    Serial.print(",");
  } else if (inputString=="..--.."){
    Serial.print("?");
  } else if (inputString==".----."){
    Serial.print("'");
  } else if (inputString=="-.-.--"){
    Serial.print("!");
  } else if (inputString=="-..-."){
    Serial.print("/");
  } else if (inputString=="-.--."){
    Serial.print("(");
  } else if (inputString=="-.--.-"){
    Serial.print(")");
  } else if (inputString==".-..."){
    Serial.print("&");
  } else if (inputString=="---..."){
    Serial.print(":");
  } else if (inputString=="-.-.-."){
    Serial.print(";");
  } else if (inputString=="-...-"){
    Serial.print("=");
  } else if (inputString==".-.-."){
    Serial.print("+");
  } else if (inputString=="-....-"){
    Serial.print("-");
  } else if (inputString=="..--.-"){
    Serial.print("_");
  } else if (inputString==".-..-."){
    Serial.print("\"");
  } else if (inputString=="...-..-"){
    Serial.print("$");
  } else if (inputString==".--.-."){
    Serial.print("@");
  } else {
      emptyLetter = 1;
  }
  #else
  // Letters a-z
  if (inputString==".-") {
    Keyboard.write('a');
  } else if (inputString=="-..."){
    Keyboard.write('b');
  } else if (inputString == "-.-."){
    Keyboard.write('c');
  } else if (inputString=="-.."){
    Keyboard.write('d');
  } else if (inputString=="."){
    Keyboard.write('e');
  } else if (inputString=="..-."){
    Keyboard.write('f');
  } else if (inputString=="--."){
    Keyboard.write('g');
  } else if (inputString=="...."){
    Keyboard.write('h');
  } else if (inputString==".."){
    Keyboard.write('i');
  } else if (inputString==".---"){
    Keyboard.write('j');
  } else if (inputString=="-.-"){
    Keyboard.write('k');
  } else if (inputString==".-.."){
    Keyboard.write('l');
  } else if (inputString=="--"){
    Keyboard.write('m');
  } else if (inputString=="-."){
    Keyboard.write('n');
  } else if (inputString=="---"){
    Keyboard.write('o');
  } else if (inputString==".--."){
    Keyboard.write('p');
  } else if (inputString=="--.-"){
    Keyboard.write('q');
  } else if (inputString==".-."){
    Keyboard.write('r');
  } else if (inputString=="..."){
    Keyboard.write('s');
  } else if (inputString=="-"){
    Keyboard.write('t');
  } else if (inputString=="..-"){
    Keyboard.write('u');
  } else if (inputString=="...-"){
    Keyboard.write('v');
  } else if (inputString==".--"){
    Keyboard.write('w');
  } else if (inputString=="-..-"){
    Keyboard.write('x');
  } else if (inputString=="-.--"){
    Keyboard.write('y');
  } else if (inputString=="--.."){
    Keyboard.write('z');
    // Numbers
  } else if (inputString==".----"){
    Keyboard.write('1');
  } else if (inputString=="..---"){
    Keyboard.write('2');
  } else if (inputString=="...--"){
    Keyboard.write('3');
  } else if (inputString=="....-"){
    Keyboard.write('4');
  } else if (inputString=="....."){
    Keyboard.write('5');
  } else if (inputString=="-...."){
    Keyboard.write('6');
  } else if (inputString=="--..."){
    Keyboard.write('7');
  } else if (inputString=="---.."){
    Keyboard.write('8');
  } else if (inputString=="----."){
    Keyboard.write('9');
  } else if (inputString=="-----"){
    Keyboard.write('0');
    //Special
  } else if (inputString=="...-.-"){
    Keyboard.write('\n');
  } else if (inputString==".-.-.-"){
    Keyboard.write('.');
  } else if (inputString=="--..--"){
    Keyboard.write(',');
  } else if (inputString=="..--.."){
    Keyboard.write('?');
  } else if (inputString==".----."){
    Keyboard.write('\'');
  } else if (inputString=="-.-.--"){
    Keyboard.write('!');
  } else if (inputString=="-..-."){
    Keyboard.write('/');
  } else if (inputString=="-.--."){
    Keyboard.write('(');
  } else if (inputString=="-.--.-"){
    Keyboard.write(')');
  } else if (inputString==".-..."){
    Keyboard.write('&');
  } else if (inputString=="---..."){
    Keyboard.write(':');
  } else if (inputString=="-.-.-."){
    Keyboard.write(';');
  } else if (inputString=="-...-"){
    Keyboard.write('=');
  } else if (inputString==".-.-."){
    Keyboard.write('+');
  } else if (inputString=="-....-"){
    Keyboard.write('-');
  } else if (inputString=="..--.-"){
    Keyboard.write('_');
  } else if (inputString==".-..-."){
    Keyboard.write('"');
  } else if (inputString=="...-..-"){
    Keyboard.write('$');
  } else if (inputString==".--.-."){
    Keyboard.write('@');
  } else {
    emptyLetter = 1;
  }
  
  #endif
  inputString = ""; // re-initialise inputString ready for new letter
}

// Turns Morse key into USB keyboard

// Macro stuff detecting chip.
#if defined(__AVR_ATmega328P__)
#define __SERIAL__ // Assume UNO or similar
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

void setup()
{
  pinMode(led, OUTPUT); // configure the pin connected to the led as an output
  pinMode(audioPin, OUTPUT);
  pinMode(audioSelect, INPUT_PULLUP);
  pinMode(keyPin, INPUT_PULLUP); // configure the pin connected to the morse key as a pullup
  morseKey.attach(keyPin);
  morseKey.interval(10); // 10 ms debounce
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
      noTone(audioPin);
      keyUp();
    } else if (morseKey.fallingEdge()) { // if input from key has gone to 0 and model is still 1, update model
      if (digitalRead(audioSelect) == 0)
        tone(audioPin,700);
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
      #if !defined(__SERIAL__)
      Serial.println("DOT");
      #endif
    } else if (changeDuration>=dashThresh) {
      inputString = inputString + "-";
      #if !defined(__SERIAL__)
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
  #ifdef __SERIAL__
  Serial.print(" ");
  #else
  press(KEY_SPACE);
  #endif
}

#if !defined(__SERIAL__)
void press(char key){
  Keyboard.press(key);
  Keyboard.release(key);
}
#endif

void evaluateLetter()
{
  #ifdef __SERIAL__
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
      Serial.print("æ");
  } else if (inputString=="---."){
      Serial.print("ø");
  } else if (inputString==".--.-"){
      Serial.print("å");
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
  } else if (inputString=="...-.-"){
      Serial.print("\n");
  } /*else {
      Serial.print("");
  }*/
  #else
  switch (inputString) {
    case ".-":
      press(KEY_A);
    case "-...":
        press(KEY_B);
    case "-.-.":
        press(KEY_C);
    case "-..":
        press(KEY_D);
    case ".":
        press(KEY_E);
    case "..-.":
        press(KEY_F);
    case "--.":
        press(KEY_G);
    case "....":
        press(KEY_H);
    case "..":
        press(KEY_I);
    case ".---":
        press(KEY_J);
    case "-.-":
        press(KEY_K);
    case ".-..":
        press(KEY_L);
    case "--":
        press(KEY_M);
    case "-.":
        press(KEY_N);
    case "---":
        press(KEY_O);
    case ".--.":
        press(KEY_P);
    case "--.-":
        press(KEY_Q);
    case ".-.":
        press(KEY_R);
    case "...":
        press(KEY_S);
    case "-":
        press(KEY_T);
    case "..-":
        press(KEY_U);
    case "...-":
        press(KEY_V);
    case ".--":
        press(KEY_W);
    case "-..-":
        press(KEY_X);
    case "-.--":
        press(KEY_Y);
    case "--..":
        press(KEY_Z);
    case ".----":
        press(KEY_1);
    case "..---":
        press(KEY_2);
    case "...--":
        press(KEY_3);
    case "....-":
        press(KEY_4);
    case ".....":
        press(KEY_5);
    case "-....":
        press(KEY_6);
    case "--...":
        press(KEY_7);
    case "---..":
        press(KEY_8);
    case "----.":
        press(KEY_9);
    case "-----":
        press(KEY_0);
    default:
        press(KEY_MINUS);
  }
  #endif
  inputString = ""; // re-initialise inputString ready for new letter

}

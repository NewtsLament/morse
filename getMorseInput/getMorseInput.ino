// Turns Morse key into USB keyboard

#include <Bounce2.h> // include de-bounce library

const int led = 13; // led is connected to pin 13
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

      keyUp();
  
    } else if (morseKey.fallingEdge()) { // if input from key has gone to 0 and model is still 1, update model
  
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

    if (changeDuration>0 and changeDuration<dashThresh){
      inputString = inputString + ".";
      //Serial.println("DOT");

    } else if (changeDuration>=dashThresh) {
      inputString = inputString + "-";
      //Serial.println("DASH");

    }

    pauseFlag = 1;
    
}

void checkPause()
{   
    if (pauseFlag)
    {
      timeNow = millis();
      pauseDuration = timeNow-upTime;
  
      if (pauseDuration>=letterThresh and pauseDuration<wordThresh and pauseFlag == 1){ // if the preceding pause was long enough AND a pause hasn't just been evaluated, evaluate the previous inputs as a single letter
        //Serial.print(pauseDuration);
        //Serial.println();
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
  Serial.print(" ");
}

void evaluateLetter()
{

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
      Serial.println();
  } else {
      Serial.print("");
  }

  inputString = ""; // re-initialise inputString ready for new letter

}




### Morse Key USB Keyboard

#### What's this?

This repo contains code to convert a physical Morse key into a serial device, which translates correct Morse codes into their ASCII equivalent.

The keyboard currently supports lowercase letters of the alphabet from a-z and numbers 0-9, the Danish characters æ, ø and å and the end sign implemented as newline is also supported.

Unrecognised characters are not printed.

The dot timing is hard coded, and all other timings are dependent on this dot timing in the following way:
* A dash is 3 times the length of a dot.
* A letter is separated by a pause of the same length as a dash.
* A word is separated by a pause of 7 times the length of a dot.

The dot timing is currently hard coded in ms. This isn't hugely accurate since different operators will key at different speeds, but the hard coded timings can easily be tweaked. 

#### What do I need to replicate this?

An Arduino Nano or similar is sufficient, currently the pin used for the Morse key input is pin 2, but this is easily changed. The internal pull up of pin 2 is active, so the Morse key must connect pin 2 to ground when active.

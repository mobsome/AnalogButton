#include "AnalogButtons.hpp"

using namespace analog_button;

AnalogButtons buttons(A0);

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);

  buttons.add_button(std::shared_ptr<AnalogButton>(new AnalogButton(250, 400, [](){
      Serial.println("Clicked 1");
    }, [](){
      Serial.println("Long pressed 1");
    })));

  buttons.add_button(std::shared_ptr<AnalogButton>(new AnalogButton(450, 600, [](){
      Serial.println("Clicked 2");
    }, [](){
      Serial.println("Long pressed 2");
    })));
}

void loop() {
  buttons.check();
}

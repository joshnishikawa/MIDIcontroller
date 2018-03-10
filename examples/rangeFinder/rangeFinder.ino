/*
  TODO: needs to handle capsens, and auto-store min/max values
  This is just here for your convenience.
  You can read any analog sensor and have it
  printed to the serial monitor while you
  find the desired upper and lower limits
  to use in the 'inputRange()' function.
*/

void setup(){}

void loop(){
  int val = analogRead(23); //Change if needed
  Serial.println(val);
  delay(200);
}

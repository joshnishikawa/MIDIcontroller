/*This is just here for your convenience.
  You can read any analog sensor and have it
  printed to the serial monitor while you
  find the desired upper and lower limits
  to use in the 'inputRange()' function.
*/

void setup(){                
  Serial.begin(38400);
}
int val;
void loop(){
  val = analogRead(A4); //Change if needed
  Serial.println(val);
  delay(1000);
}

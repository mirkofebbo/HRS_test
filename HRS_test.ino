const int sensorPin = A0;

void setup() {
  Serial.begin(9600); 
  matrix.begin();
}

void loop() {

  int rawValue = analogRead(sensorPin);  
  Serial.println(rawValue);


  delay(100); 
}
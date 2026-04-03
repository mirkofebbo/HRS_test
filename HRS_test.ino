#include "Arduino_LED_Matrix.h"
#include "myQueue.h"

ArduinoLEDMatrix matrix;

// Heart Rate Sensor
const int sensorPin = A0;
// Led Matrix
uint8_t frame[8][12] = { 0 };

// millis instead of delay
unsigned long lastSampleTime = 0;

// Ciruclar queue or buffer of previous sensor data
myQueue pulseHistory(100);
const int sampleInterval = 20;  // 20ms = 50Hz sampling rate

void setup() {
  Serial.begin(9600);
  matrix.begin();
}
void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastSampleTime >= sampleInterval) {
    lastSampleTime = currentMillis;

    int rawValue = analogRead(sensorPin);

    // 1. Add newest data to history
    if (pulseHistory.isFull()) {
      pulseHistory.dequeue();
    }
    pulseHistory.enqueue(rawValue);

    // 2. SCAN THE QUEUE FOR REAL MIN/MAX
    int dynamicMin = 1023;
    int dynamicMax = 0;

    for (int i = 0; i < pulseHistory.getSize(); i++) {
      int val = pulseHistory.getAt(i);
      if (val < dynamicMin) dynamicMin = val;
      if (val > dynamicMax) dynamicMax = val;
    }

    // 3. Add a small "Padding" so the wave doesn't hit the absolute edges
    // This also prevents division by zero if min == max
    int displayMin = dynamicMin - 2;
    int displayMax = dynamicMax + 2;
    if (displayMax - displayMin < 10) displayMax = displayMin + 10;

    // 4. Map using the actual history bounds
    int mappedValue = map(rawValue, displayMin, displayMax, 0, 8);
    mappedValue = constrain(mappedValue, 0, 8);

    // 5. Update Frame (Scrolling logic)
    for (int col = 0; col < 11; col++) {
      for (int row = 0; row < 8; row++) {
        frame[row][col] = frame[row][col + 1];
      }
    }

    // 6. Draw Bar
    for (int row = 0; row < 8; row++) {
      frame[row][11] = (row >= (8 - mappedValue)) ? 1 : 0;
    }

    matrix.renderBitmap(frame, 8, 12);

    // Debugging
    Serial.print(displayMin); Serial.print(",");
    Serial.print(displayMax); Serial.print(",");
    Serial.println(rawValue);
  }
}
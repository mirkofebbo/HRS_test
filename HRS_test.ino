#include "Arduino_LED_Matrix.h"
#include "myQueue.h"

ArduinoLEDMatrix matrix;

// Heart Rate Sensor
const int sensorPin = A0;
// Led Matrix
uint8_t frame[8][12] = { 0 };
// Min Max for dynamic mapping
int localMin = 1023;
int localMax = 0;
static int globalMin = 855;
static int globalMax = 865;

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
  //Get raw data and map
  if (currentMillis - lastSampleTime >= sampleInterval) {
    lastSampleTime = currentMillis;

    int rawValue = analogRead(sensorPin);

    // 2. Manage the Queue with RAW values (for better scaling math)
    if (pulseHistory.isFull()) {
      pulseHistory.dequeue();
    }
    pulseHistory.enqueue(rawValue);

    // 3. True Dynamic Scaling Logic
    // We find the min/max of the LAST 100 samples (about 2 seconds of data)
    int dynamicMin = 1023;
    int dynamicMax = 0;

    // To do this efficiently without a 'peek' function, we use your
    // globalMin/Max drift logic, but we adjust the drift speed:
    if (rawValue < globalMin) globalMin = rawValue;
    if (rawValue > globalMax) globalMax = rawValue;

    // Slow drift back to center keeps the wave "filling" the screen
    if (currentMillis % 100 == 0) {  // Every 100ms, nudge the bounds
      globalMin++;
      globalMax--;
    }

    // 4. Map and Constrain
    int mappedValue = map(rawValue, globalMin, globalMax, 0, 8);
    mappedValue = constrain(mappedValue, 0, 8);

    // 5. Shift Frame (Scrolling logic)
    for (int col = 0; col < 11; col++) {
      for (int row = 0; row < 8; row++) {
        frame[row][col] = frame[row][col + 1];
      }
    }

    // 6. Draw Bar
    for (int row = 0; row < 8; row++) {
      frame[row][11] = (row >= (8 - mappedValue)) ? 1 : 0;
    }

    // 7. Render
    matrix.renderBitmap(frame, 8, 12);

    // Send to matrix
    matrix.renderBitmap(frame, 8, 12);
    Serial.print("Min:");
    Serial.print(globalMin);
    Serial.print(" Max:");
    Serial.print(globalMax);
    Serial.print(" Raw:");
    Serial.println(rawValue);
  }
  // delay(50);
}
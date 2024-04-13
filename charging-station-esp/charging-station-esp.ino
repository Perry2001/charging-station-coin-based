#include "connection.h"

#define signal D3
FirebaseData firebaseData;
FirebaseJson json;

bool lastSignalState = false;
bool signalTriggered = false; // Added variable to track if the signal has been triggered
int lastIncome = 0; // Variable to store the last updated income value
int lastUse = 0; // Variable to store the last updated use value

void setup() {
  Serial.begin(9600);
  connectToWiFi();
  connectToFirebase();

  pinMode(signal, INPUT_PULLUP);
}

void loop() {
  // Read the current state of the signal pin
  bool currentSignalState = digitalRead(signal);
  Serial.println(currentSignalState);

  // Check if the signal state has changed (signal is triggered)
  if (currentSignalState != lastSignalState) {
    if (currentSignalState == LOW) {
      // Signal is triggered
      signalTriggered = true; // Set signalTriggered to true
    }
    // Update the last signal state
    lastSignalState = currentSignalState;
  }

  // If the signal is triggered and not already processed
  if (signalTriggered) {
    // Retrieve current values from Firebase
    Firebase.getInt(firebaseData, "/income/income");
    int currentIncome = firebaseData.intData();
    Firebase.getInt(firebaseData, "/income/use");
    int currentUse = firebaseData.intData();

    // Update Firebase values only if the new values are different from the last ones
    int newIncome = currentIncome + 5;
    int newUse = currentUse + 1;
    if (newIncome != lastIncome || newUse != lastUse) {
      updateFirebaseValues(newIncome, newUse); // Update Firebase values
    }
    signalTriggered = false; // Reset signalTriggered
  }
}

void updateFirebaseValues(int newIncome, int newUse) {
  // Update both income and use values in Firebase
  Firebase.setInt(firebaseData, "/income/use", newUse);
  Firebase.setInt(firebaseData, "/income/income", newIncome);



  // Update the last updated values
  lastIncome = newIncome;
  lastUse = newUse;
}

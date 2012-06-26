#include "AutoRanging.h"
#include "Watchdog.h"
#include "Nunchuk.h"
#include "Streaming.h"


#define ENCODER_OPTIMIZE_INTERRUPTS
#include "Encoder.h"

Encoder enc(2, 3);


Nunchuk nunchuk;
// initial values
Nunchuk::DataReport report = Nunchuk::defaultReport;

Watchdog checkVCC(1000);


inline int16_t getEncoderValue() {
  return enc.read();
}

AutoRanging<int, uint8_t> counterRange(0, 255);
void sendChange(Nunchuk & n) {
  uint8_t mapped = counterRange.process(getEncoderValue());
  report.joystickAxes[0] = mapped;
  n.sendChange(report);
}


void handleRequest(Nunchuk & n) {
  sendChange(n);
  checkVCC.reset();
  digitalWrite(LED_BUILTIN, HIGH);
}


enum {
  WiimoteVCCPin = A0
};

bool connected = false;

void doConnect() {

  connected = true;
  nunchuk.begin(handleRequest);
  digitalWrite(LED_BUILTIN, HIGH);
}

void setup() {
  // setup code goes here
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //Initialize serial
  Serial.begin(115200);

  // get first reading
  Serial.print("Encoder setup complete, counter currently at ");
  Serial.println(getEncoderValue());
  counterRange.setCenter(getEncoderValue());

  doConnect();
}

Watchdog dumpMapStatus(5000);

enum {
  startThreshold = 500,
  stopThreshold = 300
};

void doDisconnect() {
  connected = false;  
  digitalWrite(LED_BUILTIN, LOW);
  Nunchuk::setDeviceDetectLow();
}
void checkWiimoteStatus() {
  if (checkVCC.hasExpired()) {
    int val = analogRead(WiimoteVCCPin);
    if (connected && val < stopThreshold) {
      Serial << "Looks like we got unplugged." << endl;
      doDisconnect();
    } 
    else if (!connected && val > startThreshold) {
      Serial << "Probably connected!" << endl;
      doConnect();
    }

    checkVCC.reset();
  }
}

void loop() {
  // code that should be repeated goes here  
  //checkWiimoteStatus();
  if (connected) {
    sendChange(nunchuk);
  }



  if (dumpMapStatus.hasExpired()) {
    counterRange.process(getEncoderValue());
    counterRange.dumpStatus(Serial);
    dumpMapStatus.reset();
  }
  //sendChange(nunchuk);

  delay(500);
  //Serial << "Counter value: " << _DEC(myCounter) << " Mapped: " << _DEC(mapped) << endl;
  //Serial.println(mapcounter, DEC);
  Serial.println(getEncoderValue(), DEC);
  //report.joystickAxes[0] = lowByte(counter);
}






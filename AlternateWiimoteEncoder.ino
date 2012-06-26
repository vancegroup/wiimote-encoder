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

Watchdog shouldGetCalled(5000);


inline int16_t getEncoderValue() {
  return enc.read();
}

AutoRanging<int, uint8_t> counterRange(0, 255);
void sendChange() {
  report.joystickAxes[0] = counterRange.process(getEncoderValue());
  nunchuk.sendChange(report);
}


void handleRequest(Nunchuk & n) {
  //sendChange(n);
  shouldGetCalled.reset();
}


void doConnect() {
  nunchuk.begin(handleRequest);
}

void setup() {
  // setup code goes here
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  doConnect();

  //Initialize serial
  Serial.begin(115200);

  // get first reading
  Serial.print("Encoder setup complete, counter currently at ");
  Serial.println(getEncoderValue());
  counterRange.setCenter(getEncoderValue());

}

Watchdog dumpMapStatus(5000);

void loop() {
  // code that should be repeated goes here  
  //checkWiimoteStatus();

  if (shouldGetCalled.hasExpired()) {
   Serial << "Attempting to re-connect" << endl;
   doConnect();
   shouldGetCalled.reset();
  }


  if (dumpMapStatus.hasExpired()) {
    counterRange.process(getEncoderValue());
    counterRange.dumpStatus(Serial);
    dumpMapStatus.reset();
  }

  sendChange();

  digitalWrite(LED_BUILTIN, LOW);
  delay(10);
  //Serial << "Counter value: " << _DEC(myCounter) << " Mapped: " << _DEC(mapped) << endl;
  //Serial.println(mapcounter, DEC);
  //Serial.println(getEncoderValue(), DEC);
  //report.joystickAxes[0] = lowByte(counter);
}






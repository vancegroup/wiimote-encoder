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
  // Flipping the sign so pushing forward is a positive value.
  return - enc.read();
}

enum {
  MaxEncoderValue = 4904,
  MinEncoderValue = -4904
};

/// I have skewed the calibration values in our fake nunchuk so 0x01 is "center" (and 0x00 is min)
// to give us as much dynamic range as possible.
enum {
  MinOutputValue = 0x01,
  MaxOutputValue = 0xff
};

// AutoRanging<int, uint8_t> counterRange(MinOutputValue, MaxOutputValue);

void sendChange() {
  // Auto ranging:
  //uint8_t val = counterRange.process(getEncoderValue());

  // Manually ranging:
  uint8_t val = map( constrain(getEncoderValue(), MinEncoderValue, MaxEncoderValue), MinEncoderValue, MaxEncoderValue, MinOutputValue, MaxOutputValue );

  // putting the output on both axes changes magnitude by sqrt(2)
  // Output will roughly be in [0, sqrt(2)]
  report.joystickAxes[0] = val;
  report.joystickAxes[1] = val;
  nunchuk.sendChange(report);
}

void handleRequest(Nunchuk & n) {
  shouldGetCalled.reset();
}

void doConnect() {
  nunchuk.begin(handleRequest);
}

void setup() {
  doConnect();

  //Initialize serial
  Serial.begin(115200);

  // get first reading
  Serial.print("Encoder setup complete, counter currently at ");
  Serial.println(getEncoderValue());
  //counterRange.setCenter(getEncoderValue());
}

void loop() {
  if (shouldGetCalled.hasExpired()) {
    Serial << "Attempting to re-connect" << endl;
    doConnect();
    shouldGetCalled.reset();
  }

  sendChange();
  delay(25);
}







/** @file
 * 	@brief Implementation
 * 
 * 	@date 2012
 * 
 * 	@author
 * 	Ryan Pavlik
 * 	<rpavlik@iastate.edu> and <abiryan@ryand.net>
 * 	http://academic.cleardefinition.com/
 * 	Iowa State University Virtual Reality Applications Center
 * 	Human-Computer Interaction Graduate Program
 * 
 */

//           Copyright Iowa State University 2012.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include "Nunchuk.h"
#include "wiimote.h"

// Library/third-party includes
#include <Arduino.h>

// Standard includes
// - none

/*
Nunchuk::CalibData Nunchuk::calibration = {
 	{0, 0, 0},
 	{0, 0, 0x20 }, // arbitrary choice
 	{
 		{0x00, 0xff, 0x01},
 		{0x00, 0xff, 0x01}
 	}
 };
 */

/* originally based on log from http://www.wiire.org/Talk:Wii/protocols/wiimote_bus
 starting at the START A5 Control Byte: Slave Address 52 Read line
 following a write of 20 to slave 52.
 
 Now combined with log from http://blog.21ic.com/user1/3633/archives/2008/50560.html
 */
 
#define USE_SKEWED_CALIBRATION
Nunchuk::CalibData Nunchuk::calibration = {
  {    0x58, 0xf5, 0x32  }
  ,
  0x0,
  {    0xcc, 0xbf, 0x2b  }
  ,
  0x0,
  {
#ifdef USE_SKEWED_CALIBRATION
    {0xff, 0x00, 0x01},
    {0xff, 0x00, 0x01}
#else

    {0xff, 0x00, 0x7f},
    {0xff, 0x00, 0x7f}
#endif
  }
};

static const uint8_t NunchukID[] = {
  0x00, 0x00, 0xA4, 0x20, 0x00, 0x00};

const Nunchuk::DataReport Nunchuk::defaultReport = {
  { 
    0x7f, 0x7f   }
  ,
  { 
    0, 0, 0x07 /*arbitrary choice*/   }
  ,
  0,
  0,
  0,
  false,
  false
};

Nunchuk * Nunchuk::self = 0;

void Nunchuk::setDeviceDetectLow() {
  // Make sure we start with the wiimote thinking we're disconnected.
  pinMode(dev_detect_pin, OUTPUT);
  digitalWrite( dev_detect_pin, LOW);
}

void Nunchuk::begin(callbackFnPtr yourFunction, Nunchuk::DataReport const & initialData) {
  self = this;
  userCallback = yourFunction;
  typedef void (*voidFuncPtr)(void);
  voidFuncPtr callback = NULL;
  if (userCallback) {
    callback = &Nunchuk::trampoline;
  }

  uint8_t initialRpt[sizeof(DataReport)];
  memcpy(&(initialRpt[0]), &initialData, sizeof(DataReport));

  uint8_t calib[sizeof(CalibData)];
  memcpy(&(calib[0]), &calibration, sizeof(CalibData));


  wm_init(const_cast<uint8_t *>(&(NunchukID[0])), &(initialRpt[0]), &(calib[0]), sizeof(calibration), callback);
}

void Nunchuk::sendChange(Nunchuk::DataReport const & data) {
  uint8_t rpt[sizeof(DataReport)];
  memcpy(&(rpt[0]), &data, sizeof(DataReport));
  wm_newaction(&(rpt[0]));
}

void Nunchuk::trampoline() {
  self->userCallback(*self);
}


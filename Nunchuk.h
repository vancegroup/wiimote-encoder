/** @file
	@brief Header

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program

*/

//           Copyright Iowa State University 2012.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_Nunchuk_h_GUID_9F45DC8C_5684_4F9B_0369_E7141D93BE28
#define INCLUDED_Nunchuk_h_GUID_9F45DC8C_5684_4F9B_0369_E7141D93BE28


// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
#include <stdint.h>

class Nunchuk {
	public:

		typedef void (*callbackFnPtr)(Nunchuk &);
		struct DataReport {
			uint8_t joystickAxes[2];
			uint8_t accelAxesHigh[3];
			uint8_t accelZLow : 2;
			uint8_t accelYLow : 2;
			uint8_t accelXLow : 2;
			bool buttonC : 1;
			bool buttonZ : 1;
		};

		struct CalibData {
			uint8_t accelCalibZero[3];
			uint8_t accelCalibG[3];
			struct JSCalibAxis {
				uint8_t maxVal;
				uint8_t minVal;
				uint8_t centerVal;
			} jsCalibAxes[2];
		};
		static CalibData calibration;
		static const DataReport defaultReport;
                static void setDeviceDetectLow();
		void begin(callbackFnPtr yourFunction = 0, DataReport const & initialData = defaultReport);

		void sendChange(DataReport const & data);
	private:
		static void trampoline();
		callbackFnPtr userCallback;
		static Nunchuk * self;

};

#endif // INCLUDED_Nunchuk_h_GUID_9F45DC8C_5684_4F9B_0369_E7141D93BE28


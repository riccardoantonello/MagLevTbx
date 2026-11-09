/*
 * Riccardo Antonello (riccardo.antonello@unipd.it)
 *
 * May 02, 2026
 *
 * Dept. of Information Engineering, University of Padova
 *
 */

#ifndef MATLAB_MEX_FILE

/*  Includes ------------------------------------------------------------*/
#include <Arduino.h>
#include <math.h>

#include "TMAG5170.h"
#include "board_v4_4_0_rc_1.h"

#include <rtwtypes.h>

/*  Defines -------------------------------------------------------------*/

namespace BOARD = BOARD_V4_4_0_RC_1;

using Revision = BOARD::Sensor_Revision;
using Range    = Revision::Range;

constexpr Range                         RANGE     = Range::PM_75mT;
constexpr TMAG5170::ConversionAveraging AVERAGING = TMAG5170::ConversionAveraging::X4;


/*  Private variables ---------------------------------------------------*/
static TMAG5170::Sensor<Revision> magSensor[BOARD::NUM_SENSORS];
static bool                       magInitialized[BOARD::NUM_SENSORS] = {false};
static double                     lastGood[BOARD::NUM_SENSORS][3]    = {{0.0}};


/*  Private functions ---------------------------------------------------*/

static TMAG5170::Sensor<Revision> magSensorConfig(void) {
  TMAG5170::Sensor<Revision> config = {};

  config.magnetic_channels    = TMAG5170::MagneticChannels::XYZ;
  config.x_range              = RANGE;
  config.y_range              = RANGE;
  config.z_range              = RANGE;
  config.trigger_mode         = TMAG5170::TriggerMode::SPI;
  config.conversion_averaging = AVERAGING;
  config.operating_mode       = TMAG5170::OperatingMode::ACTIVE_TRIGGER;

  return config;
}


static void sensorToOutputFrame(const double sensorFrame[3], double outputFrame[3]) {
  BOARD::SensorToSilkscreen(sensorFrame, outputFrame);
}


#endif

/* Callback functions definitions ---------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


void sfun_MagLevTbx_MagSens_v44_WrappedBusStart(void) {
#ifndef MATLAB_MEX_FILE

  for (uint8_t i = 0; i < BOARD::NUM_SENSORS; ++i) {
    pinMode(BOARD::SENSORS[i].cs_pin, OUTPUT);
    digitalWrite(BOARD::SENSORS[i].cs_pin, HIGH);
  }

#endif
}


void sfun_MagLevTbx_MagSens_v44_WrappedStart(uint8_T sensorId) {
#ifndef MATLAB_MEX_FILE

  //  Return in case of invalid sensor
  if (sensorId >= BOARD::NUM_SENSORS) {
    return;
  }

  //  Configure the sensor
  magSensor[sensorId] = magSensorConfig();

  const TMAG5170::Status status = TMAG5170::init_sensor(&magSensor[sensorId], BOARD::SENSORS[sensorId].cs_pin);
  magInitialized[sensorId]      = (status == TMAG5170::Status::OK);

#endif
}


void sfun_MagLevTbx_MagSens_v44_WrappedOutput(uint8_T sensorId, double *y0) {
#ifndef MATLAB_MEX_FILE

  //  Return in case of invalid sensor
  if (sensorId >= BOARD::NUM_SENSORS) {
    y0[0] = y0[1] = y0[2] = NAN;
    return;
  }

  if (magInitialized[sensorId]) {
    TMAG5170::Reading      reading;
    const TMAG5170::Status status = TMAG5170::get_magfield_mT(&magSensor[sensorId], &reading);

    // Update the reading when the status is OK.
    // Note that this means the last reading will be kept on errors
    if (status == TMAG5170::Status::OK) {
      const double sensorFrame[3] = {reading.x_mT, reading.y_mT, reading.z_mT};
      sensorToOutputFrame(sensorFrame, lastGood[sensorId]);
    }
  }

  y0[0] = lastGood[sensorId][0];
  y0[1] = lastGood[sensorId][1];
  y0[2] = lastGood[sensorId][2];

#endif
}


void sfun_MagLevTbx_MagSens_v44_WrappedTerminate(void) {
#ifndef MATLAB_MEX_FILE

  //  Nothing to release: every chip-select is deasserted after each transfer

#endif
}

#ifdef __cplusplus
}
#endif

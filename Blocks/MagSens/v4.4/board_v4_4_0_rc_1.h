/*
 * Board 4.4.0-rc.1: pins, geometry, sensor axes and mapping between chip-select and sensors
 *
 * 5 x TMAG5170A2 on SPI (LPSPI4, i.e. the driver default TMAG5170_SPI_PORT).
 *
 * Arduino-independent
 */

#ifndef BOARD_V4_4_0_RC_1_H
#define BOARD_V4_4_0_RC_1_H

#include "TMAG5170.h"

#include <stdint.h>

namespace BOARD_V4_4_0_RC_1 {

/* Board ---------------------------------------------------------------*/

constexpr const char *NAME = "4.4.0-rc.1";

// Revision of every fitted sensor
using Sensor_Revision = TMAG5170::TMAG5170A2;


/* Pins ----------------------------------------------------------------*/

// Number of TMAG5170 sensors
constexpr uint8_t NUM_SENSORS = 5;

// ALERT line shared by all sensors, 10k pull-up to 3.3 V.
//  The sensor ALERT outputs are open-drain and wired-AND: the Teensy must only pull the line low or
//  release it (OUTPUT_OPENDRAIN), never drive it high.
constexpr uint8_t ALERT_PIN = 29;


/* Frames --------------------------------------------------------------*/

// Silkscreen frame: the axis markings at the sensors, origin at the centre sensor (U15).
//   +X towards U18 and +Y towards U16 (each arm points at a solenoid), +Z into the PCB (⊗).
//   Its X and Y are the layout x and y rotated by ARRAY_ROTATION_DEG from x towards y.
//   Left-handed. The positions below are given in this frame.
//
// Sensor frame: the sensing directions of the TMAG5170s as mounted, the same for all 5 sensors.
//   +X towards U14, +Y towards U17, +Z out of the PCB (⊙). Also left-handed.
//   Measured: X and Y with a magnet over the array, Z by turning the board over in the Earth's field.

// constexpr double ARRAY_ROTATION_DEG = 27.811;

struct Axis_Map {
  uint8_t sensor_axis; // 0 = X, 1 = Y, 2 = Z
  int8_t  sign;        // +1 or -1
};

// Conversion: X_silk = -Y_sensor, Y_silk = -X_sensor, Z_silk = -Z_sensor
constexpr Axis_Map SILKSCREEN_FROM_SENSOR[3] = {
  {1, -1},
  {0, -1},
  {2, -1},
};

// Convert a reading from the sensor frame to the silkscreen frame
static inline void SensorToSilkscreen(const double sensor[3], double silkscreen[3]) {
  for (uint8_t axis = 0; axis < 3; ++axis) {
    silkscreen[axis] = SILKSCREEN_FROM_SENSOR[axis].sign * sensor[SILKSCREEN_FROM_SENSOR[axis].sensor_axis];
  }
}


/* Sensors -------------------------------------------------------------*/

struct Point_mm {
  double x;
  double y;
};

// Position of a sensor in the cross (silkscreen frame), used as the index into SENSORS.
// The values follow CS order (CS1 first).
namespace Sensor_Position {
enum Value : uint8_t {
  ARM_NEG_Y = 0,
  CENTRE    = 1,
  ARM_POS_Y = 2,
  ARM_NEG_X = 3,
  ARM_POS_X = 4,
};
} // namespace Sensor_Position

struct Sensor {
  uint8_t     cs_index; // CS line number in KiCad
  uint8_t     cs_pin;   // Teensy pin
  const char *ref;      // Reference designator
  Point_mm    position_mm;
};

// Distance from the centre sensor to each arm sensor
constexpr double SENSOR_SPACING_MM = 8.0;

// Sensors, indexed by Sensor_Position
// clang-format off
constexpr Sensor SENSORS[NUM_SENSORS] = {
  {1, 24, "U14", {0.0,                -SENSOR_SPACING_MM}}, // ARM_NEG_Y
  {2, 25, "U15", {0.0,                0.0               }}, // CENTRE
  {3, 26, "U16", {0.0,                +SENSOR_SPACING_MM}}, // ARM_POS_Y
  {4, 27, "U17", {-SENSOR_SPACING_MM, 0.0               }}, // ARM_NEG_X
  {5, 28, "U18", {+SENSOR_SPACING_MM, 0.0               }}, // ARM_POS_X
};
// clang-format on

// Each row must lie on the arm its Sensor_Position names
static_assert(
  SENSORS[Sensor_Position::CENTRE].position_mm.x == 0.0 && SENSORS[Sensor_Position::CENTRE].position_mm.y == 0.0,
  "SENSORS[CENTRE] is not at the centre"
);
static_assert(
  SENSORS[Sensor_Position::ARM_POS_X].position_mm.x > 0.0 && SENSORS[Sensor_Position::ARM_POS_X].position_mm.y == 0.0,
  "SENSORS[ARM_POS_X] is not on the +X arm"
);
static_assert(
  SENSORS[Sensor_Position::ARM_NEG_X].position_mm.x < 0.0 && SENSORS[Sensor_Position::ARM_NEG_X].position_mm.y == 0.0,
  "SENSORS[ARM_NEG_X] is not on the -X arm"
);
static_assert(
  SENSORS[Sensor_Position::ARM_POS_Y].position_mm.x == 0.0 && SENSORS[Sensor_Position::ARM_POS_Y].position_mm.y > 0.0,
  "SENSORS[ARM_POS_Y] is not on the +Y arm"
);
static_assert(
  SENSORS[Sensor_Position::ARM_NEG_Y].position_mm.x == 0.0 && SENSORS[Sensor_Position::ARM_NEG_Y].position_mm.y < 0.0,
  "SENSORS[ARM_NEG_Y] is not on the -Y arm"
);


/* Solenoids and permanent magnets -------------------------------------*/

// Solenoids sit on the array axes, in line with the sensor arms; permanent
// magnets sit on the array diagonals.
constexpr uint8_t NUM_SOLENOIDS = 4;
constexpr uint8_t NUM_MAGNETS   = 4;

constexpr double SOLENOID_RADIUS_MM = 20.0;
constexpr double MAGNET_RADIUS_MM   = 17.0;


// Solenoids, ordered +x, +y, -x, -y
// clang-format off
constexpr Point_mm SOLENOIDS_MM[NUM_SOLENOIDS] = {
  {+SOLENOID_RADIUS_MM, 0.0                },
  {0.0,                 +SOLENOID_RADIUS_MM},
  {-SOLENOID_RADIUS_MM, 0.0                },
  {0.0,                 -SOLENOID_RADIUS_MM},
};

constexpr double DIAGONAL = 0.70710678118654752; // cos(45 deg)

// Permanent magnets, ordered (+x,+y), (-x,+y), (-x,-y), (+x,-y)
constexpr Point_mm MAGNETS_MM[NUM_MAGNETS] = {
  {+MAGNET_RADIUS_MM * DIAGONAL, +MAGNET_RADIUS_MM * DIAGONAL},
  {-MAGNET_RADIUS_MM * DIAGONAL, +MAGNET_RADIUS_MM * DIAGONAL},
  {-MAGNET_RADIUS_MM * DIAGONAL, -MAGNET_RADIUS_MM * DIAGONAL},
  {+MAGNET_RADIUS_MM * DIAGONAL, -MAGNET_RADIUS_MM * DIAGONAL},
};
// clang-format on

} // namespace BOARD_V4_4_0_RC_1

#endif // BOARD_V4_4_0_RC_1_H

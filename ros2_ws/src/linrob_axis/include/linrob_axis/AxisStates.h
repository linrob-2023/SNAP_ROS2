#ifndef LINROB_AXIS_STATES_H
#define LINROB_AXIS_STATES_H

#include <cstdint>

namespace linrob
{
/**
 * Enum class representing Linrob Axis state.
 */
enum class AxisState : uint8_t
{
  ABORTING = 0U,
  CONTINUOUS_MOTION,
  COORDINATED_MOTION,
  DISABLED,
  DISCRETE_MOTION,
  ERRORSTOP,
  GANTRY_SLAVE,
  HOMING,
  STANDSTILL,
  STANDSTILL_PENDING,
  STOPPING,
  SYNCHRONIZED_MOTION,
  STOPPED
};

}

#endif // LINROB_AXIS_STATES_H

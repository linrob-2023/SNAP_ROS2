#ifndef LINROB_AXIS_MODES_H
#define LINROB_AXIS_MODES_H

#include <cstdint>

namespace linrob
{
/**
 * Enum class representing Linrob Axis system modes.
 */
enum class Mode : uint8_t
{
  MANUAL = 0U,
  AUTO,
  AUTO_EXTERNAL
};
}

#endif // LINROB_AXIS_MODES_H

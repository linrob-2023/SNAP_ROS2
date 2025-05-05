#ifndef LINROB_POSITIONSETTINGS_H
#define LINROB_POSITIONSETTINGS_H

#include <cstdint>
namespace linrob
{
/**
 * Structure for storing position settings.
 */
struct PositionSettings
{
  /// Initial position index.
  uint16_t initialIndex;
  /// Index where next position will be written in PLC.
  uint16_t nextPositionIndex;
  /// Max index value for next position before it will be restored to initial index.
  uint16_t maxPositionIndices;
  /// Number of new positions received.
  uint32_t newPositionsReceivedCount;
  /// Value at which command to PLC will be sent to execute movements.
  uint16_t executeMovementsOnIndex;
};
}

#endif // LINROB_POSITIONSETTINGS_H

#ifndef LINROB_VARIANT_TYPE_H
#define LINROB_VARIANT_TYPE_H

#include <comm/datalayer/datalayer_system.h>

#include <unordered_map>

namespace linrob
{
const std::unordered_map<std::string, comm::datalayer::VariantType> VARIANT_ARRAY_TYPE_MAP = {
  {typeid(int).name(), comm::datalayer::VariantType::ARRAY_OF_INT32},
  {typeid(float).name(), comm::datalayer::VariantType::ARRAY_OF_FLOAT32}};
}

#endif // LINROB_VARIANT_TYPE_H

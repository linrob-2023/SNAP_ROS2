#ifndef LINROB_CONNECTION_H
#define LINROB_CONNECTION_H

#include <comm/datalayer/variant.h>

#include <string>
#include <unordered_map>

namespace linrob
{
/**
 * Structure for storing data layer connection information.
 */
struct Connection
{
  /// IP address.
  std::string ip;
  /// Username.
  std::string user;
  /// Password.
  std::string password;
  /// SSL port.
  int sslPort;
  /// Data layer nodes addresses.
  std::unordered_map<std::string, std::pair<std::string, comm::datalayer::Variant> > datalayerNodeMap;
};
}
#endif // LINROB_CONNECTION_H

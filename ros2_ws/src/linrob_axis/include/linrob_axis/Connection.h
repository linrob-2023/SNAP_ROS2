#ifndef LINROB_CONNECTION_H
#define LINROB_CONNECTION_H

#include <string>

namespace linrob
{
struct Connection
{
  std::string ip;
  std::string user;
  std::string password;
  int sslPort;
};
}
#endif // LINROB_CONNECTION_H

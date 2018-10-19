#pragma once
#include "..\..\lib\data\db\influxdb.hpp"
#include <string>
using namespace std;
class InputDataIO {

public:
  InputDataIO();
  virtual ~InputDataIO();
  /**
  * add a new entry to the influx db
  * TODO method for dynamic params
  */
  void writeEntry();
  /**
  * send query to the influx server
  * @param resp: answer given by the server
  * @param query: influxdb query (influxQL syntax)
  */
  void query(string & resp, string & query);
private:
  string serverAddress;
  int serverPort;
  string userName;
  string userPwd;
  influxdb_cpp::server_info si;
};

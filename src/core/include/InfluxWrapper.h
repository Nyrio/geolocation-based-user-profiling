#pragma once
#include "influxdb.hpp"
#include <string>
#include <vector>
#include <utility>
using namespace std;
namespace data{
class InfluxWrapper {

public:
  InfluxWrapper(string serverAddress_p,int serverPort_p,string userName_p, string userPwd_p);
  virtual ~InfluxWrapper();
  /**
  * add a new entry to the influx db
  */
  void writeEntry(long timestamp,string measurement, vector<pair<string,string>>& tags, vector<pair<string,string>>& fields);
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
  const string dbName="db";
  influxdb_cpp::server_info serverInfo;
};
}

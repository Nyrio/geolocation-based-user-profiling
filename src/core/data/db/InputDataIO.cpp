#include "InputDataIO.h"
InputDataIO::InputDataIO(string serverAddress_p,int serverPort_p,string userName_p, string userPwd_p):
  serverAddress(serverAddress_p),serverPort(serverPort_p),userName(userName_p),userPwd(userPwd_p){
serverInfo = influxdb_cpp::server_info(serverAddress,serverPort,dbName,userName,userPwd);
}
InputDataIO::~InputDataIO(){

}

void InputDataIO::writeEntry(long timestamp,string measurement, vector<pair<string,string>>& tags, vector<pair<string,string>>& fields){
  influxdb_cpp::builder().meas(measurement).timestamp(timestamp);
  for(int i = 0;i<tags.size();i++){
    influxdb_cpp::builder().tag(tags[i].first,tags[i].second);
  }
  for(int i = 0;i<fields.size();i++){
    influxdb_cpp::builder().field(fields[i].first,fields[i].second);
  }
influxdb_cpp::builder().post_http(serverInfo);
}

void InputDataIO::query(string & resp, string & query){
  influxdb_cpp::query(resp,query, serverInfo);
}

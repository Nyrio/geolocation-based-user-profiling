#include "../include/InfluxWrapper.h"
#include "assert.h"
data::InfluxWrapper::InfluxWrapper(string serverAddress_p,int serverPort_p,string userName_p, string userPwd_p):
  serverAddress(serverAddress_p),serverPort(serverPort_p),userName(userName_p),userPwd(userPwd_p),serverInfo(serverAddress,serverPort,dbName,userName,userPwd)
{

}

data::InfluxWrapper::~InfluxWrapper()
{

}

void data::InfluxWrapper::writeEntry(long timestamp,string measurement, vector<pair<string,string>>& tags, vector<pair<string,string>>& fields)
{
    influxdb_cpp::detail::tag_caller *tagRef=  &influxdb_cpp::builder().meas(measurement);
    for(int i = 0;i<tags.size();i++){
      tagRef=&(tagRef->tag(tags[i].first,tags[i].second));
    }
    assert(fields.size()>0);
    influxdb_cpp::detail::field_caller *fieldRef= &tagRef->field(fields[0].first,fields[0].second);
    for(int i = 1;i<fields.size();i++){
      fieldRef = &fieldRef->field(fields[i].first,fields[i].second);
    }
    fieldRef->timestamp(timestamp).post_http(serverInfo);

}

void data::InfluxWrapper::query(string & resp, string & query)
{
  influxdb_cpp::query(resp,query, serverInfo);
}

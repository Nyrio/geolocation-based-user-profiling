#pragma once
#include "influxdb.hpp"
#include <string>
#include <vector>
#include <utility>
#include "assert.h"
#include "json.hpp"

using json = nlohmann::json;

namespace data
{
    class InfluxWrapper
    {

    public:
        InfluxWrapper(std::string serverAddress_p, int serverPort_p,
            std::string userName_p, std::string userPwd_p):
          serverAddress(serverAddress_p), serverPort(serverPort_p),
          userName(userName_p), userPwd(userPwd_p),
          serverInfo(serverAddress, serverPort, dbName, userName, userPwd)
        {
        }
        ~InfluxWrapper()
        {
        }

        /**
        * add a new entry to the influx db
        */
        void writeEntry(long timestamp, std::string measurement, 
                        vector<pair<std::string,std::string>>& tags,
                        vector<pair<std::string,std::string>>& fields)
        {
            influxdb_cpp::detail::tag_caller *tagRef=  &influxdb_cpp::builder().meas(measurement);
            for(uint i = 0;i<tags.size();i++){
                tagRef=&(tagRef->tag(tags[i].first,tags[i].second));
            }
            assert(fields.size()>0);
            influxdb_cpp::detail::field_caller *fieldRef= &tagRef->field(fields[0].first,fields[0].second);
            for(uint i = 1;i<fields.size();i++){
                fieldRef = &fieldRef->field(fields[i].first,fields[i].second);
            }
            fieldRef->timestamp(timestamp).post_http(serverInfo);

        }

        /**
        * send query to the influx server
        * @param resp: answer given by the server
        * @param query: influxdb query (influxQL syntax)
        */
        json query(std::string & query)
        {
            std::string resp;
            influxdb_cpp::query(resp, query, serverInfo);

            return json::parse(resp);
        }

    private:
        std::string serverAddress;
        int serverPort;
        std::string userName;
        std::string userPwd;
        const std::string dbName="rawdata";
        influxdb_cpp::server_info serverInfo;
    };
}

# Data Management layer
Including : 
- I/O with DB
- I/O with Workflow input files
- external API callers

## DataBase
Our DB uses InfluxDB.

For developpement (under Linux) : 
- install docker
- install docker-compose
- go to scripts/influxdb folder and call *docker-compose up* to setup docker with local parameters.
Populate DB (requirements : python 3) :
- execute scripts/populate_influxdb.py (call *python3 populate_influxdb.py --help* for more information about that).

A server hosts the data to avoid local db while developping. To obtain access to this server, please contact the team.

## Workflow parameters
The parameters used for the algorithms are listed in a config file to avoid re-compiling the project.

### Config file location
This config file must be a text file.
The default location is *./config.txt*.
You can change this location while calling the *./cli* command using the parameter *-wp* :
```./cli -wp /my/path/to/another/config/file```.

### Config file format
In this file the format is :
``` parameter_name = value```
Empty lines and lines begining with *#* are ignored (considered as comments).
Example : 
```
# DJ parameters
eps=0.0002
min_pts=20

# Radius of search of an amenity around coordinates
search_radius=10.

# Interval in minutes between two points to consider them in different
# unique visites
sep_minutes=10.

# OpenStreetMap attributes to consider when tagging a position
place_types=amenity,leisure,tourism,building
```

## External API
External API calls are formatted for the corresponding API. Must be checked case by case.
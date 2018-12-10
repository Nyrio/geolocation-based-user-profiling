Big Brother - profiling users from geolocation data
===

## Introduction and architecture documents

See [report.pdf](report.pdf).


## Installation

### Loading the data

Our tool reads data from an InfluxDB database. You need to setup a database. An easy way to get started is to use Docker Compose with [docker-compose.yml](scripts/influxdb/docker-compose.yml). The default settings of our scripts match the default settings of a local InfluxDB database (on `localhost:8086` with username `root` and password `root`).

The script [populate_influxdb.py](scripts/populate_influxdb.py) loads data from a file to the database. You will want to use it with the `--gps` option (use `-h` to see a short manual). The file must contain on each line the following tab-separated values: user id, date, longitude, latitude. The data will be added in the `gps` measurement of the `rawdata` database. Each data point has a tag `id` with the user id, and fields `lat` and `lon` (and of course a timestamp `time`).

The [extract_sample.py](scripts/extract_sample.py) is intended to extract from a data file the points corresponding to the given user and time range. The only purpose of this file is to reduce the size of the data loaded in the database as loading gigabytes of data was quite long. There are also options in our tool to load only a given time range from the db.


### Compiling the tool

In the `src/core` directory, run `make cli`. This will create a `cli` executable and cache the compilation files in the `build` folder.

### Using custom parameters

The [config.txt](src/core/config.txt) file contains parameters read at runtime by our tool. As your data is different from ours, you will probably need to use custom parameters. Check the comments in the file for more information.

### Running the program

You can simply run `./cli`. If you move the executable, you also need to move the config file. To see the available commands, type `help` in the tool's command-line interface.

## Disclaimer

We cannot be held responsible for any malicious of this software.

This is a school project: this program is distributed without any warranty; without even the implied warranty of merchantability or fitness for a particular purpose. The program might fail if it cannot connect to the database or an external API, or with data that doesn't match the expected format.
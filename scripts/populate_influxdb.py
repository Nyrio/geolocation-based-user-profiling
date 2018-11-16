import sys
import argparse
from datetime import datetime
from influxdb import InfluxDBClient

BUFFER_SIZE = 10000
TIME_IN = "%Y-%m-%d %H:%M:%S"
TIME_OUT = "%Y-%m-%dT%H:%M:%SZ"


def main(args):
    client = InfluxDBClient("localhost", 8086, "root", "root", "rawdata")
    client.create_database("rawdata")

    if args.gps:
        read_file(client, args.gps, "gps", args.limit)
    if args.gsm:
        read_file(client, args.gsm, "gsm", args.limit)
    if args.wifi:
        read_file(client, args.wifi, "wifi", args.limit)


def read_file(client, filename, mode, limit=None):
    """Read the content of the file and save it in the InfluxDB
    """
    points_buffer = []
    with open(filename, 'r') as fhandler:
        # Read lines in the input file one at a time because the file
        # might be larger than the memory
        nb_lines = 0
        for line in fhandler:
            nb_lines += 1
            data = line.strip().split("\t")

            # Extract the data from the line
            time = datetime.strptime(data[1].split(".")[0], TIME_IN)
            time_str = time.strftime(TIME_OUT)
            tags = {"id": int(data[0])}
            if mode == "gps":
                fields = {"lat": float(data[3]), "lon": float(data[2])}
            elif mode == "gsm":
                fields = {"gsm": int(data[2])}
            elif mode == "wifi":
                fields = {"ip": data[2]}

            write_point(client, points_buffer, mode, time_str, tags,
                        fields, nb_lines)

            if limit and nb_lines >= limit:
                break

        # Write remaining points
        client.write_points(points_buffer)
        points_buffer.clear()
        sys.stdout.write("\r%s: %d\n" % (mode, nb_lines))


def write_point(client, points_buffer, measurement, time, tags, fields,
                nb_lines=None):
    """Add the point to a buffer and write the buffer in DB if it's full.
    """
    point = {
        "measurement": measurement,
        "time": time,
        "tags": tags,
        "fields": fields
    }
    points_buffer.append(point)
    if len(points_buffer) >= BUFFER_SIZE:
        client.write_points(points_buffer)
        points_buffer.clear()

        if nb_lines:
            sys.stdout.write("\r%s: %d" % (measurement, nb_lines))


def get_args():
    """Get the command-line arguments
    """
    parser = argparse.ArgumentParser(
        description="Populate the InfluxDB database.")

    parser.add_argument("--gps", help="Path to gps data file")
    parser.add_argument("--gsm", help="Path to gsm data file")
    parser.add_argument("--wifi", help="Path to wifi data file")

    parser.add_argument("-l", "--limit", type=int,
                        help="Limit the number of entries")

    return parser.parse_args()


if __name__ == '__main__':
    args = get_args()
    main(args)

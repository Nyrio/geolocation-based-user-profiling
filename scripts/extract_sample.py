import argparse
import sys


def main(args):
    lines = []
    cpt = 0
    with open(args.source, 'r') as fhandler:
        for line in fhandler:
            sline = line.strip()

            data = sline.split("\t")
            time = data[1]
            userid = data[0]

            if userid == args.userid \
               and time >= args.start and time <= args.end:
                lines.append(sline)

            cpt += 1
            if cpt % 1000 == 0:
                sys.stdout.write("\r%d" % cpt)

    with open(args.dest, 'w') as fhandler:
        fhandler.write("\n".join(lines))


def get_args():
    """Get the command-line arguments
    """
    parser = argparse.ArgumentParser(
        description="Extract a sample of the data.")

    parser.add_argument("userid", help="User ID")
    parser.add_argument("start", help="Start")
    parser.add_argument("end", help="End")
    parser.add_argument("source", help="Source data file")
    parser.add_argument("dest", help="Destination file")

    return parser.parse_args()


if __name__ == '__main__':
    args = get_args()
    main(args)

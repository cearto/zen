import sys, getopt


def main(argv):
	s = "acc 3 3 4 6 7 8 9 9 6 5"
	print s.split()


if __name__ == "__main__":
    main(sys.argv[1:])
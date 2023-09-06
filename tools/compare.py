import sys


def main() -> int:
	args = sys.argv

	first_file = str(args[1])
	second_file = str(args[2])

	first_bytes: bytes = bytes()
	second_bytes: bytes = bytes()

	with open(first_file, "rb") as fp:
		first_bytes = fp.read()

	with open(second_file, "rb") as fp:
		second_bytes = fp.read()

	print(f"File 1: {len(first_bytes):d}B")
	print(f"File 2: {len(second_bytes):d}B")

	return 0


if __name__ == "__main__":
	exit(main())

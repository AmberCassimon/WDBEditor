import os
import sys

from typing import Optional


class DifferenceReport:
	# The length of both files i bytes
	original_length: int
	modified_length: int

	# The index of the first byte that differs between both files
	first_different_byte: Optional[int]

	# The number of bytes that differ between both files
	num_differing_bytes: int

	def __init__(self, original: bytes, modified: bytes):
		self.original_length = len(original)
		self.modified_length = len(modified)

		self.first_different_byte = None
		self.num_differing_bytes = 0

		for i in range(min(self.original_length, self.modified_length)):
			if original[i] != modified[i]:
				if self.first_different_byte is None:
					self.first_different_byte = i

				self.num_differing_bytes += 1

	def __str__(self) -> str:
		return f"Modified Length: {self.modified_length:d} ({100.0 * (float(self.modified_length) / float(self.original_length)):5.2f}%){os.linesep}" \
			f"First differing byte: {self.first_different_byte}{os.linesep}" \
			f"Number of differing bytes: {self.num_differing_bytes:d} ({100.0 * (float(self.num_differing_bytes) / float(self.original_length)):5.2f}%)"


def main() -> int:
	args = sys.argv

	original_filename = str(args[1])
	modified_filename = str(args[2])

	original_bytes: bytes = bytes()
	modified_bytes: bytes = bytes()

	with open(original_filename, "rb") as fp:
		original_bytes = fp.read()

	with open(modified_filename, "rb") as fp:
		modified_bytes = fp.read()

	report = DifferenceReport(original_bytes, modified_bytes)

	print(report)

	return 0


if __name__ == "__main__":
	exit(main())

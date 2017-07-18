#!usr/bin/python3

import sys
import cs50

def main():
	while True:
		print("Height: ", end="")
		height = cs50.get_int()
		if height >= 0 and height <= 23:
			break

	print(height)

	for i in range(height):
		print(" " * (height-i-1), end="")
		print("#" * (i+2))
	return 0

if __name__ == "__main__":
	ret = main()
	exit(ret)
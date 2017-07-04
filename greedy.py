#!usr/bin/python3

import sys
import cs50

def main():
	print("O hai! ", end="")
	while True:
		print("How much change is owned?: ", end="")
		money = cs50.get_float()
		if money >= 0:
			break

	money_in_cents = int(money*100)
	avaliable_coins = [25, 10, 5, 1]
	total_coins = 0
	for coin in avaliable_coins:
		coins_to_add = money_in_cents//coin
		total_coins += coins_to_add
		if money_in_cents % coin == 0:
			break

		money_in_cents -= coins_to_add * coin

	print(total_coins)
	return 0

if __name__ == "__main__":
	ret = main()
	exit(ret)
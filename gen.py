#!/usr/bin/env python3
import random
import string
import hashlib

letters = list("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")

def sha256(plaintext : str) -> int:
	m = hashlib.sha256()
	m.update(str.encode(plaintext))
	return int.from_bytes(m.digest(), byteorder='big')

def rand_nonce(n : int) -> str:
	return "".join(random.choice(letters) for i in range(n))

def basilisk(n : int) -> str:
	return "basilisk|{0:0>10}|{1}".format(n, rand_nonce(64))

if __name__ == "__main__":
	for y in range(5):
		minimum = -1
		final = ""
		for x in range(1000000):
			example = basilisk(y)
			result = sha256(example)
			if minimum == -1 or result < minimum:
				minimum = result
				final = example

		print(final + " " + hashlib.sha256(str.encode(final)).hexdigest())


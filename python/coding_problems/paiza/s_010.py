# ひとりすごろく

def solve(dice_orientation, map_data):
	opposite_data = { }
	for i in range(6):
		opposite_data[dice_orientation[0]] = dice_orientation[1]
		opposite_data[dice_orientation[1]] = dice_orientation[0]
		opposite_data[dice_orientation[2]] = dice_orientation[3]
		opposite_data[dice_orientation[3]] = dice_orientation[2]
		opposite_data[dice_orientation[4]] = dice_orientation[5]
		opposite_data[dice_orientation[5]] = dice_orientation[4]
	def is_opposite(n1, n2):
		return opposite_data[n1] == n2
	total_cost = 0
	for i in range(1, len(map_data)):
		if (map_data[i - 1] == map_data[i]):
			pass
		elif (is_opposite(map_data[i - 1], map_data[i])):
			total_cost += 2
		else:
			total_cost += 1
	return total_cost

def main(input):
	lines = input.split('\n')

	def read_line():
		return lines.pop(0)

	dice_orientation = tuple(int(x) for x in read_line().split())
	total_length = int(read_line())
	map_data = []
	for _ in range(total_length):
		map_data.append(int(read_line()))


	return str(solve(dice_orientation, map_data))

import sys
#print(main(sys.stdin.read()))



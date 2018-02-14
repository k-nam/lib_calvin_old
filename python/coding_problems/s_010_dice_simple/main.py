# T B U D L R (looking from goal's direction. U means front, D means back)
test_a = r"""1 6 2 5 4 3
4
1
5
3
4"""
# 4

test_b = r"""6 5 4 3 2 1
6
6
2
1
3
4
3"""
# 8

test_c = r"""6 5 4 3 2 1
6
6
6
6
6
6
6"""
# 0

for_paiza_submission = False
test_input = test_b
lines = test_input.split('\n')

def read_line():
	if (for_paiza_submission):
		return input()
	else:
		return lines.pop(0)

dice_orientation = tuple(int(x) for x in read_line().split())
total_length = int(read_line())
map_data = []

for i in range(total_length):
	map_data.append(int(read_line()))

opposite_data = { }
for i in range(6):
	opposite_data[dice_orientation[0]] = dice_orientation[1]
	opposite_data[dice_orientation[1]] = dice_orientation[0]
	opposite_data[dice_orientation[2]] = dice_orientation[3]
	opposite_data[dice_orientation[3]] = dice_orientation[2]
	opposite_data[dice_orientation[4]] = dice_orientation[5]
	opposite_data[dice_orientation[5]] = dice_orientation[4]

# (index, dice_orientation) => optimal cost
record = {}

# if new_top is on side, there will be only one option
# if new_top is on bottom, there wil be two options 
def rotate_dice(original_orientation, new_top):
	T, B, U, D, L, R = original_orientation
	if (B == new_top):
		return [(B, T, U, D, R, L), (B, T, D, U, L, R)]
	elif (U == new_top):
		return [(U, D, B, T, L, R)]
	elif (D == new_top):
		return [(D, U, T, B, L, R)]
	elif (L == new_top):
		return [(L, R, U, D, T, B)]
	elif (R == new_top):
		return [(R, L, U, D, B, T)]
	else:
		return [(T, B, U, D, L, R)]

def solve():
	def is_opposite(n1, n2):
		return opposite_data[n1] == n2
	total_cost = 0
	for i in range(1, total_length):
		if (map_data[i - 1] == map_data[i]):
			pass
		elif (is_opposite(map_data[i - 1], map_data[i])):
			total_cost += 2
		else:
			total_cost += 1
	return total_cost

print(solve())


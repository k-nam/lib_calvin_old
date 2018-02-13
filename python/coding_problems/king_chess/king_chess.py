test_a = r"""4
0 2
2 1""" # 3

test_b = r"""5
0 4
4 0""" # 4


input_text = test_a
lines = input_text.split("\n")
for_paiza_submission = False

def read_line():
	if (for_paiza_submission):
		return input()
	else:
		return lines.pop(0)

board_size = int(read_line())

my_position = [int(x) for x in read_line().split()]
enemy_position = [int(x) for x in read_line().split()]

print(my_position)

def distance_from_center(position):
	center = [board_size / 2,  board_size / 2]
	x = position[0] - center[0]
	y = position[1] - center[1]
	return (x*x + y*y) ** 0.5


print(distance_from_center([1, 1]))


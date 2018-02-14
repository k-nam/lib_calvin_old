test_a = r"""7 7
#######
#X.2..#
#.#.#.#
#.X.1.#
#.#1#.#
#4....#
#######""" # No

test_b = r"""6 3
###
#3#
#1#
#.#
#X#
###""" # Yes

test_c = r"""5 3
###
#2#
###
#X#
###""" # No


input_text = test_c
lines = input_text.split("\n")
for_paiza_submission = False

def read_line():
	if (for_paiza_submission):
		return input()
	else:
		return lines.pop(0)

h, w = [int(x) for x in read_line().split()]

data = []

for i in range(h):
    data.append([x for x in list(read_line())])

def get_explosion_cells(data, bomb_position, firepower):
	upward = True
	downward = True
	rightward = True
	leftward = True

	candidates = [bomb_position]

	def in_range(position):
		return position[0] >= 0 and position[0] < h and position[1] >= 0 and position[1] < w and \
			data[position[0]][position[1]] != '#' 

	for i in range(1, firepower + 1):
		up = [bomb_position[0] - i, bomb_position[1]]
		down = [bomb_position[0] + i, bomb_position[1]]
		right = [bomb_position[0], bomb_position[1] + i]
		left = [bomb_position[0], bomb_position[1] - i]
		if (not in_range(up)):
			upward = False
		if (not in_range(down)):
			downward = False
		if (not in_range(right)):
			rightward = False
		if (not in_range(left)):
			leftward = False
		if (upward):
			candidates.append(up)
		if (downward):
			candidates.append(down)
		if (rightward):
			candidates.append(right)
		if (leftward):
			candidates.append(left)

	return candidates

def processExplosion(data):
	explosion_map = [[0 for j in data[0]] for i in range(len(data))]
	for i in range(len(data)):
		for j in range(len(data[0])):
			if (data[i][j] != 'X' and data[i][j] != '.' and data[i][j] != '#'):
				cells = get_explosion_cells(data, [i, j], int(data[i][j]))
				for cell in cells:
					explosion_map[cell[0]][cell[1]] = 1
	#print(explosion_map)
	unkilled_enemy = 0
	for i in range(len(data)):
		for j in range(len(data[0])):
			if (data[i][j] == 'X' and explosion_map[i][j] == 0):
				unkilled_enemy += 1
	return unkilled_enemy

def is_clean(data):
	for row in data:
		for cell in row:
			if (cell == 'X'): # X means enemy
				return False;
	return True;

print("YES" if processExplosion(data) == 0 else "NO")

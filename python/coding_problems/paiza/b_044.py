# 爆弾ゲーム

def get_explosion_cells(data, h, w, bomb_position, firepower):
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

def processExplosion(data, h, w):
	explosion_map = [[0 for _ in data[0]] for _ in range(len(data))]
	for i in range(len(data)):
		for j in range(len(data[0])):
			if (data[i][j] != 'X' and data[i][j] != '.' and data[i][j] != '#'):
				cells = get_explosion_cells(data, h, w, [i, j], int(data[i][j]))
				for cell in cells:
					explosion_map[cell[0]][cell[1]] = 1
	#print(explosion_map)
	unkilled_enemy = 0
	for i in range(len(data)):
		for j in range(len(data[0])):
			if (data[i][j] == 'X' and explosion_map[i][j] == 0):
				unkilled_enemy += 1
	return unkilled_enemy

def main(input):
	lines = input.split("\n")
	def read_line():
		return lines.pop(0)

	h, w = [int(x) for x in read_line().split()]
	data = []	
	for _ in range(h):
		data.append(list(read_line()))

	return "YES" if processExplosion(data, h, w) == 0 else "NO"

import sys
#print(main(sys.stdin.read()))

# 追いかけっこ
# unfinished

def distance_from_center(board_size, position):
	center = [board_size / 2,  board_size / 2]
	x = position[0] - center[0]
	y = position[1] - center[1]
	return (x*x + y*y) ** 0.5

def main(input):
	lines = input.split("\n")

	def read_line():
		return lines.pop(0)

	board_size = int(read_line())
	my_position = [int(x) for x in read_line().split()]
	enemy_position = [int(x) for x in read_line().split()]

	return str(distance_from_center(board_size, [1, 1]))


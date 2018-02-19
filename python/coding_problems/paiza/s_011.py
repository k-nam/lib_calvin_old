# 追いかけっこ

# Reflecting board won't affect the structure of this game
# Reflect so that enemy is on the upper-right corner of me
# and I am on or under the diagonal line (from lower-left to upper-right)
def reflect_board(board_size, my_position, enemy_position):
	def reflect(number):
		return board_size - number - 1

	def reflect_horizontal(my_position, enemy_position):
		return ((reflect(my_position[0]), my_position[1]), (reflect(enemy_position[0]), enemy_position[1]))

	def reflect_vertical(my_position, enemy_position):
		return ((my_position[0], reflect(my_position[1])), (enemy_position[0], reflect(enemy_position[1])))

	def reflect_45(my_position, enemy_position):
		return (my_position[1], my_position[0]), (enemy_position[1], enemy_position[0])

	if (my_position[0] > enemy_position[0]):
		my_position, enemy_position = reflect_horizontal(my_position, enemy_position)
	if (my_position[1] > enemy_position[1]):
		my_position, enemy_position = reflect_vertical(my_position, enemy_position)
	if (my_position[1] > my_position[0]):
		my_position, enemy_position = reflect_45(my_position, enemy_position)

	return (my_position, enemy_position)

def distance_from_center(board_size, position):
	center = [board_size / 2,  board_size / 2]
	x = position[0] - center[0]
	y = position[1] - center[1]
	return (x*x + y*y) ** 0.5

# Calculate how many turns left to win
def turns_to_win(my_enemy_dist, my_wall_dist):
	# Begins with my turn, so return value will be (my_moves_to_win * 2 - 1)
	#print("aa" + str(my_enemy_dist), str(my_wall_dist))
	# I will win when I reach wall (enemy cannnot run away sideways because I can also chase diagonally)
	# One turn difference due to initial position
	if (my_enemy_dist % 2 == 0):
		return my_wall_dist
	else:
		return my_wall_dist - 1


def solve(board_size, my_position, enemy_position):
	my_position, enemy_position = reflect_board(board_size, my_position, enemy_position)
	my_x, my_y, enemy_x, enemy_y = (my_position[0], my_position[1], enemy_position[0], enemy_position[1])
	if (my_x > enemy_x or my_y > enemy_y):
		print("reflect_board error")

	adjascent_x = abs(my_x - enemy_x) <= 1
	adjascent_y = abs(my_y - enemy_y) <= 1

	if (adjascent_x and adjascent_y):
		return 1
	elif (adjascent_x):
		# I will chase to upper side
		return turns_to_win(enemy_y - my_y, board_size - 1 - my_y)		
	elif (adjascent_y):
		# I will chase to right side
		return turns_to_win(enemy_x - my_x, board_size - 1 - my_x)
	else:
		# Enemy will run to upper side (because upper end of board if further than right end of board for me)
		return turns_to_win(enemy_y - my_y, board_size - 1 - my_y)

def main(input):
	lines = input.split("\n")

	def read_line():
		return lines.pop(0)

	board_size = int(read_line())
	my_position = [int(x) for x in read_line().split()]
	enemy_position = [int(x) for x in read_line().split()]
	#print(reflect_board(board_size, my_position, enemy_position))
	return str(solve(board_size, my_position, enemy_position))

import sys
#print(main(sys.stdin.read()))
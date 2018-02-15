# ビリヤードの球突き 

import math

# Solving simplified version (1-dimensional, radius 0)
def get_position_after_moving(table_width, initial_x, move_distance):
	virutal_final_x = initial_x  + move_distance
	# ball's position is periodic function of distance
	virutal_final_x = virutal_final_x % (table_width * 2)
	#print(virutal_final_x)
	if (virutal_final_x <= table_width):
		return virutal_final_x
	else:
		return table_width * 2 - virutal_final_x

def get_total_move_distance(L, theta):
	return (L * math.cos(math.radians(theta)), L * math.sin(math.radians(theta)))

# Consider only the center of the ball
def solve(a, b, x, y, r, theta, L):
	table_width = a - 2 * r
	table_height = b - 2 * r
	initial_x = x - r
	initial_y = y - r
	distance_moved_x, distance_moved_y = get_total_move_distance(L, theta)

	fianl_x = get_position_after_moving(table_width, initial_x, distance_moved_x)
	fianl_y = get_position_after_moving(table_height, initial_y, distance_moved_y)

	return (r + fianl_x, r + fianl_y)

def main(input):	
	a, b, x, y, r, theta, L = [int(x) for x in input.split()]
	result = solve(a, b, x, y, r, theta, L)
	return str(result[0]) + ' ' + str(result[1])

import sys
#print(main(sys.stdin.read()))

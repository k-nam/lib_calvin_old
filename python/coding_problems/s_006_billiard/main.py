test_a = "100 100 50 50 2 90 288"	# 50 50

test_b = "127 254 63 63 3 45 1000" # 44.1067811865475 227.893218813452

import math

input_text = test_b

for_paiza_submission = False
def read_line():
	if (for_paiza_submission):
		return input()
	else:
		return input_text

a, b, x, y, r, theta, L = [int(x) for x in read_line().split()]

# Solving simplified version (1-dimensional, radius 0)
def get_position_after_moving(table_width, initial_x, move_distance):
	virutal_final_x = initial_x  + move_distance
	# ball's position is periodic function of distance
	virutal_final_x = virutal_final_x % (table_width * 2)
	print(virutal_final_x)
	if (virutal_final_x <= table_width):
		return virutal_final_x
	else:
		return table_width * 2 - virutal_final_x

def get_total_move_distance(L, theta):
	return (L * math.cos(math.radians(theta)), L * math.sin(math.radians(theta)))

# Consider only the center of the ball
def solve():
	table_width = a - 2 * r
	table_height = b - 2 * r
	initial_x = x - r
	initial_y = y - r
	distance_moved_x, distance_moved_y = get_total_move_distance(L, theta)

	fianl_x = get_position_after_moving(table_width, initial_x, distance_moved_x)
	fianl_y = get_position_after_moving(table_height, initial_y, distance_moved_y)

	return (r + fianl_x, r + fianl_y)

result = solve()
print(str(result[0]) + ' ' + str(result[1]))
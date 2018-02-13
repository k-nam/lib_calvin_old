test_a = r"""3 2 4
223
224
114"""

answer_a = r"""3 2 1
4 2 2
1 1 2
2 1 1"""

test_b = r"""3 2 3
11.
11.
..."""

answer_b = r"""3 1 1
2 1 1
1 1 1"""

test_c = r"""10 5 9
.355555...
.3544444..
.35477777.
.35111117.
.35111117.
.99111117.
.99111117.
.99111118.
.999996...
.999996..."""

answer_c = r"""6 3 6
9 2 6
8 5 4
3 2 1
5 3 1
4 4 2
7 5 3
2 4 4
1 4 4"""


test_d = r"""5 3 3
111..
111..
11222
33222
33222"""

for_paiza_submission = False
test_input = test_b
lines = test_input.split('\n')

def read_line():
	if (for_paiza_submission):
		return input()
	else:
		return lines.pop(0)

room_size, carpet_size, num_colors = [int(x) for x in read_line().split()]

data = []
# 0 is naked floor
for i in range(room_size):
	data.append([0 if x == '.' else int(x) for x in read_line()])

def rotate(matrix, size):
	result = [[0 for y in range(size)] for x in range(size)]
	for i in range(size):
		for j in range(size):
			result[i][j] = matrix[j][i]
	return result

def find_one(matrix, room_size, carpet_size, margin):
	#print(margin)
	min_i, max_i, min_j, max_j = margin
	def is_in_range(i, j):
		return i <= min_i and i + carpet_size > max_i and \
			j <= min_j and j + carpet_size > max_j

	for i in range(room_size):
		for j in range(room_size):
			if (matrix[i][j] == 1 and is_in_range(i, j)):
				return (i, j)
	return False

def mark_as_void(matrix, position, carpet_size):
	for i in range(position[0], position[0] + carpet_size):
		for j in range(position[1], position[1] + carpet_size):
			matrix[i][j] = -1

# returns the upper left coordinate of the square if found
def find_square(data, room_size, carpet_size, color):
	#print('AA: ' + str(color))
	temp_matrix = [[0 for y in range(room_size)] for x in range(room_size)]

	# first, do horizontal pass: set value to 1 if there are equal to or more than carpet_size
	# consecutive color on the right side
	# remember that -1 means any color, so it always matches
	
	min_i = room_size
	max_i = 0
	min_j = room_size
	max_j = 0

	def mark_margins(i, j):
		pass

	def find_horizontal_runs(source, result, color, first_pass):
		for i in range(room_size):
			run_length = 0
			row = source[i]
			for j in range(room_size):
				if (row[j] == color or row[j] == -1):
					run_length +=1
					if (run_length >= carpet_size):
						result[i][j - carpet_size + 1] = 1
				else:
					run_length = 0
				if (row[j] == color and first_pass):
					nonlocal min_i, max_i, min_j, max_j
					min_i = min(min_i, i)
					max_i = max(max_i, i)
					min_j = min(min_j, j)
					max_j = max(max_j, j)	
	for i in range(room_size):
		find_horizontal_runs(data, temp_matrix, color, True)

	# now, run the same routine vertically
	rotated = rotate(temp_matrix, room_size)
	result = [[0 for y in range(room_size)] for x in range(room_size)]

	find_horizontal_runs(rotated, result, 1, False)
	return find_one(rotate(result, room_size), room_size, carpet_size, (min_i, max_i, min_j, max_j))


def solve():
	# find square, mark as void, and repeat
	remaining_colors = {}
	result = []
	for color in range(1, num_colors + 1):
		remaining_colors[color] = 1

	while (len(remaining_colors) > 0):
		for color in remaining_colors:
			square_position = find_square(data, room_size, carpet_size, color)
			if (square_position):
				#print(square_position)
				result.insert(0, (color, square_position))
				mark_as_void(data, square_position, carpet_size)
				del remaining_colors[color]
				break
	return result

#print(data)
#print(find_square(data, room_size, carpet_size, 1))

for row in solve():
	print(str(row[0]) + ' ' + str(row[1][1] + 1) + ' ' + str(row[1][0] + 1))

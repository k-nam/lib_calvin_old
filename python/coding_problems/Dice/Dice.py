from heapq import heappush, heappop

a = r"""3 4
1463
1261
3463""" # 2

b = r"""3 4
6463
3562
1463""" # 0

c = r"""5 3
146
222
631
555
146""" # 0

d = r"""5 6
101111
101001
101101
100101
111101""" # 0

input_text = c
lines = input_text.split("\n")
lines.pop(0)

data = []
for line in lines:
	data.append([int(x) for x in list(line)])


def is_inside(data, position):
	return position[0] >= 0 and position[0] < len(data) and \
		position[1] >= 0 and position[1] < len(data[0])

# (face, right, top)
def get_new_dice_orientation(direction, dice_orientation):
	def get_opposite_side(n):
		return 7 - n
	face = dice_orientation[0]
	right = dice_orientation[1]
	top = dice_orientation[2]
	if (direction == 0):
		return (get_opposite_side(top), right, face)
	elif (direction == 1):
		return (get_opposite_side(right), face, top)
	elif (direction == 2):
		return (top, right, get_opposite_side(face))
	elif (direction == 3):
		return (right, get_opposite_side(face), top)
	else:
		print('get_new_dice_orientation Error')

# previous looks like [position, dice_orientation]
def get_succesors(data, previous):
	def filter_succesors(succesors):
		return [x for x in succesors if is_inside(data, x[0])]

	succesors = []
	position = previous[0]
	original_dice_orientation = previous[1]
	top = ((position[0] - 1, position[1]), get_new_dice_orientation(0, original_dice_orientation))
	right = ((position[0], position[1] + 1), get_new_dice_orientation(1, original_dice_orientation))
	bottom = ((position[0] + 1, position[1]), get_new_dice_orientation(2, original_dice_orientation))
	left = ((position[0], position[1] - 1), get_new_dice_orientation(3, original_dice_orientation))

	return filter_succesors([top, right, bottom, left])


def solve(data):
	records = {}
	# position, dice_orientation => distance
	records[((0, 0), (1, 3, 2))] = 0

	# abstract_node: (position, dice_orientation)
	# elems in heap: (total distance, abstract_node)
	heap = [(0, ((0, 0), (1, 3, 2)))]
	finished = {}
	#print(finished)

	def relax(new_distance, succesor):
		if (succesor in records and records[succesor] <= new_distance):
			return False
		else:
			#print('relaxing: ' + str(succesor))
			records[succesor] = new_distance
			return True

	def cost_of(succesor):
		map_value = data[succesor[0][0]][succesor[0][1]]
		if (map_value == 0):
			return 1000000
		return abs(map_value - succesor[1][0])

	def is_finished(abstract_node):
		return abstract_node in finished;

	while (len(heap) > 0):
		elem = heappop(heap)
		#elem = heap.pop(0)
		if not is_finished(elem[1]):
			#print('Processing: ' + str(elem[1]) + ' cost: ' + str(elem[0]))
			succesors = get_succesors(data, elem[1])
			#print('Succesors: ' + str(succesors))
			for	succesor in succesors:
				# succesor: (position, dice_orientation)
				if (is_finished(succesor[0]) == False):
					relaxing_distance = elem[0] + cost_of(succesor)
					if (elem[0] != records[(elem[1])]):
						print('Error')
					if (relax(relaxing_distance, succesor)):							
						heappush(heap, (relaxing_distance, succesor))
						
			finished[elem[1]] = True

	min_distance = 1000000000
	destination = (len(data) - 1, len(data[0]) - 1)
	#print(destination)
	for key, value in records.items():
		#print(key[0])
		if (key[0] == destination):
			if value < min_distance:
				min_distance = value
	return min_distance

#print(records)
#print(min([1,2,3]))
#print(data)

print(solve(data))


from heapq import heappush, heappop
from random import randint

test_a = r"""5 5
2 2 2 2 2
2 0 0 0 1
2 0 0 0 1
2 0 0 0 1
2 1 1 1 1"""
# answer: 9

test_b = r"""5 5
4 4 4 4 4
4 3 3 3 4
4 3 2 3 4
4 3 3 3 4
4 4 4 4 4"""
# 10

test_c = r"""8 20
1 1 1 0 0 2 0 0 3 3 3 4 4 4 4 2 0 5 0 0
1 0 0 1 2 1 2 0 1 3 1 3 3 3 4 2 5 4 5 0
1 0 0 1 2 0 2 0 0 3 0 0 2 4 2 1 5 3 5 0
1 1 1 0 2 1 2 0 0 3 0 0 0 4 0 1 5 4 5 0
1 0 0 2 2 2 2 2 0 3 0 0 4 2 0 5 5 5 5 5
1 0 0 2 1 1 1 2 0 3 0 0 4 2 0 5 4 4 4 5
1 0 0 2 1 0 1 2 1 3 1 4 2 1 1 5 4 3 4 5
1 0 0 2 1 0 1 2 3 3 3 4 4 4 4 5 4 3 4 5"""
# 55

input_text = test_c
lines = input_text.split("\n")
for_paiza_submission = False

def read_line():
	if (for_paiza_submission):
		return input()
	else:
		return lines.pop(0)


map_size = [int(x) for x in read_line().split()]
land_data = []
water_data = []
maximum_water_value = 1000000 # this should be larger than highest land

for i in range(map_size[0]):
	land_data.append([int(x) for x in read_line().split()])
	water_data.append([maximum_water_value for x in range(map_size[1])])

# run this part to create randomized test data
'''
size = 300
map_size = (size, size)
land_data = [[randint(1, 300) for x in range(size)] for x in range(size)]
water_data = [[50000 for x in range(size)] for x in range(size)]
'''

def check_error():
	num_errors = 0
	for h in range(map_size[0]):
		for w in range(map_size[1]):
			position = [h, w]
			if (isOnTheVerge(position)):
				if (get_water_at(position) != land_at(position)):
					num_errors += 1
			else:
				min_of_adjascent = min([get_water_at(p) for p in getAdjascent(position)])
				if (get_water_at(position) < land_at(position)):
					num_errors += 1
				elif (get_water_at(position) == land_at(position)):
					pass
				else:
					if (get_water_at(position) != min_of_adjascent):
						print(str(get_water_at(position)) + ', ' + str(land_at(position)) + ', ' + str(min_of_adjascent)) 
						num_errors += 1
	if (num_errors > 0):
		print("check end, num errors was: " + str(num_errors))

def get_water_at(position):
	return water_data[position[0]][position[1]]

def land_at(position):
	return land_data[position[0]][position[1]]

def set_water_at(position, value):
	water_data[position[0]][position[1]] = value

def isOnTheVerge(position):
	return position[0] == 0 or position[0] == map_size[0] - 1 or \
			position[1] == 0 or position[1] == map_size[1] - 1

def getAllVerges():
	result = []
	for h in range(map_size[0]):
		for w in range(map_size[1]):
			if (isOnTheVerge([h, w])):
				result.append([h, w])
	return result

def getAdjascent(position):
	h, w = position
	candidates = [[h + 1, w], [h - 1, w], [h, w + 1], [h, w - 1]]
	return [p for p in candidates if p[0] >= 0 and p[0] < map_size[0] and \
						p[1] >= 0 and p[1] < map_size[1]]
	
def drainToLevel(position, level):
	myInitialLevel = get_water_at(position)
	if (myInitialLevel > level):
		set_water_at(position, max(land_at(position), level))
	elif (myInitialLevel < level):
		print("drainToLevel error")
	if (myInitialLevel > get_water_at(position)):
		return True # need to propagate
	else:
		return False
		
def drain_map():
	# points that have been relaxed are stored in this heap. 
	# min element cannot be further relaxed in the futre
	heap = []
	def push_to_heap(point):
		heappush(heap, [get_water_at(point), point])

	for point in getAllVerges():
		set_water_at(point, land_at(point))
		push_to_heap(point)		

	finished = [[False for x in range(map_size[1])] for x in range(map_size[0])]
	while (len(heap) > 0):
		currentWaterLevel, currentPosition = heappop(heap)
		if (finished[currentPosition[0]][currentPosition[1]] == False):
			for adjascent in getAdjascent(currentPosition):
				if (finished[adjascent[0]][adjascent[1]] == False):
					if (drainToLevel(adjascent, currentWaterLevel)):
						push_to_heap(adjascent) 
			finished[currentPosition[0]][currentPosition[1]] = True

def solve_problem():
	drain_map()
	containedWater = 0
	for h in range(map_size[0]):
		for w in range(map_size[1]):
			containedWater += get_water_at([h, w]) - land_at([h, w])	 
	print(containedWater)

solve_problem()
#check_error()

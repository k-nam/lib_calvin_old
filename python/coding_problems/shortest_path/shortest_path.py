test_a = r"""4 5
0 s 0 1
0 0 1 0
0 1 1 0
0 0 1 g
0 0 0 0"""
# 9

test_b = r"""4 4
0 s 0 1
1 0 0 0
0 1 1 1
0 0 0 g"""
# Fail

test_c = r"""6 6
s 0 0 1 0 0
0 1 1 1 0 0
0 1 0 0 0 0
0 1 0 0 0 0
0 1 0 1 0 0
0 0 0 1 0 g"""
# Fail

test_d = r"""2 2
s 1
1 g"""

from heapq import heappush, heappop

for_paiza_submission = False
test_input = test_d
lines = test_input.split('\n')

def read_line():
	if (for_paiza_submission):
		return input()
	else:
		return lines.pop(0)

# change of order of M and N is intentional
N, M = [int(x) for x in read_line().split()]

data = []
for i in range(M):
	data.append([x for x in read_line().split()])

start = (0, 0)
goal = (0, 0)

for i in range(M):
	for j in range(N):
		if (data[i][j] == 's'):
			start = (i, j)
			data[i][j] = -2
		elif (data[i][j] == 'g'):
			goal = (i, j)
			data[i][j] = -1
		elif (data[i][j] == '1'): # wall
			data[i][j] = -2		# -2 means wall and does not play any role
		elif (data[i][j] == '0'): # road
			data[i][j] = -1		# -1 means not reachable yet
		else:
			print('Error 1')

def relax(new_distance, position):
	current_distance = data[position[0]][position[1]]
	if (current_distance == -1 or new_distance < current_distance):
		data[position[0]][position[1]] = new_distance
		return True
	else:
		return False

def get_nexts(point):
	def is_inside(point):
		return point[0] >= 0 and point[0] < M and point[1] >= 0 and point[1] < N
	
	def is_wall(point):
		return data[point[0]][point[1]] == -2

	x, y = [point[0], point[1]]

	four_points = [(x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)]
	result = [x for x in four_points if is_inside(x) and not is_wall(x)]
	#print('Nexts: ' + str(result))
	return result

def get_shortest_distance():
	heap = [(0, start)]
	finished = []
	for i in range(M):
		finished.append([False for i in range(N)])

	def is_finished(point):
		return finished[point[0]][point[1]]

	while (len(heap) > 0):
		elem = heappop(heap)
		if (not is_finished(elem[1])):
			next_points = get_nexts(elem[1])
			for point in next_points:
				if (not is_finished(point)):
					if (relax(elem[0] + 1, point)):
						heappush(heap, (elem[0] + 1, point))
			finished[elem[1][0]][elem[1][1]] = True

	return data[goal[0]][goal[1]]

result = get_shortest_distance()
if (result > 0):
	print(result)
else:
	print('Fail')
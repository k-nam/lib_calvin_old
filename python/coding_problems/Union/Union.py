a = r"""2
3 0 0 1 2 2
2 0 1 2 2 1"""
# 6

b = r"""10
17 15 16 93 89 23
51 42 26 25 60 52
60 53 43 13 35 70
55 49 40 46 36 31
39 31 23 100 89 29
0 0 0 74 9 49
28 26 18 89 44 40
54 49 32 8 5 69
9 1 6 47 45 70
14 6 14 28 76 22"""
# 597935

input_text = b

lines = input_text.split("\n")
num_cuboid = int(lines.pop(0))
data = []
for line in lines:
	data.append(tuple(int(x) for x in line.split()))

def get_intersection(cuboid1, cuboid2):
	# each line is represented as (origin, len)
	def get_line_intersection(x, y):
		start = max(x[0], y[0])
		end = min(x[0] + x[1], y[0] + y[1])
		if (end - start > 0):
			return (start, end - start)
		else:
			return False
	x_intersection = get_line_intersection((cuboid1[0], cuboid1[3]), (cuboid2[0], cuboid2[3]))
	y_intersection = get_line_intersection((cuboid1[1], cuboid1[4]), (cuboid2[1], cuboid2[4]))
	z_intersection = get_line_intersection((cuboid1[2], cuboid1[5]), (cuboid2[2], cuboid2[5]))
	if (x_intersection and y_intersection and z_intersection):
		return (x_intersection[0], y_intersection[0], z_intersection[0], \
			x_intersection[1], y_intersection[1], z_intersection[1])
	else:
		return False

def get_volumn(cuboids):
	def volumn_of(cuboid):
		return cuboid[3] * cuboid[4] * cuboid[5]
	sum = 0
	for cuboid, sign in cuboids.items():
		this_volumn = volumn_of(cuboid)
		if sign == 1:
			sum += this_volumn
		else:
			sum -= this_volumn
	return sum

# -1 means that cuboid must be subtracted
def get_union(cuboids):
	if (len(cuboids) == 1):
		return {cuboids[0]: 1}
	last = cuboids.pop(len(cuboids) - 1)
	subset = get_union(cuboids)
	result = dict(subset)
	result[last] = 1
	for key, value in subset.items():
		intersection = get_intersection(key, last)
		if (intersection != False):
			if (intersection in result):
				result[intersection] = result[intersection] + value * -1
			else:
				result[intersection] = value * -1
			if (result[intersection] == 0):
				result.pop(intersection, 2)
	return result

#print(data)
#print(tuple(int(x) for x in range(5)))

print(get_volumn(get_union(data)))
a = r"""4 4 2
SCSC
CSSS
CCCC
SSSC"""
#8

b = r"""7 5 18
CCCSS
SCSCC
SSCCS
CSSSC
CCCSS
SSSSS
CCCCS"""
#16

c = r"""10 15 12345678
CCCSSCSSSCCSCCS
CCCCCSSCSSSSSCS
CSSSSSCSCCCCCSC
SSSCCSSSSCCSCSS
SCCCCCSCCCCCCSC
CCCSSCCCCSSSCCC
CSSCSCSCSSSCSSS
SSSSCSSCCSSSSSS
CSSCCSCSCCSSCSS
CCCSCCSSCCCSSCC"""
# 75

from random import randint


input_text = c

lines = input_text.split("\n")
h, w, num_iter = [int(x) for x in lines.pop(0).split()]

data = []

for line in lines:
    data.append([1 if x == "S" else 0 for x in list(line)])


#print(data)


def getRandomData(size):
	data = []
	for i in range(size):
		#data.append([1 if x == 0 else 0 for x in range(size)])
		data.append([randint(0, 1) for x in range(size)])
		#print(data[-1])
	return data

def rotate(array):
	return [list(row) for row in zip(*array)]

# return index (starting from 1, counting from the right) of 1's 
# 7 (111) -> [1, 2, 3]
# 8 (1000) -> [4]
def get_one_positions(number):
	i = 1
	result = []
	while (number != 0):
		if (number % 2):
			result.append(i)
		number = int(number / 2)
		i += 1
	return result


def shift_integer(bit_rep, row_size, direction):
	right_most_one = 1 # 000..0001
	left_most_one = 2**(row_size - 1) # 0001000..000
	mask = 2**row_size - 1 # 00011111111

	right_most_value = bit_rep & right_most_one # 000.....?
	left_most_value = bit_rep & left_most_one # 000?......

	if (direction == True):
		shifted_to_right = bit_rep >> 1 
		if (right_most_value > 0):
			shifted_to_right = shifted_to_right | left_most_one
		return shifted_to_right & mask
	else:
		shifted_to_left = bit_rep << 1 
		if (left_most_value > 0):
			shifted_to_left = shifted_to_left | right_most_one
		return shifted_to_left & mask

def bitrep_to_list(bitrep, row_size):
	result = []
	for i in range(row_size):
		if (bitrep >> (row_size - 1)):
			result.append(1)
		else:
			result.append(0)
		bitrep = shift_integer(bitrep, row_size, False)
	return result


def applySingleStep(bit_rep, row_size):
	return bit_rep ^ shift_integer(bit_rep, row_size, True) ^ shift_integer(bit_rep, row_size, False)

def calculate_unit(row_size, num_iter):
	# start with 1000.. and repeat XOR num_iter times
	# utilize cycle when detected
	bit_representation = 2**(row_size - 1)
	history = { bit_representation: 0 }
	history2 = [ bit_representation ]
	result = 0
	found_in_history = False

	for iter in range(1, num_iter + 1):
		bit_representation = applySingleStep(bit_representation, row_size)
		
		if (row_size < 200):
			if (bit_representation in history):
				prev_occurence = history[bit_representation]
				interval = iter - prev_occurence
				#print('interval: ' + str(interval))
				remaining_iter = (num_iter - iter) % interval
				result = history2[prev_occurence + remaining_iter]
				found_in_history = True
				break
			else:
				history[bit_representation] = iter
				history2.append(bit_representation);
		
	if (found_in_history == False):
		result = bit_representation
	#print('unit of len ' + str(row_size) + ', num_iter ' + str(num_iter) + ' was ' + str(result))
	return result
	
def calculate_unit2(row_size, num_iter):
	ones_in_num_iter= get_one_positions(num_iter)
	unit = 1 << (row_size - 1)
	if (num_iter == 0):
		return unit
	current_transform = applySingleStep(unit, row_size)
	transforms = [unit, current_transform]
	for i in range(1, ones_in_num_iter[-1] + 1):
		current_transform = composite(current_transform, current_transform, row_size)
		transforms.append(current_transform)

	current_transform = unit
	for i in ones_in_num_iter:
		current_transform = composite(current_transform, transforms[i], row_size)
	return current_transform

# note that transform defined in this problem is transitive and commutative
def composite(transform1, transform2, row_size):
	result = 0
	for i in range(row_size):
		if (transform1 >> (row_size - 1)): # leftmost digit is 1
			result = result ^ transform2
		transform1 = shift_integer(transform1, row_size, False)	
		result = shift_integer(result, row_size, False)
	return result

def process_row(row, unit):
	row_bit_rep = int(''.join([str(x) for x in row]), 2)
	row_size = len(row)	
	result = composite(row_bit_rep, unit, row_size)
	return bitrep_to_list(result, row_size)

def process_matrix(data, unit):
	return [process_row(row, unit) for row in data]

def solve(data, num_iter):
	unit = calculate_unit2(len(data[0]), num_iter)
	data = process_matrix(data, unit)

	data = rotate(data)

	unit = calculate_unit2(len(data[0]), num_iter)
	data = process_matrix(data, unit)

	sum = 0
	for row in data:
		for value in row:
			sum += value
	print(str(sum))
	
solve(data, num_iter)
solve(getRandomData(300), 300000000)


'''
print(calculate_unit(6, 3)) # 1, 42
print(calculate_unit(5, 5)) # 3, 22
print(calculate_unit(5, 18)) # 16
print(calculate_unit(7, 18)) # 76
'''
'''
print(calculate_unit2(6, 3)) # 1, 42
print(calculate_unit2(5, 5)) # 3, 22
print(calculate_unit2(5, 18)) # 16
print(calculate_unit2(7, 18)) # 76
'''
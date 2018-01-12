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

size = 100

def getRandomData():
	data = []
	for i in range(size):
		data.append([randint(0, 1) for x in range(size)])
	return data

def rotate(array):
	return [list(row) for row in zip(*array)]

def applyHorizontalXorOps(array, num_iter):
	for row in array:
		applyXorToRow(row, num_iter)

def applyXorToRow(row, num_iter):
	history = [list(row)]
	def search(row):
		#print('History: ')
		#print(history)
		#for i in range(len(history)):
		for i in range(1):
			if (history[i] == row):
				#print('cp 1' + str(history[i]))
				#print('cp 2' + str(row))
				return i
		return -1

	for iter in range(1, num_iter + 1):
		prev = list(row)
		for i in range(len(row)):
			previous = i - 1
			next = i + 1
			if (previous < 0):
				previous = len(row) - 1
			if (next == len(row)):
				next = 0
			row[i] = prev[i] ^ prev[previous] ^ prev[next]	
		#print('Row: ')
		#print(row)
		prev_occurence = search(row)
		if (prev_occurence >= 0):
			#print('Found !!!')
			interval = iter - prev_occurence
			remaining_iter = (num_iter - iter) % interval
			#row = history[prev_occurence + remaining_iter]
			for i in range(len(row)):
				row[i] = history[prev_occurence + remaining_iter][i]
			return
		else:
			history.append(list(row))

def solve(data, num_iter):
	applyHorizontalXorOps(data, num_iter)
	data = rotate(data)
	applyHorizontalXorOps(data, num_iter)
	sum = 0
	for row in data:
		for value in row:
			sum += value
	print(str(sum))

#print(rotate(data))
#applyHorizontalXorOps(data, num_iter)
#solve(data, num_iter)
solve(getRandomData(), 1000000000)


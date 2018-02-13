test_data = []

test_a = r"""abcdefg10h12(ij2(3k))l9mnop4(3(2(6(qq)r)s5tu)7v5w)x15(yz)"""
# 5 2 3 4 1 5

answer_a = r"""a 1
b 1
c 1
d 1
e 1
f 1
g 1
h 10
i 12
j 12
k 72
l 1
m 9
n 1
o 1
p 1
q 288
r 24
s 12
t 60
u 12
v 28
w 20
x 1
y 15
z 15"""

test_b = r"""10000(10000(10000(2000(ab)500(dz)c200h)2mu3000(fpr)))"""

answer_b = r"""a 2000000000000000
b 2000000000000000
c 1000000000000
d 500000000000000
e 0
f 300000000000
g 0
h 200000000000000
i 0
j 0
k 0
l 0
m 200000000
n 0
o 0
p 300000000000
q 0
r 300000000000
s 0
t 0
u 100000000
v 0
w 0
x 0
y 0
z 500000000000000"""

test_data.append((test_a, answer_a))
test_data.append((test_b, answer_b))

test_c = "e2(a2(bc))d"

# term => alphabet
# term => digit | term
# term => ( exp )
# exp => term | exp
# exp => 
	

def get_empty():
	result = { }
	for alphabet in [chr(i) for i in range(ord('a'), ord('z') + 1)]:
		result[alphabet] = 0
	return result

def multiply(data_set, number):
	for key in data_set:
		data_set[key] *= number
	return data_set

def sum(set1, set2):
	for key in set2:
		if key in set1:
			set1[key] += set2[key]
		else:
			set1[key] = set2[key]
	return set1

def solve(compressed_string):
	current_index = 0

	def parse_char(char):
		nonlocal current_index
		if (compressed_string[current_index] == char):
			current_index += 1
		else:
			print('parse_char errer')
	
	def parse_int():
		nonlocal current_index
		start_index = current_index
		while (compressed_string[current_index].isdigit()):
			current_index += 1
		substring = compressed_string[start_index : current_index]
		return int(substring)

	def parse_term():
		nonlocal current_index
		first_letter = compressed_string[current_index]		
		if (first_letter.isdigit()): 
			number = parse_int()	
			result = parse_term()
			return multiply(result, number)
		elif (first_letter == '('):
			current_index += 1
			result = parse_exp()
			parse_char(')')
			return result
		else: # alphabet
			current_index += 1
			return { first_letter: 1 }

	def parse_exp():
		if (current_index == len(compressed_string)):
			return get_empty()
		else:
			first_letter = compressed_string[current_index]
			if (first_letter == ')'):
				return get_empty()
			result1 = parse_term()
			result2 = parse_exp()
			return sum(result1, result2)

	result = parse_exp()
	result_string = ""
	for key in sorted(result):
		result_string = result_string + key + ' ' + str(result[key]) + '\n'
	return result_string.strip()
 


#print(multiply({'a': 2, 'b': 3}, 3))
#print(sum({'a': 2, 'b': 3 }, { 'b': 1, 'd': 3}))
#print(get_empty())

for_paiza_submission = False

if (for_paiza_submission):
	print(solve(get_paiza_input()))
else:
	for test_case in test_data:
		if (solve(test_case[0]) == test_case[1]):
			print('OK')
		else:
			print('Wrong')











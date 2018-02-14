test_a = r"""qpmnuthkahg
uyianhjak
jta"""
# 14

test_b = r"""zclgthiu
nyxomiaskq
giku"""
# 17

test_c = r"""ababababababababababababababababab
bababababababababababababababac
abababababababababababababababababababababababababc"""
# 51

test_d = r"""aacdd
bbbdd
abc"""

input_text = test_c
lines = input_text.split("\n")

for_paiza_submission = False
def read_line():
	if (for_paiza_submission):
		return input()
	else:
		return lines.pop(0)

mountain1 = read_line()
mountain2 = read_line()
key_string = read_line()

# (index in mountain1, index in mountain2, index in key_string => optimal cost)
# value of (0, 0, 0) is what we want
records = { }

# match to the first appearance of letter
def find_match_index(input_string, index, letter):
	for i in range(index, len(input_string)):
		if (input_string[i] == letter):
			return i
	# no match 
	return -1

fail_cost = len(mountain1) + len(mountain2)

def calculate(index1, index2, index_in_key):
	if ((index1, index2, index_in_key) in records):
		return records[(index1, index2, index_in_key)]

	if (index_in_key == len(key_string)):
		records[(index1, index2, index_in_key)] = index1 + index2
		return index1 + index2

	if (index1 == len(mountain1) or index2 == len(mountain2)):
		return fail_cost

	match_index1 = find_match_index(mountain1, index1, key_string[index_in_key])
	match_index2 = find_match_index(mountain2, index2, key_string[index_in_key])

	min_cost = fail_cost
	if (match_index1 >= 0):
		this_cost = calculate(match_index1 + 1, index2, index_in_key + 1)
		min_cost = min(min_cost, this_cost)
	if (match_index2 >= 0):
		this_cost = calculate(index1, match_index2 + 1, index_in_key + 1)
		min_cost = min(min_cost, this_cost)

	records[(index1, index2, index_in_key)] = min_cost
	return min_cost

def solve():
	return calculate(0, 0, 0)

print(solve())


# データヒストグラム 

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
 
def main(input):
	return solve(input.strip())

import sys
#print(main(sys.stdin.read()))

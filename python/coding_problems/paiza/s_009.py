# 辞書順最小

def apply_permutation(original, permutation):
	new_array = [0 for x in original]
	for i in range(len(original)):
		new_array[permutation[i] - 1] = original[i]
	return new_array

def apply_multiple_permutation(original, permutations):
	for permutation in permutations:
		original = apply_permutation(original, permutation)
	return original

# [1, 2] => [[1, 2], [2, 1]]
def get_all_permutation(input_array):
	if (len(input_array) == 0):
		return [[]]
	if (len(input_array) == 1):
		return [input_array]
	result = []
	first = input_array.pop(0)
	for subset_permutation in get_all_permutation(list(input_array)):
		#print(len(input_array))
		for i in range(0, len(input_array) + 1):
			temp = list(subset_permutation)
			temp.insert(i, first)
			result.append(temp)
	return result

# [1, 2] => [[], [1], [2], [1, 2]]
def get_all_subsets(input_array):
	if (len(input_array) == 0):
		return [[]]
	result = []
	first = input_array.pop(0)
	other_subset = get_all_subsets(input_array)
	for one_set in other_subset:
		result.append([first] + one_set)
		result.append(one_set)
	return result

# [1, 2] => [[], [1], [2], [1, 2], [2, 1]]
def get_all_permutation_of_all_subset(input_array):
	result = []
	for subset in get_all_subsets(input_array):
		#print(subset)
		for permutation in get_all_permutation(subset):
			result.append(permutation)
	return result



#print(apply_permutation([3, 1, 2], [1, 3, 2]))
#print(get_all_permutation([1, 2, 3]))
#print(len(get_all_subsets([1, 2, 3, 5, 4])))
#print(get_all_permutation_of_all_subset([1, 2, 3]))

def solve(original_string, given_permutations):
	result_string_list = []
	for selected_order in get_all_permutation_of_all_subset(list(range(1, len(given_permutations) + 1))):
		reordered_permutations = []
		for i in range(len(selected_order)):
			reordered_permutations.append(given_permutations[selected_order[i] - 1])
		#print(reordered_permutations)
		result_string_list.append(apply_multiple_permutation(original_string, reordered_permutations))

	first_string = sorted(result_string_list)[0]
	return " ".join(str(elem) for elem in first_string)

def main(input):
	lines = input.split("\n")

	def read_line():
		return lines.pop(0)

	length = int(read_line())
	original_string = [int(x) for x in read_line().split()]
	num_permutations = int(read_line())
	given_permutations = []

	for _ in range(num_permutations):
	   given_permutations.append([int(x) for x in read_line().split()])

	return solve(original_string, given_permutations)

import sys
#print(main(sys.stdin.read()))




test_a = r"""3
3 1 2
2
1 3 2
2 3 1"""
# 1 3 2

test_b = r"""5
1 4 2 5 3
2
5 2 1 4 3
1 3 4 5 2"""
# 1 3 4 2 5

input_text = test_b
lines = input_text.split("\n")
for_paiza_submission = False

def read_line():
	if (for_paiza_submission):
		return input()
	else:
		return lines.pop(0)

length = int(read_line())
original_string = [int(x) for x in read_line().split()]
num_permutations = int(read_line())
given_permutations = []

for i in range(num_permutations):
   given_permutations.append([int(x) for x in read_line().split()])

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


result_string_list = []
for permutation in get_all_permutation_of_all_subset(list(range(1, len(given_permutations) + 1))):
	reordered_permutations = []
	for i in range(len(permutation)):
		reordered_permutations.append(given_permutations[permutation[i] - 1])
	#print(reordered_permutations)
	result_string_list.append(apply_multiple_permutation(original_string, reordered_permutations))

first_string = sorted(result_string_list)[0]
print(" ".join(str(elem) for elem in first_string))
#print(apply_permutation([3, 1, 2], [1, 3, 2]))
#print(get_all_permutation([1, 2, 3]))
#print(len(get_all_subsets([1, 2, 3, 5, 4])))
#print(get_all_permutation_of_all_subset([1, 2, 3]))



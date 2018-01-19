a = r"""2 3 6""" # BCBA
b = r"""10 123 139""" # BAAABCBABCCBAABCB

input = b

# index t means end of output string, but inclusive
N, s, t = [int(x) for x in input.split()]


max_n = 51

calculated_length = []

# 1 => ABC
for i in range(max_n + 1):
	if i == 0:
		calculated_length.append(0)
	else:
		calculated_length.append(calculated_length[i - 1] * 2 + 3)

def length_of(N):
	return calculated_length[N] 

def letter_of(N, index):
	if (N == 1):
		return list("ABC")[index]
	else:
		length = length_of(N)
		middle_index = int(length / 2)
		if (index == 0):
			return "A"
		elif (index < middle_index):
			return letter_of(N - 1, index - 1)
		elif (index == middle_index):
			return "B"
		elif (index < length - 1):
			return letter_of(N - 1, index - 2 - length_of(N - 1))
		else:
			return "C"

def get_all_letters(N, begin, end):
	return [letter_of(N, x) for x in range(begin, end)]

		
#print(str(N), str(s), str(t))
#print(length_of(4))

print(''.join(get_all_letters(N, s - 1, t)))
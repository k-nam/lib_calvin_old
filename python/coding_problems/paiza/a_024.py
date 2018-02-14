# Dividing_items

def main(input):
	N, k = [int(x) for x in input.split()]
	found_answers = []
	for i in range (N + 1):
		found_answers.append([-1 for x in range(k + 1)])

	# what if cannot be divided? ex) (1234), (5678) which means minial prefix is the whole input
	# in this case, diversion goes up very high. For N == 4, minimum diversion is already 10 (1246), (3578)
	def solve_exact_unfair(N, k):
		if (N == 1):
			if (k == 1):
				return 2
			else:
				return 0
		elif (N == 2): #(12) (34) type
			if (k == 4):
				return 2
			else:
				return 0
		elif (N == 3): #(123)(456) or (124)(356) type
			if (k == 7 or k == 9):
				return 2
			else:
				return 0
		else: #(1245)(3678) 
			return 0

	def solve_exact(N, k):
		if (found_answers[N][k] >= 0):
			return found_answers[N][k]

		final_answer = 0

		if (N == 1):
			if (k == 1):
				final_answer = 2
			else:
				final_answer = 0
		elif (k == 1): # note that the case of N == 1 has already been taken care of
			final_answer = 0
		else:
			# dynamic programming by dividing N to (i, N-i)
			# left part is defined by minial prefix in which two players get same num of items
			total_num_case = 0
			for i in range(1, N):
				for left_part_k in range(1, k):
					total_num_case += solve_exact_unfair(i, left_part_k) * solve_exact(N - i, k - left_part_k)
			# add the case when the whole input is unfair
			total_num_case += solve_exact_unfair(N, k)
			final_answer = total_num_case

		found_answers[N][k] = final_answer;
		return final_answer

	def solve(N, k):
		sum = 0
		for i in range(1, k + 1):
			sum += solve_exact(N, i)
		return sum
	return str(solve(N, k))

import sys
#print(main(sys.stdin.read()))
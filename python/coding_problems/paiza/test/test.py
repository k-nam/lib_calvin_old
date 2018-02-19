import importlib

def run_test():
	# s_011, s_013 are unfinished
	problems = ["a_024", "b_044", "s_002", "s_004", "s_006", "s_007", "s_008", "s_009", "s_010", "s_011", \
		"s_013","s_014", "s_015", "s_016", "s_017", "s_018"];

	num_success = 0
	num_failure = 0
	print("Starting tests")
	for problem in problems:
		print("\tStarting to test: " + problem.upper(), end="...")
		problem_module = importlib.import_module(problem)
		test_case_module = importlib.import_module(problem + "_test")
		test_cases = getattr(test_case_module, "test_case")
		test_success = 0
		for case in test_cases:
			input = case[0]
			answer = case[1]
			output = getattr(problem_module, "main")(input)
			test_success = True
			def report_error():
				nonlocal test_success
				test_success= False
				print("\nTest error. Problem name: " + problem + " \nInput: " + input + "\nOutput: " + output +\
						"\nAnswer: " + answer)
			if (problem == "s_006"):
				error = 0.1 ** 10
				output1, output2 = [float(x) for x in output.split()]
				answer1, answer2 = [float(x) for x in answer.split()]
				if (abs(output1 - answer1) > error or abs(output2 - answer2) > error):
					report_error()
			else:
				if (answer != output):
					report_error()

		if (test_success):
			num_success += 1
		else:
			num_failure += 1
		print("\tFinished")

	print("\nTest result\nTotal: " + str(len(problems)) + " problems\nSucess: " + \
		str(num_success) + "\nFailure: " + str(num_failure))

run_test()







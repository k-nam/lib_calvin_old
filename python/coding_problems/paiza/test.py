problems = ["a_024", ];

test_data = { }

def register_test_case(problem_name, input, answer):
	if (problem_name not in test_data):
		test_data[problem_name] = []
	test_data[problem_name].append((input, answer))
	pass

print("Starting tests\n\n")

for problem in problems:
	problem_module = __import__(problem)
	test_case_module = __import__(problem + "_test")
	test_cases = getattr(test_case_module, "test_case")
	for case in test_cases:
		input = case[0]
		answer = case[1]
		output = getattr(problem_module, "main")(input)
		if (answer != output):
			print("Test error. Problem name: " + problem + " \nInput: " + input + "\nOutput: " + output +\
				"\nAnswer: " + answer + "\n\n")

print("All tests completed")







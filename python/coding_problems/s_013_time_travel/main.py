test_a = r"""5
5 10
ssdss"""
# 5 2 3 4 1 5

test_b = r"""4
18 12
sdds"""
# 4 1 3 2 1 4


for_paiza_submission = False
test_input = test_a
lines = test_input.split('\n')

def read_line():
	if (for_paiza_submission):
		return input()
	else:
		return lines.pop(0)

timeline_size = int(read_line())
cost_future, cost_back = [int(x) for x in read_line().split()]
timeline_status = list(read_line())


# returns (optimal_cost, path)
def solve(status, visited, position):
	# just need to reach rightmost position
	if (len(visited) == timeline_size):
		if (status[timeline_size - 1] == 's'):
			return (cost_future, [timeline_size - 1])


print(timeline_status)
# 理想のタイムトラベル
# unfinished

# Status: (current_node, [safe, dangerous, ...], [visited, unvisited, ...], [history])
# current_node: [0, size)
# history does not include current_node
def get_next_status(status, next_node):
	safety = list(status[1])
	for i in range(len(safety)):
		if (next_node < i):
			safety[i] = not safety[i]
	
	visited = list(status[2])
	visited[next_node] = True

	history = list(status[3])
	history.append(status[0])

	return (next_node, safety, visited, history)

# Move to one of unvisited place if current node has been visited second time already
# otherwise, move to anywhere safe
def get_all_next_status(status):
	result = [] # possible next status
	safe_nodes = []
	for i in range(len(status[1])):
		if (status[1][i] and status[0] != i): # cannnot stay at the same node
			safe_nodes.append(i)

	is_revisiting = status[0] in status[3]
	for i in safe_nodes:
		if (is_revisiting): # revisiting current node
			if (not status[2][i]):
				result.append(get_next_status(status, i))
		else:
			result.append(get_next_status(status, i))

	return result

def is_finished(status, timeline_size):
	return status[0] == timeline_size - 1 and all(elem == True for elem in status[2])

def solve(size, cost_future, cost_back, timeline_status):
	initial_status = (size - 1, timeline_status, [False for _ in range(size)], [])
	infite_cost = 1000000000
	
	# return (final status of optimal solution, optimal cost)
	def get_optimal(current_status, current_cost):
		if (is_finished(current_status, size)):
			return (current_status, current_cost)

		next_status_list = get_all_next_status(current_status)
		min_cost = infite_cost
		optimal_final_status = 0
		for next_status in next_status_list:
			if (next_status[0] > current_status[0]):
				this_move_cost = cost_future
			else:
				this_move_cost =  cost_back

			final_status, optimal_cost = get_optimal(next_status, current_cost + this_move_cost)
			if (optimal_cost < min_cost):
				optimal_final_status = final_status
				min_cost = optimal_cost

		return (optimal_final_status, min_cost)

	return get_optimal(initial_status, 0)

def main(input):
	lines = input.split('\n')

	def read_line():
		return lines.pop(0)

	timeline_size = int(read_line())
	cost_future, cost_back = [int(x) for x in read_line().split()]
	timeline_status = [elem == 's' for elem in list(read_line())]
	
	final_status, cost = solve(timeline_size, cost_future, cost_back, timeline_status)
	history = final_status[3] + [final_status[0]]
	#print(final_status)
	#print("cost: " + str(cost))
	return str(cost)
	
	# For Paiza submission
	#return " ".join([str(elem + 1) for elem in history]) 

import sys
#print(main(sys.stdin.read()))
#include <deque>
#include <utility>
#include <exception>
#include "graph.h"

using std::deque;
using std::pair;
using std::cout;
using std::endl;

// For graph algorithms (priority queue)
using namespace lib_calvin_container;

void lib_calvin_graph::dfs(vector<vector<size_t>> const &graph,
		vector<size_t> const &visitOrder, vector<size_t> &returnOrder) {
	dfs2(graph, visitOrder, returnOrder);
}

// dfs1 is useless. I implemented IntStack for this algorithm, but it is a waste of time.
// only left for hindsight
// 2015-12-14. Replaced IntStack to Stack. The size of stack will grow to O(E), which is bad, 
// but this is much faster than using IntStack. Still not preferable over dfs2
void lib_calvin_graph::dfs1(vector<vector<size_t>> const &graph,
	vector<size_t> const &visitOrder, vector<size_t> &returnOrder) {
	size_t numV = graph.size();
	// in dfs1, stack stores nodes that haven't been visited yet.
	Stack<size_t> stack;
	// stores whether the vertex has been visited or not
	vector<bool> isVisited(numV, false);
	vector<bool> hasReturned(numV, false);
	// push all vertices in reverse order (this is a stack)
	vector<size_t>::const_reverse_iterator iter;
	for (iter = visitOrder.rbegin(); iter != visitOrder.rend(); ++iter) {
		stack.push(*iter);
	}
	// If I cleared returnOrder before this, it is a bug (consider when 
	// ..visitOrder and returnOrder are the same object!!)
	returnOrder.clear();
	while (stack.size() != 0) {
		size_t top = stack.peek();
		if (isVisited[top] == true) { // already visited, need pop
			stack.pop();
			if (hasReturned[top] == false) {
				returnOrder.push_back(top); // save return record
				hasReturned[top] = true;
			}
		} else { // visiting now
			isVisited[top] = true;
			size_t src = top;
			// push every unvisited adjacent vertices
			for (auto iter = graph[src].rbegin(); iter != graph[src].rend(); ++iter) {
				size_t dest = *iter;
				if (isVisited[dest]) { // do nothing for already visited vertex
					continue;
				} else {
					stack.push(dest);
				}
			}
		}
	}
}

// Using simple stack method; much faster than dfs using priority queue
void lib_calvin_graph::dfs2(vector<vector<size_t>> const &graph,
	vector<size_t> const &visitOrder, vector<size_t> &returnOrder) {
	size_t numV = static_cast<size_t>(graph.size());
	size_t index = 0; // indicates a posize_t of visitOrder to visit
	size_t curVertex; // current operation vertex
	Stack<size_t> stack; // in dfs2, stack stores current execution path of the entire algorithm.
	vector<size_t> edgeSelector(numV, 0); // Edge to choose next (for each vertex)
	vector<bool> isVisited(numV, false);
	vector<size_t> visitOrderCopy(visitOrder); // Check for aliasing
	returnOrder.clear(); // Now safe to clear

	while (true) {
		while (index < numV && isVisited[visitOrderCopy[index]] == true) {
			++index;
		}
		if (index == numV) { // all done
			return;
		}
		curVertex = visitOrder[index]; // initialize
		while (true) { // execute dfs starting from given vertex
			isVisited[curVertex] = true; // mark as visited
			while (edgeSelector[curVertex] < graph[curVertex].size() &&
				isVisited[graph[curVertex][edgeSelector[curVertex]]] == true) {
				edgeSelector[curVertex]++;
			}
			if (edgeSelector[curVertex] == graph[curVertex].size()) { // this vertex is to return
				returnOrder.push_back(curVertex);
				if (stack.size() == 0) {
					break; // done for this source vertex
				} else {
					curVertex = stack.pop();
					continue;
				}
			} else { // there are still some places to go
				stack.push(curVertex);
				curVertex = graph[curVertex][edgeSelector[curVertex]];
				continue;
			}
		}
	}
}

void lib_calvin_graph::bfs(vector<vector<size_t>> const &graph, size_t source, vector<Tail<size_t>> &result) {
	result.clear();
	result.resize(graph.size());
	for (size_t i = 0; i < graph.size(); ++i) {
		result[i].predecessor_ = UNREACHABLE_VERTEX; // mark as unreachable 
	}
	// My convention for source vertex
	result[source].predecessor_ = source;
	result[source].weight_ = 0;
	// Queue for bfs
	deque<size_t> queue(graph.size());
	// black (undiscovered), grey (frontier), white (finished)
	// Only black vertices are not yet visited.
	vector<bool> isVisited(graph.size(), false);
	// Initialization
	queue.push_back(source);
	isVisited[source] = true;
	while (!queue.empty()) {
		size_t front = queue.front();
		size_t distance = result[front].weight_;
		size_t src = front; // rename
		for (size_t i = 0; i < graph[src].size(); ++i) {
			size_t target = graph[src][i];
			if (isVisited[target] == false) { // if newly discovered, relax it
				isVisited[target] = true;
				result[target].predecessor_ = front;
				result[target].weight_ = distance + 1;
				queue.push_back(target);
			}
		}
		// This vertex is finished
		queue.pop_front();
	}
}




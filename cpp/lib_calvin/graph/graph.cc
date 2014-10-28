
#include <deque>
#include <utility>
#include <exception>
#include "graph.h"

using std::deque;
using std::pair;
using std::cout; 
using std::endl;


/**************************** Algorithms ****************************/
 
// For graph algorithms (priority queue)
using namespace lib_calvin_adt;

void lib_calvin_graph::dfs (vector<vector<int>> const &graph, 
    vector<int> const &visitOrder, vector<int> &returnOrder) {
   dfs2(graph, visitOrder, returnOrder);
}

// dfs1 is useless. I implemented IntStack for this algorithm, but it is a waste of time.
// only left for hindsight
void lib_calvin_graph::dfs1 (vector<vector<int>> const &graph, 
															vector<int> const &visitOrder, vector<int> &returnOrder) {
  int numV = static_cast<int>(graph.size()); 
  IntStack stack(numV);
  // stores whether the vertex has been visited or not
  vector<bool> isVisited(numV, false); 
  // push all vertices in reverse order (this is a stack)
  vector<int>::const_reverse_iterator iter;
  for (iter = visitOrder.rbegin(); iter != visitOrder.rend(); ++iter) {
    stack.push (*iter);
  }
  // If I cleared returnOrder before this point, it is a bug (consider when 
  // ..visitOrder and returnOrder are the same object!!)
  returnOrder.clear();
  while (stack.size() != 0) {
    int top = stack.peek(); 
    if (isVisited[top] == true) { // already visited, need pop
      stack.pop();
      returnOrder.push_back(top); // save return record
    } else { // visiting now
      isVisited[top] = true;
			int src = top;
      // push every unvisited adjacent vertices
			for (size_t i = 0; i < graph[src].size(); ++i) {
        int dest = graph[src][i];
				if (isVisited[dest]) { // do nothing for already visited vertex
          continue;
				}
				else {
          stack.push(dest);
				}
      }
    }
  }
}

// Using simple stack method; much faster than dfs using priority queue
void lib_calvin_graph::dfs2 (vector<vector<int>> const &graph, 
															vector<int> const &visitOrder, vector<int> &returnOrder) {
  int numV = static_cast<int>(graph.size());
  int index   = 0; // indicates a point of visitOrder to visit
  int curVertex; // current operation vertex
  Stack<int> stack;
  vector<int> edgeSelector(numV, 0); // Edge to choose next (for each vertex)
  for (int src = 0; src < numV; src++) {
    edgeSelector[src] = static_cast<int>(graph[src].size()) - 1;
  }
  vector<bool> isVisited(numV, false);
  vector<int> visitOrderCopy(visitOrder); // Check for aliasing
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
      while (edgeSelector[curVertex] >= 0 &&
          isVisited[graph[curVertex][edgeSelector[curVertex]]] == true) {
        edgeSelector[curVertex]--;
      }
      if (edgeSelector[curVertex] < 0) { // this vertex is to return
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

void lib_calvin_graph::bfs (vector<vector<int>> const &graph, int source, vector<Arc<int>> &result) {
  result.clear();
  result.resize(graph.size());
	for (size_t i = 0; i < graph.size(); ++i) {
    result[i].predecessor_ = -1; // mark as unreachable 
	}
  // My convention for source vertex
  result[source].predecessor_ = source; 
  result[source].weight_ = 0;
  // Queue for bfs
  deque<int> queue(graph.size());
  // black (undiscovered), grey (frontier), white (finished)
	// Only black vertices are not yet visited.
  vector<bool> isVisited(graph.size(), false);
  // Initialization
  queue.push_back(source);
  isVisited[source] = true;
  while (!queue.empty()) {
    int front = queue.front();
    int distance = result[front].weight_;
		int src = front; // rename
    for (size_t i = 0; i < graph[src].size(); ++i) {
			int target = graph[src][i];
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




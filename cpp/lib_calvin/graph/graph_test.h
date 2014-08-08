#ifndef LIB_CALVIN__GRAPH__GRAPH_TEST_H
#define LIB_CALVIN__GRAPH__GRAPH_TEST_H
/*
 * 2008-02-04
 */

// Include additional test to see difference of array and dynamic(set)
// graph data in bfs
//#define BFS_GRAPH_DATA_STRUCTURE

#include "graph.h"
#include "stopwatch.h"

namespace lib_calvin_graph {

	void graphTest();
	void algorithmTest(int numV, int numE, int numNegativeE);
	void get_closest_pathTest();
	void insertionTest();

	struct SampleEdge {
		SampleEdge(int weight = 0): weight_(weight) { }
		int getWeight() const {
			return weight_; }
		int weight_;
	};
	struct SampleEdgeWeight {
		int operator()(SampleEdge sampleEdge) {
			return sampleEdge.getWeight();}
	};

	struct my {
		my(): value_(0) { }  
		my(int value): value_(value) { }
		operator int() { return value_; }
		bool operator== (const struct my &rhs) const {
			if (value_ == rhs.value_) return true;
			return false;
		}
		bool operator!= (struct my const &rhs) const {
			return !(operator==(rhs));
		}
		bool operator< (struct my const &rhs) const {
			if (value_ < rhs.value_) return true;
			return false;
		}
		bool operator<= (struct my const &rhs) const {
			if (value_ <= rhs.value_) return true;
			return false;
		}
		bool operator> (struct my const &rhs) const {
			if (value_ > rhs.value_) return true;
			return false;
		}
		struct my & operator= (struct my const &rhs) {
			value_ = rhs.value_;
			return *this;
		}
		struct my & operator+= (struct my const &rhs) {
			value_ += rhs.value_;
			return *this;
		}
		struct my & operator-= (struct my const &rhs) {
			value_ -= rhs.value_;
			return *this;
		}
		struct my const operator+ (struct my const &rhs) const {
			return my(value_ + rhs.value_);
		}
		int value_;
		int dummy1;
		//int dummy2;
		//int dummy3;
		//double dummy4;
	};
} // end namespace lib_calvin_graph


#endif
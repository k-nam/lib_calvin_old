#pragma once

#include <vector>

struct user {
	int group_;
	int score_;
};

class user_group_compare {
public:
	bool operator()(user const &user1, user const &user2) const {
		return user1.group_ < user2.group_;
	}
};

class user_score_compare {
public:
	bool operator()(user const &user1, user const &user2) const {
		return user1.score_ < user2.score_;
	}
};

class user_all_compare {
public:
	bool operator()(user const &user1, user const &user2) const {
		return user1.group_ < user2.group_ ||
			(user1.group_ == user2.group_ && user1.score_ < user2.score_);
	}
};


void record_sort_two_phase(std::vector<user> &input) {

}
#pragma once

#include <vector>

struct user {
	size_t group_;
	size_t score_;

	bool operator==(user const &rhs) const {
		return group_ == rhs.group_ && score_ == rhs.score_;
	}
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

void user_sort_test();
void user_sort_one_pass(std::vector<user> &input);
void user_sort_two_pass(std::vector<user> &input);
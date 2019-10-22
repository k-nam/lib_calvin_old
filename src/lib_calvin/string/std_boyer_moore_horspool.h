#ifndef LIB_CALVIN__STRING__STD_BOYER_MOORE_H
#define LIB_CALVIN__STRING__STD_BOYER_MOORE_H

namespace lib_calvin_string
{
	template <typename Alphabet>
	void std_boyer_moore_horspool(lib_calvin::abstract_string<Alphabet> const &text,
									 lib_calvin::abstract_string<Alphabet> const &pattern, std::vector<size_t> &result) {

		auto matchStart = text.begin();
		while (true) {
			matchStart = std::search(matchStart, text.end(),
									 std::boyer_moore_horspool_searcher<decltype(matchStart)>(
									 pattern.begin(), pattern.end()));
			if (matchStart == text.end()) {
				break;
			} else {
				//std::cout << "The string " << pattern << " found at offset "<< matchStart - text.begin() << '\n';
				result.push_back(matchStart - text.begin());
				matchStart++;
			}
		}
	}
}

#endif
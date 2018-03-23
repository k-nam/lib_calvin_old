#ifndef LIB_CALVIN__SORT__BUCKET_SORT_H
#define LIB_CALVIN__SORT__BUCKET_SORT_H

#include <vector>
#include <iterator>

namespace lib_calvin_sort
{
	using std::vector;
	using std::iterator_traits;

	// Comparison sort with sampling, binary search, and buckets
	// Not in-place, stable. Cache optimal. 
	template <typename Iterator, typename Comparator = std::less<typename std::iterator_traits<Iterator>::value_type>>
	void sampleSort(Iterator first, Iterator last, Comparator const comp = Comparator());

	// Binary search with predefined set of elements (gotten from sorting sample set)
	// Returns the index to which the given element is to belong [0, partitioners.size()]
	template <typename Iterator, typename Comparator = std::less<std::iterator_traits<Iterator>::value_type>>
	size_t binSearch(typename std::iterator_traits<Iterator>::value_type elem,
					 Iterator firstPartitioner, Iterator lastPartitioner, Comparator comp = Comparator());

	// Select samples from given array. 
	// ratio = # samples / # totalElems, but only roughly (accuracy not guaranteed)
	// result will be a sorted array of different samples
	template <typename Iterator>
	void getSamples(Iterator first, Iterator last, size_t sampleSize,
					vector<typename iterator_traits<Iterator>::value_type> &result);

	template <typename T>
	void removeDuplicateElements(vector<T> const &original, vector<T> &copy);
}

template <typename Iterator, typename Comparator>
void lib_calvin_sort::sampleSort(Iterator first, Iterator last, Comparator comp) {
	typedef typename iterator_traits<Iterator>::value_type elemType;
	typedef typename iterator_traits<Iterator>::pointer pointerType;
	vector<elemType> partitioners;
	int bucketSize = L2_CACHE_SIZE / sizeof(elemType) / 2;
	size_t sampleSize = (last - first) / bucketSize;
	if (sampleSize == 0) {
		sampleSize = 1;
	}

	getSamples(first, last, sampleSize, partitioners);

	size_t numberOfPartitions = partitioners.size() + 1;
	vector<vector<elemType>> queues(numberOfPartitions);
	for (size_t i = 0; i < numberOfPartitions; ++i) {
		queues[i].reserve(bucketSize);
	}
	for (Iterator iter = first; iter < last; iter++) {
		//size_t bucketIndex = binSearch(*iter, &*partitioners.begin(), 
		//&*partitioners.begin() + partitioners.size());
		size_t bucketIndex = binSearch(*iter, partitioners.begin(), partitioners.end());
		queues[bucketIndex].push_back(*iter);
	}
	Iterator target = first;
	for (size_t i = 0; i < numberOfPartitions; ++i) {
		if (queues[i].empty())
			continue;
		mergeSort(queues[i].begin(), queues[i].end());
		for (size_t j = 0; j < queues[i].size(); j++) {
			*target++ = queues[i][j];
		}
	}
}

template <typename Iterator, typename Comparator>
size_t lib_calvin_sort::binSearch(typename std::iterator_traits<Iterator>::value_type elem,
								  Iterator first, Iterator last, Comparator comp) {
	//return 1;
	//static int random = 0;
	//return random++ % (partitioners.size() + 1);
	if (comp(*first, elem) == false) {
		return 0;
	}
	//int left = 0, right = partitioners.size() - 1, middle = (left + right) / 2;
	Iterator left = first, right = last, middle = left + (right - left) / 2;
	while (right - left > 1) {
		if (comp(*middle, elem)) {
			left = middle;
		} else {
			right = middle;
		}
		middle = left + (right - left) / 2;
	}
	return right - first;
}

template <typename Iterator>
void lib_calvin_sort::getSamples(Iterator first, Iterator last, size_t sampleSize,
								 vector<typename iterator_traits<Iterator>::value_type> &result) {

	typedef typename iterator_traits<Iterator>::value_type elemType;
	size_t const overSamplingRate = 20;
	ptrdiff_t size = last - first;
	size_t numberOfElementsInCacheLine = CACHE_LINE_SIZE / sizeof(elemType);
	if (numberOfElementsInCacheLine == 0) {
		numberOfElementsInCacheLine = 1;
	}
	vector<elemType> tempResult; // may contain duplicate elements
	size_t candidateSize = overSamplingRate * sampleSize;
	for (size_t resultSize = 0; resultSize < candidateSize; ) {
		ptrdiff_t startIndex = rand() % size;
		ptrdiff_t endIndex = startIndex + numberOfElementsInCacheLine;
		if (endIndex > size) {
			endIndex = size;
		}
		for (ptrdiff_t index = startIndex; index < endIndex; index++) {
			tempResult.push_back(*(first + index));
			resultSize++;
		}
	}
	// Sort and remove duplicate elements
	introSort(tempResult.begin(), tempResult.end());
	vector<elemType> tempResult2; // without duplicate elements
	removeDuplicateElements(tempResult, tempResult2);
	result.clear();
	result.reserve(sampleSize);

	for (ptrdiff_t i = overSamplingRate / 2; i < static_cast<ptrdiff_t>(tempResult2.size()); i += overSamplingRate) {
		result.push_back(tempResult2[i]);
	}
	if (result.empty()) {
		result.push_back(*first);
	}
}

template <typename T>
void lib_calvin_sort::removeDuplicateElements(vector<T> const &original,
											  vector<T> &copy) {
	copy.clear();
	copy.reserve(original.size());
	copy.push_back(original[0]);
	for (size_t i = 1; i < original.size(); ++i) {
		if (original[i - 1] < original[i]) {
			copy.push_back(original[i]);
		}
	}
}

#endif
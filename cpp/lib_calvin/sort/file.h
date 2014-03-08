#include <iostream>
#include <fstream>

namespace lib_calvin_sort
{
template <typename T>
class file_iterator_value {
public:
	file_iterator_value(std::fstream &file, size_t offset): 
			file_(&file), offset_(offset) { }
	operator T() const;
	// assignment operator writes to the file
	void operator=(T const &rhs);
private:
	std::fstream *file_;
	size_t offset_;
};

template <typename T>
class file_iterator {  
public:
	typedef T value_type;
	typedef T & reference;
	typedef T * pointer;
	typedef ptrdiff_t difference_type;
	typedef std::random_access_iterator_tag iterator_category;
	
public:
	file_iterator(): file_(NULL), offset_(0) { }
	file_iterator(std::fstream &file, size_t offset): file_(&file), offset_(offset) { }
	file_iterator(file_iterator const &rhs): file_(rhs.file_), offset_(rhs.offset_) { }
	file_iterator & operator=(file_iterator const &rhs);
public:
	file_iterator & operator++() { offset_++; return *this; }
	file_iterator & operator--() { offset_--; return *this; }
	file_iterator const operator++(int) { file_iterator copy(*this); ++(*this); 
		return copy; }
	file_iterator const operator--(int) { file_iterator copy(*this); --(*this); 
		return copy; }
	file_iterator_value<T> 
		operator*() const { return file_iterator_value<T>(*file_, offset_); }
public:
	size_t getOffset() const { return offset_; }
	void setOffset(int offset) { offset_ = offset; }
	difference_type operator-(file_iterator const &rhs) const 
		{ return offset_ - rhs.offset_; }
	file_iterator const operator+(ptrdiff_t diff) const
		{ return file_iterator(*file_, offset_ + diff); }
	file_iterator const operator-(ptrdiff_t diff) const
		{ return file_iterator(*file_, offset_ - diff); }
public:
	friend bool operator==(file_iterator const &lhs, file_iterator const &rhs) {
		return lhs.offset_ == rhs.offset_; }
	friend bool operator!=(file_iterator const &lhs, file_iterator const &rhs) {
		return lhs.offset_ != rhs.offset_; }
	friend bool operator< (file_iterator const &lhs, file_iterator const &rhs) {
		return lhs.offset_ < rhs.offset_; }
	friend bool operator> (file_iterator const &lhs, file_iterator const &rhs) {
		return lhs.offset_ > rhs.offset_; }
	friend bool operator<= (file_iterator const &lhs, file_iterator const &rhs) {
		return lhs.offset_ <= rhs.offset_; }
	friend bool operator>= (file_iterator const &lhs, file_iterator const &rhs) {
		return lhs.offset_ >= rhs.offset_; }
private:
	std::fstream *file_;
	size_t offset_;
};

template <typename Iterator>
void writeToFile(Iterator first, Iterator last, std::string);

template <typename T>
std::pair<file_iterator<T>, file_iterator<T>> openFile(std::string fileName);

//-------------------------- file_iterator_value impl ---------------------//
template <typename T>
file_iterator_value<T>::operator T() const {
	char object[sizeof(T)];
	file_->seekg(offset_*sizeof(T), std::ios::beg);	
	file_->read(object, sizeof(T));
	return *(T *)object;
}

template <typename T>
void
file_iterator_value<T>::operator= (T const &rhs) {
	char *object = (char *)(&rhs);
	file_->seekp(offset_*sizeof(T), std::ios::beg);
	file_->write(object, sizeof(T));
}

template <typename T>
file_iterator<T> &
file_iterator<T>::operator= (file_iterator const &rhs) {
	if (this == &rhs) {
		return *this;
	}
	file_ = rhs.file_;
	offset_ = rhs.offset_;
	return *this;
}

//------------------------- functions definitions -----------------------//
template <typename Iterator>
void writeToFile(Iterator first, Iterator last, std::string fileName) {
	typedef typename std::iterator_traits<Iterator>::value_type valueType;
	Iterator cur = first;
	std::fstream file(fileName, std::ios::out | std::ios::binary);
	file_iterator<valueType> fileIter(file, 0);
	for (; cur != last; ) {
		*fileIter++ = *cur++;
	}
	file.close();
}

}
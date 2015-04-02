#ifndef LIB_CALVIN__MATRIX__MATRIX_H
#define LIB_CALVIN__MATRIX__MATRIX_H 

#define NOMINMAX

#include <iostream>
#include "utility.h" 
#include "stopwatch.h"
#include "common_thread.h"


// type T should have default constructor as initializer
namespace lib_calvin_matrix 
{
template <typename T>
class matrix_base;
}

namespace lib_calvin 
{
// Matrix only for viewing (does not manage its own data)
template <typename T>
class view_matrix;

template <typename T>
class matrix;
}

namespace lib_calvin_matrix
{
using lib_calvin::matrix;

// ---------------------- multiply wrapping functions ---------------------

template <typename T>
matrix<T> const
matrixMultiply(matrix<T> const &lhs, matrix<T> const &rhs);

template <typename T>
void matrixMultiAdd(matrix<T> const &lhs, matrix<T> const &rhs, 
										matrix<T> &result);

//--------------------------- size_termediates ---------------------------

template <typename T>
void 
naiveMultiAdd(matrix<T> const &lhs, matrix<T> const &rhs, matrix<T> &result);

template <typename T>
void 
naiveMultiAdd2(matrix<T> const &lhs, matrix<T> const &rhs, matrix<T> &result);

template <typename T>
void 
naiveMultiAdd3(matrix<T> const &lhs, matrix<T> const &rhs, matrix<T> &result);

template <typename T>
void 
simpleMultiAdd(matrix<T> const &lhs, matrix<T> const &rhs, matrix<T> &result);

template <typename T>
void
blockedMultiAdd(matrix<T> const &lhs, matrix<T> const &rhs, matrix<T> &result);

template <typename T>
void
recursiveMultiAdd(matrix<T> const &lhs, matrix<T> const &rhs, matrix<T> &result);

template <typename T>
void
recursiveMultiAddParallel(matrix<T> const &lhs, matrix<T> const &rhs, matrix<T> &result);

template <typename T>
void
recursiveMultiAdd(matrix<T> const &lhs, matrix<T> const &rhs, matrix<T> &result,
	bool toBeParallel);

template <typename T>
void
strassenMultiAdd(matrix<T> const &lhs, matrix<T> const &rhs, matrix<T> &result);

template <typename T>
void strassenMultiAdd(matrix<T> const &lhs, matrix<T> const &rhs, matrix<T> &result,
	bool toBeParallel);

template <typename T>
void strassenMultiAddParallel(
	matrix<T> const &lhs, matrix<T> const &rhs, matrix<T> &result);

// For A += A * A (graph algorithms)
template <typename T>
void recursiveSelfMultiAdd(matrix<T> &lhs);

//----------------------------- algorithms ------------------------------

template <typename T>
void add(T const *src, T const *target, size_t height, size_t width, 
				 size_t lw, size_t rw);

// all height and width are measured from source.
// height, width, rwidth: operation region
// l1w, l2w, rw: matrix dimensions
template <typename T>
void transCopy(T const *src, T *tar, 
    size_t height, size_t width, size_t lw, size_t rw, size_t trans_thre);

// Normal layout
template <typename T>
void naiveMultiAdd(T const *A, T const *B, T *C, 
    size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw);

// using loop unrolling with k
template <typename T>
void naiveMultiAdd2(T const *A, T const *B, T *C, 
    size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw);

// Normal layout, transpose B for sequential access
template <typename T>
void naiveMultiAdd3(T const *A, T const *B, T *C, 
    size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw);

// normal layout multiply (row * row)
// lheight, lwidth, rwidth: operating region
template <typename T>
void simpleMultiAdd(T const * __restrict A, T const * __restrict B, T * __restrict C, 
    size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw);

template <>
void simpleMultiAdd<float>(float const * __restrict A, float const * __restrict B, float * __restrict C, 
    size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw);

// normal layout multiply (cache-optimized)
template <typename T>
void blockedMultiAdd(T const *A, T const *B, T *C, 
    size_t lheight, size_t lwidth, size_t rwidth,
      size_t Aw, size_t Bw, size_t blockH, size_t blockW);

template <typename T>
void blockedMultiAdd(T const *A, T const *B, T *C, 
    size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw);

// This assumes recursive layout of matrices
template <typename T>
void recursiveMultiAdd(T const *A, T const *B, T *C,
    size_t lheight, size_t lwidth, size_t rwidth, size_t remainingRecursion);

template <typename T>
void recursiveMultiAddParallelAdvanced(T const *A, T const *B, T *C,
		size_t lheight, size_t lwidth, size_t rwidth, size_t remainingRecursion);

template <typename T>
void recursiveMultiAddParallel(T const *A, T const *B, T *C,
    size_t lheight, size_t lwidth, size_t rwidth, 
		size_t remainingRecursion, size_t parallelDepth);

template <typename T>
void strassenMultiAdd(T const *A, T const *B, T *C,
											size_t n, size_t remainingRecursion);

template <typename T>
void strassenMultiAddParallel(T const *A, T const *B, T *C,
											size_t n, size_t remainingRecursion);
template <typename T>
void strassenMultiAddParallel(T const *A, T const *B, T *C,
											size_t n, size_t remainingRecursion, size_t parallelDepth);

template <typename T>
struct recursiveMultiThreadArg {
	recursiveMultiThreadArg(T const *A, T const *B, T *C, size_t l, size_t m, size_t n,
		size_t remainingRecursion, size_t parallelDepth): 
			A_(A), B_(B), C_(C), l_(l), m_(m), n_(n), 
				remainingRecursion_(remainingRecursion), parallelDepth_(parallelDepth) { }
	T const *A_;
	T const *B_;
	T *C_;
	size_t l_;
	size_t m_;
	size_t n_;
	size_t remainingRecursion_; 
	size_t parallelDepth_;
};


template <typename T>
struct StrassenThreadArg {
	StrassenThreadArg(T const *A, T const *B, T *C, size_t n, 
			size_t remainingRecursion, size_t parallelDepth):
		A_(A), B_(B), C_(C), n_(n), 
		remainingRecursion_(remainingRecursion), parallelDepth_(parallelDepth) { }
	T const *A_;
	T const *B_;
	T *C_;
	size_t n_;
	size_t remainingRecursion_;
	size_t parallelDepth_;
};

template <typename T>
void* recursiveMultiThreadFunc(void *lpParam);


template <typename T>
void* strassenThreadFunc(void *lpParam);

// Rearrange data for maximum cache efficiency
// srcW is the width of the entire matrix
// Go recursively until we reach certain threshold: in this case, we aim for L1
// ..cache. L2 level optimization is guaranteed from this recursive structure. 
// Using one functions for both transformation
template <typename T>
void recursiveArrange(T *src, T *dest, 
    size_t height, size_t width, size_t srcW, size_t threshold, bool direction);

} // end namespace lib_calvin_matrix


// 2008-04-12: First felt the need for parameterizing matrix dimension
// ..in template too; it will cause performance up because array access
// ..will be done with constants rather than variables.
namespace lib_calvin {

template <typename T>  // --> typename T, size_t height, size_t width
class matrix {
public:
  explicit matrix(size_t);
  matrix(size_t, size_t); // height * width
  matrix(matrix const &matrix); 
  ~matrix();
  
  size_t height() const { return height_; }
  size_t width() const {return width_; }
  void reset(size_t dimension); // for square matrix
  void reset(size_t height, size_t width);
  void reset(matrix<T> const &rhs); // assignment with arbitrary dimensions

  // No boundary check; fast
  T const &getval(size_t row, size_t col) const;
  T &setval(size_t row, size_t col);
  
  struct matrixRow {
    size_t width_; // number of elements in a single row
    T *start_;
    T &operator[] (size_t col) const;
  };
  // meant to be used as: m1[row][col] = val;
  // Does boundary check; safe and clean size_terface
  matrixRow const operator[] (size_t row);

  bool operator== (matrix<T> const &rhs) const;
  bool operator!= (matrix<T> const &rhs) const;
  matrix<T> const operator+ (matrix<T> const &rhs) const; 
  matrix<T> const operator- (matrix<T> const &rhs) const; 
	matrix<T> const operator- () const; // unary minus
  
  // Using global function (not method) for multiplication operator
  // Twofold function calling is not necessary here (just for practice),
  // ..but it is useful for automatic type conversion with multiplication.
  friend matrix<T> const 
    operator* (matrix<T> const &lhs, matrix<T> const &rhs) 
			{ return lib_calvin_matrix::matrixMultiply<T> (lhs, rhs); }

  friend void lib_calvin_matrix::naiveMultiAdd<T> (matrix<T> const &, 
		matrix<T> const &, matrix<T> &); 
	friend void lib_calvin_matrix::naiveMultiAdd2<T> (matrix<T> const &, 
		matrix<T> const &, matrix<T> &); 
	friend void lib_calvin_matrix::naiveMultiAdd3<T> (matrix<T> const &, 
		matrix<T> const &, matrix<T> &); 
  friend void lib_calvin_matrix::simpleMultiAdd<T> (matrix<T> const &, 
		matrix<T> const &, matrix<T> &); 
  friend void lib_calvin_matrix::blockedMultiAdd<T> (matrix<T> const &, 
		matrix<T> const &, matrix<T> &); 
  friend void lib_calvin_matrix::recursiveMultiAdd<T> (matrix<T> const &, 
		matrix<T> const &, matrix<T> &, bool); 
	friend void lib_calvin_matrix::strassenMultiAdd<T> (matrix<T> const &, 
		matrix<T> const &, matrix<T> &, bool); 
  friend void lib_calvin_matrix::recursiveSelfMultiAdd<T> (matrix<T> &);
  
  // Assignment can take only same dimensioned operands!
  // Use reset() function to assign different dimension matrix
  matrix<T> & operator= (matrix<T> const &rhs); 
  matrix<T> & operator+= (matrix<T> const &rhs);
  matrix<T> & operator-= (matrix<T> const &rhs);
  matrix<T> & operator*= (matrix<T> const &rhs);

  matrix<T> const transpose() const; 
	matrix<T> const naive_transpose() const;
  // This may raise an exception if it is not inversable!
  //const matrix<T> inverse (const matrix<T> &rhs); 
  
  // show the content. Assuming that "cout <<  E" works!
  void prsize_t() const;      
  void check(); // unit test 
  void test();
  void randomize (); // for unit testing

  static size_t const trans_thre_;  
  static size_t const mul_thre_;  
  static size_t const blockHeight_;
  static size_t const blockWidth_;

private:
  T *elements_; // array of E (raw oriented representation)
  size_t height_; 
  size_t width_;
  // size_terval of each row; this might be different from width_
  size_t rowsize_terval_; 
  // Static data for algorithm optimization
  
  void init ();
  void copy (matrix<T> const &rhs);
};
} // end namespace lib_calvin





/*********************** matrix<T> public methods ********************/
namespace lib_calvin  // for definitions
{ 

template <typename T>
matrix<T>::matrix(size_t height): height_(height), width_(height) {
	init();
}

template <typename T>
matrix<T>::matrix(size_t height, size_t width): height_(height), width_(width) {
	init();
}

template <typename T> 
matrix<T>::matrix(matrix<T> const &rhs):
    height_(rhs.height_), width_(rhs.width_) {
  init();
  copy (rhs);
}

template <typename T>
matrix<T> & 
matrix<T>::operator= (matrix<T> const &rhs) {
  if(height_ != rhs.height_ || width_ != rhs.width_) {
    std::cout << "matrix assignment dimension error.\n";
    return *this;
  }
  if(&rhs == this) {
    return *this;
  }
  copy (rhs);
  return *this;
}

template <typename T>
matrix<T>::~matrix() {
	size_t const num = height_ * width_;
	for (size_t i = 0; i < num; ++i) {
		elements_[i].~T();
	}
  operator delete (elements_);
}

/*************** reset ***************/

template <typename T>
void matrix<T>::reset(size_t height) {
  reset(height, height);
}

template <typename T>
void matrix<T>::reset(size_t height, size_t width) {
  delete[] elements_;
  height_ = height;
  width_ = width;
  init();
}

template <typename T>
void matrix<T>::reset(matrix<T> const &rhs) {
  if(this == &rhs)
    return;
  height_ = rhs.height_;
  width_ = rhs.width_;
  delete[] elements_;
  init();
  copy(rhs);
}

/*---------------- getval ------------------*/

template <typename T>
T const & 
matrix<T>::getval(size_t row, size_t col) const {
  // No boundary check!
  return (elements_[width_ * row + col]);
}

/*---------------- setval-------------*/

template <typename T>
T & 
matrix<T>::setval(size_t row, size_t col) {
  // No boundary check!
  return (elements_[width_ * row + col]);
}

/*--------------- operator[] --------------*/

template <typename T>
T & matrix<T>::matrixRow::operator[] (size_t col) const {
  if (col >= width_ ) {
    cout << "matirx operator[][] boundary error.\n";
    return;
  }
  return start_[col];
}

template <typename T>
typename matrix<T>::matrixRow const 
matrix<T>::operator[] (size_t row) {
  if (row >= height_) {
    cout << "matrix operator[][] boundary error.\n";
    return;
  }
  typename matrix<T>::matrixRow rowElements;
  rowElements.start_ = elements_ + width_ * row;
  rowElements.width_ = width_;
  return rowElements;
}


template <typename T>
bool matrix<T>::operator== (matrix<T> const &rhs) const {
  if (height_ != rhs.height_ || width_ != rhs.width_) 
    return false;
  size_t num = height_ * width_;
  // supposing T has != operation defined
  for (size_t i = 0; i < num; ++i) {
    if (elements_[i] != rhs.elements_[i])
      return false;
  }
  return true;
}

template <typename T>
bool matrix<T>::operator!= (matrix<T> const &rhs) const {
  if (height_ != rhs.height_ || width_ != rhs.width_) 
    return true;
  size_t num = height_ * width_;
  // supposing T has != operation defined
  for (size_t i = 0; i < num; ++i) {
    if (elements_[i] != rhs.elements_[i])
      return true;
  }
  return false;
}


template <typename T>
matrix<T> const
matrix<T>::operator+ (matrix<T> const &rhs) const {
  size_t num = height_ * width_;
  matrix<T> result(height_, width_);
  if(height_ != rhs.height_ || width_ != rhs.width_) {
    std::cout << "raise error here.\n";
    return result;
  }
  for(size_t i = 0; i < num; ++i) {
    result.elements_[i] = elements_[i] + rhs.elements_[i]; 
  }
  return result;
}

template <typename T>
matrix<T> const
matrix<T>::operator- (matrix<T> const &rhs) const {
  size_t num = height_ * width_;
  matrix<T> result(height_, width_);
  if(height_ != rhs.height_ || width_ != rhs.width_) {
    cout << "raise error here.\n";
    return result;
  }
  for(size_t i = 0; i < num; ++i) {
    result.elements_[i] = elements_[i] - rhs.elements_[i]; 
  }
  return result;
}


template <typename T>
matrix<T> &
matrix<T>::operator+= (matrix<T> const &rhs) {
  size_t num = height_ * width_;
  if(height_ != rhs.height_ || width_ != rhs.width_) {
    cout << "raise exception here.\n";
    return *this;
  }
  for(size_t i = 0; i < num; ++i) {
    elements_[i] = elements_[i] + rhs.elements_[i];
  }
  return *this;
}

template <typename T>
matrix<T> &
matrix<T>::operator-= (matrix<T> const &rhs) {
  size_t num = height_ * width_;
  if(height_ != rhs.height_ || width_ != rhs.width_) {
    cout << "raise exception here.\n";
    return *this;
  }
  for(size_t i = 0; i < num; ++i) {
    elements_[i] = elements_[i] - rhs.elements_[i];
  }
  return *this;
}

template <typename T>
matrix<T> &
matrix<T>::operator*= (matrix<T> const &rhs) {
  matrix<T> result = (*this) * rhs;
  return ((*this) = result);
}

template <typename T>
matrix<T> const
matrix<T>::transpose() const {
  //cout << "trans thre is" << trans_thre_ << endl;
  matrix<T> result(width_, height_); // transposed matrix
	lib_calvin_matrix::transCopy(elements_, result.elements_, height_, width_, 
  	width_, height_, trans_thre_);
  return result;
}

template <typename T>
matrix<T> const
	matrix<T>::naive_transpose() const {
	matrix<T> result(width_, height_);
	lib_calvin_matrix::transCopy(elements_, result.elements_, height_, width_, 
  	width_, height_, 1000000);
  return result;
}

/*------------- prsize_t --------------*/
template <typename T>
void matrix<T>::prsize_t() const {
  for(size_t i = 0; i < height_; ++i) {
    for(size_t j = 0; j < height_; ++j) {
      std::cout << elements_[width_*i + j] << "  "; 
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

/*----------- check -------------*/
template <typename T>
void matrix<T>::check() {
	using namespace lib_calvin_matrix;
	using std::cout;
	using std::endl;
	size_t const GIGA = 1000000000;
	double const multiProblemSize = static_cast<double>(height_)*height_*height_*2;
  cout << "Testing with matrix dimension: " << height_ << " * " << width_ 
    << endl;
  stopwatch watch;
  // test transpose 
  matrix<T> m1 (height_, width_);
  m1.randomize();
  matrix<T> m2 (height_, width_);
  m2.randomize();
  watch.start();
  m2 = m1.naive_transpose();
	watch.stop();
	cout << "Naive transpose took " << watch.read() << "\n";
	watch.start();
  m2 = m1.transpose();
  watch.stop();
	cout << "Transpose took " << watch.read() << "\n";
  for (size_t i = 0; i < height_; ++i) {
    for (size_t j = 0; j < width_; ++j) {
      if (m1.getval(i, j) != m2.getval(j, i)) {
        cout << "transpose Error!!!\n";
				exit(0);
      } else {
				// cout << "transpose GOOD.\n";
			}
    }
  }
  // test multiplication
  m2.randomize();	
  // Naive	
  matrix<T> m3(m1.height(), m2.width());
  watch.start();
  naiveMultiAdd2(m1, m2, m3);
  watch.stop();
  cout << "Naive multiply 2 time " << watch.read() << " GFLOPS: " << 
		multiProblemSize / watch.read() / GIGA << "\n";			
	 // Naive	2
  matrix<T> m8(m1.height(), m2.width());
  watch.start();
  naiveMultiAdd3(m1, m2, m8);
  watch.stop();
  cout << "Naive multiply3 time " << watch.read() << " GFLOPS: " << 
		multiProblemSize / watch.read() / GIGA << "\n";		
	if (m3 == m8) {
  } else {
    cout << "Naive multiply3 Error!!!\n";
		exit(0);
  }	
	// Blocked
	matrix<T> m5(m1.height(), m2.width());
  watch.start();  
  blockedMultiAdd(m1, m2, m5);
  watch.stop();
  cout << "Blocked multiply time " << watch.read() << " GFLOPS: " << 
		multiProblemSize / watch.read() / GIGA << "\n";	
  if (m3 == m5) {
  } else {
    cout << "blockedMulti Error!!!\n";
		exit(0);
  }	
  // Simple
  matrix<T> m4(m1.height(), m2.width());
  watch.start();
  simpleMultiAdd (m1, m2, m4);
  watch.stop();
  cout << "Simple multiply time " << watch.read() << " GFLOPS: " << 
		multiProblemSize / watch.read() / GIGA << "\n";		
	if (m3 == m4) {
  } else {
    cout << "Simple Error!!!\n";
		exit(0);
  }	
	// recursive
  matrix<T> m6(m1.height(), m2.width());
  watch.start();
  recursiveMultiAdd(m1, m2, m6);
  watch.stop();
  cout << "Recursive multiply time " << watch.read() << " GFLOPS: " << 
		multiProblemSize / watch.read() / GIGA << "\n";	
  if (m3 == m6) {
  } else {
    cout << "recursiveMulti Error!!!\n"; 
    cout << "Right answer is:\n";
    m4.prsize_t();
    cout << "Recursive multi result is:\n";
    m6.prsize_t();
		exit(0);
	}
	m6.reset(m1.height(), m2.width());
	watch.start();
  recursiveMultiAddParallel(m1, m2, m6);
  watch.stop();
  cout << "Recursive multiply 2 (parallel) time " << watch.read() << " GFLOPS: " << 
		multiProblemSize / watch.read() / GIGA << "\n";	
	if (m3 == m6) {
  } else {
    cout << "Recursive multiply 2 (parallel) Error!!!\n";  
		exit(0);
	}
	// Strassen
	matrix<T> m7(m1.height(), m2.width());
	watch.start();
	strassenMultiAdd(m1, m2, m7);
	watch.stop();
  cout << "Strassen multiply time " << watch.read() << " GFLOPS: " << 
		multiProblemSize / watch.read() / GIGA << "\n";	
	if (m4 == m7) {
  } else {
    cout << "Strassen Error!!!\n";  
		cout << "A = \n";
		m1.prsize_t();
		cout << "B = \n";
		m2.prsize_t();
    cout << "Right answer is:\n";
    m4.prsize_t();
    cout << "Strassen multi result is:\n";
    m7.prsize_t(); 
    for (size_t i = 0; i < m4.height(); ++i) {
      for (size_t j = 0; j < m4.width(); ++j) {
        if (m4.getval(i, j) != m7.getval(i, j)) {
          cout << i << " , " << j << " is error\t" << 
            m4.getval(i, j) << " " << m7.getval(i, j) << endl;
        }
      }
    }
		exit(0);
  }
	m7.reset(m1.height(), m2.width());
	watch.start();
	strassenMultiAddParallel(m1, m2, m7);
	watch.stop();
	cout << "Strassen parallel multiply GFLOPS " << 
		multiProblemSize / watch.read() / GIGA << "\n";
	if (m4 == m7) {
  } else {
		cout << "Strassen parallel Error!!!\n"; 
		exit(0);
	}
}

template <typename T>
void matrix<T>::test() {
}


/******************** matrix<T> private methods ******************/
 
/*----------- init ------------*/

// height_ and width_ must have been set properly and 
// elements_ must have been deleted
template <typename T>
void matrix<T>::init() {
  size_t num = height_ * width_;
  elements_ = (T *)operator new(sizeof(T)*num); // default constructor should work
	//std::cout << "alignmemnt is " << ((size_t)elements_ % 256) << "\n";
  for(size_t i = 0; i < num; ++i) {
    // important! needed for built-in type initialization
    elements_[i] = T();
  }
}

/*------------- copy --------------*/

// Assuming that rhs has same dimension!
template <typename T>
void matrix<T>::copy (matrix<T> const &rhs) {
	size_t num = rhs.height_ * rhs.width_;
  for(size_t i = 0; i < num; ++i) {
    elements_[i] = rhs.elements_[i];
  }
}

template <typename T>
void matrix<T>::randomize() {
  srand(static_cast<int>(height_ * width_));
  for (size_t i = 0; i < height_; ++i) {
    for (size_t j = 0; j < width_; ++j) {
      setval(i, j) = T(rand() % 10); 
    }
  }
}

/************************ matrix<T> static fields ************************/

template <typename T>
size_t const matrix<T>::trans_thre_ =  
	std::max<size_t>((size_t)(sqrt((float)lib_calvin_misc::L1_SIZE/2.0/sizeof(T))), 3); 
    
template <typename T>
size_t const matrix<T>::mul_thre_ = 32;
	//std::max((size_t)(sqrt((float)L1_SIZE/3.0/sizeof(T))/2), 3);
			

template <typename T>
size_t const matrix<T>::blockWidth_ = 64;
	//std::max((size_t)((float)L1_SIZE/sizeof(T)/3.0), 300);

// Note that we cannot use blockWidth_ value here
// (order of computation not guaranteed)
template <typename T>
size_t const matrix<T>::blockHeight_ = 48;
	//(size_t)(((float)L2_SIZE)/sizeof(T)/
	//std::max ((size_t)((float)L1_SIZE/sizeof(T)/3.0), 300))/ 1.5);

} // end namesapce lib_calvin for definitions


/************************ Global functions ***********************/

// copy with transposing: source and destination should be different
template <typename T>
void lib_calvin_matrix::transCopy (T const *src, T * __restrict dest, 
    size_t height, size_t width, size_t lw, size_t rw, size_t trans_thre) {
  if (height < trans_thre || width < trans_thre) {
    for (size_t i = 0; i < height; ++i) {
      for (size_t j = 0; j < width; ++j) {
         *(dest + rw*j + i) = *(src + lw*i + j);
      }
    }
    return;
  }
  size_t up_height = height / 2;
  size_t down_height = height - up_height;
  size_t left_width = width / 2;
  size_t right_width = width - left_width;
  transCopy(src, dest, up_height, left_width, lw, rw, trans_thre);
  transCopy(src + left_width, dest + rw * left_width, up_height, right_width, lw, rw, trans_thre);
  transCopy(src + lw * up_height, dest + up_height, down_height, left_width, lw, rw, trans_thre);
  transCopy(src + lw * up_height + left_width, dest + rw * left_width + up_height, down_height, right_width, 
						lw, rw, trans_thre);
}

//----------------------------- wrapping functions ----------------------------

template <typename T>
void
lib_calvin_matrix::matrixMultiAdd(matrix<T> const &lhs, matrix<T> const &rhs,
																	matrix<T> &result) {
	recursiveMultiAddParallel(lhs1, rhs, result);
}

template <typename T>
lib_calvin::matrix<T> const
lib_calvin_matrix::matrixMultiply(matrix<T> const &lhs, matrix<T> const &rhs) {
  matrix<T> result(lhs.height(), rhs.width());
  //blockedMultiAdd(lhs, rhs, result);
	//naiveMultiAdd2(lhs, rhs, result);
	//naiveMultiAdd3(lhs, rhs, result);
	//simpleMultiAdd(lhs, rhs, result);
	//recursiveMultiAdd(lhs, rhs, result);
	recursiveMultiAddParallel(lhs, rhs, result);
  return result;
}

/*----------------------- multiply size_termediates -------------------------*/

template <typename T>
void lib_calvin_matrix::naiveMultiAdd (
    matrix<T> const &A, matrix<T> const &B, matrix<T> &C) {
  if (!(A.width_ == B.height_ && A.height_ == C.height_ && B.width_ == C.width_)) {
		std::cout << "multiply not compatible\n";
    exit(0);
  }
  naiveMultiAdd<T> (A.elements_, B.elements_, C.elements_,
      A.height_, A.width_, B.width_, 
      A.width_, B.width_);
}

template <typename T>
void lib_calvin_matrix::naiveMultiAdd2 (
    matrix<T> const &A, matrix<T> const &B, matrix<T> &C) {
  if (!(A.width_ == B.height_ && A.height_ == C.height_ && B.width_ == C.width_)) {
		std::cout << "multiply not compatible\n";
    exit(0);
  }
  naiveMultiAdd2<T> (A.elements_, B.elements_, C.elements_,
      A.height_, A.width_, B.width_, 
      A.width_, B.width_);
}

template <typename T>
void lib_calvin_matrix::naiveMultiAdd3 (
    matrix<T> const &A, matrix<T> const &B, matrix<T> &C) {
  if (!(A.width_ == B.height_ && A.height_ == C.height_ && B.width_ == C.width_)) {
		std::cout << "multiply not compatible\n";
    exit(0);
  }
  naiveMultiAdd3<T> (A.elements_, B.elements_, C.elements_,
      A.height_, A.width_, B.width_, 
      A.width_, B.width_);
}
template <typename T>
void lib_calvin_matrix::simpleMultiAdd (
    matrix<T> const &A, matrix<T> const &B, matrix<T> &C) {
  if (!(A.width_ == B.height_ && A.height_ == C.height_ && B.width_ == C.width_)) {
		std::cout << "multiply not compatible\n";
    exit(0);
  }
	//std::cout << "mul_thre_ is: " << A.mul_thre_ << "\n";
  simpleMultiAdd<T> (A.elements_, B.elements_, C.elements_,
      A.height_, A.width_, B.width_, 
      A.width_, B.width_);
}

template <typename T>
void lib_calvin_matrix::blockedMultiAdd (
    matrix<T> const &A, matrix<T> const &B, matrix<T> &C) {
  //std::cout << "blockWidth: " << A.blockWidth_ << "  blockHeight: " <<
   // A.blockHeight_ << std::endl;
  if (!(A.width_ == B.height_ && A.height_ == C.height_ && B.width_ == C.width_)) {
		std::cout << "multiply not compatible\n";
    exit(0);
  }
  blockedMultiAdd<T> (A.elements_, B.elements_, C.elements_,
      A.height_, A.width_, B.width_, 
      A.width_, B.width_, A.blockHeight_, A.blockWidth_);
}

template <typename T>
void lib_calvin_matrix::recursiveMultiAdd(
    matrix<T> const &A, matrix<T> const &B, matrix<T> &C) 
{
	recursiveMultiAdd(A, B, C, false);
}

template <typename T>
void lib_calvin_matrix::recursiveMultiAddParallel (
    matrix<T> const &A, matrix<T> const &B, matrix<T> &C) 
{
	recursiveMultiAdd(A, B, C, true);
}

template <typename T>
void lib_calvin_matrix::recursiveMultiAdd(
    matrix<T> const &A, matrix<T> const &B, matrix<T> &C, bool toBeParallel) 
{	
	using lib_calvin_util::log;
  if (!(A.width_ == B.height_ && A.height_ == C.height_ && B.width_ == C.width_)) {
		std::cout << "multiply not compatible\n";
    exit(0);
  }
  T *lhs = new T[A.height_*A.width_];
  T *rhs = new T[B.height_*B.width_];
  T *result = new T[A.height_*B.width_];
  T *temp = new T[A.height_*B.width_];
	size_t mul_thre = A.mul_thre_;
	size_t initialRecursionDepth = log(A.width_) - log(mul_thre);
	std::cout << "recursion depth starting: " << initialRecursionDepth << "\n";
	lib_calvin::stopwatch watch;
  recursiveArrange (A.elements_, lhs, 
      A.height_, A.width_, A.width_, initialRecursionDepth, true);
  recursiveArrange (B.elements_, rhs, 
      B.height_, B.width_, B.width_, initialRecursionDepth, true);
  recursiveArrange (C.elements_, result, 
      C.height_, C.width_, C.width_, initialRecursionDepth, true);
	// actual multiplication takes place here
	if (toBeParallel == true) {
		recursiveMultiAddParallelAdvanced(lhs, rhs, result,
      A.height_, A.width_, B.width_, initialRecursionDepth);
	} else {
		recursiveMultiAdd(lhs, rhs, result,
      A.height_, A.width_, B.width_, initialRecursionDepth);
	}
	// now multiplication ended; make final results
  recursiveArrange (C.elements_, result, 
      C.height_, C.width_, C.width_, initialRecursionDepth, false);
  // testing
	/*
  recursiveArrange (C.elements_, temp, 
      C.height_, C.width_, C.width_, initialRecursionDepth, true);
  for (size_t i = 0; i < A.height_*B.width_; ++i) {
    if (result[i] != temp[i]) {
			std::cout << "recursiveArrange error\n";
      exit(0);
    }
  }*/
  delete[] lhs;
  delete[] rhs;
  delete[] result;
  delete[] temp;
}

template <typename T>
void lib_calvin_matrix::strassenMultiAdd (
    matrix<T> const &A, matrix<T> const &B, matrix<T> &C) {
	strassenMultiAdd(A, B, C, false);
}

template <typename T>
void lib_calvin_matrix::strassenMultiAddParallel (
    matrix<T> const &A, matrix<T> const &B, matrix<T> &C) {
	strassenMultiAdd(A, B, C, true);
}

template <typename T>
void lib_calvin_matrix::strassenMultiAdd (
    matrix<T> const &A, matrix<T> const &B, matrix<T> &C, bool toBeParallel) 
{
	using lib_calvin_util::log;
  if (!(A.height_ == A.width_ && B.height_ == B.width_ && C.height_ == C.width_ &&
				A.height_ == B.height_ && B.height_ == C.height_)) {
		std::cout << "Strassen multiply not compatible\n";
    exit(0);
  }
	size_t n = A.height_;
	size_t initialRecursionDepth = log(n) - log(A.mul_thre_);
  T *lhs =		new T[n*n];
  T *rhs =		new T[n*n];
  T *result = new T[n*n];
  recursiveArrange (A.elements_, lhs, n, n, n, initialRecursionDepth, true);
  recursiveArrange (B.elements_, rhs, n, n, n, initialRecursionDepth, true);
  recursiveArrange (C.elements_, result, n, n, n, initialRecursionDepth, true);
	// actual multiplication takes place here
	std::cout << "recursion depth starting: " << initialRecursionDepth << "\n";
	if (toBeParallel == true) {
		strassenMultiAddParallel(lhs, rhs, result, n, initialRecursionDepth);
	} else {
		strassenMultiAdd(lhs, rhs, result, n, initialRecursionDepth);
	}
	// now multiplication ended; make final results
  recursiveArrange (C.elements_, result, 
      C.height_, C.width_, C.width_, initialRecursionDepth, false);  
	delete[] lhs;
  delete[] rhs;
  delete[] result;
}

template <typename T>
void lib_calvin_matrix::recursiveSelfMultiAdd (matrix<T> &A) {
  if (A.width() != A.height()) {
    cout << "self multiply not compatible\n";
    exit(0);
  }
  T *lhs = new T[A.width() * A.height()];
  recursiveArrange(A.elements_, lhs, 
      A.height(), A.width(), A.width(), A.mul_thre_, true);
  recursiveMultiAdd(lhs, lhs, lhs, A.height(), A.width(), A.width(), A.mul_thre_);
  recursiveArrange(A.elements_, lhs, 
      A.height(), A.width(), A.width(), A.mul_thre_, false);
  delete[] lhs;
}



/**************************** Algorithms ***************************/
template <typename T>
void lib_calvin_matrix::naiveMultiAdd(T const *A, T const *B, T * __restrict C, 
    size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw) {
  size_t Cw = Bw;
  for (size_t i = 0; i < lheight; ++i) {
		for (size_t k = 0; k < rwidth; k++) {
			for (size_t j = 0; j < lwidth; ++j) {
				C[Cw * i + k] +=  A[Aw * i + j] * B[Bw * j + k];
			}
		}  
  }
}

template <typename T>
void lib_calvin_matrix::naiveMultiAdd2(T const *A, T const *B, T * __restrict C, 
    size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw) {
  size_t Cw = Bw;
  for (size_t i = 0; i < lheight; ++i) {
		size_t const loopUnroll = 4;
		size_t k = 0;
		for ( ; k + loopUnroll - 1 < rwidth; k += loopUnroll) {
			for (size_t j = 0; j < lwidth; ++j) {
				C[Cw * i + (k + 0)] += A[Aw * i + j] * B[Bw * j + (k + 0)];
				C[Cw * i + (k + 1)] += A[Aw * i + j] * B[Bw * j + (k + 1)];
				C[Cw * i + (k + 2)] += A[Aw * i + j] * B[Bw * j + (k + 2)];
				C[Cw * i + (k + 3)] += A[Aw * i + j] * B[Bw * j + (k + 3)];
			}
		}  
		for ( ; k < rwidth; ++k) {
			for (size_t j = 0; j < lwidth; ++j) {
				C[Cw * i + k] += ( A[Aw * i + j] * B[Bw * j + k] );
			}
		}
  }
}

template <typename T>
void lib_calvin_matrix::naiveMultiAdd3(T const *A, T const *B, T * __restrict C, 
    size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw) {
  size_t Cw = Bw;
	T *transposedB = new T[lwidth*rwidth];
	transCopy(B, transposedB, lwidth, rwidth, Bw, lwidth, 15);
  for (size_t i = 0; i < lheight; ++i) {
		for (size_t k = 0; k < rwidth; k++) {
			for (size_t j = 0; j < lwidth; j++) {
				C[Cw * i + k] +=  A[Aw * i + j] * transposedB[lwidth * k + j];
			}
		}  
  }
	delete[] transposedB;
}
template <typename T>
void
lib_calvin_matrix::simpleMultiAdd (T const *A, T const *B, T * __restrict C, 
		size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw) {
  size_t Cw = Bw;
  for (size_t i = 0; i < lheight; ++i) {
		for (size_t j = 0; j < lwidth; j++) {
			for (size_t k = 0; k < rwidth; k++) {
				C[Cw * i + k] += A[Aw * i + j] * B[Bw * j + k];
			}
		}
  }
}

// method by CSPP, CMU
// block is set in lhs2 only.
// lheight, lwidth, rheight, rwidth: operation region
// Aw, Bw: widths or total matrices
template <typename T>
void lib_calvin_matrix::blockedMultiAdd (T const *A, T const *B, T *C, 
		size_t lheight, size_t lwidth, size_t rwidth, 
		size_t Aw, size_t Bw, size_t blockH, size_t blockW ) {
	//std::cout << "blockH: " << blockH << " blockW: " << blockW << "\n";
  size_t rheight = lwidth, Cw = Bw; // for convenience
  // right, down: offset
  // h, w: size of current working block size
  // blockH, blockW: size of default block (determined by cache and sizeof T)
  size_t right, down, h, w, i = 0, j = 0, k = 0;
	// status: is there remaing rows or columns
  bool status1 = true, status2 = true;
  for(right = 0; status1; right += blockW) {
		//std::cout << "loop 1\n";
    status2 = true; // begin operation on the column
    if(right + blockW < rwidth) {
      w = blockW;
    } else {
      w = rwidth - right; // take all remaining
      status1 = false; // no more colums to proceed
    }
    for(down = 0; status2; down += blockH) {
			//std::cout << "loop 2\n";
      if(down + blockH < rheight) {
        h = blockH;
      } else {
        h = rheight - down;
        status2 = false;
      }
			naiveMultiAdd2(A + down, B + Bw*down + right, C + right, 
				lheight, h, w, lwidth, rwidth);				
    }
  }
}

template <typename T>
void lib_calvin_matrix::blockedMultiAdd(T const *A, T const *B, T *C, 
		size_t lheight, size_t lwidth, size_t rwidth, size_t Aw, size_t Bw) {
	matrix<T> temp(100);
	blockedMultiAdd(A, B, C, lheight, lwidth, rwidth, Aw, Bw, 
		temp.blockHeight_, temp.blockWidth_);
}

// Work only on recursively arranged matrix
template <typename T>
void lib_calvin_matrix::recursiveMultiAdd(T const *A, T const *B, T *C,
    size_t l, size_t m, size_t n, size_t remainingRecursion) 
{  
  if (remainingRecursion <= 0) { 
		naiveMultiAdd2(A, B, C, l, m, n, m, n);
    return;
  }
  // Recursive calls for sub-problems
	// The order is important for cache optimization: As I use simple multiplication
	// for small problems, matrix B will be full cached. Therefore, I group two 
	// operations which shares right side. 
	// 2011-05-05 averted above logic. I use naive multi for small problems, and
	// the order of sub-problems are now rearranged for convenience of parallel
	// logic.
	// Note: using parallel logic for recursiveMultiAdd harms the performance of 
	// strassenMultiAdd (which uses recursiveMultiAdd as subroutine), as it results in
	// too many threads.
  
	// (1,1) (1,1) (1,1)
	recursiveMultiAdd(A, B, C, l/2, m/2, n/2, remainingRecursion - 1);
  // (1,2) (2,1) (1,1)
	recursiveMultiAdd(A + (l/2)*(m/2), B + (m/2)*n, C, 
      l/2, m - m/2, n/2, remainingRecursion - 1);
  // (1,1) (1,2) (1,2)
	recursiveMultiAdd(A, B + (m/2)*(n/2), C + (l/2)*(n/2), 
      l/2, m/2, n - n/2, remainingRecursion - 1);
  // (1,2) (2,2) (1,2)
	recursiveMultiAdd(A + (l/2)*(m/2), B + (m/2)*n + (m - m/2)*(n/2), 
      C + (l/2)*(n/2), 
      l/2, m - m/2, n - n/2, remainingRecursion - 1);
  // (2,1) (1,1) (2,1)
	recursiveMultiAdd(A + (l/2)*m, B, C + (l/2)*n, 
      l - l/2, m/2, n/2, remainingRecursion - 1);
  // (2,2) (2,1) (2,1)
	recursiveMultiAdd(A + (l/2)*m + (l - l/2)*(m/2), B + (m/2)*n, C + (l/2)*n,
      l - l/2, m - m/2, n/2, remainingRecursion - 1);
  // (2,1) (1,2) (2,2)
	recursiveMultiAdd(A + (l/2)*m, B + (m/2)*(n/2), C + (l/2)*n + (l - l/2)*(n/2),
      l - l/2, m/2, n - n/2, remainingRecursion - 1);
  // (2,2) (2,2) (2,2)
	recursiveMultiAdd(A + (l/2)*m + (l - l/2)*(m/2), B + (m/2)*n + (m - m/2)*(n/2),
      C + (l/2)*n + (l - l/2)*(n/2), 
      l - l/2, m - m/2, n - n/2, remainingRecursion - 1);
}
 
// multi-threading only when problem becomes small enough to fit in cache
// does not brings performance gain; refer to one-note files
template <typename T>
void lib_calvin_matrix::recursiveMultiAddParallelAdvanced(T const *A, T const *B, T *C,
    size_t l, size_t m, size_t n, size_t remainingRecursion) 
{  
	size_t const parallelBeginRecursion = 6;
	size_t const parallelRecursionDepth = 3;
  if (remainingRecursion <= parallelBeginRecursion) { 
		recursiveMultiAddParallel(A, B, C, l, m, n, remainingRecursion, parallelRecursionDepth);
    return;
  }
  // Recursive calls for sub-problems
	// The order is important for cache optimization: As I use simple multiplication
	// for small problems, matrix B will be full cached. Therefore, I group two 
	// operations which shares right side. 
	// 2011-05-05 averted above logic. I use naive multi for small problems, and
	// the order of sub-problems are now rearranged for convenience of parallel
	// logic.
	// Note: using parallel logic for recursiveMultiAdd harms the performance of 
	// strassenMultiAdd (which uses recursiveMultiAdd as subroutine), as it results in
	// too many threads.
  
	// (1,1) (1,1) (1,1)
	recursiveMultiAddParallelAdvanced(A, B, C, l/2, m/2, n/2, remainingRecursion - 1);
  // (1,2) (2,1) (1,1)
	recursiveMultiAddParallelAdvanced(A + (l/2)*(m/2), B + (m/2)*n, C, 
      l/2, m - m/2, n/2, remainingRecursion - 1);
  // (1,1) (1,2) (1,2)
	recursiveMultiAddParallelAdvanced(A, B + (m/2)*(n/2), C + (l/2)*(n/2), 
      l/2, m/2, n - n/2, remainingRecursion - 1);
  // (1,2) (2,2) (1,2)
	recursiveMultiAddParallelAdvanced(A + (l/2)*(m/2), B + (m/2)*n + (m - m/2)*(n/2), 
      C + (l/2)*(n/2), 
      l/2, m - m/2, n - n/2, remainingRecursion - 1);
  // (2,1) (1,1) (2,1)
	recursiveMultiAddParallelAdvanced(A + (l/2)*m, B, C + (l/2)*n, 
      l - l/2, m/2, n/2, remainingRecursion - 1);
  // (2,2) (2,1) (2,1)
	recursiveMultiAddParallelAdvanced(A + (l/2)*m + (l - l/2)*(m/2), B + (m/2)*n, C + (l/2)*n,
      l - l/2, m - m/2, n/2, remainingRecursion - 1);
  // (2,1) (1,2) (2,2)
	recursiveMultiAddParallelAdvanced(A + (l/2)*m, B + (m/2)*(n/2), C + (l/2)*n + (l - l/2)*(n/2),
      l - l/2, m/2, n - n/2, remainingRecursion - 1);
  // (2,2) (2,2) (2,2)
	recursiveMultiAddParallelAdvanced(A + (l/2)*m + (l - l/2)*(m/2), B + (m/2)*n + (m - m/2)*(n/2),
      C + (l/2)*n + (l - l/2)*(n/2), 
      l - l/2, m - m/2, n - n/2, remainingRecursion - 1);
}

template <typename T>
void lib_calvin_matrix::recursiveMultiAddParallel(T const *A, T const *B, T *C,
    size_t l, size_t m, size_t n, size_t remainingRecursion, size_t parallelDepth) {
	if (remainingRecursion <= 0) {
		naiveMultiAdd2(A, B, C, l, m, n, m, n);
		return;
	}
	if (parallelDepth <= 0) { // fall back to simple method
    recursiveMultiAdd(A, B, C, l, m, n, remainingRecursion);
    return;
  }
	// (1,1) (1,1) (1,1)
	recursiveMultiThreadArg<T> arg1(A, B, C, l/2, m/2, n/2, 
		remainingRecursion - 1, parallelDepth - 1);
  // (1,2) (2,1) (1,1)
	recursiveMultiThreadArg<T> arg2(A + (l/2)*(m/2), B + (m/2)*n, C, 
      l/2, m - m/2, n/2, remainingRecursion - 1, parallelDepth - 1);
  // (1,1) (1,2) (1,2)
	recursiveMultiThreadArg<T> arg3(A, B + (m/2)*(n/2), C + (l/2)*(n/2), 
      l/2, m/2, n - n/2, remainingRecursion - 1, parallelDepth - 1);
  // (1,2) (2,2) (1,2)
	recursiveMultiThreadArg<T> arg4(A + (l/2)*(m/2), B + (m/2)*n + (m - m/2)*(n/2), 
      C + (l/2)*(n/2), 
			l/2, m - m/2, n - n/2, remainingRecursion - 1, parallelDepth - 1);
  // (2,1) (1,1) (2,1)
	recursiveMultiThreadArg<T> arg5(A + (l/2)*m, B, C + (l/2)*n, 
      l - l/2, m/2, n/2, remainingRecursion - 1,  parallelDepth - 1);
  // (2,2) (2,1) (2,1)
	recursiveMultiThreadArg<T> arg6(A + (l/2)*m + (l - l/2)*(m/2), 
		B + (m/2)*n, C + (l/2)*n,
      l - l/2, m - m/2, n/2, remainingRecursion - 1, parallelDepth - 1);
  // (2,1) (1,2) (2,2)
	recursiveMultiThreadArg<T> arg7(A + (l/2)*m, B + (m/2)*(n/2), 
		C + (l/2)*n + (l - l/2)*(n/2),
      l - l/2, m/2, n - n/2, remainingRecursion - 1, parallelDepth - 1);
  // (2,2) (2,2) (2,2)
	recursiveMultiThreadArg<T> arg8(A + (l/2)*m + (l - l/2)*(m/2), 
		B + (m/2)*n + (m - m/2)*(n/2),
      C + (l/2)*n + (l - l/2)*(n/2), 
      l - l/2, m - m/2, n - n/2, remainingRecursion - 1, parallelDepth - 1);

	using namespace lib_calvin_thread;
	thread_type thread1 = create_thread(recursiveMultiThreadFunc<T>, &arg1);
	thread_type thread3 = create_thread(recursiveMultiThreadFunc<T>, &arg3);
	thread_type thread5 = create_thread(recursiveMultiThreadFunc<T>, &arg5);
	thread_type thread7 = create_thread(recursiveMultiThreadFunc<T>, &arg7);
	wait_for_thread(thread1);
	wait_for_thread(thread3);
	wait_for_thread(thread5);
	wait_for_thread(thread7);
	thread_type thread2 = create_thread(recursiveMultiThreadFunc<T>, &arg2);
	thread_type thread4 = create_thread(recursiveMultiThreadFunc<T>, &arg4);
	thread_type thread6 = create_thread(recursiveMultiThreadFunc<T>, &arg6);
	thread_type thread8 = create_thread(recursiveMultiThreadFunc<T>, &arg8);
	wait_for_thread(thread2);
	wait_for_thread(thread4);
	wait_for_thread(thread6);
	wait_for_thread(thread8);
}

template <typename T>
void lib_calvin_matrix::strassenMultiAdd(T const *A, T const *B, T *C, 
																				size_t n, size_t remainingRecursion) {
	if (n % 2 != 0) {
		recursiveMultiAdd(A, B, C, n, n, n, remainingRecursion);
		return;
	}
	if (remainingRecursion <= 0) {
		naiveMultiAdd(A, B, C, n, n, n, n, n);
		return;
	}
	T *lhs = new T[(n/2)*(n/2)];
	T *rhs = new T[(n/2)*(n/2)];
	T *p1 = new T[(n/2)*(n/2)];
	T *p2 = new T[(n/2)*(n/2)];
	T *p3 = new T[(n/2)*(n/2)];
	T *p4 = new T[(n/2)*(n/2)];
	T *p5 = new T[(n/2)*(n/2)];
	T *p6 = new T[(n/2)*(n/2)];
	T *p7 = new T[(n/2)*(n/2)];
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		p1[i] = T();
		p2[i] = T();
		p3[i] = T();
		p4[i] = T();
		p5[i] = T();
		p6[i] = T();
		p7[i] = T();
	}
	// A = a, B = f - h
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs[i] = A[i];
	  rhs[i] = B[(n/2)*(n/2) + i] - B[(n/2)*(n/2)*3 + i];		
	}
	strassenMultiAdd(lhs, rhs, p1, n/2, remainingRecursion - 1);
	// A2 = a + b, B2 = h
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs[i] = A[i] + A[(n/2)*(n/2) + i];
	  rhs[i] = B[(n/2)*(n/2)*3 + i];		
	}
	strassenMultiAdd(lhs, rhs, p2, n/2, remainingRecursion - 1);
	// A3 = c + d, B3 = e
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs[i] = A[(n/2)*(n/2)*2 + i] + A[(n/2)*(n/2)*3 + i];		
	  rhs[i] = B[i];		
	}
	strassenMultiAdd(lhs, rhs, p3, n/2, remainingRecursion - 1);
	// A4 = d, B4 = g - e
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs[i] = A[(n/2)*(n/2)*3 + i];
	  rhs[i] = B[(n/2)*(n/2)*2 + i] - B[i];	
	}
	strassenMultiAdd(lhs, rhs, p4, n/2, remainingRecursion - 1);
	// A5 = a + d, B5 = e + h
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs[i] = A[i] + A[(n/2)*(n/2)*3 + i];
	  rhs[i] = B[i] + B[(n/2)*(n/2)*3 + i];		
	}
	strassenMultiAdd(lhs, rhs, p5, n/2, remainingRecursion - 1);
	// A6 = b - d, B6 = g + h
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs[i] = A[(n/2)*(n/2) + i] - A[(n/2)*(n/2)*3 + i];
	  rhs[i] = B[(n/2)*(n/2)*2 + i] + B[(n/2)*(n/2)*3 + i];	
	}
	strassenMultiAdd(lhs, rhs, p6, n/2, remainingRecursion - 1);
	// A7 = a - c, B7 = e + f
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs[i] = A[i] - A[(n/2)*(n/2)*2 + i];
	  rhs[i] = B[i] + B[(n/2)*(n/2) + i];		
	}
	strassenMultiAdd(lhs, rhs, p7, n/2, remainingRecursion - 1);

	// r = -P2 + P4 + P5 + P6
	// s = P1 + P2
	// t = P3 + P4
	// u = P1 - P3 + P5 - P7
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		//std::cout << "p1 i= "  << p1[i] << "p2 i= "  << p2[i] << "p3 i= "  << p3[i] <<
		//	"p4 i= "  << p4[i] << "p5 i= "  << p5[i] << "p6 i= "  << p6[i] <<	
			//"p7 i= "  << p7[i] << "\n";
		C[i]									+= (-p2[i] + p4[i] + p5[i] + p6[i]);
		C[(n/2)*(n/2) + i]		+= (p1[i] + p2[i]);
		C[(n/2)*(n/2)*2 + i]	+= (p3[i] + p4[i]);
		C[(n/2)*(n/2)*3 + i]	+= (p1[i] - p3[i] + p5[i] - p7[i]);
	}	
	delete[] lhs;
	delete[] rhs;
	delete[] p1;
	delete[] p2;
	delete[] p3;
	delete[] p4;
	delete[] p5;
	delete[] p6;
	delete[] p7;
}

template <typename T>
void lib_calvin_matrix::strassenMultiAddParallel(T const *A, T const *B, T *C, 
		size_t n, size_t remainingRecursion) {
	strassenMultiAddParallel(A, B, C, n, remainingRecursion, 1);
}

template <typename T>
void lib_calvin_matrix::strassenMultiAddParallel(T const *A, T const *B, T *C, 
		size_t n, size_t remainingRecursion, size_t parallelDepth) {
	if (n % 2 != 0) {
		recursiveMultiAddParallel(A, B, C, n, n, n, remainingRecursion, 2);
		return;
	}
	if (remainingRecursion <= 0) {
		naiveMultiAdd2(A, B, C, n, n, n, n, n);
		return;
	}
	if (parallelDepth <= 0) { // fall back to single thread
		strassenMultiAdd(A, B, C, n, remainingRecursion);
		return;
	}
	T *lhs1 = new T[(n/2)*(n/2)];
	T *lhs2 = new T[(n/2)*(n/2)];
	T *lhs3 = new T[(n/2)*(n/2)];
	T *lhs4 = new T[(n/2)*(n/2)];
	T *lhs5 = new T[(n/2)*(n/2)];
	T *lhs6 = new T[(n/2)*(n/2)];
	T *lhs7 = new T[(n/2)*(n/2)];
	T *rhs1 = new T[(n/2)*(n/2)];
	T *rhs2 = new T[(n/2)*(n/2)];
	T *rhs3 = new T[(n/2)*(n/2)];
	T *rhs4 = new T[(n/2)*(n/2)];
	T *rhs5 = new T[(n/2)*(n/2)];
	T *rhs6 = new T[(n/2)*(n/2)];
	T *rhs7 = new T[(n/2)*(n/2)];
	T *p1 = new T[(n/2)*(n/2)];
	T *p2 = new T[(n/2)*(n/2)];
	T *p3 = new T[(n/2)*(n/2)];
	T *p4 = new T[(n/2)*(n/2)];
	T *p5 = new T[(n/2)*(n/2)];
	T *p6 = new T[(n/2)*(n/2)];
	T *p7 = new T[(n/2)*(n/2)];
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		p1[i] = T();
		p2[i] = T();
		p3[i] = T();
		p4[i] = T();
		p5[i] = T();
		p6[i] = T();
		p7[i] = T();
	}
	// A = a, B = f - h
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs1[i] = A[i];
	  rhs1[i] = B[(n/2)*(n/2) + i] - B[(n/2)*(n/2)*3 + i];		
	}
	StrassenThreadArg<T> arg1(lhs1, rhs1, p1, n/2, 
		remainingRecursion - 1, parallelDepth - 1);
	// A2 = a + b, B2 = h
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs2[i] = A[i] + A[(n/2)*(n/2) + i];
	  rhs2[i] = B[(n/2)*(n/2)*3 + i];		
	}
	StrassenThreadArg<T> arg2(lhs2, rhs2, p2, n/2, 
		remainingRecursion - 1, parallelDepth - 1);
	// A3 = c + d, B3 = e
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs3[i] = A[(n/2)*(n/2)*2 + i] + A[(n/2)*(n/2)*3 + i];		
	  rhs3[i] = B[i];		
	}
	StrassenThreadArg<T> arg3(lhs3, rhs3, p3, n/2, 
		remainingRecursion - 1, parallelDepth - 1);
	// A4 = d, B4 = g - e
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs4[i] = A[(n/2)*(n/2)*3 + i];
	  rhs4[i] = B[(n/2)*(n/2)*2 + i] - B[i];	
	}
	StrassenThreadArg<T> arg4(lhs4, rhs4, p4, n/2, 
		remainingRecursion - 1, parallelDepth - 1);
	// A5 = a + d, B5 = e + h
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs5[i] = A[i] + A[(n/2)*(n/2)*3 + i];
	  rhs5[i] = B[i] + B[(n/2)*(n/2)*3 + i];		
	}
	StrassenThreadArg<T> arg5(lhs5, rhs5, p5, n/2, 
		remainingRecursion - 1, parallelDepth - 1);
	// A6 = b - d, B6 = g + h
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs6[i] = A[(n/2)*(n/2) + i] - A[(n/2)*(n/2)*3 + i];
	  rhs6[i] = B[(n/2)*(n/2)*2 + i] + B[(n/2)*(n/2)*3 + i];	
	}
	StrassenThreadArg<T> arg6(lhs6, rhs6, p6, n/2, 
		remainingRecursion - 1, parallelDepth - 1);
	// A7 = a - c, B7 = e + f
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		lhs7[i] = A[i] - A[(n/2)*(n/2)*2 + i];
	  rhs7[i] = B[i] + B[(n/2)*(n/2) + i];		
	}
	StrassenThreadArg<T> arg7(lhs7, rhs7, p7, n/2, 
		remainingRecursion - 1, parallelDepth - 1);
	
	using namespace lib_calvin_thread;
	thread_type thread1 = create_thread(strassenThreadFunc<T>, &arg1);
	thread_type thread2 = create_thread(strassenThreadFunc<T>, &arg2);
	thread_type thread3 = create_thread(strassenThreadFunc<T>, &arg3);
	thread_type thread4 = create_thread(strassenThreadFunc<T>, &arg4);
	thread_type thread5 = create_thread(strassenThreadFunc<T>, &arg5);
	thread_type thread6 = create_thread(strassenThreadFunc<T>, &arg6);
	thread_type thread7 = create_thread(strassenThreadFunc<T>, &arg7);
	wait_for_thread(thread1);
	wait_for_thread(thread2);
	wait_for_thread(thread3);
	wait_for_thread(thread4);
	wait_for_thread(thread5);
	wait_for_thread(thread6);
	wait_for_thread(thread7);

	// r = -P2 + P4 + P5 + P6
	// s = P1 + P2
	// t = P3 + P4
	// u = P1 - P3 + P5 - P7
	for (size_t i = 0; i < (n/2)*(n/2); ++i) {
		C[i]									+= (-p2[i] + p4[i] + p5[i] + p6[i]);
		C[(n/2)*(n/2) + i]		+= (p1[i] + p2[i]);
		C[(n/2)*(n/2)*2 + i]	+= (p3[i] + p4[i]);
		C[(n/2)*(n/2)*3 + i]	+= (p1[i] - p3[i] + p5[i] - p7[i]);
	}	
	delete[] lhs1;
	delete[] lhs2;
	delete[] lhs3;
	delete[] lhs4;
	delete[] lhs5;
	delete[] lhs6;
	delete[] lhs7;
	delete[] rhs1;
	delete[] rhs2;
	delete[] rhs3;
	delete[] rhs4;
	delete[] rhs5;
	delete[] rhs6;
	delete[] rhs7;
	delete[] p1;
	delete[] p2;
	delete[] p3;
	delete[] p4;
	delete[] p5;
	delete[] p6;
	delete[] p7;
}

template <typename T>
void *
lib_calvin_matrix::recursiveMultiThreadFunc(void *lpParam) {
	recursiveMultiThreadArg<T> *threadArg =	(recursiveMultiThreadArg<T> *)lpParam;
	recursiveMultiAddParallel<T>(threadArg->A_, threadArg->B_, threadArg->C_, 
		threadArg->l_, threadArg->m_, threadArg->n_, 
		threadArg->remainingRecursion_, threadArg->parallelDepth_);
	return NULL;
}

template <typename T>
void *
lib_calvin_matrix::strassenThreadFunc(void *lpParam) {
	StrassenThreadArg<T> *threadArg = (StrassenThreadArg<T> *)lpParam;
	strassenMultiAddParallel<T>(threadArg->A_, threadArg->B_, threadArg->C_, 
		threadArg->n_, threadArg->remainingRecursion_, threadArg->parallelDepth_);
	return NULL;
}

template <typename T>
void lib_calvin_matrix::recursiveArrange(T *src, T *dest, 
    size_t height, size_t width, size_t srcW, size_t remainingDepth, bool isToRecursive) {
  
  //cout << "recursiveArrange for " << height << " , " << width << "\n";
  if (remainingDepth <= 0) { // fall back to row-major order
    if (isToRecursive) { // going from raw-major to recursive
      for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
          dest[width*i + j] = src[srcW*i + j];
        }
      }
    } else { // restoring from recursive to raw-major
      for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
          src[srcW*i + j] = dest[width*i + j];
        }
      }
    }
    return;
  }
  // Recursive calls for sub-problems
  size_t upHeight = height / 2;
  size_t downHeight = height - upHeight;
  size_t leftWidth = width / 2;
  size_t rightWidth = width - leftWidth;
  recursiveArrange(src, dest, upHeight, leftWidth, 
      srcW, remainingDepth - 1, isToRecursive);
  recursiveArrange(src + leftWidth, dest + upHeight*leftWidth, 
      upHeight, rightWidth, 
      srcW, remainingDepth - 1, isToRecursive);
  recursiveArrange(src + srcW*upHeight, dest + upHeight*width,
      downHeight, leftWidth, 
      srcW, remainingDepth - 1, isToRecursive);
  recursiveArrange(src + srcW*upHeight + leftWidth, 
      dest + upHeight*width + downHeight*leftWidth,
      downHeight, rightWidth, 
      srcW, remainingDepth - 1, isToRecursive);
}

#undef MAX
#endif



#ifndef LIB_CALVIN__CONTAINER__CONTAINER_TEST_TYPES_H
#define LIB_CALVIN__CONTAINER__CONTAINER_TEST_TYPES_H

#include "vector_test.h"
#include "set_test.h"
#include "map_test.h"
#include "vector.h"
#include "set.h"
#include <vector>

namespace lib_calvin_container
{
	typedef size_t Numeric;

	struct LightObject {
	public:
		LightObject() : value_(0) { }
		LightObject(size_t value) : value_(value) { }
		bool operator==(LightObject const &rhs) const { return value_ == rhs.value_; }
		bool operator!=(LightObject const &rhs) const { return value_ != rhs.value_; }
		bool operator<(LightObject const &rhs) const { return value_ < rhs.value_; }
		friend std::ostream &operator<< (std::ostream &os, LightObject const &lightObject)
		{
			os << lightObject.value_; return os;
		}
		operator size_t () const { return value_; }
	private:
		ptrdiff_t value_;
		size_t dummy_;
	};

	struct LinkNodeObject {
		LinkNodeObject(int value) : value_(value) { }
		int value_;
		LinkNodeObject *next_;
	};

	struct HeavyObject {
	public:
		typedef lib_calvin::vector<size_t> ContainerType;
		HeavyObject() : value_() { init(); }
		HeavyObject(size_t size);
		HeavyObject(HeavyObject const &rhs) : value_(rhs.value_) { init(); }
		HeavyObject(HeavyObject &&rhs) :value_(std::move(rhs.value_)) { init(); }
		~HeavyObject();
		void init();
		HeavyObject &operator=(HeavyObject const &rhs) { value_ = rhs.value_; return *this; }
		HeavyObject &operator=(HeavyObject &&rhs)
		{
			value_ = std::move(rhs.value_); return *this;
		}
		HeavyObject &operator+=(HeavyObject const &rhs) { return *this; }
		bool operator==(HeavyObject const &rhs) const { return value_ == rhs.value_; }
		bool operator!=(HeavyObject const &rhs) const { return value_ != rhs.value_; }
		bool operator<(HeavyObject const &rhs) const { return value_ < rhs.value_; }
		friend std::ostream &operator<< (std::ostream &os, HeavyObject const &heavyObject)
		{
			os << heavyObject.value_.size(); return os;
		}
		operator size_t () const;
		static void countThisObject();
	private:
		ContainerType value_;
		size_t sanityCheckNumber;
		static size_t objectCount;
		static void objectCreated();
		static void objectDestroyed();
	};

	struct HeavyObjectWithMessage : HeavyObject {
		HeavyObjectWithMessage() : HeavyObject() { std::cout << "HeavyObject: default ctor\n"; }
		HeavyObjectWithMessage(int size) :HeavyObject(size) { std::cout << "HeavyObject: explicit ctor\n"; }
		HeavyObjectWithMessage(HeavyObjectWithMessage const &rhs) :
			HeavyObject(rhs) {
			std::cout << "HeavyObject: copy ctor\n";
		}
		HeavyObjectWithMessage(HeavyObjectWithMessage &&rhs) :
			HeavyObject(std::forward<HeavyObjectWithMessage>(rhs)) {
			std::cout << "HeavyObject: move ctor\n";
		}
		HeavyObjectWithMessage &operator=(HeavyObjectWithMessage const &rhs)
		{
			std::cout << "HeavyObject: copy assignment\n"; HeavyObject::operator=(rhs); return *this;
		}
		HeavyObjectWithMessage &operator=(HeavyObjectWithMessage &&rhs)
		{
			std::cout << "HeavyObject: move assignment\n";
			HeavyObject::operator=(std::forward<HeavyObjectWithMessage>(rhs)); return *this;
		}
		bool operator==(HeavyObjectWithMessage const &rhs) const
		{
			return HeavyObject::operator==(rhs);
		}
		bool operator!=(HeavyObjectWithMessage const &rhs) const
		{
			return HeavyObject::operator!=(rhs);
		}
		bool operator<(HeavyObjectWithMessage const &rhs) const
		{
			return HeavyObject::operator<(rhs);
		}
		friend std::ostream &operator<< (std::ostream &os, HeavyObjectWithMessage const &object)
		{
			return operator<< (os, static_cast<HeavyObject>(object));
		}
	};

	std::string getRandomString(int length);
}

#endif

namespace lib_calvin_iterator
{
	template <typename ConstIterator>
	class Iterator: public ConstIterator {
	public:
		Iterator() { }
		Iterator(Iterator const &rhs) { }
		operator ConstIterator() const { return ConstIterator(*this); }
		friend void function(ConstIterator iter) { }
	};

	class const_iterator {
	};

	void test() {
		const_iterator a;
		Iterator<const_iterator> iter1;
		function(a);
	}
}


	using lib_calvin_iterator::Iterator;
	using lib_calvin_iterator::const_iterator;

	void test() {
		const_iterator a;
		Iterator<const_iterator> iter1;
		function(a);
	}


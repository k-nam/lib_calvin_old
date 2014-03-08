namespace calvin 
{

template <typename ReturnType, typename ParamType>
class unary_predicate {
public:
	virtual ReturnType operator()(ParamType) const = 0;
	typedef ParamType arguemnt_type;
	typedef ReturnType return_type;
};

template <typename ReturnType, typename Param1, typename Param2>
class binary_predicate {
public:
	virtual ReturnType operator()(Param1, Param2) const= 0;
	typedef Param1 first_param_type;
	typedef Param2 second_param_type;
	typedef ReturnType return_type;
};

template <typename BinOp>
class bind_2nd: 
	public unary_predicate
		<typename BinOp::return_type, typename BinOp::first_param_type> {
public:
	typedef typename BinOp::first_param_type first_param_type;
	typedef typename BinOp::second_param_type second_param_type;
	typedef typename BinOp::return_type return_type;

	bind_2nd(BinOp const &binOp, second_param_type param2): 
			binOp_(binOp), param2_(param2) { }
	return_type operator()(first_param_type param1) const
			{ return binOp_(param1, param2_); }
private:
	BinOp const binOp_;
	second_param_type const param2_;
};

template <typename BinOp>
bind_2nd<BinOp> 
bind2nd(BinOp const &binOp, typename BinOp::second_param_type const &param2) {
	return bind_2nd<BinOp> (binOp, param2);
}

template <typename Class, typename ReturnType>
class memfun_ptr_const: public unary_predicate<ReturnType, Class const *> {
public:
	memfun_ptr_const(ReturnType (Class::*pMemberFunc)() const): 
			pMemberFunc_(pMemberFunc) { }
	ReturnType operator()(Class const *pObject) const
			{ return (pObject->*pMemberFunc_)(); }
private:
	ReturnType (Class::*pMemberFunc_)() const;
};

template <typename Class, typename ReturnType>
class memfun_ref_nonconst: public unary_predicate<ReturnType, Class &> {
public:
	memfun_ref_nonconst(ReturnType (Class::*pMemberFunc)()): 
			pMemberFunc_(pMemberFunc) { }
	ReturnType operator()(Class &pObject) const { return (pObject.*pMemberFunc_)(); }
private:
	ReturnType (Class::*pMemberFunc_)();
};

template <typename Class, typename ReturnType>
class memfun_ref_const: public unary_predicate<ReturnType, Class const &> {
public:
	memfun_ref_const(ReturnType (Class::*pMemberFunc)() const): 
			pMemberFunc_(pMemberFunc) { }
	ReturnType operator()(Class const &pObject) const { return (pObject.*pMemberFunc_)(); }
private:
	ReturnType (Class::*pMemberFunc_)() const;
};

template <typename Class, typename ParamType, typename ReturnType>
class memfun_ref_1_nonconst: 
	public binary_predicate<ReturnType, Class &, ParamType> {
public:
	memfun_ref_1_nonconst(ReturnType (Class::*pMemberFunc)(ParamType)): 
			pMemberFunc_(pMemberFunc) { }
	ReturnType operator()(Class &pObject, ParamType param) const 
	{ return (pObject.*pMemberFunc_)(param); }
private:
	ReturnType (Class::*pMemberFunc_)(ParamType);
};

template <typename Class, typename ParamType, typename ReturnType>
class memfun_ref_1_const: 
	public binary_predicate<ReturnType, Class const &, ParamType> {
public:
	memfun_ref_1_const(ReturnType (Class::*pMemberFunc)(ParamType) const): 
			pMemberFunc_(pMemberFunc) { }
	ReturnType operator()(Class const &pObject, ParamType param) const 
	{ return (pObject.*pMemberFunc_)(param); }
private:
	ReturnType (Class::*pMemberFunc_)(ParamType) const;
};


template <typename Class, typename ReturnType>
memfun_ref_nonconst<Class, ReturnType>
memfun(ReturnType (Class::*pMemberFunc)()) {
	return memfun_ref_nonconst<Class, ReturnType>(pMemberFunc);
}

template <typename Class, typename ReturnType>
memfun_ref_const<Class, ReturnType>
memfun(ReturnType (Class::*pMemberFunc)() const) {
	return memfun_ref_const<Class, ReturnType>(pMemberFunc);
}

template <typename Class, typename ParamType, typename ReturnType>
memfun_ref_1_nonconst<Class, ParamType, ReturnType>
memfun(ReturnType (Class::*pMemberFunc)(ParamType)) {
	return memfun_ref_1_nonconst<Class, ParamType, ReturnType>(pMemberFunc);
}

template <typename Class, typename ParamType, typename ReturnType>
memfun_ref_1_const<Class, ParamType, ReturnType>
memfun(ReturnType (Class::*pMemberFunc)(ParamType) const) {
	return memfun_ref_1_const<Class, ParamType, ReturnType>(pMemberFunc);
}

template <typename Iter, typename Callback>
Callback foreach(Iter first, Iter last, Callback callback) {
	while (first != last) {
		callback(*first);
		++first;
	}
	return callback;
};

} // end namespace

/***************************************************************************
                          adfutil.h  -  description
                             -------------------
    begin                : Tue Sep 18 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef ADFUTIL_H
#define ADFUTIL_H

#include "adf++.h"

#include <map>

namespace ADF {

template<bool>
struct Assert;
template<>
struct Assert<true> {};
#define CT_ASSERT_GLUE(a,b) a ## b
#define CT_ASSERT_XGLUE(a,b) CT_ASSERT_GLUE(a,b)
//#define CT_ASSERT(expr) static ::ADF::Assert<(expr)> CT_ASSERT_XGLUE(assert,__LINE__)
#define CT_ASSERT(expr) struct CT_ASSERT_XGLUE(assert_struct,__LINE__) { ::ADF::Assert<(expr)> assertion; }

/** Checks whether an error occured and brings it to attention.
  */
void Check(int iErr);

/** Emits the specified error.
  */
#ifdef __GNUC__
void Emit(int iErr) __attribute__((noreturn));
#else
void Emit(int iErr);
#endif

/** Represents a checker object which is usable as an * int, but handles errors if they occur.
  */
struct Checker {
	~Checker();
	operator int *() { return &mErrorCode; }
	int mErrorCode;
};

int DataSize(DataType iType);

template<typename T>
refcnt_ptr<T>::~refcnt_ptr() { dec(); }

template<typename T>
struct child_container : private std::map<std::string, T> {
	typedef typename std::map<std::string, T> base;
	typedef typename base::value_type value_type;
	typedef typename base::iterator iterator;
	typedef typename base::const_iterator const_iterator;
	typedef T dummy[1/(sizeof(iterator)==sizeof(void */*ADF::iterator<T>*/))];
	using base::begin;
	using base::end;
	using base::size;
	using base::clear;
	using base::empty;
	using base::erase;
	bool has(std::string const & iName) const {
		return base::find(iName)!=base::end(); }
	void swap(child_container & rhs) { base::swap(rhs); }
	void insert(T const & iT) {
		base::insert(value_type(iT.getName(),iT)); }
	void insert(T const & iT, std::string const & iName) {
		base::insert(value_type(iName,iT)); }
	T & find(std::string const & iName) {
//		assert(base::find(iName)!=base::end());
		return base::find(iName)->second; }
	T const & find(std::string const & iName) const {
		return const_cast<child_container *>(this)->find(iName); }
};

template<typename T>
iterator<T>::iterator(void * iData) {
	if (iData)
		*reinterpret_cast<typename child_container<T>::iterator *>(&mData)=
			*reinterpret_cast<typename child_container<T>::iterator *>(&iData);
	else new(reinterpret_cast<typename child_container<T>::iterator *>(&mData))
		typename child_container<T>::iterator();
}

template<typename T>
std::string const & iterator<T>::getName() const {
	return (*reinterpret_cast<typename child_container<T>::iterator const *>(&mData))->first;
}

template<typename T>
typename iterator<T>::reference iterator<T>::operator*() const {
	return const_cast<typename iterator<T>::reference>(
		(*reinterpret_cast<typename child_container<T>::iterator const *>(&mData))->second);
}

template<typename T>
typename iterator<T>::pointer iterator<T>::operator->() const {
	return const_cast<typename iterator<T>::pointer>(
		&(*reinterpret_cast<typename child_container<T>::iterator const *>(&mData))->second);
}

template<typename T>
iterator<T> const & iterator<T>::operator++() { //prefix
	++*reinterpret_cast<typename child_container<T>::iterator *>(&mData);
	return *this;
}

template<typename T>
iterator<T> iterator<T>::operator++(int) {      //postfix
	iterator temp(*this);
	++*reinterpret_cast<typename child_container<T>::iterator *>(&mData);
	return temp;
}

template<typename T>
bool iterator<T>::operator!=(iterator const & rhs) const {
	return *reinterpret_cast<typename child_container<T>::iterator const *>(&mData)
		!=*reinterpret_cast<typename child_container<T>::iterator const *>(&rhs.mData);
}

template<typename T>
bool iterator<T>::operator==(iterator const & rhs) const {
	return *reinterpret_cast<typename child_container<T>::iterator const *>(&mData)
		==*reinterpret_cast<typename child_container<T>::iterator const *>(&rhs.mData);
}
	
} // namespace ADF

#endif

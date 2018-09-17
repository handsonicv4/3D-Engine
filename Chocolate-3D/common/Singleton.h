#pragma once
#ifndef SINGLETON_DIFINE_H_

#define SINGLETON_DIFINE_H_

#define SINGLETON(_CLASS_)\
public:\
	inline static _CLASS_& GetInstance()\
	{\
		static _CLASS_ instance;\
		return instance;\
	}\
private:	\
	_CLASS_();\
	_CLASS_(_CLASS_ const&) : _CLASS_() {}\
	_CLASS_& operator= (_CLASS_ const&) { return *this; }	\
	~_CLASS_();

#define SINGLETON_C_D(_CLASS_)	\
	_CLASS_::_CLASS_() {}\
	_CLASS_::~_CLASS_() {}\

#endif //SINGLETON_DIFINE_H_

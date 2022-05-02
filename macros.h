#pragma once

#define CREATE_NEW_DATA_TYPE(name)	struct name																\
									{																		\
										unsigned int val;													\
										explicit name(unsigned int v) : val(v)	{ }							\
										operator unsigned int() const			{ return val; }				\
										name operator|(name v) const			{ return name(val | v); }	\
										name operator|(unsigned int v) const	{ return name(val | v); }	\
									}
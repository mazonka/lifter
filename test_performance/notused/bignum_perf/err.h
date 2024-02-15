#pragma once

#include <string>

using std::string;

template <class T> inline string tos(T x) { return std::to_string(x); }

#define here ("["+string(__func__)+"] "+(__FILE__)+":"+tos(__LINE__))
#define never throw string()+"bug at ["+(__func__)+"] "+(__FILE__)+":"+tos(__LINE__);
#define nevers(x) throw string(x)+" ["+(__func__)+"] "+(__FILE__)+":"+tos(__LINE__);

#pragma once


#include <string>


namespace logger {
template<typename T>
inline void ToString(T, std::string& out) {}
}
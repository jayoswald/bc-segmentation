#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <tuple>

using std::string;
using std::vector;
using std::fstream;
using std::cout;

// Returns true if the string starts with the substring.
inline bool startswith(const string &s, const char *beg) {
    if (s.size()==0 && *beg != '\0') return false;
    for (size_t i=0; i<s.size(); ++i) {
        if (beg[i] == '\0') return true;
        if (s[i] != beg[i]) return false;
    }
    return true;
}

//! Writes a vector to the console.
template<class T>
std::ostream& operator<<(std::ostream& o, std::vector<T> x) {
    o << "{";
    if (x.size()) o << x.front();
    for (size_t i=1; i<x.size(); ++i)  o << ", " << x[i];
    return o << "}\n";
}

//! Reads the next line of a file.
template <typename STREAM>
inline std::string read_line(STREAM &fid) {
    std::string line;
    getline(fid, line);
    return line;
}

//! Converts a string to type T.
template <typename T>
inline T from_string(const std::string &s) {
    std::istringstream ss(s);
    T t;  ss >> t;
    if (ss.fail()) throw "Bad string conversion: " + s;
    return t;
}

//! Extracts a tuple of arbitrary types from a vector of strings at an index.
template<typename... A>
std::tuple<A...> args_to_tuple(std::vector<std::string> args, int index) {
    auto end = args.begin() + index + sizeof...(A);   
    return std::make_tuple(from_string<A>(*--end)...);
}

//! Shortcut to convert strings to unsigned.
inline unsigned str2u32(const std::string &s) {
    return from_string<unsigned>(s);
}
//! Shortcut to convert strings to double.
inline double str2dbl(const std::string &s) {
    return from_string<double>(s);
}

//! Returns a copy of the string without trailing/preceding whitespace.
inline std::string trim(std::string s, std::string ws=" \t\n\r") {
    if (s.empty()) return s;
    size_t a=s.find_first_not_of(ws);
    size_t b = s.find_last_not_of(ws)+1;
    return s.substr(a, b-a);
}

//! Returns any part of str that occurs before any characters in spl.
inline std::string before(string str, string spl) {
    auto pos = str.find_first_of(spl);
    if (pos == std::string::npos) return str;
    return str.substr(0, pos);
}


//! Splits a string like the python function.
inline std::vector<std::string> split(std::string s, std::string delims=" \t\n\r")
{
    std::vector<std::string> pieces;
    size_t begin=0, end=0;
    while (end != std::string::npos) {
        begin = s.find_first_not_of(delims, end);
        end   = s.find_first_of(delims, begin);
        if (begin != std::string::npos)
            pieces.push_back(s.substr(begin, end-begin));
    }
    return pieces;
}

//! Joins a vector, string back together.
inline std::string join(const std::vector<std::string> &v)
{
    if (v.empty()) return "";
    std::string s(v.front());
    for (auto viter =v.begin()+1; viter != v.end(); ++viter) s += " " + *viter;
    return s;
}

//! Joins strings together from iterators to strings.
template <class _Iter>
inline std::string join(_Iter beg, _Iter end) {
    if (beg == end) return "";
    std::string s(*(beg++));
    for (; beg!=end; ++beg) s += " " + *beg;
    return s;
}

//! Converts anything to a string
template<typename T>
inline string make_string(const T& x, int width=-1, int precision=-1) {
    using namespace std;
    ostringstream o;
    if (width>=0)     o << setw(width) << setfill('0');
    if (precision>=0) o << fixed << setprecision(precision);
    o << x;
    return o.str();
}

//! Returns a copy of the string in lowercase.
inline string lower(string s) {
    std::transform(s.begin(), s.end(), s.begin(),
            static_cast<int(*)(int)> (tolower));
    return s;
}

//! Returns the index of the closing brace matching index i (or string::npos).
inline size_t find_matching_brace(const string &s, size_t i) {

    char open  = s[i];
    char close = ')';
    if      (open == '[') close = ']';
    else if (open == '{') close = '}';
    int p = 1;  // Open bracket counter.
    for (i=i+1; i<s.size(); ++i) {
        if      (s[i] == close) --p;
        else if (s[i] == open)  ++p;
        if (p == 0) return i;
    }
    return string::npos;
}


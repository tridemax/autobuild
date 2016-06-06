#pragma once

//-------------------------------------------------------------------------------------------------
/// posix
//-------------------------------------------------------------------------------------------------
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <iso646.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>
#include <wchar.h>
#include <wctype.h>
#include <smmintrin.h>
#include <libgen.h>

//-------------------------------------------------------------------------------------------------
/// std
//-------------------------------------------------------------------------------------------------
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <random>
#include <numeric>
#include <array>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <unordered_map>
#include <locale>
#include <memory>
#include <typeindex>
#include <algorithm>
#include <chrono>

//-------------------------------------------------------------------------------------------------
/// boost/tbb
//-------------------------------------------------------------------------------------------------
#include <boost/config.hpp>
#include <boost/noncopyable.hpp>
#include <boost/variant.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <tbb/tbb.h>

//-------------------------------------------------------------------------------------------------
/// third party
//-------------------------------------------------------------------------------------------------
#include "auxiliary/Clock.h"

//-------------------------------------------------------------------------------------------------
/// byte type declaration
//-------------------------------------------------------------------------------------------------
typedef uint8_t byte;

static_assert(sizeof(byte) == 1u, "Size of \"byte\" typedef not equal to 8 bit.");

//-------------------------------------------------------------------------------------------------
///
//-------------------------------------------------------------------------------------------------
constexpr uint32_t makefourcc(char ch0, char ch1, char ch2, char ch3)
{
	return ( uint32_t(uint8_t(ch0)) | (uint32_t(uint8_t(ch1)) << 8) | (uint32_t(uint8_t(ch2)) << 16) | (uint32_t(uint8_t(ch3)) << 24) );
}

template <typename ArrayType, size_t Dimension>
constexpr size_t _countof( ArrayType (&)[Dimension] )
{
	return std::extent<ArrayType[Dimension]>::value;
}

//-------------------------------------------------------------------------------------------------
/// defines
//-------------------------------------------------------------------------------------------------
#define forceinline					inline __attribute__((always_inline))
#define selectany					__attribute__((weak))

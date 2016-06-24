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
#include <list>
#include <locale>
#include <memory>
#include <typeindex>
#include <algorithm>
#include <chrono>
#include <thread>

//-------------------------------------------------------------------------------------------------
/// third party
//-------------------------------------------------------------------------------------------------
#include <boost/config.hpp>
#include <boost/noncopyable.hpp>
#include <boost/variant.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread/tss.hpp>

#include <auxiliary.h>
#include <postgres.h>

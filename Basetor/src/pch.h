#pragma once

#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>
#include <thread>
#include <utility>
#include <exception>
#include <stdint.h>
#include <ctime>

#include <string>
#include <stack>
#include <deque>
#include <array>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

//Add checks to see if we are in cpp-17
#include <filesystem>

#include <sstream>
#include <fstream>

// Project headers
#include "utils/Instrumentor.h"
#include "Log.h"
#include "debugbreak.h"
#include "utils/base_assert.h"
//#if defined MLIST_WINDOWS_BUILD
//	#include <Windows.h>
//#endif
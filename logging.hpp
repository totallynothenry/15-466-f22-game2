#pragma once

#include <iostream>

#define DEBUG

#ifdef DEBUG
#define LOG(ARGS) (std::cout << ARGS << std::endl)
#else
#define LOG(ARGS)
#endif

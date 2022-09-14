#pragma once

#include <iostream>

#ifdef DEBUG
#define LOG(ARGS) (std::cout << ARGS << std::endl)
#else
#define LOG(ARGS)
#endif

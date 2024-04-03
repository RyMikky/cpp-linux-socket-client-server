#pragma once

// Общие компоненты STL
#include <iostream>
#include <atomic>
#include <cstring>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <exception>
#include <cerrno>
#include <ctime>
#include <chrono>
#include <iomanip>

#ifdef __linux__ // __GNUC__
    #include <unistd.h>
    #include <pthread.h>

    // Компоненты относящиеся к сетевому взаимодействию
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>

#elif _WIN32

    // Под винду нужны другие компоненты
    // #include <thread>
    // #include <sdkddkver.h>
    // ....

#endif

#define __LOCAL_HOST__ "127.0.0.1"
#define __LOG_FILE__ "log.txt"
#define __BUFFER_SIZE__ 512
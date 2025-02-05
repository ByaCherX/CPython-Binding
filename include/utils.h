#ifndef UTILS_H
#define UTILS_H
#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <functional>
#include <string>

#pragma once

#if defined(_WIN64)
typedef __int64 r_ssize_t;
#else
typedef __int32 r_ssize_t;
#endif /* _WIN64 */

struct Core {
public:
	std::string ref;

	Core() : alloc_size{ 1 } {
		printf("Core created > default\n");
		Core::m_data = new char[Core::alloc_size];
		Core::ref = "cpp_object_references";
	}

	Core(const char* string) {   /* --- Constructor --- */
		printf("Core created > explicit\n");
		Core::alloc_size = std::strlen(string);			// target alloc_size
		Core::m_data = new char[Core::alloc_size];	    // allocate to memory
		memcpy(Core::m_data, string, Core::alloc_size); // memory copy to m_data
	}

	Core(const Core& other) {	 /* --- Copy Constructor --- */
		printf("Copied!\n");
		Core::alloc_size = other.alloc_size;
		Core::m_data = new char[Core::alloc_size];
		memcpy(Core::m_data, other.m_data, Core::alloc_size);
	}

	Core(Core&& other) noexcept { /* --- Move Constructor --- */
		printf("Moved!\n");
		Core::alloc_size = other.alloc_size;
		Core::m_data = other.m_data;

		/* deleting old memory blockand references */
		other.alloc_size = 0;
		other.m_data = nullptr;
	}

	~Core() {
		std::cout << "Core desctructed" << std::endl;
		delete[] m_data;
	}

	void Print() {
		for (uint32_t i = 0; i < alloc_size; i++) {
			printf("%c", m_data[i]);
		}
		printf("\n");
	}

	void Core_move(Core& _src) {
		//Core _dest = std::move(_src);
		//return _dest;
	}
private:
	int alloc_size;
	char* m_data{ nullptr };
};

class Timer {
public:
    Timer();
    void start(std::function<void()> callback, int interval_ms);
    void stop();
    ~Timer();

private:
    std::atomic<bool> running;
    std::thread timer_thread;
};

std::string colorize_typedefs(const std::string& code);
std::string random_hex(int length);
// ------------------------------
// Python Module Functions
// ------------------------------
void pydef();

#endif // UTILS_H
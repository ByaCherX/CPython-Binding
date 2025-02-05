#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <functional>
#include <regex>
#include <random>
#include <iomanip>
#include <sstream>

#pragma once

/* Timer */
class Timer {
public:
	Timer() : running(false) {}

	// Timer starter function
	void start(std::function<void()> callback, int interval_ms) {
		running = true;
		timer_thread = std::thread([=]() {
			while (running) {
				std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
				if (running) callback();
			}
			});
	}

	// Timer stopper function
	void stop() {
		running = false;
		if (timer_thread.joinable()) {
			timer_thread.join();
		}
	}

	~Timer() {
		stop();
	}

private:
	std::atomic<bool> running;  // Thread-Safety state
	std::thread timer_thread;
};


std::string colorize_typedefs(const std::string& code) {
	std::string result = code;

	// Renk kodlarý
	const std::string green = "\033[32m";
	const std::string blue = "\033[34m";
	const std::string turquoise = "\033[36m";
	const std::string white = "\033[97m";
	const std::string reset = "\033[0m";

	// Anahtar kelimeleri renklendir
	result = std::regex_replace(result, std::regex("(//.*)"), green + "$1" + reset);
	result = std::regex_replace(result, std::regex("\\b(typedef|int|void|char)\\b"), blue + "$1" + reset);
	result = std::regex_replace(result, std::regex("(\\*|\\(|\\)|;)"), white + "$1" + reset);
	result = std::regex_replace(result, std::regex("\\b(PyObject|PyTypeObject|Py_ssize_t|Py_hash_t)\\b"), turquoise + "$1" + reset);

	return result;
}

std::string random_hex(int length) {
	// Rastgele sayý oluþturmak için
	std::random_device rd;                     // Rastgele cihaz
	std::mt19937 gen(rd());                    // Mersenne Twister motoru
	std::uniform_int_distribution<int> dist(0, 15);  // 0'dan 15'e (0x0 - 0xF)

	std::ostringstream oss;
	oss << std::hex;  // Onaltýlýk (hex) formatý ayarla

	for (int i = 0; i < length; ++i) {
		int random_value = dist(gen);          // Rastgele 0-15 arasý sayý al
		oss << random_value;                   // Onaltýlýk karaktere dönüþtür
	}

	return oss.str();                          // Hex string döndür
}

// ------------------------------
// Python Module Functions
// ------------------------------
void pydef() {
	std::string code = R"(
//--- Python C/API Defination ---//
typedef PyObject * (*unaryfunc)(PyObject *);
typedef PyObject * (*binaryfunc)(PyObject *, PyObject *);
typedef PyObject * (*ternaryfunc)(PyObject *, PyObject *, PyObject *);
typedef int (*inquiry)(PyObject *);
typedef Py_ssize_t (*lenfunc)(PyObject *);
typedef PyObject *(*ssizeargfunc)(PyObject *, Py_ssize_t);
typedef PyObject *(*ssizessizeargfunc)(PyObject *, Py_ssize_t, Py_ssize_t);
typedef int(*ssizeobjargproc)(PyObject *, Py_ssize_t, PyObject *);
typedef int(*ssizessizeobjargproc)(PyObject *, Py_ssize_t, Py_ssize_t, PyObject *);
typedef int(*objobjargproc)(PyObject *, PyObject *, PyObject *);

typedef int (*objobjproc)(PyObject *, PyObject *);
typedef int (*visitproc)(PyObject *, void *);
typedef int (*traverseproc)(PyObject *, visitproc, void *);

typedef void (*freefunc)(void *);
typedef void (*destructor)(PyObject *);
typedef PyObject *(*getattrfunc)(PyObject *, char *);
typedef PyObject *(*getattrofunc)(PyObject *, PyObject *);
typedef int (*setattrfunc)(PyObject *, char *, PyObject *);
typedef int (*setattrofunc)(PyObject *, PyObject *, PyObject *);
typedef PyObject *(*reprfunc)(PyObject *);
typedef Py_hash_t (*hashfunc)(PyObject *);
typedef PyObject *(*richcmpfunc) (PyObject *, PyObject *, int);
typedef PyObject *(*getiterfunc) (PyObject *);
typedef PyObject *(*iternextfunc) (PyObject *);
typedef PyObject *(*descrgetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*descrsetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*initproc)(PyObject *, PyObject *, PyObject *);
typedef PyObject *(*newfunc)(PyTypeObject *, PyObject *, PyObject *);
typedef PyObject *(*allocfunc)(PyTypeObject *, Py_ssize_t);
)";
	// output code defination
	std::cout << colorize_typedefs(code) << std::endl;
};
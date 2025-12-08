#ifndef LOG_H
#define LOG_H
#include <iostream>
#include <fstream>
#include <type_traits>
#include "DebugColours.h"
#include "KTYMath.h"
#include "KTYTime.h"

using namespace KTYTime;

class Debug {
private:
	enum LogType {
		Log,
		Warn,
		Err
	};

public:
	//To be used without defining a new Log class.
	static void Empty(std::streambuf* buffer = std::cout.rdbuf()) {
		std::streambuf* oldBuf = std::cout.rdbuf();
		std::cout.rdbuf(buffer);

		std::cout << "" << std::endl;

		std::cout.rdbuf(oldBuf);
	}
	template <typename T> 
	static void CLog(T obj = NULL, const char* col = DBG_WHITE, bool newLine = true, std::streambuf* buffer = std::cout.rdbuf()) {
		Out(LogType::Log, obj, col, newLine, buffer);
	}
	template <typename T>
	static void Error(T obj, const char* col = DBG_RED, bool newLine = true, std::streambuf* buffer = std::cout.rdbuf()) {
		Out(LogType::Err, obj, col, newLine, buffer);
	}
	template <typename T >
	static void Warning(T obj, const char* col = DBG_YELLOW, bool newLine = true, std::streambuf* buffer = std::cout.rdbuf()) {
		Out(LogType::Warn, obj, col, newLine, buffer);
	}
private:
	template <typename T>
	static void Out(LogType t, T obj, const char* col, bool newLine, std::streambuf* buffer) {

		std::streambuf* oldBuf = std::cout.rdbuf();
		std::cout.rdbuf(buffer);

		std::string prefix = "[" + Time::timestamp() + "] [DEBUG] ";
		std::string prefix2 = "";
		switch (t) {
		case Log:
			break;
		case Warn:
			prefix2 = "[WARN] ";
			break;
		case Err:
			prefix2 = "[ERROR] ";
			break;
		}
		if (newLine)
			std::cout << col << prefix << prefix2 << obj << DBG_RESET << std::endl;
		else
			std::cout << col << prefix << prefix2 << obj << DBG_RESET;
		std::cout.rdbuf(oldBuf);
	}
};

class DebugLogger {
private:
	enum LogType {
		Log,
		Warn,
		Err
	};

public:

	bool enabled = true; 
	const char* defaultColour = DBG_WHITE;

	std::string LoggerName = "LOG";
	DebugLogger(std::string name = "DEBUG") {
		LoggerName = name;
	}

	void Empty(std::streambuf* buffer = std::cout.rdbuf()) {
		std::streambuf* oldBuf = std::cout.rdbuf();
		std::cout.rdbuf(buffer);

		std::cout << "" << std::endl;

		std::cout.rdbuf(oldBuf);
	}

	template <typename T>
	void CLog(T obj = NULL, const char* col = 0, bool newLine = true, std::streambuf* buffer = std::cout.rdbuf()) {
		if (col == 0 && defaultColour != DBG_WHITE)
			col = defaultColour;
		else if (col == 0)
			col = DBG_WHITE;
		Out(LogType::Log, obj, col, newLine, buffer);
	}
	template <typename T>
	void Error(T obj, const char* col = DBG_RED, bool newLine = true, std::streambuf* buffer = std::cout.rdbuf()) {
		Out(LogType::Err, obj, col, newLine, buffer);
	}
	template <typename T>
	void Warning(T obj, const char* col = DBG_YELLOW, bool newLine = true, std::streambuf* buffer = std::cout.rdbuf()) {
		Out(LogType::Warn, obj, col, newLine, buffer);
	}

private:
	template <typename T>
	void Out(LogType t, T obj, const char* col, bool newLine, std::streambuf* buffer) {
		if (!enabled)
			return;
		std::streambuf* oldBuf = std::cout.rdbuf();
		std::cout.rdbuf(buffer);

		std::string prefix = "[" + Time::timestamp() + "] [" + LoggerName + "] ";
		std::string prefix2 = "";
		switch (t) {
		case Log:
			break;
		case Warn:
			prefix2 = "[WARN] ";
			break;
		case Err:
			prefix2 = "[ERROR] ";
			break;
		}
		if (newLine)
			std::cout << col << prefix << prefix2 << obj << DBG_RESET << std::endl;
		else
			std::cout << col << prefix << prefix2 << obj << DBG_RESET;
		std::cout.rdbuf(oldBuf);
	}
};

#endif
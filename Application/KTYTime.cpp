#include "KTYTime.h"

//this is messy and i dislike it

double KTYTime::Time::timeSinceDeviceStartup() {
	std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
	auto dur = std::chrono::duration<double>(t.time_since_epoch());
	return dur.count();
}
void KTYTime::Time::Init() {
	if (!_init) {
		_init = true;
		_timeAtStartup = timeSinceDeviceStartup();
	}
}
double KTYTime::Time::AtStartup() {
	return _timeAtStartup;
}

double KTYTime::Time::SinceStartup() {
	return timeSinceDeviceStartup() - AtStartup();
}
double KTYTime::Time::SinceStartup_m() { 
	return SinceStartup() * 1000;
}
std::string KTYTime::Time::timestamp() {
	return conv(dmy::Hour) + ":" + conv(dmy::Minute) + ":" + conv(dmy::Second);
}
double KTYTime::Time::_timeAtStartup;
double KTYTime::Time::_deltaTime;
bool KTYTime::Time::_init;
double KTYTime::Time::timescale = 1;
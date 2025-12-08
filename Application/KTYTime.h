#ifndef KTY_TIME
#define KTY_TIME

#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <chrono>

namespace KTYTime {
	class Time {
	private:

		static struct tm localtime() {
			struct tm t;
			time_t now = time(0);
			localtime_s(&t, &now);
			return t;
		};
		enum dmy {
			Day,
			Month,
			Year,
			Hour,
			Minute,
			Second
		};
		static std::string conv(dmy Dmy) {
			int t = 0;
			switch (Dmy) {
			case Day:
				t = localtime().tm_mday;
				break;
			case Month:
				t = localtime().tm_mon;
				break;
			case Year:
				t = localtime().tm_year;
				break;

			case Hour:
				t = localtime().tm_hour;
				break;
			case Minute:
				t = localtime().tm_min;
				break;
			case Second:
				t = localtime().tm_sec;
				break;
			}
			if (t < 10) {
				return "0" + std::to_string(t);
			}
			return std::to_string(t);

		}
		static double _timeAtStartup;
		static double _deltaTime;
		static bool _init;
	public:

		static void SetDeltaTime(double dt) { _deltaTime = dt; }
		
		static double timescale;
		/// <summary>
		/// Returns deltaTime multiplied by Time::timescale.
		/// </summary>
		/// <returns></returns>
		static double deltaTime() { return _deltaTime * timescale; }
		/// <summary>
		/// Returns deltatime without being multiplied by Time::timescale. Useful for controlling timescale smoothly.
		/// </summary>
		/// <returns></returns>
		static double rawDeltaTime() { return _deltaTime; }

		static double timeSinceDeviceStartup();

		static void Init();
		static double AtStartup();
		static double SinceStartup();

		static double SinceStartup_m();
		static std::string timestamp();

		class Timer {
		public:
			double Value() {
				if (_counting)
					return SinceStartup() - _timerStart;
				return _timerStop;
			}
			const bool& Counting() const { return _counting; }

			void StartNew() {
				Reset();
				Play();
			}
			void Play() {
				if (_counting)
					return; 
				_counting = true;
				_timerStart = SinceStartup() - _timerStop;
			}
			void Stop() {
				if (!_counting)
					return; 
				_counting = false;
				_timerStop = SinceStartup() - _timerStart;
			}
			void Restart() {
				StartNew();
			}
			void Reset() {
				_timerStart = SinceStartup();
				_timerStop = 0;
			}
		private:
			double _val;
			bool _counting;
			double _timerStart; 
			double _timerStop;
		};

		class dateTime {
		public:
			static std::string dmy() {				return conv(dmy::Day) + "/" + conv(dmy::Month) + "/" + conv(dmy::Year);
			}
			static std::string mdy() {				return conv(dmy::Month) + "/" + conv(dmy::Day) + "/" + conv(dmy::Year);
			}
			static int daylightSavings() { return localtime().tm_isdst; }
			static int year() { return localtime().tm_year + 1900; }
			static int month() { return localtime().tm_mon; }
			static int hour() { return localtime().tm_hour; }
			static int minute() { return localtime().tm_min; }
			static int second() { return localtime().tm_sec; }
			static int day() { return localtime().tm_mday; }
		};
	};


}
#endif
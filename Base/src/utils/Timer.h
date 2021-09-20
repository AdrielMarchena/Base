#pragma once
#include <string>
#include <chrono>
namespace Base
{
	template<typename Fn>
	class Timer
	{
	private:
		const char* m_Name;
		bool m_Stopped;
		Fn m_ResultLamb = nullptr;
		std::chrono::time_point<std::chrono::steady_clock> m_StartPoint;
	public:
		Timer(const char* name,Fn&& function)
			:m_Name(name), m_Stopped(false), m_ResultLamb(function)
		{
			m_StartPoint = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			if(!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimePoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartPoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

			m_Stopped = true;
			float duration = (end - start) * 0.001f;
			if (result_destination)
				*result_destination = duration;

			m_ResultLamb({ m_Name,duration });
		}
	};
}
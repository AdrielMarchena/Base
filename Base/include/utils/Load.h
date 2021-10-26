#pragma once

#include "Base/Base.h"
#include "utils/base_assert.h"
#include <vector>
#include <future>
#include <functional>
#include <unordered_map>
#include <string>
#include "utils/Files.h"
#include <mutex>
namespace Base
{
namespace utils
{
	template<typename InfoType>
	class Loader
	{
	private:
		std::unordered_map<std::string,std::future<InfoType>> m_Futures;
		std::vector<std::pair<std::string, std::string>> m_NameDirs;
		std::mutex m_Mutex;
	public:
		Loader(const std::vector<std::pair<std::string, std::string>>& name_dirs)
			:m_NameDirs(name_dirs)
		{}
		Loader(const std::string& dir,const std::string& exts = std::string())
		{
			if(exts == "" || exts.empty())
				m_NameDirs = utils::Files::GetPairText(dir);
			else
				m_NameDirs = utils::Files::GetPairText(dir,exts);
		}
		~Loader() { m_Futures.clear(); m_NameDirs.clear(); }

		/* Create a bunch of threads using std::async and store the std::future's */
		bool LauchAsync(const std::function<InfoType(const std::string&, const std::string&)>& lamb)
		{
			BASE_CORE_ASSERT(!m_NameDirs.empty(), "None Information to create something");

			for (auto& dir : m_NameDirs)
			{
				m_Futures[dir.first] = std::async(std::launch::async, lamb, dir.first, dir.second);
			}
			return true;
		}

		/* 
		* Create a Ref<'Result'> obj using the 'InfoType' returned by the std::future's
		* Return true if all threads finish their works
		* Call this in until you get a true, then you can destroy this instance
		* DoAsyncOne Create one Result then break and return
		*/
		template<typename InfoType,typename Result>
		bool DoAsyncOne(std::unordered_map<std::string,Ref<Result>>& results)
		{
			bool success = true;
			std::lock_guard<std::mutex> lock(m_Mutex);
			for (auto& pair : m_Futures)
			{
				if (pair.second._Is_ready())
				{
					results[pair.first] = MakeRef<Result>(pair.second.get());
					m_Futures.erase(pair.first);
					return false;
				}
				else
					success = false;
			}
			return success;
		}

		/*
		* Create a 'Result' obj using the 'InfoType' returned by the std::future's
		* Return true if all threads finish their works
		* Call this in until you get a true, then you can destroy this instance
		* DoAsyncOne Create one Result then break and return
		*/
		template<typename InfoType, typename Result>
		bool DoAsyncOne(std::unordered_map<std::string, Result>& results)
		{
			bool success = true;
			std::lock_guard<std::mutex> lock(m_Mutex);
			for (auto& pair : m_Futures)
			{
				if (pair.second._Is_ready())
				{
					//results[pair.first] = Result(pair.second.get());
					results.emplace(pair.first, pair.second.get());
					m_Futures.erase(pair.first);
					return false;
				}
				else
					success = false;
			}
			return success;
		}

		/*
	* Create a 'Result' obj using the 'InfoType' returned by the std::future's
	* Return true if all threads finish their works
	* Call this in until you get a true, then you can destroy this instance
	*/
		template<typename InfoType, typename Result>
		bool DoAsync(std::unordered_map<std::string, Ref<Result>>& results)
		{
			bool success = true;
			std::lock_guard<std::mutex> lock(m_Mutex);

			auto it = m_Futures.begin();
			while (it != m_Futures.end())
			{
				if (it->second._Is_ready())
				{
					results[it->first] = MakeRef<Result>(it->second.get());
					it = m_Futures.erase(it);
					success = false;
				}
				else
				{
					++it;
					success = false;
				}
			}
			return success;
		}
		/*
		* Create a 'Result' obj using the 'InfoType' returned by the std::future's
		* Return true if all threads finish their works
		* Call this in until you get a true, then you can destroy this instance
		*/
		template<typename InfoType, typename Result>
		bool DoAsync(std::unordered_map<std::string, Result>& results)
		{
			bool success = true;
			std::lock_guard<std::mutex> lock(m_Mutex);

			auto it = m_Futures.begin();
			while (it != m_Futures.end())
			{
				if (it->second._Is_ready())
				{
					results.emplace(it->first, it->second.get());
					it = m_Futures.erase(it);
					success = false;
				}
				else
				{
					++it;
					success = false;
				}
			}
			return success;
		}
	};
	
}
}
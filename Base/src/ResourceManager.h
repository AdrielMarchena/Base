#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <functional>

struct DumbTest
{
	DumbTest(std::string a)
	{
	}
};

template<typename T>
class ResourceManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<T>> m_Resources;
public:
	ResourceManager() = default;
	~ResourceManager() = default;

	// Iterate throw the resources and give as parameter to the lamb
	bool Map(const std::function<void(std::shared_ptr<T> sptr)>& lamb)
	{
		for (auto& sptr : m_Resources)
		{
			lamb(sptr.second);
		}
		return false;
	}

	template<class... _Args>
	std::shared_ptr<T> AddResource(std::string key, _Args&&... args)
	{
		m_Resources[key] = std::make_shared<T>(std::forward<_Args>(args)...);
		return m_Resources[key];
	}

	std::shared_ptr<T> operator[](const std::string& key) noexcept
	{
		if(m_Resources.find(key) != m_Resources.end())
			return m_Resources[key];
		//TODO: Do something when there is no thing here
	}
	std::shared_ptr<T> operator[](const char* key) noexcept
	{
		if (m_Resources.find(key) != m_Resources.end())
			return m_Resources[key];
		//TODO: Do something when there is no thing here
	}
};

/*template<typename T>
template<typename ..._Args>
inline std::shared_ptr<T> ResourceManager<T>::AddResource(std::string key,_Args ...resource_construct_args)
{
	m_Resources[key] = std::make_shared<T>(resource_construct_args);
	return m_Resources[key];
}*/

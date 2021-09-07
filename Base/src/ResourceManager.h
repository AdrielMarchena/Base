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

/* 
* Use as this example
* ResourceManager<T> manager;
* manager.AddResource("Name of my resource",arguments,to,construct,the,object);
* std::shared_ptr<T> my_resource = manager["Name of my resource"];
*/
template<typename T>
class ResourceManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<T>> m_Resources;
	const char* d_text = "default";
public:
	ResourceManager()
	{
		m_Resources[d_text] = std::make_shared<T>();
	}

	/*template<class... _Args>
	ResourceManager(_Args&&... args)
	{
		m_Resources[d_text] = std::make_shared<T>(std::forward<_Args>(args)...);
	}*/
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
	std::shared_ptr<T> AddResource(const std::string& key, _Args&&... args)
	{
		m_Resources[key] = std::make_shared<T>(std::forward<_Args>(args)...);
		return m_Resources[key];
	}

	std::shared_ptr<T> operator[](const std::string& key) noexcept
	{
		if (!key.empty())
			if(m_Resources.find(key) != m_Resources.end())
				return m_Resources[key];
		return m_Resources[d_text];
	}
	std::shared_ptr<T> operator[](const char* key) noexcept
	{
		if(key && key != "")
			if (m_Resources.find(key) != m_Resources.end())
				return m_Resources[key];
		return m_Resources[d_text];
	}
};

/*template<typename T>
template<typename ..._Args>
inline std::shared_ptr<T> ResourceManager<T>::AddResource(std::string key,_Args ...resource_construct_args)
{
	m_Resources[key] = std::make_shared<T>(resource_construct_args);
	return m_Resources[key];
}*/

#pragma once
#include <stdint.h>

#ifdef BASE_WINDOWS_BUILD

#include <xhash>

#else

#include <bits/functional_hash.h>

#endif
#include <unordered_map>

namespace Base {
	/* uuid type */
	typedef uint64_t UUID_T;
#define BASE_USING_UUID_64_T
	class UUID
	{
	public:
		UUID();
		UUID(UUID_T uuid);
		UUID(const UUID&) = default;

		operator UUID_T() const { return m_UUID; }
		static bool MetaInit();
	private:
		UUID_T m_UUID = NULL;
	};
}

namespace std {
#ifdef BASE_USING_UUID_64_T
	template<>
	struct hash<Base::UUID>
	{
		std::size_t operator()(const Base::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
#endif
}
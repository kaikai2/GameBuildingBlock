#ifndef _Common_AtomString_H_
#define _Common_AtomString_H_

#include <string>
#include <hash_map>
#include <vector>

namespace t4
{
	namespace internal
	{
		struct AtomStringManager
		{
			stdext::hash_map<std::string, size_t> mapName2Id;
			std::vector<std::string> vecAllNames;
		};
		template<typename Type>
		AtomStringManager &atomstring_manager()
		{
			static AtomStringManager s_id;
			return s_id;
		}

		template<typename Type>
		size_t alloc_id(const char *szName)
		{
			AtomStringManager &s_id = atomstring_manager<Type>();

			auto it = s_id.mapName2Id.find(szName);
			if (it == s_id.mapName2Id.end())
			{
				it = s_id.mapName2Id.insert(it, std::make_pair(szName, s_id.vecAllNames.size()));
				s_id.vecAllNames.push_back(szName);
			}
			return it->second;
		}

		template<typename Type>
		std::string getname(size_t id)
		{
			AtomStringManager &s_id = atomstring_manager<Type>();

			if (id < s_id.vecAllNames.size())
			{
				return s_id.vecAllNames[id];
			}
			return "";
		}
	}

	template<typename Type>
	class CAtomString
	{
	public:
		CAtomString() : m_nId(-1)
		{

		}
		CAtomString(const char *szName) : m_nId(internal::alloc_id<Type>(szName))
		{

		}
		std::string Name() const
		{
			return internal::getname<Type>(m_nId);
		}
		operator size_t() const
		{
			return m_nId;
		}
		bool operator < (const CAtomString &rhs) const
		{
			return m_nId < rhs.m_nId;
		}
		bool operator == (const CAtomString &rhs) const
		{
			return m_nId < rhs.m_nId;
		}
	private:
		size_t m_nId;
	};
}

#endif//_Common_AtomString_H_

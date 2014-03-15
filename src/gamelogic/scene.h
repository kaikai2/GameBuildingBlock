#ifndef _GameLogic_Scene_H_
#define _GameLogic_Scene_H_

#include "gameobject.h"

#include <set>
#include <map>
#include <limits>
#include <algorithm>

namespace stdext 
{
	template<class First, typename Second>
	inline size_t hash_value(const std::pair<First, Second>& _Keyval)
	{	// hash _Keyval to size_t value one-to-one
		return (hash_value(_Keyval.first) << 1) ^ hash_value(_Keyval.second);
	}
}

namespace t4
{
	namespace gamelogic
	{
		struct SceneTag{};
		typedef CAtomString<SceneTag> CSceneTagAtom;
		namespace internal
		{
			template<typename Object, typename TagType>
			class CTag
			{
			public:
				/// <FIXME> on object leave scene, all tag associated with it should be removed
				typedef CTag<Object, TagType> SelfType;
				CTag(const std::set<Object *> &setObjects) : m_setObjects(setObjects)
				{
				}
				void AddTag(Object &oObj, CSceneTagAtom key, TagType value) // add new tag-value pair
				{
					if (m_setObjects.count(&oObj))
					{
						auto tagkey = std::make_pair(key, value);
						m_mapTag.insert(std::make_pair(tagkey, &oObj));
					}
				}
				void SetTag(Object &oObj, CSceneTagAtom key, TagType value) // set remove all key-matching tag's and add new tag-value pair
				{
					if (m_setObjects.count(&oObj))
					{
						auto tagkey = std::make_pair(key, value);
						auto tagkeyLow = std::make_pair(key, std::numeric_limits<TagType>::min());
						auto tagkeyHigh = std::make_pair(key, std::numeric_limits<TagType>::max());
						auto ii = make_pair(m_mapTag.lower_bound(tagkeyLow), m_mapTag.upper_bound(tagkeyHigh));
						if (ii.first != ii.second)
						{
							m_mapTag.erase(ii.first, ii.second);
						}
						m_mapTag.insert(make_pair(tagkey, &oObj));
					}
				}
				void DelTag(Object &oObj, CSceneTagAtom key, TagType value) // remove tag-value pair
				{
					auto tagkey = std::make_pair(key, value);
					auto ii = m_mapTag.equal_range(tagkey);
					for (auto it = ii.first; it != ii.second;)
					{
						if (it->second == &oObj)
						{
							m_mapTag.erase(it++);
						}
						else
						{
							++it;
						}
					}
				}
				void AddTagKey(Object &oObj, CSceneTagAtom key) // add new tag-value pair with value = MAX_UINT
				{
					AddTag(oObj, key, std::numeric_limits<TagType>::max());
				}
				void DelTagKey(Object &oObj, CSceneTagAtom key) // remove all key-matching tag-value pair 
				{
					auto tagkeyLow = std::make_pair(key, std::numeric_limits<TagType>::min());
					auto tagkeyHigh = std::make_pair(key, std::numeric_limits<TagType>::max());
					auto ii = std::make_pair(m_mapTag.lower_bound(tagkeyLow), m_mapTag.upper_bound(tagkeyHigh));
					for (auto it = ii.first; it != ii.second;)
					{
						if (it->second == &oObj)
						{
							m_mapTag.erase(it++);
						}
						else
						{
							++it;
						}
					}
				}

				std::set<Object*> Find(CSceneTagAtom key, TagType value) // find all with tag-value
				{
					return std::set<Object*>();
				}
				struct FindHelper
				{
					FindHelper(SelfType &oTag) : m_oTag(oTag), m_bFirstCondition(true)
					{
					}
					FindHelper& HasKey(CSceneTagAtom key)
					{
						if (NeedCalc())
						{
							UpdateResult(m_oTag._Find(key, std::numeric_limits<TagType>::min(), std::numeric_limits<TagType>::max()));
						}
						return *this;
					}
					FindHelper& Value(CSceneTagAtom key, TagType value)
					{
						if (NeedCalc())
						{
							UpdateResult(m_oTag._Find(key, value, value));
						}
						return *this;
					}
					operator std::set<Object *> &()
					{
						return m_setCandidates;
					}
					operator Object *()
					{
						if (m_setCandidates.empty())
							return 0;
						return *m_setCandidates.begin();
					}
				private:
					bool NeedCalc() const
					{
						return m_bFirstCondition || !m_setCandidates.empty();
					}
					void UpdateResult(std::set<Object*> &setCandidates)
					{
						if (m_bFirstCondition)
						{
							m_setCandidates.swap(setCandidates);
							m_bFirstCondition = false;
						}
						else
						{
							std::set<Object *> setResult;
							std::set_intersection(m_setCandidates.begin(), m_setCandidates.end(), setCandidates.begin(), setCandidates.end(), std::inserter(setResult, setResult.end()));
							setResult.swap(m_setCandidates);
						}
					}
					SelfType &m_oTag;
					std::set<Object *> m_setCandidates;
					bool m_bFirstCondition;
				};
				FindHelper $()
				{
					return FindHelper(*this);
				}
			private:
				std::set<Object*> _Find(CSceneTagAtom key, TagType valueLow, TagType valueHigh) // find all with tag-value within [valueLow, valueHigh]
				{
					auto tagkeyLow = std::make_pair(key, std::numeric_limits<TagType>::min());
					auto tagkeyHigh = std::make_pair(key, std::numeric_limits<TagType>::max());
					auto ii = std::make_pair(m_mapTag.lower_bound(tagkeyLow), m_mapTag.upper_bound(tagkeyHigh));
					std::set<Object*> setResult;
					if (ii.first != ii.second)
					{
						for (auto it = ii.first; it != ii.second; ++it)
						{
							setResult.insert(it->second);
						}
					}
					return setResult;
				}
				const std::set<Object *> &m_setObjects;
				std::multimap<std::pair<CSceneTagAtom, TagType>, Object*> m_mapTag;
			};
		}

		class CScene
		{
		public:
			CScene() : m_oTag(m_setGameObjects)
			{
			}
			void Enter(CGameObject &oObj)
			{
				m_setGameObjects.insert(&oObj);
			}
			void Leave(CGameObject &oObj)
			{
				m_setGameObjects.erase(&oObj);
			}
			bool Has(CGameObject &oObj) const
			{
				return 0 != m_setGameObjects.count(&oObj);
			}
			typedef internal::CTag<CGameObject, UINT32> CTag32;

			CTag32 &Tag()
			{
				return m_oTag;
			}
			const CTag32 &Tag() const
			{
				return m_oTag;
			}
		private:

			std::set<CGameObject *> m_setGameObjects;
			CTag32 m_oTag;
		};
	}
}


#endif//_GameLogic_Scene_H_

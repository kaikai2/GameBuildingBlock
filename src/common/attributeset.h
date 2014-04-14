#ifndef _Common_AttributeSet_H_
#define _Common_AttributeSet_H_

#include "atomstring.h"
#include "cast.h"
#include "eventdispatcher.h"

#include <map>

namespace t4
{
	struct Attribute{};
	typedef CAtomString<Attribute> CAttributeAtom;

	class IAttributeHolder;
	class CAttributeSet;

	typedef CAtomString<IAttributeHolder> CAttributeHolderAtom;

	class IAttributeHolder
	{
	public:
		IAttributeHolder() : m_poSet(0){}
		virtual ~IAttributeHolder();
	protected:
		template<typename DataType, typename ConcreteAttributeHolder>
		bool Register(DataType ConcreteAttributeHolder::*pData)
		{
			return Register(Type2Atom<DataType, CAttributeAtom>(), pData);
		}
		template<typename DataType, typename ConcreteAttributeHolder>
		bool Register(CAttributeAtom asName, DataType ConcreteAttributeHolder::*pData);
	private:
		friend class CAttributeSet;
		void _Bind(CAttributeSet &oSet, CAttributeHolderAtom asHolder)
		{
			m_poSet = &oSet;
			m_asHolder = asHolder;
			//oSet.RegisterHolder(*this); call this in discreet class
		}
		CAttributeSet *m_poSet;
		CAttributeHolderAtom m_asHolder;
	};


	class CAttributeSet
	{
	public:
		typedef int IAttributeHolder::*DataTypeOfHolder;
		template<typename AttributeHolder>
		void RegisterHolder(AttributeHolder &oHolder)
		{
			RegisterHolder(Type2Atom<AttributeHolder, CAttributeHolderAtom>(), oHolder);
		}

		void RegisterHolder(CAttributeHolderAtom asHolder, IAttributeHolder &oHolder)
		{
			_HolderInfo(asHolder).poHolder = &oHolder;
			oHolder._Bind(*this, asHolder);
			m_mapHolderAtom[&oHolder] = asHolder;
		}
		void UnregisterHolder(IAttributeHolder &oHolder)
		{
			auto it = m_mapHolderAtom.find(&oHolder);
			if (it != m_mapHolderAtom.end())
			{
				if (_HolderInfo(it->second).poHolder == &oHolder)
				{
					m_mapInfo.erase(it->second);
					m_mapHolderAtom.erase(it);
				}
			}
		}

		template<typename AttributeHolder, typename DataType>
		bool Register(DataType IAttributeHolder::*pData)
		{
			return Register(Type2Atom<AttributeHolder, CAttributeHolderAtom>(),
				Type2Atom<DataType, CAttributeAtom>(),
				strong_cast<DataTypeOfHolder>(pData));
		}
		template<typename AttributeHolder, typename DataType>
		bool Register(CAttributeAtom asName, DataType IAttributeHolder::*pData)
		{
			return Register(Type2Atom<AttributeHolder, CAttributeHolderAtom>(),
				asName,
				pData);
		}
		bool Register(CAttributeHolderAtom asHolder, CAttributeAtom asName, DataTypeOfHolder pData);

		template<typename DataType>
		void Set(CAttributeHolderAtom asHolder, CAttributeAtom asName, const DataType &oData)
		{
			if (auto poAttribute = strong_cast<DataType IAttributeHolder::*>(_FindAttribute(asHolder, asName)))
			{
				*poAttribute = oData;
			}
		}


		template<typename DataType>
		const DataType& Get(CAttributeHolderAtom asHolder, CAttributeAtom asName, const DataType &oDefaultData)
		{
			if (auto poAttribute = strong_cast<DataType IAttributeHolder::*>(_FindAttribute(asHolder, asName)))
			{
				return *poAttribute;
			}
			return oDefaultData;
		}

		template<typename EventHandler, typename Handler>
		void BindChange(CAttributeAtom key, EventHandler *poHandler, Handler pFunc)
		{
			CEventAtom eventKey = strong_cast<CEventAtom>(key);
			m_oEventMgr.Bind(eventKey, poHandler, pFunc);
		}
	private:
		struct HolderInfo
		{
			IAttributeHolder *poHolder;
			std::map<CAttributeAtom, DataTypeOfHolder> mapAttribute;
		};
		int *_FindAttribute(CAttributeHolderAtom asHolder, CAttributeAtom asName)
		{
			HolderInfo &hi = _HolderInfo(asHolder);
			auto it = hi.mapAttribute.find(asName);
			if (it == hi.mapAttribute.end())
			{
				return 0;
			}
			return &(hi.poHolder->*(it->second));
		}
		HolderInfo &_HolderInfo(CAttributeHolderAtom asHolder)
		{
			return m_mapInfo[asHolder];
		}
		const HolderInfo &_HolderInfo(CAttributeHolderAtom asHolder) const
		{
			auto it = m_mapInfo.find(asHolder);
			if (it == m_mapInfo.end())
			{
				static HolderInfo h;
				return h;
			}
			return it->second;
		}
		std::map<CAttributeHolderAtom, HolderInfo> m_mapInfo;
		std::map<IAttributeHolder *, CAttributeHolderAtom> m_mapHolderAtom;

		CEventManager m_oEventMgr;
	};

	template<typename DataType, typename ConcreteAttributeHolder>
	bool IAttributeHolder::Register(CAttributeAtom asName, DataType ConcreteAttributeHolder::*pData)
	{
		if (m_poSet)
		{
			return m_poSet->Register(m_asHolder, asName, strong_cast<typename CAttributeSet::DataTypeOfHolder>(pData));
		}
		return false;
	}
} // namespace t4

#endif//_Common_AttributeSet_H_


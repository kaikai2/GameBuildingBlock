#include "attributeset.h"
namespace t4
{
	IAttributeHolder::~IAttributeHolder()
	{
		if (m_poSet)
		{
			m_poSet->UnregisterHolder(*this);
		}
	}

	bool CAttributeSet::Register(CAttributeHolderAtom asHolder, CAttributeAtom asName, DataTypeOfHolder pData)
	{
		if (!_HolderInfo(asHolder).mapAttribute.insert(std::make_pair(asName, pData)).second)
		{
			assert(false && "unable to register dupilicated attributes");
			return false;
		}
		return true;
	}


}

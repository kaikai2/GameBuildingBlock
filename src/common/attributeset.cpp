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


}

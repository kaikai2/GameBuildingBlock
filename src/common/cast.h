#ifndef _Common_Cast_H_
#define _Common_Cast_H_

#include <cassert>

namespace t4
{
	template<typename ToType, typename FromType>
	ToType strong_cast(FromType from)
	{
		//static_assert(sizeof(FromType) == sizeof(ToType));
		assert(sizeof(FromType) == sizeof(ToType));
		union CastUnion{
			FromType from;
			ToType to;
		}tmp;
		tmp.from = from;
		return tmp.to;
	}
}

#endif//_Common_Cast_H_

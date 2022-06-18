#pragma once
#include "Types/Type.h"

namespace RefLib
{
	class ContainerViewIterator;

	struct IContainerView
	{
		virtual ContainerViewIterator begin() = 0;
		virtual ContainerViewIterator end() = 0;
		virtual size_t Length() = 0;
		virtual Type GetType() = 0;
		virtual Type GetElementType() = 0;
	};
}
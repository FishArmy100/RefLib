#include "TypeData.h"
#include "Property/Property.h"

namespace RefLib
{
	TypeData::~TypeData()
	{
		for (auto propPtr : Properties)
			delete propPtr.second;

		Properties.clear();
	}
}
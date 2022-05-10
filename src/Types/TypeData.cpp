#include "TypeData.h"
#include "Property/Property.h"

namespace RefLib
{
	TypeData::~TypeData()
	{
		delete Properties;
		delete Methods;
	}
}
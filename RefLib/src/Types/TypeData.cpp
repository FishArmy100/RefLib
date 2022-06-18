#include "TypeData.h"
#include "Property/Property.h"
#include "Constructor/Constructor.h"
#include "Container/ContainerView.h"

namespace RefLib
{
	TypeData::~TypeData()
	{
		delete Properties;
		delete Methods;
		delete Constructors;
		delete EnumValue;
		delete BaseTypes;
		delete NestedTypes;
		delete AsContainerFunc;
	}
}
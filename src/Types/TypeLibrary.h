#pragma once
#include <memory>
#include <unordered_map>
#include "TypeData.h"

namespace RefLib
{
	class TypeLibrary
	{
	public:
		void AddData(TypeData data);
		const TypeData* GetData(TypeId id);
		uint64_t NumberOfTypes() { return m_Types.size(); }

	private:
		std::unordered_map<TypeId, TypeData> m_Types;
	};
}


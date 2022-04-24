#pragma once
#include <inttypes.h>
#include "TypeData.h"
#include "TypeLibrary.h"

namespace RefLib
{
	class Type
	{
	public:
		template<typename T>
		static Type Get()
		{
			static TypeId id = NoneType;
			if (id == NoneType)
			{
				id = s_CurrentId++;
				std::string name = Utils::GetTypeName<T>();
				s_Library.AddData(TypeData(name, id));
			}

			return Type(s_Library.GetData(id));
		}

	public:
		Type(const TypeData* data) : m_Data(data) {}

	private:
		static TypeId s_CurrentId;
		static TypeLibrary s_Library;

	private:
		const TypeData* m_Data;
	};
}


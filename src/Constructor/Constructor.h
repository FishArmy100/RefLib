#pragma once
#include "ConstructorData.h"
#include "Utils/Ref.h"

namespace RefLib
{
	class Constructor
	{
	public:
		Constructor(ConstructorData* data) : m_Data(data) {}
		Constructor(const Constructor& other) = default;
		~Constructor() = default;

		Variant Construct(std::vector<Argument> args)
		{
			return m_Data->ConstructorFunc(std::move(args));
		}

		Variant ConstructPtr(std::vector<Argument> args)
		{
			return m_Data->PtrConstructorFunc(std::move(args));
		}

		Type GetCreatedType() { return m_Data->InstatiatedType; }
		AccessLevel GetAccessLevel() { return m_Data->Level; }

		const Ref<std::vector<ParameterData>> GetParameters() 
		{ 
			return Ref<std::vector<ParameterData>>(&(m_Data->Parameters)); 
		}

	private:
		ConstructorData* m_Data;
	};
}


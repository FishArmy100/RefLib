#pragma once
#include "ConstructorData.h"
#include "Utils/Ref.h"

namespace RefLib
{
	class Constructor
	{
	public:
		Constructor(ConstructorData* data) : m_Data(data) {}
		Constructor() : m_Data(nullptr) {}
		Constructor(const Constructor& other) = default;
		~Constructor() = default;

		bool IsValid() { return m_Data != nullptr; }

		Variant Construct(std::vector<Argument> args)
		{
			return m_Data->ConstructorFunc(std::move(args));
		}

		Variant ConstructPtr(std::vector<Argument> args)
		{
			return m_Data->PtrConstructorFunc(std::move(args));
		}

		Type GetCreatedType() { return IsValid() ? m_Data->InstatiatedType : Type::Invalid(); }
		AccessLevel GetAccessLevel() { return IsValid() ? m_Data->Level : AccessLevel::Private; }

		const Ref<std::vector<ParameterData>> GetParameters() 
		{ 
			if(IsValid())
				return Ref<std::vector<ParameterData>>(&(m_Data->Parameters)); 

			return {};
		}

	private:
		ConstructorData* m_Data;
	};
}


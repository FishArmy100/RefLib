#pragma once
#include "ConstructorData.h"
#include "Utils/Ref.h"
#include "Attributes/AttributeImpl.h"

namespace RefLib
{
	class Constructor
	{
	public:
		Constructor(Ref<ConstructorData> data) : m_Data(data)  
		{
			if (data.IsNull()) 
				throw std::exception("Cannot contruct a constructor from a nullptr");
		}

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

		const std::vector<ParameterData>& GetParameters() const
		{ 
			return m_Data->Parameters; 
		}

		REFLIB_ATTRIBUTE_HOLDER_OBJECT_IMPL(*m_Data->Attributes);

	private:
		Ref<ConstructorData> m_Data; 
	};
}


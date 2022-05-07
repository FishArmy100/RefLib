#pragma once
#include "MethodData.h"

namespace RefLib
{
	class Method
	{
	public:
		Method() : m_Data(nullptr) {}
		Method(MethodData* data) : m_Data(data) { }
		Method(const Method& other) = default;
		~Method() = default;

		std::string_view GetName() { return IsValid() ? m_Data->Name : ""; }
		Type GetReturnType() { return IsValid() ? m_Data->ReturnType : Type::Invalid(); }
		const std::vector<ParameterData>& Parameters() { return IsValid() ? m_Data->Parameters : std::vector<ParameterData>(); }

		Variant Invoke(Reference ref, std::vector<Argument> args)
		{
			if (!IsValid())
				return Variant();

			return m_Data->CallFunc(ref, args);
		}

		bool IsValid() { return m_Data != nullptr; }

	private:
		MethodData* m_Data;
	};
}


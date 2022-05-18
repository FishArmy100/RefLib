#pragma once
#include "MethodData.h"

namespace RefLib
{
	class Method
	{
	public:
		Method(MethodData* data) : m_Data(data) { }
		Method(const Method& other) = default;
		~Method() = default;

		std::string_view GetName() { return m_Data->Name; }
		Type GetReturnType() { return m_Data->ReturnType; }
		const std::vector<ParameterData>& Parameters() { return m_Data->Parameters; }

		Variant Invoke(Instance ref, std::vector<Argument> args)
		{
			return m_Data->CallFunc(ref, args, m_Data->Parameters);
		}

	private:
		MethodData* m_Data;
	};
}


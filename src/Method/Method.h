#pragma once
#include "MethodData.h"

namespace RefLib
{
	class Method
	{
	public:
		Method(Ref<MethodData> data) : m_Data(data) 
		{
			if (data.IsNull())
				throw std::exception("Cannot have a method data of nullptr");
		}

		Method(const Method& other) = default;
		~Method() = default;

		std::string_view GetName() const { return m_Data->Name; }
		Type GetReturnType() const { return m_Data->ReturnType; }
		Type GetDeclaringType() const { return m_Data->DeclaringType; }
		const std::vector<ParameterData>& Parameters() const { return m_Data->Parameters; }
		AccessLevel GetAccessLevel() const { return m_Data->Level; }
		bool IsVoid() { return m_Data->ReturnType == Type::Get<void>(); }

		Variant Invoke(Instance ref, std::vector<Argument> args)
		{
			return m_Data->CallFunc(ref, args, m_Data->Parameters);
		}

	private:
		Ref<MethodData> m_Data;
	};
}


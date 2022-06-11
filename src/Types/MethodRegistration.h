#pragma once
#include "Type.h"
#include "Method/MethodData.h"
#include "Method/MethodContainer.h"
#include "TypeData.h"

namespace RefLib
{
	template<typename TClass, typename TReturn, typename... TArgs>
	void Type::RegisterMethod(const std::string& name, TReturn(TClass::* method)(TArgs...), std::vector<Type> templateArgs, AccessLevel level, const std::vector<std::string>& paramNames)
	{
		Type t = Type::Get<TClass>();

		if (t.IsRegistered())
		{
			t.GetData()->Methods->AddMethod(MethodData(name, method, templateArgs, level, paramNames));
		}
		else
		{
			if (t.GetData()->PreRegisteredMethods == nullptr)
			{
				t.GetData()->PreRegisteredMethods = new std::vector<MethodData>();
			}

			t.GetData()->PreRegisteredMethods->push_back(MethodData(name, method, templateArgs, level, paramNames));
		}
	}
}
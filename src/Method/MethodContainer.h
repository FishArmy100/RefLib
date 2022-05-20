#pragma once
#include "MethodData.h"
#include <map>
#include <unordered_map>

namespace RefLib
{
	class MethodContainer
	{
	public:
		MethodContainer(const std::vector<MethodData>& methods);

		Ref<MethodData> GetMethod(const std::string& name);
		Ref<MethodData> GetOverloadedMethod(const std::string& name, Type signature);
		std::vector<MethodData>& GetAll() { return m_Methods; }

	private:
		std::map<std::string, std::unordered_map<Type, MethodData>> m_MethodMap;
		std::vector<MethodData> m_Methods;
	};
}


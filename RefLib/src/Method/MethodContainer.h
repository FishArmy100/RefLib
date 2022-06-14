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
		MethodContainer(const MethodContainer&) = delete;
		~MethodContainer() = default;

		std::optional<Method> GetMethod(const std::string& name);
		std::optional<Method> GetMethod(size_t id);
		Ref<MethodData> GetMethodData(size_t id);
		Ref<std::vector<size_t>> GetMethods(const std::string& name);
		Ref<std::vector<size_t>> GetTemplatedMethods(const std::string& name);

		std::vector<MethodData>& GetAll() { return m_Methods; }

		void AddMethod(const MethodData& data);

	private:
		std::map<std::string, std::vector<size_t>> m_TemplatedMethodsMap;
		std::map<std::string, std::vector<size_t>> m_MethodsMap;
		std::vector<MethodData> m_Methods;
	};
}


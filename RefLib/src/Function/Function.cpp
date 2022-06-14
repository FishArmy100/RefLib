#include "Function.h"

namespace RefLib
{
	std::optional<Function> Function::Get(const std::string& name)
	{
		auto it = s_FunctionMap.find(name);
		if (it == s_FunctionMap.end())
			return {};

		return Function(*it->second.begin());
	}

	std::optional<Function> Function::Get(const std::string& name, const std::vector<Type>& paramTypes)
	{
		auto it = s_FunctionMap.find(name);
		if (it == s_FunctionMap.end())
			return {};

		std::vector<size_t>& indexes = it->second;
		for (size_t index : indexes)
		{
			if (Utils::TypeListCanCallParamData(s_FunctionDatas[index].Parameters, paramTypes))
				return Function(index);
		}

		return {};
	}

	std::optional<Function> Function::GetTemplated(const std::string& name, std::vector<Type> templateParams)
	{
		auto it = s_TemplatedFunctionMap.find(name);
		if (it == s_TemplatedFunctionMap.end())
			return {};

		std::vector<size_t>& indexes = it->second;
		for (size_t index : indexes)
		{
			if (s_FunctionDatas[index].TemplateParams == templateParams)
				return Function(index);
		}

		return {};
	}

	std::optional<Function> Function::GetTemplated(const std::string& name, const std::vector<Type>& templateParams, const std::vector<Type>& paramTypes)
	{
		auto it = s_TemplatedFunctionMap.find(name);
		if (it == s_TemplatedFunctionMap.end())
			return {};

		std::vector<size_t>& indexes = it->second;
		for (size_t index : indexes)
		{
			if (Utils::TypeListCanCallParamData(s_FunctionDatas[index].Parameters, paramTypes) &&
				s_FunctionDatas[index].TemplateParams == templateParams)
			{
				return Function(index);
			}
		}

		return {};
	}

	Variant Function::Invoke(const std::string& name, std::vector<Argument> args)
	{
		auto func = Get(name);
		if (func.has_value())
			return func->Invoke(args);

		return Variant();
	}

	Variant Function::Invoke(const std::string& name, const std::vector<Type> paramTypes, std::vector<Argument> args)
	{
		auto func = Get(name, paramTypes);
		if (func.has_value())
			return func->Invoke(args);

		return Variant();
	}

	Variant Function::InvokeTemplate(const std::string& name, const std::vector<Type>& templateArgs, std::vector<Argument> args)
	{
		auto func = GetTemplated(name, templateArgs);
		if (func.has_value())
			return func->Invoke(args);

		return Variant();
	}

	Variant Function::InvokeTemplate(const std::string& name, const std::vector<Type>& templateArgs, const std::vector<Type> paramTypes, std::vector<Argument> args)
	{
		auto func = GetTemplated(name, templateArgs, paramTypes);
		if (func.has_value())
			return func->Invoke(args);

		return Variant();
	}

	Variant Function::Invoke(std::vector<Argument> args) const
	{
		return GetData()->CallFunc(std::move(args), GetData()->Parameters);
	}
}
#pragma once
#include "FunctionData.h"
#include <memory>
#include <map>
#include "Attributes/AttributeImpl.h"

namespace RefLib
{
	class Function
	{
	public:
		template<typename TReturn, typename... TArgs>
		static void Register(const std::string& name, TReturn(*func)(TArgs...), const std::vector<std::string>& paramNames = {}, const std::vector<Variant>& attributes = {})
		{
			Register(name, func, {}, paramNames, attributes); 
		}

		template<typename TReturn, typename... TArgs>
		static void Register(const std::string& name, TReturn(*func)(TArgs...), const std::vector<TypeId>& templateParams, const std::vector<std::string>& paramNames = {}, const std::vector<Variant>& attributes = {})
		{
			s_FunctionDatas.emplace_back(name, func, templateParams, paramNames, attributes);
			size_t index = s_FunctionDatas.size() - 1;

			if (s_FunctionDatas[index].IsTemplated())
				s_TemplatedFunctionMap[s_FunctionDatas[index].Name].push_back(index);
			else
				s_FunctionMap[s_FunctionDatas[index].Name].push_back(index);
		}

		static std::optional<Function> Get(const std::string& name);
		static std::optional<Function> Get(const std::string& name, const std::vector<Type>& paramTypes);
		static std::optional<Function> GetTemplated(const std::string& name, std::vector<Type> templateParams);
		static std::optional<Function> GetTemplated(const std::string& name, const std::vector<Type>& templateParams, const std::vector<Type>& paramTypes);

		static Variant Invoke(const std::string& name, std::vector<Argument> args);
		static Variant Invoke(const std::string& name, const std::vector<Type> paramTypes, std::vector<Argument> args);
		static Variant InvokeTemplate(const std::string& name, const std::vector<Type>& templateArgs, std::vector<Argument> args);
		static Variant InvokeTemplate(const std::string& name, const std::vector<Type>& templateArgs, const std::vector<Type> paramTypes, std::vector<Argument> args);

	public:
		Function() = delete;
		Function(const Function&) = default;
		~Function() = default;

		Variant Invoke(std::vector<Argument> args) const;
		const std::string& GetName() const { return GetData()->Name; }
		const ParameterList& GetParameters() const { return GetData()->Parameters; }
		const std::vector<Type> GetTemplateParams() const { return GetData()->TemplateParams; }
		bool IsTemplated() const { return GetData()->IsTemplated(); }
		Type GetSignatureType() const { return GetData()->SignatureType; }

		REFLIB_ATTRIBUTE_HOLDER_OBJECT_IMPL(*GetData()->Attributes);

	private:
		Function(size_t index) : m_Index(index) 
		{}

	private:
		size_t m_Index;
		Ref<FunctionData> GetData() const { return Ref(s_FunctionDatas[m_Index]); }

	private:
		inline static std::vector<FunctionData> s_FunctionDatas;
		inline static std::map<std::string, std::vector<size_t>> s_FunctionMap;
		inline static std::map<std::string, std::vector<size_t>> s_TemplatedFunctionMap;
	};
}

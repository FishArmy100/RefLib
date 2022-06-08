#pragma once
#include "MethodData.h"
#include "MethodContainer.h"
#include "Attributes/AttributeImpl.h"

namespace RefLib
{
	class Method
	{
	public:
		Method(size_t id, Ref<MethodContainer> container) : m_Index(id), m_Container(container)
		{
			if (container.IsNull())
				throw std::exception("Cannot have a method data of nullptr");
		}

		Method(const Method& other) = default;
		~Method() = default;

		std::string_view GetName() const { return  GetData()->Name; }
		Type GetReturnType() const { return  GetData()->ReturnType; }
		Type GetDeclaringType() const { return  GetData()->DeclaringType; }
		const std::vector<ParameterData>& GetParameters() const { return GetData()->Parameters; }
		const std::vector<TypeId> GetTemplateParams() const { return GetData()->TemplateParams; }
		bool IsTemplate() { return GetData()->IsTemplated(); }
		AccessLevel GetAccessLevel() const { return  GetData()->Level; }
		bool IsVoid() const { return GetData()->ReturnType == Type::Get<void>(); }

		Variant Invoke(Instance ref, std::vector<Argument> args)
		{
			return GetData()->CallFunc(ref, args, GetData()->Parameters);
		}

		Variant Invoke(std::vector<Argument> args);

		REFLIB_ATTRIBUTE_HOLDER_OBJECT_IMPL(*GetData()->Attributes);

	private:
		Ref<MethodData> GetData() const { return m_Container->GetAll().at(m_Index); }

	private:
		size_t m_Index;
		mutable Ref<MethodContainer> m_Container;
	};
}


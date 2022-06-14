#pragma once
#include "Types/TypeId.h"
#include "Utils/Ref.h"
#include <map>
#include <string>
#include <vector>

namespace RefLib
{
	class Type;
	class Variant;

	class AttributeHolder
	{
	public:
		AttributeHolder(const std::vector<Variant>& attributes);
		AttributeHolder(const AttributeHolder&) = default;
		~AttributeHolder() = default;

		Ref<const Variant> GetAttribute(Type t) const;
		Ref<const Variant> GetAttribute(const std::string& name) const;
		const std::vector<Variant>& GetAttributes() const { return m_Attributes; }

	private:
		std::map<TypeId, size_t> m_AttributeMap;
		std::vector<Variant> m_Attributes;
	};
}


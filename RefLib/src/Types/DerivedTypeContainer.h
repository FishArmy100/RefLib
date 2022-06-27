#pragma once
#include "Type.h"

namespace RefLib
{
	class DerivedTypeContainer
	{
	public:
		DerivedTypeContainer(const std::vector<Type>& types);

		const std::vector<Type>& GetDerivedTypes() const { return m_DerivedTypes; }
		std::optional<Type> GetDerivedType(const std::string& name) const;
		void AddDerivedType(Type t);

	private:
		std::vector<Type> m_DerivedTypes;
	};
}


#pragma once
#include "Types/Type.h"
#include <type_traits>

namespace RefLib
{
	class Varient;
	class Reference;

	class Argument
	{
		template<typename T, typename Tp = Utils::decay_t<T>>
		using decay_argument_t = Utils::enable_if_t<!std::is_same<Argument, Tp>::value, T>;

	public:
		Argument() : m_Data(nullptr), m_IsVarient(false), m_Type(Type()) {}



	private:
		const void* m_Data;
		const bool m_IsVarient;
		const Type m_Type;
	};
}


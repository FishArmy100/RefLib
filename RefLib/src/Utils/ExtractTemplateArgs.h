#pragma once
#include "Types/Type.h"

namespace RefLib
{
	namespace Utils
	{
		template<typename T>
		struct ExtractTemplateArgs
		{
			static constexpr bool IsTemplate = false;
			static constexpr size_t ArgNumber = 0;
			static std::vector<Type> GetTypes() { return {}; }
		};

		template<template<typename...> class T, typename... TArgs>
		struct ExtractTemplateArgs<T<TArgs...>>
		{
			static constexpr bool IsTemplate = true;
			static constexpr size_t ArgNumber = sizeof...(TArgs);
			static std::vector<Type> GetTypes() { return Type::GetMultiple<TArgs...>(); }
		};
	}
}
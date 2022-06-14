#pragma once

namespace RefLib
{
	template<typename T>
	struct TypeRegistrationFactory
	{
		TypeRegistrationFactory() = default;
		bool operator()() { return false; }
	};
}
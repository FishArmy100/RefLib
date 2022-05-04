#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iterator>

namespace RefLib
{
	template<typename T> 
	using MemberContainer = std::unordered_map<std::string, T>;

	namespace Utils
	{
		template<typename T>
		std::vector<T> GetMemberVector(MemberContainer<T>& container)
		{
			std::vector<T> members = std::vector<T>(container.size());
			std::transform(container.begin(), container.end(), std::back_inserter(members), [](auto& kv) { return kv.second; });
			return members;
		}

		template<typename T, typename TOut, typename TConverter>
		std::vector<TOut> GetMemberVector(MemberContainer<T>& container, TConverter converter)
		{
			std::vector<TOut> members = std::vector<TOut>(container.size());
			std::transform(container.begin(), container.end(), std::back_inserter(members), [=](auto& kv) { return converter(kv); });
			return members;
		}
	}
}
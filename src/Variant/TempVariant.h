#pragma once
#include "Types/Type.h"
#include <optional>

namespace RefLib
{
	class TempVariant
	{
	public:
		template<typename T>
		static TempVariant Create(void* data)
		{
			auto deleteData = [](void* ptr) { delete(T*)ptr; };
			auto copyData = [](void* ptr) { return (void*)new T(*(T*)ptr); };
			auto deleteDataPtr = [](void* ptr)
			{
				if constexpr (std::is_pointer_v<T>)
				{
					delete* (T*)ptr;
					return true;
				}

				return false;
			};

			auto getDereferenced = [](void*)
			{
				if constexpr (std::is_pointer_v<T>)
				{
					void* derefData = *(T*)Data;
					return TempVariant::Create<std::remove_pointer_t<T>>(derefData);
				}

				return std::optional<TempVariant>{};
			};

			Type t = Type::Get<T>();
			return TempVariant(t, data, deleteData, copyData, deleteDataPtr, getDereferenced);
		}

		Type DataType;
		void* Data;

		void(*DeleteData)(void*);
		void*(*CopyData)(void*);
		bool(*DeleteDataPtr)(void*);
		std::optional<TempVariant>(*GetDereferenced)(void*);

		TempVariant() = default;

	private:
		TempVariant(Type type, void* data, void(*deleteData)(void*), void*(*copyData)(void*), bool(*deleteDataPtr)(void*), std::optional<TempVariant>(*getDereferenced)(void*)) :
			DataType(type), Data(data), DeleteData(deleteData), CopyData(copyData), DeleteDataPtr(deleteDataPtr), GetDereferenced(getDereferenced)
		{}
	};
}
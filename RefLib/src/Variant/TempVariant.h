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
					delete *(T*)ptr;
					return true;
				}

				return false;
			};

			std::optional<std::function<void(void*, void*)>> setData = {};

			if constexpr (std::is_assignable_v<T, T>)
			{
				setData = [](void* self, void* data)
				{
					T* selfPtr = (T*)self;
					T* dataPtr = (T*)data;
					*selfPtr = *dataPtr;
				};
			}

			std::function<std::optional<TempVariant>()> getDereferenced = [=]()
			{
				if constexpr (std::is_pointer_v<T> && !std::is_const_v<std::remove_pointer_t<T>>)
				{
					void* derefData = *(T*)Data;
					return std::optional<TempVariant>(TempVariant::Create<std::remove_pointer_t<T>>(derefData));
				}

				return std::optional<TempVariant>{};
			};

			Type t = Type::Get<T>();
			return TempVariant(t, data, deleteData, copyData, deleteDataPtr, setData, getDereferenced);
		}

		Type DataType;
		void* Data;

		void(*DeleteData)(void*);
		void*(*CopyData)(void*);
		bool(*DeleteDataPtr)(void*);
		std::optional<std::function<void(void*, void*)>> SetData;;
		std::function<std::optional<TempVariant>()> GetDereferenced;

		TempVariant() = default;

	private:
		TempVariant(Type type, void* data, void(*deleteData)(void*), void*(*copyData)(void*), bool(*deleteDataPtr)(void*), std::optional<std::function<void(void*, void*)>> setData, std::function<std::optional<TempVariant>()> getDereferenced) :
			DataType(type), Data(data), DeleteData(deleteData), CopyData(copyData), DeleteDataPtr(deleteDataPtr), SetData(setData), GetDereferenced(getDereferenced)
		{}
	};
}
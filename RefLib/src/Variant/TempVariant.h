#pragma once
#include "Types/Type.h"
#include <optional>

namespace RefLib
{
	class TempVariant
	{
	public:
		template<typename TBase>
		static TempVariant Create(void* data)
		{
			auto deleteData = [](void* ptr) { delete(TBase*)ptr; };
			auto copyData = [](void* ptr) { return (void*)new TBase(*(TBase*)ptr); };
			auto deleteDataPtr = [](void* ptr)
			{
				if constexpr (std::is_pointer_v<TBase>)
				{
					delete *(TBase*)ptr;
					return true;
				}

				return false;
			};

			auto getDereferenced = [](void*)
			{
				if constexpr (std::is_pointer_v<TBase>)
				{
					void* derefData = *(TBase*)Data;
					return TempVariant::Create<std::remove_pointer_t<TBase>>(derefData);
				}

				return std::optional<TempVariant>{};
			};

			Type t = Type::Get<TBase>();
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
#include <iostream>
#include "Types/TypeUtils.h"
#include <type_traits>
#include "Types/Type.h"
#include "Instance/Instance.h"
#include "Variant/Variant.h"
#include "Property/Property.h"
#include "Registration/TypeBuilder.h"
#include "Registration/EnumBuilder.h"
#include "Method/Method.h"
#include "Constructor/Constructor.h"
#include "Enum/Enum.h"
#include "Enum/EnumData.h"

template<typename T>
struct Position
{
	Position(const T& x, const T& y, const T& z) :
		X(x), Y(y), Z(z) 
	{}

	T X;
	T Y;
	T Z;
};

namespace RefLib
{
	template<typename T>
	struct TypeRegistrationFactory<Position<T>>
	{
		TypeRegistrationFactory() = default;
		bool operator()()
		{
			TypeBuilder<Position<T>> builder = TypeBuilder<Position<T>>("Position<" + (std::string)Type::Get<T>().GetName() + ">");
			builder.AddConstructor<const T&, const T&, const T&>({ "x", "y", "z" });
			builder.AddProperty("X", &Position<T>::X);
			builder.AddProperty("Y", &Position<T>::Y);
			builder.AddProperty("Z", &Position<T>::Z);
			builder.Register();
			return true;
		}
	};
}

using namespace RefLib;

int main()
{
	Type t = Type::Get<Position<int>>();
	Variant var = t.Create({ 5, 6, 7 });

	Position pos = var.TryConvert<Position<int>>().value();

	std::cout << "Type name: " << t.GetName() << ", Values: (" << pos.X << ", " << pos.Y << ", " << pos.Z << ")";
} 
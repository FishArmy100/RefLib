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
#include "Types/MethodRegistration.h"

struct Test
{
	Test() = default;

	template<typename T>
	void Print(const T& value)
	{
		std::cout << value << "\n";
	}
};

using namespace RefLib;

int main()
{
	Type::RegisterMethod("Print", &Test::Print<int>, { Type::Get<int>().GetId() });

	TypeBuilder<Test> builder = TypeBuilder<Test>("Test");
	builder.AddConstructor<>();
	builder.AddMethod("Print", &Test::Print<std::string>, { Type::Get<std::string>().GetId() });
	builder.Register();

	Type::RegisterMethod("Print", &Test::Print<float>, { Type::Get<float>().GetId() });

	Type t = Type::Get<Test>();
	t.InvokeTemplatedMethod("Print", { Type::Get<int>().GetId() }, { 42 });
	t.InvokeTemplatedMethod("Print", { Type::Get<std::string>().GetId() }, { std::string("Hello World!") });
	t.InvokeTemplatedMethod("Print", { Type::Get<float>().GetId() }, { 4.5f });
} 
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
#include "Function/Function.h"

template<typename T>
void Print(const T& value)
{
	std::cout << value << "\n";
}

int Add(int x, int y) { return x + y; }
float Add(float x, float y) { return x + y; }

using namespace RefLib;

int main()
{
	Function::Register("Add", static_cast<int(*)(int, int)>(&Add), {std::string("x"), std::string("y") });
	Function::Register("Add", static_cast<float(*)(float, float)>(&Add), { std::string("x"), std::string("y") });

	Function::Register("Print", &Print<int>, { Type::Get<int>().GetId() }, { "value" });
	Function::Register("Print", &Print<float>, { Type::Get<float>().GetId() }, { "value" });
	Function::Register("Print", &Print<std::string>, { Type::Get<std::string>().GetId() }, { "value" });

	Function addInt = Function::Get("Add", Type::GetMultiple<int, int>()).value();
	Function addFloat = Function::Get("Add", Type::GetMultiple<float, float>()).value();

	std::cout
		<< "Add ints 5 and 12: " << addInt.Invoke({ 5, 12 }).TryConvert<int>().value() << "\n"
		<< "Add floats 2.3f and 5.2f: " << addFloat.Invoke({ 2.3f, 5.2f }).TryConvert<float>().value() << "\n";
	
	std::cout << "Print an int: "; 
	Function::InvokeTemplate("Print", { Type::Get<int>().GetId() }, { 4 });

	std::cout << "Print an float: ";
	Function::InvokeTemplate("Print", { Type::Get<float>().GetId() }, { 4.2f });

	std::cout << "Print an std::string: ";
	Function::InvokeTemplate("Print", { Type::Get<std::string>().GetId() }, { std::string("Hello World!") });
}
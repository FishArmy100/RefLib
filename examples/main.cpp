#include <iostream>
#include "Types/TypeUtils.h"
#include <type_traits>
#include "Types/Type.h"
#include "Instance/Instance.h"
#include "Variant/Variant.h"
#include "Property/Property.h"
#include "Registration/TypeBuilder.h"
#include "Method/Method.h"
#include "Constructor/ConstructorData.h"
#include "Enum/Enum.h"
#include "Enum/EnumData.h"

struct Position
{
	int x = 0;
	float y = 0;
	float z = 0;
};

struct Entity 
{
	virtual uint64_t GetEntityId() = 0;
};

class Person : public Entity
{
public:
	Position Pos{};

	Person(const std::string& name, int id) : m_Name(name), m_ID(id) {}
	Person(const char* name, int id) : m_Name(name), m_ID(id) {}

	uint64_t GetEntityId() override { return m_ID; }

	int GetId() const { return m_ID; }
	const std::string& GetName() const { return m_Name; }

private:
	void SayHello() { std::cout << "hello" << std::endl; }

	int m_ID;
	std::string m_Name;
};

struct Test
{
	Test(int x, int y) : X(x), Y(y) {}

	int X;
	const int Y;

	int Add(int a, int b) { std::cout << "Version 1 called \n"; return a + b; }
	float Add(float a, float b) { std::cout << "Version 2 called \n"; return a + b; }
};

using namespace RefLib;

int main()
{
	TypeBuilder<Test> builder = TypeBuilder<Test>("Test");
	builder.AddConstructor<int, int>();
	builder.AddProperty("X", &Test::X);
	builder.AddProperty("Y", &Test::Y);
	builder.AddMethod("Add", static_cast<int(Test::*)(int,int)>(&Test::Add));
	builder.AddMethod("Add", static_cast<float(Test::*)(float, float)>(&Test::Add));
	builder.Register();

	Enum e = new EnumData()
}
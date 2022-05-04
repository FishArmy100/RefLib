#include <iostream>
#include "Types/TypeUtils.h"
#include <type_traits>
#include "Types/Type.h"
#include "Reference/Reference.h"
#include "Variant/Variant.h"
#include "Property/Property.h"
#include "Registration/TypeBuilder.h"

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
};

using namespace RefLib;

int main()
{
	TypeBuilder<Test> builder = TypeBuilder<Test>("Test");
	builder.AddProperty("X", &Test::X);
	builder.AddProperty("Y", &Test::Y);
	builder.Register();

	Property prop = Type::Get<Test>().GetProperty("Hello");
	Test t = Test(5, 6);
	const int&& value = 200;
	prop.Set(t, value);
	std::cout << prop.Get(t).TryConvert<int>().value();
}
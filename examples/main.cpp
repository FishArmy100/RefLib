#include <iostream>
#include "Types/TypeUtils.h"
#include <type_traits>
#include "Types/Type.h"
#include "Reference/Reference.h"
#include "Variant/Variant.h"
#include "Property/Property.h"
#include "Registration/TypeBuilder.h"
#include "Method/Method.h"
#include "Constructor/ConstructorData.h"

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
	/*TypeBuilder<Test> builder = TypeBuilder<Test>("Test");
	builder.AddProperty("X", &Test::X);
	builder.AddProperty("Y", &Test::Y);
	builder.AddMethod("Add", static_cast<int(Test::*)(int,int)>(&Test::Add));
	builder.AddMethod("Add", static_cast<float(Test::*)(float, float)>(&Test::Add));
	builder.Register();

	Test test = Test(1, 2);
	Type type = Type::Get<Test>();
	Method m = type.GetMethod("Add");
	m.Invoke(test, { 5, 5 });*/

	std::shared_ptr<Test>(*ctor)(int, int) = [](int x, int y) {return std::make_shared<Test>(5, 5); };
	ConstructorData data = ConstructorData(ctor, AccessLevel::Public);

	std::shared_ptr<Test> test = data.ConstructorFunc({ 5, 5 }).TryConvert<std::shared_ptr<Test>>().value();
	std::cout << test->X;
}
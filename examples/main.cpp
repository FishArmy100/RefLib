#include <iostream>
#include "Types/TypeUtils.h"
#include <type_traits>
#include "Types/Type.h"
#include "Reference/Reference.h"
#include "Variant/Variant.h"
#include "Property/Property.h"

struct Position
{
	int x{};
	float y{};
	float z{};
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

}
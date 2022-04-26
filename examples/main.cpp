#include <iostream>
#include "Types/TypeUtils.h"
#include <type_traits>
#include "Types/Type.h"
#include "Reference/Reference.h"
#include "Varient/Varient.h"
#include "Property/PropertyData.h"

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

using namespace RefLib;

int main()
{
	Position pos{};
	pos.x = 3;

	PropertyData data = PropertyData("x", &Position::x);
	Variant v = 5;
	data.Set(pos, v);

	std::cout << data.Get(pos).TryConvert<int>().value();
}
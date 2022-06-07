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

struct Player
{
	Player(const std::string& name) : Name(name) {}
	Player(const Player&) = default;
	~Player() = default;

	std::string Name;
};

struct ComponentAttribute {};

using namespace RefLib;

int main()
{
	auto attribBuilder = TypeBuilder<ComponentAttribute>("ComponentAttribute");
	attribBuilder.Register();

	TypeBuilder<Player> builder = TypeBuilder<Player>("Player");
	builder.AddConstructor<const std::string&>();
	builder.AddProperty("Name", &Player::Name);
	builder.AddAttribute<ComponentAttribute>({});
	builder.Register();

	std::cout << std::boolalpha << Type::Get<Player>().HasAttribute("ComponentAttribute");
}
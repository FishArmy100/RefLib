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

	const std::string& GetName() const
	{
		return Name;
	}

	std::string Name;
};

enum class Difficulty
{
	Easy,
	Medium,
	Bannanas
};

bool RegisterPlayer(RefLib::Ref<Player> player)
{
	std::cout << "Player registered!: " << player->Name << "\n";
	return true;
}

struct ComponentAttribute {};
struct NameAttribute {};
struct MethodAttribute {};
struct ConstructorAttribute {};
struct EnumAttribute {};
struct FunctionAttribute {};

using namespace RefLib;

int main()
{
	TypeBuilder<ComponentAttribute>("ComponentAttribute").Register();
	TypeBuilder<NameAttribute>("NameAttribute").Register();
	TypeBuilder<MethodAttribute>("MethodAttribute").Register();
	TypeBuilder<ConstructorAttribute>("ConstructorAttribute").Register();;
	TypeBuilder<EnumAttribute>("EnumAttribute").Register();
	TypeBuilder<FunctionAttribute>("FunctionAttribute").Register();

	TypeBuilder<Player> builder = TypeBuilder<Player>("Player");  
	builder.AddConstructor<const std::string&>(AccessLevel::Public, { "name" }, {ConstructorAttribute()}); 
	builder.AddProperty("Name", &Player::Name, AccessLevel::Public, {NameAttribute()}); 
	builder.AddMethod("GetName", &Player::GetName, AccessLevel::Public, {}, { MethodAttribute() }); 
	builder.AddAttribute<ComponentAttribute>({});  
	builder.Register(); 

	auto enumBuilder = EnumBuilder<Difficulty>("Difficulty");
	enumBuilder.AddValue("Easy", Difficulty::Easy);
	enumBuilder.AddValue("Medium", Difficulty::Medium);
	enumBuilder.AddValue("Bannanas", Difficulty::Bannanas);
	enumBuilder.AddAttribute(EnumAttribute());
	enumBuilder.Register();

	Function::Register("RegisterPlayer", &RegisterPlayer, { "player" }, { FunctionAttribute() });

	std::cout << std::boolalpha << Type::Get<Player>().HasAttribute("ComponentAttribute") << "\n";
	std::cout << std::boolalpha << Type::Get<Player>().GetProperty("Name")->HasAttribute("NameAttribute") << "\n";
	std::cout << std::boolalpha << Type::Get<Player>().GetMethod("GetName")->HasAttribute("MethodAttribute") << "\n";
	std::cout << std::boolalpha << Type::Get<Player>().GetConstructor(Type::GetMultiple<std::string>())->HasAttribute("ConstructorAttribute") << "\n\n";

	Type enumType = Type::Get<Difficulty>();
	std::cout << std::boolalpha << "Enum Type: " << enumType.HasAttribute("EnumAttribute") << "\n";
	std::cout << std::boolalpha << "Enum: " << enumType.AsEnum()->HasAttribute("EnumAttribute") << "\n\n";

	std::cout << std::boolalpha << Function::Get("RegisterPlayer")->HasAttribute("FunctionAttribute");
}
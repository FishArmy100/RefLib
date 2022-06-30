#pragma once
#include "TypeBuilder.h"

// ------------------ Auto Registration ------------------
#define REFLIB_REGISTRATION									\
void __Registration_Func__();								\
struct __Registration_Struct__								\
{															\
	__Registration_Struct__()								\
	{														\
		__Registration_Func__();							\
	}														\
};															\
static const __Registration_Struct__ __structInstance{};	\
void __Registration_Func__()								\


// ------------------ Registration Friend ------------------
#define REFLIB_FRIEND									\
friend void __Registration_Func__();					\

// ------------------ Begin Registration ------------------
#define REFLIB_BEGIN_CLASS(className)			\
{												\
	using namespace RefLib;						\
	using ClassType = className;				\
	TypeBuilder<ClassType>(#className)			\

// ------------------ End Registration ------------------
#define REFLIB_END_CLASS()	\
	.Register();			\
}							\

// ------------------ Register Constructor ------------------
#define REFLIB_CTOR(...)		\
	.AddConstructor<__VA_ARGS__>()	\

// ------------------ Register Property ------------------
#define REFLIB_PROP(propertyName, accessLevel, attributes)							\
	.AddProperty(#propertyName, &ClassType::propertyName, accessLevel, attributes)	\

#define REFLIB_PROP_ACCESS(propertyName, accessLevel)								\
.AddProperty(#propertyName, &ClassType::propertyName, accessLevel, {})				\

#define REFLIB_PROP_BASIC(propertyName)												\
.AddProperty(#propertyName, &ClassType::propertyName, AccessLevel::Public, {})		\

// ------------------ Register Method ------------------
#define REFLIB_METH_BASIC(returnType, methodName, params)																	\
.AddMethod(#methodName, static_cast<returnType(ClassType::*)params>(&ClassType::methodName), AccessLevel::Public, {}, {})	\

#define REFLIB_METH(returnType, methodName, params, accessLevel, paramNames, attributes)												\
.AddMethod(#methodName, static_cast<returnType(ClassType::*)params>(&ClassType::methodName), accessLevel, paramNames, attributes)		\

#define REFLIB_METH_TEMP_BASIC(returnType, methodName, params, templateArgs)															\
.AddTemplateMethod(#methodName, static_cast<returnType(ClassType::*)params>(&ClassType::methodName##templateArgs), Type::GetMultiple##templateArgs##(), AccessLevel::Public, {}, {})	\

#define REFLIB_METH_TEMP(returnType, methodName, params, templateArgs, accessLevel, paramNames, attributes)								\
.AddTemplateMethod(#methodName, static_cast<returnType(ClassType::*)params>(&ClassType::methodName##templateArgs), Type::GetMultiple##templateArgs##(), accessLevel, paramNames, attributes)	\

// const versions
#define REFLIB_CONST_METH_BASIC(returnType, methodName, params)																		\
.AddMethod(#methodName, static_cast<returnType(ClassType::*)params const>(&ClassType::methodName), AccessLevel::Public, {}, {})	\

#define REFLIB_CONST_METH(returnType, methodName, params, accessLevel, paramNames, attributes)													\
.AddMethod(#methodName, static_cast<returnType(ClassType::*)params const>(&ClassType::methodName), accessLevel, paramNames, attributes)		\

#define REFLIB_CONST_METH_TEMP_BASIC(returnType, methodName, params, templateArgs)																\
.AddTemplateMethod(#methodName, static_cast<returnType(ClassType::*)params const>(&ClassType::methodName##templateArgs), Type::GetMultiple##templateArgs##(), AccessLevel::Public, {}, {})	\

#define REFLIB_CONST_METH_TEMP(returnType, methodName, params, templateArgs, accessLevel, paramNames, attributes)										\
.AddTemplateMethod(#methodName, static_cast<returnType(ClassType::*)params const>(&ClassType::methodName##templateArgs), Type::GetMultiple##templateArgs##(), accessLevel, paramNames, attributes)	\

// ------------------ Register Heirarchy ------------------
#define REFLIB_BASE_CLASS(type)	\
.AddBaseType<type>()			\

#define REFLIB_BASE_CLASS_ACCESS(type, accessLevel)	\
.AddBaseType<type>(accessLevel)						\

// ------------------ Register Enums ------------------
#define REFLIB_BEGIN_ENUM(enumName)		\
{										\
	using namespace RefLib;				\
	using EnumType = enumName;			\
	EnumBuilder<EnumType>(#enumName)	\

#define REFLIB_END_ENUM()	\
	.Register();			\
}							\

#define REFLIB_ENUM_VAL(valName)		\
.AddValue(#valName, EnumType::valName)	\

// ------------------ Register Attributes ------------------
#define REFLIB_ATTRIBUTE(attributeVal)	\
.AddAttribute(attributeVal)				\

// ------------------ Register Nested Classes ------------------
#define REFLIB_BEGIN_NESTED_CLASS(name)\
.AddNestedClass<ClassType::name>(#name, [](TypeBuilder<ClassType::name>& builder)	\
{																					\
	using ClassType = ClassType::name;												\
	builder																			\

#define REFLIB_END_NESTED_CLASS() ;})

// ------------------ Register Nested Enums ------------------
#define REFLIB_BEGIN_NESTED_ENUM(name)									\
.AddNestedEnum<ClassType::name>(#name, [](EnumBuilder<ClassType::name>& builder)	\
{																		\
	using EnumType = ClassType::name;									\
	builder																\

#define REFLIB_END_NESTED_ENUM() ;})


#pragma once
#include "AttributeHolder.h"

#define REFLIB_ATTRIBUTE_HOLDER_OBJECT_IMPL(attributeHolder)														\
Ref<const Variant> GetAttribute(Type t) const { return (attributeHolder).GetAttribute(t); }							\
Ref<const Variant> GetAttribute(const std::string& name) const { return (attributeHolder).GetAttribute(name); }		\
bool HasAttribute(Type t) const { return GetAttribute(t).IsNotNull(); }												\
bool HasAttribute(const std::string& name) const { return GetAttribute(name).IsNotNull(); }							\
const std::vector<Variant>& GetAttributes() const { return (attributeHolder).GetAttributes(); }						\

#pragma once
#include "Types/Type.h"
#include <functional>
#include "IContainerView.h"

namespace RefLib
{
	class ContainerView
	{
	public:
		using Iterator = ContainerViewIterator;
		ContainerView(std::shared_ptr<IContainerView> view) : m_View(view) {}
		ContainerView(const ContainerView&) = default;
		~ContainerView() = default;

		Iterator begin();
		Iterator end();

		size_t Length() { return m_View->Length(); }
		Type GetType() { return m_View->GetType(); }
		Type GetElementType() { return m_View->GetElementType(); }
		bool IsEmpty() { return Length() == 0; }

	private:
		std::shared_ptr<IContainerView> m_View;
	};
}

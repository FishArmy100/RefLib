#include "ContainerView.h"
#include "ContainerViewIterator.h"

namespace RefLib
{
	using Iterator = ContainerViewIterator;

	Iterator ContainerView::begin()
	{
		return m_View->begin();
	}

	Iterator ContainerView::end()
	{
		return m_View->end();
	}
}
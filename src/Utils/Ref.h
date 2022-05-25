#pragma once

namespace RefLib
{
	template<typename TBase>
	class Ref
	{
	public:
		Ref(TBase* data) : m_Data(data) {}
		Ref(TBase& dataRef) : m_Data(&dataRef) {}
		Ref() : m_Data(nullptr) {}
		Ref(const Ref<TBase>& other) = default;
		~Ref() = default;

		bool IsNull() const { return m_Data == nullptr; }
		bool IsNotNull() const { return m_Data != nullptr; }

		TBase* Get() { return m_Data; }
		const TBase* Get() const { return m_Data; }

		TBase* operator->()
		{
			if (m_Data)
				return m_Data;

			return (TBase*)nullptr; // Tried to access a null value
		}

		const TBase* operator->() const
		{
			if (m_Data)
				return m_Data;

			return (TBase*)nullptr; // Tried to access a null value
		}

	private:
		TBase* m_Data;
	};
}
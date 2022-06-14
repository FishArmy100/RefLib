#pragma once

namespace RefLib
{
	template<typename T>
	class Ref
	{
	public:
		Ref(T* data) : m_Data(data) {}
		Ref(T& dataRef) : m_Data(&dataRef) {}
		Ref() : m_Data(nullptr) {}
		Ref(const Ref<T>& other) = default;
		~Ref() = default;

		bool IsNull() const { return m_Data == nullptr; }
		bool IsNotNull() const { return m_Data != nullptr; }

		T* Get() { return m_Data; }
		const T* Get() const { return m_Data; }

		T* operator->()
		{
			if (m_Data)
				return m_Data;

			return (T*)nullptr; // Tried to access a null value
		}

		const T* operator->() const
		{
			if (m_Data)
				return m_Data;

			return (T*)nullptr; // Tried to access a null value
		}

	private:
		T* m_Data;
	};
}
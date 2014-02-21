#pragma once

namespace me {
	template<class T>
	class Optional {
		T *m_object;
	public:
		Optional() : m_object(0) {}

		Optional(const T& other) : m_object(new T(other)) {}

		Optional& operator=(const T& other) {
			if (m_object != 0) {
				delete m_object;
			}
			m_object = new T(other);
			return *this;
		}

		operator bool() const {
			return m_object != 0;
		}

		T& operator *() const {
			return *m_object;
		}

		T* operator ->() const {
			return m_object;
		}

		T* pointer() const {
			return m_object;
		}
	};

	// splitting.
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
}
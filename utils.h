#pragma once

namespace me {
	// basic optional implementation.
	template<class T>
	class Optional {
		T *m_object;

	public:
		Optional() : m_object(0) { }

		// copy constructor
		Optional(const Optional<T>& other) : m_object(new T(*other)) { }

		// move constructor
		Optional(Optional<T>&& other) : m_object(other.m_object) {
			other.m_object = 0;
		}

		Optional(const T& other) : m_object(new T(other)) { }

		// destructor
		~Optional() {
			delete m_object;  // delete 0 is a no-op, so we are fine here.
		}

		// copy assignment operator
		Optional& operator=(const Optional<T>& other) {
			delete m_object;  // delete 0 is a no-op
			m_object = new T(*other);
			return *this;
		}

		Optional& operator=(const T& other) {
			delete m_object;  // delete 0 is a no-op
			m_object = new T(other);
			return *this;
		}

		// move assignment operator
		Optional& operator=(Optional<T>&& other) {
			delete m_object;  // delete 0 is a no-op
			m_object = other.m_object;
			other.m_object = 0;
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
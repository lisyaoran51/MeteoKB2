#ifndef SINGLETON_H
#define SINGLETON_H


/*
 * https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
 *
 */


namespace Util {

	template<typename T>
	class Singleton
	{
	protected:
		Singleton() noexcept = default;

		Singleton(const Singleton&) = delete;

		Singleton& operator=(const Singleton&) = delete;

		virtual ~Singleton() = default; // to silence Base class Singleton<T> has a
										// non-virtual destructor [-Weffc++]

	public:
		static T& GetInstance() noexcept(std::is_nothrow_constructible<T>::value)
		{
			// Guaranteed to be destroyed.
			// Instantiated on first use.
			// Thread safe in C++11
			static T instance;

			return instance;
		}
	};

	/* --DEPRICATED
	class Singleton {
		Singleton() {}

		Singleton(const Singleton&);
		void operator=(const Singleton&);

	public:
		static Singleton get_instance() {
			static Singleton instance; // Guaranteed to be destroyed.
										 // Instantiated on first use.
			return instance;
		}

		Singleton(const Singleton&) = delete;
		void operator=(const Singleton&) = delete;
	};
	*/




}


#endif
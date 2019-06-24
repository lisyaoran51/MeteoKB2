#if !defined(__MAIN_H__INCLUDED__)
#define __MAIN_H__INCLUDED__

#include <cstdlib>
#include <iostream>
#include <cassert>

using namespace std;

// #define BOOST_NO_SFINAE
// #define BOOST_NO_VOID_RETURNS
// #define BOOST_NO_STD_ALLOCATOR

#include <fd/delegate.hpp>
#include <fd/delegate/bind.hpp>
// #include <fd/resolution.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
// #include <boost/signal.hpp>

struct foobar
{
  int id_;

  void foo(int) { }
  static void bar(int) { }

  void operator ()(int) const { }
  void operator ()(long) const { }

  friend bool operator ==(foobar const & rhs, foobar const & lhs)
  {
    return lhs.id_ == rhs.id_;
  }
};

void hello(int);

struct stateless
{
  void operator ()(int) const { }
};

inline bool operator ==(stateless const &, stateless const &)
{
  return true;
}

template<typename T> inline
void print_type(T t)
{
  std::cout<< typeid( T ).name() << std::endl;
}

#endif // #if !defined(__MAIN_H__INCLUDED__)

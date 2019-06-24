#if !defined(__TEST3_H__INCLUDED__)
#define __TEST3_H__INCLUDED__

struct int_div
{
  float operator()(int x, int y) const { return ((float)x)/y; };
};

struct print_sum
{
  void operator()(int x, int y) const { std::cout << x+y << std::endl; }
};

struct print_product
{
  void operator()(int x, int y) const { std::cout << x*y << std::endl; }
};

struct print_difference
{
  void operator()(int x, int y) const { std::cout << x-y << std::endl; }
};

struct print_quotient
{
  void operator()(int x, int y) const { std::cout << x/-y << std::endl; }
};

struct make_int
{
  make_int(int n, int cn) : N(n), CN(cn) {}

  int operator()() { return N; }
  int operator()() const { return CN; }

  int N;
  int CN;
};

inline bool operator ==(make_int const & lhs, make_int const & rhs)
{
  return lhs.N == rhs.N;
}

FD_DELEGATE_EQUALITY_COMPARABLE_TYPE(make_int); // (A)

template<typename T>
struct last_value
{
  typedef T result_type;

  template<typename InputIterator>
  T operator()(InputIterator first, InputIterator last) const
  {
    // If there are no slots to call, just return the
    // default-constructed value
    if (first == last)
      return T();

    return *--last;
  }

};  // template<typename T> struct last_value

template<typename T>
struct maximum
{
  typedef T result_type;

  template<typename InputIterator>
  T operator()(InputIterator first, InputIterator last) const
  {
    // If there are no slots to call, just return the
    // default-constructed value
    if (first == last)
      return T();

    T max_value = *first++;
    while (first != last) {
      if (max_value < *first)
        max_value = *first;
      ++first;
    }

    return max_value;
  }

};  // template<typename T> struct maximum

template<typename T>
struct first_positive {
  typedef T result_type;

  template<typename InputIterator>
  T operator()(InputIterator first, InputIterator last) const
  {
    while (first != last && !(*first > 0))
    {
      ++first;
    }
    return (first == last) ? 0 : *first;
  }
};

template<typename T>
struct noisy_divide {
  typedef T result_type;

  T operator()(T const & x, T const & y) const
  {
    std::cout << "Dividing " << x << " and " << y << std::endl;
    return x/y;
  }
};

template<typename Container>
struct aggregate_values
{
  typedef Container result_type;

  template<typename InputIterator>
  Container operator()(InputIterator first, InputIterator last) const
  {
    return Container(first, last);
  }

};  // template<typename Container> struct aggregate_values

template<typename T>
struct my_ptr
{
  T * pt_;
  my_ptr(T * pt) : pt_( pt ) { }

  struct proxy2
  {
    T * pt_;
    proxy2(T * pt) : pt_( pt ) { }
    T * operator ->() const { return pt_; }
  };

  struct proxy
  {
    T * pt_;
    proxy(T * pt) : pt_( pt ) { }
    proxy2 operator ->() const { return proxy2( pt_ ); }
    T & operator *() const { return *pt_; }
  };

  proxy operator ->() const
  {
    return proxy(pt_);
  }
};

template<typename T>
typename my_ptr<T>::proxy get_pointer(my_ptr<T> const & t)
{
  return t.operator ->();
}

struct my_obj
{
  int hello(int) { return 0; }

  int operator ()(int) const { return 0; }
};

struct foba
{
  int  foo(int)    { std::cout << "(A)" << std::endl; return 0; } // (A)
  long foo(long)   { std::cout << "(B)" << std::endl; return 0; } // (B)

  int  bar(int)    { std::cout << "(C)" << std::endl; return 0; } // (C)
  int  bar(long)   { std::cout << "(D)" << std::endl; return 0; } // (D)
};

void test3();

#endif  // #if !defined(__TEST3_H__INCLUDED__)

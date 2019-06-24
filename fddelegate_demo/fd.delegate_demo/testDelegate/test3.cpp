#include "main.h"
#include "test3.h"

#include <boost/bind.hpp>
#include <fd/resolution.hpp>
#include <vector>

using fd::resolution;

void do_sum_avg(int values[], int n, int& sum, float& avg)
{
  sum = 0;
  for (int i = 0; i < n; i++)
    sum += values[i];
  avg = (float)sum / n;
}

void test3()
{
  fd::delegate<float (int, int)> f;

  f = int_div();

  std::cout << f(5, 3) << std::endl; // 1.66667

  // --------------------------------------------------------------------------------

  // The second parameter should be int[], but some compilers (e.g., GCC)
  // complain about this
  fd::delegate<void (int*, int, int&, float&)> sum_avg;

  sum_avg = &do_sum_avg;

  int values[5] = { 1, 1, 2, 3, 5 };
  int sum;
  float avg;
  sum_avg(values, 5, sum, avg);

  std::cout << "sum = " << sum << std::endl;
  std::cout << "avg = " << avg << std::endl;

  // --------------------------------------------------------------------------------

  fd::delegate2<void, int, int> dg1;

  dg1 += print_sum();
  dg1 += print_product();

  dg1(3, 5); // prints 8 and 15

  // --------------------------------------------------------------------------------

  fd::delegate2<void, int, int> dg2;

  dg2 += print_sum();
  dg2 += print_product();

  dg2(3, 5);

  fd::multicast::token print_diff_tok = dg2.add(print_difference());

  // print_diff_tok is still connected to dg2
  assert(print_diff_tok.valid());

  dg2(5, 3); // prints 8, 15, and 2

  print_diff_tok.remove(); // remove the print_difference delegate

  dg2(5, 3);  // now prints 8 and 15, but not the difference

  assert(!print_diff_tok.valid()); // not connected anymore

  {
    fd::multicast::scoped_token t = dg2.add(print_quotient());
    dg2(5, 3); // prints 8, 15, and 1

  } // t falls out of scope, so print_quotient is not a member of dg

  dg2(5, 3); // prints 8 and 15

  // --------------------------------------------------------------------------------

  fd::delegate2<int, int, int> dg_max;
  dg_max += std::plus<int>();
  dg_max += std::multiplies<int>();
  dg_max += std::minus<int>();
  dg_max += std::divides<int>();

  std::cout << dg_max(5, 3, maximum<int>()) << std::endl; // prints 15

  // --------------------------------------------------------------------------------

  fd::delegate0<void> dg3, dg4;

  dg3 = make_int( 1, 10 );
  dg4 = make_int( 1, 20 );

  assert( dg3.equal_to( dg4 ) == true ); // (B)

  // --------------------------------------------------------------------------------

  fd::delegate1<int, int> dg5;

  dg5 += std::negate<int>();

  fd::multicast::token tk1 = dg5.add( std::bind1st( std::plus<int>(), 3 ) ); // (A)
  assert( tk1.valid() == true );

  assert( dg5.count() == 2 );

  dg5.remove( std::bind1st( std::plus<int>(), 3 ) ); // Can not remove the delegate added in the
                                                     // line (A) std::bind1st( std::plus<int>(), 3 )
  assert( dg5.count() == 2 );

  tk1.remove(); // Removes the delegate added in the line (A) std::bind1st( std::plus<int>(), 3 )

  assert( dg5.count() == 1 );

  // --------------------------------------------------------------------------------

  fd::delegate2<int, int, int> dg6;

  dg6 += std::plus<int>();
  dg6 += std::multiplies<int>();
  dg6 += std::minus<int>();
  dg6 += std::divides<int>();

  int max = dg6( 5, 3, maximum<int>() );
  assert( max == 15 );

  // --------------------------------------------------------------------------------

  fd::delegate2<int, int, int> dg_positive;

  dg_positive += std::plus<int>();
  dg_positive += std::multiplies<int>();
  dg_positive += std::minus<int>();
  dg_positive += noisy_divide<int>();

  assert( dg_positive( 3, -5, first_positive<int>() ) == 8 ); // returns 8, but prints nothing.

  // --------------------------------------------------------------------------------

  std::vector<int> vec_result = dg_positive( 3, -5, aggregate_values<std::vector<int> >() );
  assert( vec_result.size() == 4 );
  assert( vec_result[0] == 3+(-5) );
  assert( vec_result[1] == 3*(-5) );
  assert( vec_result[2] == 3-(-5) );
  assert( vec_result[3] == 3/(-5) ); // prints "Dividing 3 and -5".

  // --------------------------------------------------------------------------------

  my_obj mo;
  my_ptr<my_obj> mptr( &mo );

  //
  // Boosts do not work with proxy return by get_pointer() overload for my_ptr
  // since the returned proxy does not make ->* nor .* valid member function call
  // expression. Refer to http://boost.org/libs/bind/mem_fn.html
  //
  // And I don't think it is possible to define 'one' generic ->* or .* operator
  // overloading for the proxy that is returned by get_pointer().
  //

  // boost::mem_fn( &my_obj::hello )( mptr, 1 );
  // boost::bind( &my_obj::hello, mptr, _1 )( 1 );

  // ( get_pointer( mptr )->*&my_obj::hello )( 1 );
  ( *get_pointer( mptr ).*&my_obj::hello )( 1 );

  boost::function1<int, int> fn7;
  // fn7 = boost::bind( &my_obj::hello, mptr, _1 )( 1 );

  //
  // FD will work as long as the returned proxy is valid to access members through
  // both -> or . operators.
  //

  ( &*get_pointer( mptr ) )->hello( 1 );
  ( *get_pointer( mptr ) )( 1 );
  ( &*get_pointer( mptr ) )->operator ()( 1 );

  fd::delegate1<int, int> dg7;

  dg7.bind( &my_obj::hello, mptr );
  dg7( 1 );           // ( &*get_pointer( mptr ) )->hello( 1 );

  dg7 = fd::bind( &my_obj::hello, mptr );
  dg7( 1 );           // ( &*get_pointer( mptr ) )->hello( 1 );

  dg7 = mptr;
  dg7( 1 );           // ( &*get_pointer( mptr ) )->operator ()( 1 );

  // --------------------------------------------------------------------------------

  foba fb;
  foba * pfb = &fb;

  // The following innocent expression is not valid for Boost.Bind since
  // it can not forward non-const rvalue. No problem with Boost.Mem_fn and FDs.
  // Refer to http://std.dkuug.dk/jtc1/sc22/wg21/docs/papers/2002/n1385.htm#s1

  // boost::bind( resolution<int (int)>::select( &foba::foo ), pfb, _1 )( 1 );

  int n = 1;

  boost::bind( resolution<int (int)>::select( &foba::foo ), pfb, _1 )( n );   // (A)
  boost::bind( resolution<long (long)>::select( &foba::foo ), pfb, _1 )( n ); // (B)

  boost::bind( resolution<int (int)>::select( &foba::bar ), pfb, _1 )( n );  // (C)
  boost::bind( resolution<int (long)>::select( &foba::bar ), pfb, _1 )( n ); // (D)

  boost::mem_fn( resolution<int (int)>::select( &foba::foo ) )( pfb, 1 );   // (A)
  boost::mem_fn( resolution<long (long)>::select( &foba::foo ) )( pfb, 1 ); // (B)

  boost::mem_fn( resolution<int (int)>::select( &foba::bar ) )( pfb, 1 );  // (C)
  boost::mem_fn( resolution<int (long)>::select( &foba::bar ) )( pfb, 1 ); // (D)

  boost::function<int (int)> fn;
  fn = boost::bind( resolution<int (int)>::select( &foba::bar ), pfb, _1 );  // (C)
  fn( 1 );
  fn = boost::bind( resolution<int (long)>::select( &foba::bar ), pfb, _1 ); // (D)
  fn( 1 );

  fd::bind( resolution<int (int)>::select( &foba::foo ), pfb );   // (A)
  fd::bind( resolution<long (long)>::select( &foba::foo ), pfb ); // (B)

  fd::bind( resolution<int (int)>::select( &foba::bar ), pfb )( 1 );  // (C)
  fd::bind( resolution<int (long)>::select( &foba::bar ), pfb )( 1 ); // (D)

  fd::delegate<int (int)> dg8;
  dg8 = fd::bind( resolution<int (int)>::select( &foba::bar ), pfb );  // (C)
  dg8( 1 );
  dg8 = fd::bind( resolution<int (long)>::select( &foba::bar ), pfb ); // (D)
  dg8( 1 );

  dg8.bind( resolution<int (int)>::select( &foba::bar ), pfb );  // (C)
  dg8( 1 );
  dg8.bind( resolution<int (long)>::select( &foba::bar ), pfb ); // (D)
  dg8( 1 );
}

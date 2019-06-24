#include "main.h"
#include "test1.h"

void test1()
{
  fd::delegate1<void, int> dg1;
  boost::function1<void, int> fn1;

  foobar fb;
  foobar * pfb = &fb;
  boost::shared_ptr<foobar> spfb( new foobar );
  spfb->id_ = fb.id_;

  dg1 = &hello;                               // FT01
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );                                   // hello( 1 );
  assert( dg1.contains( &hello ) );

  fn1 = &hello;
  fn1( 1 );
  assert( fn1.contains( &hello ) );

  dg1 = &foobar::bar;                         // FT01
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );                                   // foobar::bar( 1 );
  assert( dg1.contains( &foobar::bar ) );

  fn1 = &foobar::bar;
  fn1( 1 );
  assert( fn1.contains( &foobar::bar ) );

  fd::delegate2<void, foobar *, int> dg2;
  dg2 = &foobar::foo;                         // FT02
  std::cout << dg2.type().name() << std::endl;
  dg2( pfb, 1 );                              // (pfb->*&foobar::foo)( 1 );
#ifdef _MSC_VER
  assert( dg2.contains( &foobar::foo ) );
#endif

  boost::function2<void, foobar *, int> fn2;
  fn2 = &foobar::foo;
  fn2( pfb, 1 );
  // assert( fn2.contains( &foobar::foo ) );

  fd::delegate2<void, foobar &, int> dg3;
  dg3 = &foobar::foo;                         // FT03
  std::cout << dg3.type().name() << std::endl;
  dg3( fb, 1 );                               // (fb.*&foobar::foo)( 1 );
#ifdef _MSC_VER
  assert( dg3.contains( &foobar::foo ) );
#endif

  boost::function2<void, foobar &, int> fn3;
  fn3 = &foobar::foo;
  fn3( fb, 1 );
  // assert( fn3.contains( &foobar::foo ) );

  fd::delegate2<void, foobar, int> dg4;
  dg4 = &foobar::foo;                         // FT04
  std::cout << dg4.type().name() << std::endl;
  dg4( fb, 1 );                               // ((copy of fb).*&foobar::foo)( 1 );
#ifdef _MSC_VER
  assert( dg4.contains( &foobar::foo ) );
#endif

  boost::function2<void, foobar, int> fn4;
  fn4 = &foobar::foo;
  fn4( fb, 1 );
  // assert( fn4.contains( &foobar::foo ) );

  fd::delegate2<void, boost::shared_ptr<foobar>, int> dg5;
  dg5 = &foobar::foo;                         // FT04
  std::cout << dg5.type().name() << std::endl;
  dg5( spfb, 1 );                             // (get_pointer(spfb)->*&foobar::foo)( 1 );
#ifdef _MSC_VER
  assert( dg5.contains( &foobar::foo ) );
#endif

  boost::function2<void, boost::shared_ptr<foobar>, int> fn5;
  fn5 = &foobar::foo;
  fn5( spfb, 1 );
  // assert( fn5.contains( &foobar::foo ) );

  dg1.bind( &foobar::foo, pfb );              // FT05
  dg1 = boost::bind( &foobar::foo, pfb, _1 );
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );
  dg1 = fd::bind( &foobar::foo, pfb );
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );                                   // (pfb->*&foobar::foo)( 1 );
#ifdef _MSC_VER
  assert( dg1.contains( &foobar::foo ) );
#endif

  fn1 = fd::bind( &foobar::foo, pfb );
  fn1( 1 );
  fn1 = boost::bind( &foobar::foo, pfb, _1 );
  fn1( 1 );
  // assert( fn1.contains( &foobar::foo ) );
  boost::_bi::bind_t<void,boost::_mfi::mf1<void,foobar,int>,boost::_bi::list2<boost::_bi::value<foobar *>,boost::arg<1> > > const & rbi =
   boost::bind( &foobar::foo, pfb, _1 );
  assert(( fn1.contains( rbi ) ));

  dg1.bind( &foobar::foo, boost::ref( fb ) ); // FT05
  dg1 = boost::bind( &foobar::foo, boost::ref( fb ), _1 );
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );
  dg1 = fd::bind( &foobar::foo, boost::ref( fb ) );
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );                                   // (fb.*&foobar::foo)( 1 );
#ifdef _MSC_VER
  assert( dg1.contains( &foobar::foo ) );
#endif

  fn1 = fd::bind( &foobar::foo, boost::ref( fb ) );
  fn1( 1 );
  fn1 = boost::bind( &foobar::foo, boost::ref( fb ), _1 );
  fn1( 1 );
  // assert( fn1.contains( &foobar::foo ) );

  dg1.bind( &foobar::foo, fb );               // FT06
  dg1 = boost::bind( &foobar::foo, fb, _1 );
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );
  dg1 = fd::bind( &foobar::foo, fb );
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );                                   // ((copy of fb).*&foobar::foo)( 1 );
#ifdef _MSC_VER
  assert( dg1.contains( &foobar::foo ) );
#endif

  fn1 = fd::bind( &foobar::foo, fb );
  fn1( 1 );
  fn1 = boost::bind( &foobar::foo, fb, _1 );
  fn1( 1 );
  // assert( fn1.contains( &foobar::foo ) );

  dg1.bind( &foobar::foo, spfb );             // FT06
  dg1 = boost::bind( &foobar::foo, spfb, _1 );
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );
  dg1 = fd::bind( &foobar::foo, spfb );
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );                                   // (get_pointer(spfb)->*&foobar::foo)( 1 );
#ifdef _MSC_VER
  assert( dg1.contains( &foobar::foo ) );
#endif

  fn1 = fd::bind( &foobar::foo, spfb );
  fn1( 1 );
  fn1 = boost::bind( &foobar::foo, spfb, _1 );
  fn1( 1 );
  // assert( fn1.contains( &foobar::foo ) );

  dg1 = pfb;                                  // FT07
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );                                   // (*pfb)( 1 );
  assert( dg1.contains( fb ) );

  // fn1 = pfb;                               // Boost.Function does not allow to assign the pointer to function object.
  // fn1( 1 );

  dg1 = boost::ref( fb );                     // FT07
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );                                   // fb( 1 );
  assert( dg1.contains( fb ) );

  fn1 = boost::ref( fb );
  fn1( 1 );
  assert( fn1.contains( fb ) );

  dg1 = fb;                                   // FT08
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );                                   // (copy of fb)( 1 );
  assert( dg1.contains( fb ) );

  fn1 = fb;
  fn1( 1 );
  assert( fn1.contains( fb ) );

  dg1 = spfb;                                 // FT08
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );                                   // (*get_pointer(spfb))( 1 );
  assert( dg1.contains( fb ) );

  // fn1 = spfb;                              // Boost.Function does not allow to assign smart pointer class,
  // fn1( 1 );                                // even if get_pointer() overload is available.

  dg1 = stateless();                          // FT09
  std::cout << dg1.type().name() << std::endl;
  dg1( 1 );                                   // stateless()( 1 );
  assert( dg1.contains( stateless() ) );

  fn1 = stateless();
  fn1( 1 );
  assert( fn1.contains( stateless() ) );

  dg1 = 0;                                    // FT10
  std::cout << dg1.type().name() << std::endl;
  try { dg1( 1 ); }                           // throw bad_function_call();
  catch(fd::bad_function_call) { }

  fn1 = 0;
  try { fn1( 1 ); }
  catch(boost::bad_function_call) { }

  dg1 += &hello;                              // FT11
  dg1 += fd::delegate<void (int)>( &foobar::foo, spfb );
  dg1 += fb;
  std::cout << dg1.type().name() << std::endl;

  dg1( 1 );                                   // hello( 1 );
                                              // (get_pointer(spfb)->*&foobar::foo)( 1 );
                                              // (copy of fb)( 1 );
}

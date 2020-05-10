
#ifndef OPENOBEX_EXPORT_H
#define OPENOBEX_EXPORT_H

#ifdef OPENOBEX_STATIC_DEFINE
#  define OPENOBEX_EXPORT
#  define OPENOBEX_NO_EXPORT
#else
#  ifndef OPENOBEX_EXPORT
#    ifdef openobex_EXPORTS
        /* We are building this library */
#      define OPENOBEX_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define OPENOBEX_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef OPENOBEX_NO_EXPORT
#    define OPENOBEX_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef OPENOBEX_DEPRECATED
#  define OPENOBEX_DEPRECATED __attribute__ ((__deprecated__))
#  define OPENOBEX_DEPRECATED_EXPORT OPENOBEX_EXPORT __attribute__ ((__deprecated__))
#  define OPENOBEX_DEPRECATED_NO_EXPORT OPENOBEX_NO_EXPORT __attribute__ ((__deprecated__))
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define OPENOBEX_NO_DEPRECATED
#endif

#endif

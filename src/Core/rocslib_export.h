#ifndef ROCSLIB_EXPORT_H
#define ROCSLIB_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef ROCSLIB_EXPORT
# if defined(MAKE_ROCSLIB_LIB)
   // We are building this library
#  define ROCSLIB_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define ROCSLIB_EXPORT KDE_IMPORT
# endif
#endif
 
# ifndef ROCSLIB_EXPORT_DEPRECATED
#  define ROCSLIB_EXPORT_DEPRECATED KDE_DEPRECATED ROCSLIB_EXPORT
# endif
 
#endif
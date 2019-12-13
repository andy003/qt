#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(KEIL_LIB)
#  define KEIL_EXPORT Q_DECL_EXPORT
# else
#  define KEIL_EXPORT Q_DECL_IMPORT
# endif
#else
# define KEIL_EXPORT
#endif

#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(LIVE555_LIB)
#  define LIVE555_EXPORT Q_DECL_EXPORT
# else
#  define LIVE555_EXPORT Q_DECL_IMPORT
# endif
#else
# define LIVE555_EXPORT
#endif

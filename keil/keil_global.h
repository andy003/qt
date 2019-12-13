/**
 ***********************************************************************************************************************
 * @file   KEIL_GLOBAL.H
 * @author ZhangRan
 * @date   2019/12/13
 *
 * <h2><center>&copy; COPYRIGHT 2020 SCHRODER CORPORATION</center></h2>
 *
 ***********************************************************************************************************************
 */

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

#ifndef MGRS_GLOBAL_H
#define MGRS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MGRS_LIBRARY)
#  define MGRS_EXPORT Q_DECL_EXPORT
#else
#  define MGRS_EXPORT Q_DECL_IMPORT
#endif

#endif // MGRS_GLOBAL_H

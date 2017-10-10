#pragma once

#ifdef gstd_SHARED
#ifdef gstd_EXPORTS
#define GRIS_GSTD_API __declspec (dllexport)
#pragma warning( disable : 4251 ) // dll-interface warnings
#else
#define GRIS_GSTD_API __declspec (dllimport)
#endif
#else // gstd_SHARED (no shared lib)
#define GRIS_GSTD_API 
#endif // gstd_SHARED

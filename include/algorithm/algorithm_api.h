#pragma once

#ifdef algorithm_SHARED
#ifdef algorithm_EXPORTS
#define GRIS_ALGORITHM_API __declspec (dllexport)
#pragma warning( disable : 4251 ) // dll-interface warnings
#else
#define GRIS_ALGORITHM_API __declspec (dllimport)
#endif
#else // algorithm_SHARED (no shared lib)
#define GRIS_ALGORITHM_API 
#endif // algorithm_SHARED

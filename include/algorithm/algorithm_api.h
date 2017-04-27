#ifndef GRIS_GSTD_API_H
#define GRIS_GSTD_API_H

#ifdef algorithm_EXPORTS
#define GRIS_ALGORITHM_API __declspec (dllexport)
#pragma warning( disable : 4251 ) // dll-interface warnings
#else
#define GRIS_ALGORITHM_API __declspec (dllimport)
#endif

#endif
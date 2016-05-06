#ifndef GRIS_GSTD_API_H
#define GRIS_GSTD_API_H

#ifdef GRIS_GSTD_CREATE_DLL
#define GRIS_GSTD_API __declspec (dllexport)
#else
#define GRIS_GSTD_API __declspec (dllimport)
#endif

#endif
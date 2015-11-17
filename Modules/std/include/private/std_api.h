#ifndef GRIS_FRAMEWORK_STD_API_H
#define GRIS_FRAMEWORK_STD_API_H

#define GRIS_STD_EXPORT

#ifdef GRIS_STD_EXPORT
	#define GRIS_GSTD_API __declspec(dllexport)
#else
	#define GRIS_GSTD_API __declspec(dllimport)
#endif

#endif
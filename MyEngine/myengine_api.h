#ifndef MYENGINE_API_H
#define MYENGINE_API_H

#pragma warning( disable : 4251)
#pragma warning( disable : 4018)
#pragma warning( disable : 4482)
#pragma warning( disable : 4098)
#pragma warning( disable : 4244)
#pragma warning( disable : 4172)
#pragma warning( disable : 4227)

#ifdef MYENGINE_EXPORTS
	#define MYENGINE_API __declspec(dllexport)
#else
	#define MYENGINE_API __declspec(dllimport)
#endif

#endif
#pragma once
#include "claa_base.h"
#define APPKEY_LEN 16

#ifdef DLLTEST_EXPORTS //在DllTest工程的预处理中定义

#define LIB_API  extern "C" __declspec(dllexport)

#else

#define LIB_API  extern "C" __declspec(dllimport)

#endif

LIB_API void challenge_identification(uint8 appkey[APPKEY_LEN], uint64 appeui, uint32 appnonce,char*channel);
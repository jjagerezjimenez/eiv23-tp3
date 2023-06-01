#ifndef TRACE_H
#define TRACE_H

#ifdef TRACE_ON
#include <BluePillHal.h>

#define TRACE_INIT() ({\
    BP_Pin_mode(A0, OUT_2MHz);\
    BP_Pin_mode(A1, OUT_2MHz);\
    BP_Pin_mode(A2, OUT_2MHz);\
    BP_Pin_mode(A3, OUT_2MHz);\
    BP_Pin_mode(A4, OUT_2MHz);\
    BP_Pin_mode(A5, OUT_2MHz);\
    BP_Pin_set(A0, 0);\
    BP_Pin_set(A1, 0);\
    BP_Pin_set(A2, 0);\
    BP_Pin_set(A3, 0);\
    BP_Pin_set(A4, 0);\
    BP_Pin_set(A5, 0);})

/// nr_ Número: 0, 1, 2, 3, 4 o 5
#define TRACE_ENTER(nr_) BP_Pin_set(A##nr_, 1)
/// nr_ Número: 0, 1, 2, 3, 4 o 5
#define TRACE_EXIT(nr_) BP_Pin_set(A##nr_, 0)
#else
#define TRACE_INIT()
/// nr_ Número: 0, 1, 2, 3, 4 o 5
#define TRACE_ENTER(nr_)
/// nr_ Número: 0, 1, 2, 3, 4 o 5
#define TRACE_EXIT(nr_)
#endif

#endif

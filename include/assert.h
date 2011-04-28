#ifndef ASSERT_H
#define ASSERT_H

#include <kernel/kernel_panic.h>

#if 0
#define assert(EXPR) do { (void)(EXPR); } while(0)
#else
#define assert(EXPR) do { if(!(EXPR)) kernel_panic("assert(" #EXPR ") in file " __FILE__ " "); } while(0)
#endif

#endif

#ifndef LIB_HOOK_H
#define LIB_HOOK_H

#include <psputilsforkernel.h>
#include <psputility.h>
#include <pspchnnlsv.h>

#ifdef __cplusplus
extern "C" {
#endif


u32 Find2Func(const char* mod, const char* lib, u32 nid);
void* Find2Syscall(void *addr);
void Hook2Syscall(void *addr, void *func);

#ifdef __cplusplus
}
#endif

#endif

#include "common.h"
#include "hook.h"


struct hook_syscall_table_header
{
	struct hook_syscall_table_header *next;
	unsigned int offset;
	unsigned int num;
	unsigned int size;
};



u32 Find2Func(const char* mod, const char* lib, u32 nid)
{
	struct SceLibraryEntryTable *entry;
	SceModule *module;
	void *entTab;
	int entLen,i = 0;
	
	module = sceKernelFindModuleByName(mod);
	
	if(!module) return 0;
	
	entTab = module->ent_top;
	entLen = module->ent_size;
	
	while(i < entLen)
	{
		int count;
		int total;
		unsigned int *vars;
		
		entry = (struct SceLibraryEntryTable *) (entTab + i);
		
		if(entry->libname == lib || (entry->libname && lib && 0 == strcmp(entry->libname, lib)))
		{
			total = entry->stubcount + entry->vstubcount;
			vars = entry->entrytable;
			
			if(total > 0)
			{
				for(count=0; count<total; count++)
				{
					if (vars[count] == nid)
					{
						logs("Find2Func %s,%s,0x%08X = %08X\n",mod,lib,nid,vars[count+total]);
						return vars[count+total];
					}
				}
			}
		}
		
		i += (entry->len * 4);
	}
	
	return 0;
}


void Hook2Syscall(void *addr, void *func)
{
	if( !addr || !func ) return;
	
	logs("Hook2Syscall [%08X] %08X -> %08X\n",addr,*(u32*)addr,func );
	
	unsigned int intc = pspSdkDisableInterrupts();
	
	*(u32*)addr = (u32)func;
	
	sceKernelDcacheWritebackInvalidateRange( addr, sizeof( void* ) );
	sceKernelIcacheInvalidateRange( addr, sizeof( void* ) );
	
	pspSdkEnableInterrupts( intc );
}




void* Find2Syscall(void *addr)
{
	u32 *syscalls;
	int i, size;
	struct hook_syscall_table_header *head;
	
	if( !addr ) return NULL;
	
	asm("cfc0 %0, $12\n" : "=r"(syscalls));
	
	if ( !syscalls ) return NULL;
	
	head 		= (struct hook_syscall_table_header*)syscalls;
	syscalls	= (u32*)(syscalls + sizeof(struct hook_syscall_table_header*));
	size		= (head->size / sizeof(u32) ) - sizeof(struct hook_syscall_table_header*);
	
	for(i=0; i<size; ++i)
	{
		//FW6.60から!?関数の実アドレスが0x8XXXXXXXとなっていてsyscallにある物とそのままでは一致しなくなった
		//0x0FFFFFFFのマスクをかけて比較する必要あり
		if ((syscalls[i] & 0x0FFFFFFF) == ((u32)addr & 0x0FFFFFFF))
		{
			logs("Find2Syscall %08X [%08X->%08X]\n",&syscalls[i],syscalls[i],addr);
			return &syscalls[i];
		}
	}
	
	return NULL;
}

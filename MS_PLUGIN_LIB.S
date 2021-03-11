	.set noreorder

#include "pspstub.s"

	STUB_START "MS_PLUGIN_LIB",0x40090000,0x00020005
	STUB_FUNC  0xBB3A300C,MS_GetFuncInfo_Load
	STUB_FUNC  0x8629F77F,MS_GetFuncInfo_Save
	STUB_END

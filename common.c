#include "common.h"

void CreateDir(char* path)
{
	char *p = path;
	
	while( (p = strchr(p,'/')) )
	{
		*p = '\0';
		
		//存在しない、フォルダ作成
		sceIoMkdir(path, 0777);
		
		*p = '/';
		
	    p++;
 	}
}





bool MemoryStick_GetSize( sDev_Disk_Info *info )
{ 
 	if( !info ) return false;
	
	return (sceIoDevctl("ms0:",0x02425818,&info,4,NULL,0) >= 0);
}



int WriteFile(const char *path ,void *data ,SceSize size)
{
	sDev_Disk_Info info;
	
	if( MemoryStick_GetSize( &info ) )
	{
		u64 free = (info.clusters_free * info.sector_count) * info.sector_size;
		
		if( free >= size )
		{
			SceUID fd = sceIoOpen(path, PSP_O_CREAT | PSP_O_TRUNC | PSP_O_WRONLY, 0777);
			
			if( fd >= 0 )
			{
				SceSize size_ms = sceIoWrite(fd, data, size);
				sceIoClose(fd);
				
				if( size_ms == size )
				{
					//書き込みに成功
					logs("WriteFile : %s\n",path);
				}
				else
				{
					//書き込みに失敗した
					if(sceIoRemove(path) >= 0) logs("Delete File : %s\n",path);
				}
				//書き込みに失敗（サイズが違う)
			}
			//セーブするためのファイルが開けない
		}
		//空き容量が足りない
	}
	//MS情報取得失敗
	
	return 1;
}

bool ReadFile(const char *path ,void *data ,SceSize size)
{
	SceUID fd = sceIoOpen(path, PSP_O_RDONLY, 0777);
	bool ret = false;
	
	if( fd >= 0 )
	{
		if( sceIoRead(fd, data, size ) == size )
		{
			ret = true;
			
			//書き込みに成功
			logs("ReadFile : %s\n",path);
		}
		
		sceIoClose(fd);
	}
	
	return ret;
}


//MacroFireのsrcより
void strutilRemoveChar( char *__restrict__ str, const char *__restrict__ search )
{
	int offset = 0,ins_offset = 0;
	
	while( str[offset] != '\0' )
	{
		if( ! strchr( search, str[offset] )  )
		{
			str[ins_offset++] = str[offset];
		}
		
		offset++;
	}
	
	str[ins_offset] = str[offset];
}

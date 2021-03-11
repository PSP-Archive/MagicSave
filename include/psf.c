
#include <pspsdk.h>

#include <string.h>

#include "psf.h"





int find_psf_key(u8 *buf, int bufLen ,const char *key ,SFO_DATA *data_p)
{
	int curPos;
	int ret = 0;
	
	SFO_HEADER *header = (SFO_HEADER*)buf;
	SFO_TABLE *table;
	SFO_DATA data = {0};
	
	if( !data_p || !key ) return -9;
	
	//バッファーサイズがヘッダーサイズ以下
	if( !buf || bufLen < sizeof(SFO_HEADER) ) return -1;
	
    //PSFファイルではない
    if( header->signature != 0x46535000 ) return -2;
	
    // Verify strLoc is proper */
	if( (header->offset_key > header->offset_val) || (header->offset_key >= bufLen ) ) return -3;
	
    //データ部がバッファーサイズを超えている
	if( header->offset_val >= bufLen ) return -4;
	
    //アイテム数があわない
	if( header->number_ent != ((header->offset_key - 0x14) / 0x10) ) return -5;
	
	for( curPos = OFFSET_TABLE_INFO ; curPos < header->offset_key ; curPos+= sizeof(SFO_TABLE) )
	{
		table = (SFO_TABLE*)&buf[curPos];
		
		if( (table->offset_key < bufLen) && (table->size_01 < bufLen ) && (table->size_02 < bufLen) && (table->offset_val < bufLen) )
		{
			data.key		= (char *)&buf[header->offset_key + table->offset_key];
			data.type		= table->type;
			data.pos		= &buf[header->offset_val + table->offset_val];
			data.size_01	= table->size_01;
			data.size_02	= table->size_02;
			
			if( !strcmp(data.key, key) )
			{
				*data_p = data;
				
				break;
			}
		}
		else
		{
			ret = -8;
			break;
		}
    }
	
    return ret;
}



//SAVEDATA_FILE_LISTのファイルを探す
SFO_FILE* find_psf_datafile(SFO_DATA *data)
{
	if( !data || strcmp(data->key,PSF_KEY_SAVE_FLIST) != 0 ) return NULL;
	
	SFO_FILE *file;
	u8 *loc = data->curPos;
	
	while( loc < (data->pos + data->size_02) )
	{
		file = (SFO_FILE*)loc;
		
		loc += sizeof(SFO_FILE);
		data->curPos = loc;
		
		if( file->name[0] )	return file;
	}
	
	return NULL;
}


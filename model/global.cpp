#include "stdafx.h"
#include <codecvt>
#include "global.h"
#include "sqlite/sqlite3.h"
#include "stock_map.h"

	MODEL_API StockMap g_stock;

	//默认的数据库全路径名称
	MODEL_API std::string default_db_path = {};

	//获取默认的数据库全路径名称
	MODEL_API const char * GetDefaultDb()
	{
		if (default_db_path.empty())
		{
			wchar_t exeFullPath[MAX_PATH] = { 0 }; // MAX_PATH在WINDEF.h中定义了，等于260
			GetModuleFileNameW(NULL, exeFullPath, MAX_PATH);
			
			wchar_t *p = wcsrchr(exeFullPath, '\\'); 
			*p = 0x00; //去掉最后文件名

			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
			auto temp = conv.to_bytes(exeFullPath);

			default_db_path.append(temp);
			default_db_path.append("\\quote.db");
		}
		return default_db_path.c_str();
	}

	//创建默认的数据库
	MODEL_API bool create_default_database(const char * path)
	{
		sqlite3 *db = NULL;
		int rc = sqlite3_open_v2(path, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
		char * pErrMsg = 0;
		
		rc = sqlite3_exec(db,
			"PRAGMA page_size=4096;"
			"CREATE TABLE if not exists [Quote] ("
			"[Id] NCHAR(8) NOT NULL,"
			"[QuoteTime] INT  NOT NULL,"
			"[Open] FLOAT,"
			"[High] FLOAT,"
			"[Low] FLOAT,"
			"[Close] FLOAT,"
			"[Volume] FLOAT,"
			"[Amount] FLOAT,"
			"CONSTRAINT[] PRIMARY KEY([Id], [QuoteTime]));"
			"CREATE TABLE if not exists[Stock]("
			"[Id] nchar(8) PRIMARY KEY NOT NULL, "
			"[Market] tinyint , "
			"[Catalog] tinyint , "
			"[Title] nchar(8) , "
			"[Minicode] nchar(4) ,"
			"[Circulation] FLOAT,"
			"[Exright] BLOB);", 0, 0, &pErrMsg);
		if (rc!=SQLITE_OK)
		{
			printf("Error message: %s\n", pErrMsg);
			sqlite3_free(pErrMsg);//如果有错误，则需要free返回的错误信息
			sqlite3_close_v2(db);
			return false;
		}
		else
		{
			sqlite3_close_v2(db);
			return true;
		}
	}


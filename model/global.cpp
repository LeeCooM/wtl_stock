#include "stdafx.h"
#include "global.h"
#include "sqlite/sqlite3.h"
namespace global
{
	
	//Ĭ�ϵ����ݿ�ȫ·������
	MODEL_API std::string default_db_path = {};
	MODEL_API unsigned long begin_date = 0;
	MODEL_API unsigned long end_date = 0;
	//��ȡĬ�ϵ����ݿ�ȫ·������
	MODEL_API const char * GetDefaultDb()
	{
		if (default_db_path.empty())
		{
			char exeFullPath[MAX_PATH] = { 0 }; // MAX_PATH��WINDEF.h�ж����ˣ�����260
			GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
			
			char *p = strrchr(exeFullPath, '\\'); 
			*p = 0x00; //ȥ������ļ���

			default_db_path.append(exeFullPath);
			default_db_path.append("\\quote.db");
		}
		return default_db_path.c_str();
	}

	//����Ĭ�ϵ����ݿ�
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
			"[Minicode] nchar(4) ); "
			"CREATE TABLE if not exists [ExRight] ("
			"[Id] nchar(8) NOT NULL,"
			"[QuoteTime] integer NOT NULL,"
			"[AddStock] float ,"
			"[BuyStock] float ,"
			"[Price] float ,"
			"[Bonus] float ,"
			"CONSTRAINT[PK_Exrights] PRIMARY KEY([Id], [QuoteTime])"
			"); ", 0, 0, &pErrMsg);
		if (rc!=SQLITE_OK)
		{
			printf("Error message: %s\n", pErrMsg);
			sqlite3_free(pErrMsg);//����д�������Ҫfree���صĴ�����Ϣ
			sqlite3_close_v2(db);
			return false;
		}
		else
		{
			sqlite3_close_v2(db);
			return true;
		}
	}
}

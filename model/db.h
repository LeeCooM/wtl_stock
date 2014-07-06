#include <string>
#include <codecvt>
#include <fstream>
#include "sqlite/sqlite3.h"
#ifndef db_h
#define db_h
class db
{
public:
	static std::string default_;
	static void set_default(std::string _default)
	{
		default_ = _default;

		//�ж��ļ��Ƿ���ڣ�fstream�ķ�����Ȼ�Ǵ��Ƿ�����
		//���ֱ���Թ��������ݿ��Ƿ������򿪣����жϡ�
		sqlite3 *db = NULL;
		int rc = 0;
		rc = sqlite3_open_v2(default_.c_str(), &db, SQLITE_OPEN_READWRITE , NULL);
		if (rc != SQLITE_OK)
		{
			sqlite3_close_v2(db);
		}

	}
	static void set_default(std::wstring _default)
	{
		//����һ��ת����
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

		//���������ת��:conv.from_bytes(narrowStr);
		set_default(conv.to_bytes(_default));
	}
};

std::string db::default_="";

#endif
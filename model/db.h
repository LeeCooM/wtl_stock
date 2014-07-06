#include <string>
#include <codecvt>
#include <fstream>
#include "sqlite/sqlite3.h"
#include <functional>
#ifndef db_h
#define db_h
class db
{
public:
	static std::string default_;
	static bool set_default(std::string _default,std::function<bool(const char *)> create_database=nullptr)
	{
		default_ = _default;

		//�ж��ļ��Ƿ���ڣ�fstream�ķ�����Ȼ�Ǵ��Ƿ�����
		//���ֱ���Թ��������ݿ��Ƿ������򿪣����жϡ�
		if (!is_exist(default_))
		{
			if (create_database!=nullptr)
				return create_database(default_.c_str());
			return false;
		}
		return true;

	}
	static bool set_default(std::wstring _default, std::function<bool(std::string)> create_database = nullptr)
	{
		//����һ��ת����
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

		//���������ת��:conv.from_bytes(narrowStr);
		return set_default(conv.to_bytes(_default));
	}

private:

	//ע�⣬��ʹ����һ��0�ֽڵ�ͬ���ļ���checkҲ�᷵��false
	static bool is_exist(std::string _default)
	{
		sqlite3 *db = NULL;
		int rc = 0;
		rc = sqlite3_open_v2(_default.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
		if (rc != SQLITE_OK)
		{
			sqlite3_close_v2(db);
			return true;
		}
		return false;
	}
};

std::string db::default_="";

#endif
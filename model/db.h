#include <string>
#include <vector>
#include <codecvt>
#include <fstream>
#include "sqlite/sqlite3.h"
#include <functional>
#include <memory>
#include <type_traits>
#ifndef db_h
#define db_h

#ifdef MODEL_EXPORTS
#define MODEL_API __declspec(dllexport)
#else
#define MODEL_API __declspec(dllimport)
#endif

class sql;
class DbConnection
{
//��̬����
public:
	//�����þ�̬������ָ��
	static std::string *get_default_pointer()
	{
		static std::string default_;
		return &default_;
	}

	static bool set_default(const char *_default, std::function<bool(const char *)> create_database = nullptr)
	{
		*get_default_pointer() = _default;
		//���ֱ���Թ��������ݿ��Ƿ������򿪣����жϡ�
		if (!is_exist(get_default_pointer()->c_str()))
		{
			if (create_database != nullptr)
				return create_database(get_default_pointer()->c_str());
			return false;
		}
		return true;
	};
	static bool set_default(const wchar_t *_default, std::function<bool(const char *)> create_database = nullptr)
	{
		//����һ��ת����
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		auto temp = conv.to_bytes(_default);//���������ת��:conv.from_bytes(narrowStr);
		return set_default(temp.c_str(), create_database);
	}
	static const char *get_default(){
		return get_default_pointer()->c_str();
	}

private:
	static bool is_exist(const char *_default)
	{
		sqlite3 *pdb = NULL;
		int rc = 0;
		rc = sqlite3_open_v2(_default, &pdb, SQLITE_OPEN_READWRITE, NULL);
		if (pdb)
			sqlite3_close_v2(pdb);
		return (rc == SQLITE_OK);
	}


//�Ǿ�̬����
public:
	std::shared_ptr<sqlite3> connection_=nullptr;
	bool conected_ = false;
	//�������ݿ�


	DbConnection(const char *filename=nullptr)
	{
		initDbConnection(filename);
	}

	//���ﲻ��ʹ��=nullptr����������ظ�������
	DbConnection(const wchar_t *filename)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		auto temp = conv.to_bytes(filename);//���������ת��:conv.from_bytes(narrowStr);
		initDbConnection(temp.c_str());
	}

	//�Ƿ�������
	bool operator () ()
	{
		return (connection_ != nullptr);
	};

	std::shared_ptr<sqlite3> connection()
	{
		return connection_;
	}
private:
	void initDbConnection(const char *filename = nullptr)
	{
		if (filename == nullptr)
			filename = DbConnection::get_default();
		sqlite3 *connection = nullptr;
		int result = sqlite3_open_v2(filename, &connection, SQLITE_OPEN_READWRITE, nullptr);  // you can treat errors by throwing exceptions

		if (result == SQLITE_OK)
		{
			connection_ = std::shared_ptr<sqlite3>(connection, sqlite3_close_v2);
		}
		else
		{
			if (connection)
				sqlite3_close_v2(connection);
			connection = nullptr;
		}
	}
};



class MODEL_API DbCommand
{
public:

	//ʹ�����أ�����ascii��unicode��sql�ı���ͬ���ڱ�����
	DbCommand(DbConnection connection,char *sql)
	{
		sqlite3_stmt *stmt_buffer = 0;
		sqlite3_prepare_v2(connection.connection().get(), sql, (int)strlen(sql), &stmt_buffer, 0);
		//sqlite3_prepare16_v2
		stmt = std::shared_ptr<sqlite3_stmt>(stmt_buffer, sqlite3_finalize);
	}
	DbCommand(DbConnection connection, wchar_t *sql)
	{
		sqlite3_stmt *stmt_buffer = 0;
		sqlite3_prepare16_v2(connection.connection().get(), sql, (int)wcslen(sql), &stmt_buffer, 0);
		//sqlite3_prepare16_v2
		stmt =std::shared_ptr<sqlite3_stmt>(stmt_buffer, sqlite3_finalize);
	}

private:
	std::shared_ptr<sqlite3_stmt> stmt=nullptr;



};

#endif
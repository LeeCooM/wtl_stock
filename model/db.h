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

//class DbConnection;

//���ݿ��쳣
class DbException : public std::runtime_error {
public:
	//�������޹�
	DbException(const char *error_message) : std::runtime_error(error_message)
	{

	}

	//�������й�
	DbException(sqlite3 *sql_connection) : runtime_error(sqlite3_errmsg(sql_connection))
	{

	}
};

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
	std::shared_ptr<sqlite3> Connection=nullptr;
	bool Conected = false;	//�������ݿ�


	DbConnection(const char *filename=nullptr)
	{
		open(filename);
	}

	//���ﲻ��ʹ��=nullptr����������ظ�������
	DbConnection(const wchar_t *filename)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		auto temp = conv.to_bytes(filename);//���������ת��:conv.from_bytes(narrowStr);
		open(temp.c_str());
	}

	//�Ƿ�������
	bool operator () ()
	{
		return (Connection.get() != nullptr);
	};


	std::string DbConnection::LastError() {
		return std::string((char *)sqlite3_errmsg(Connection.get()));
	}
private:
	void open(const char *filename = nullptr)
	{
		if (filename == nullptr)
			filename = DbConnection::get_default();
		sqlite3 *connection_buffer = nullptr;
		int result = sqlite3_open_v2(filename, &connection_buffer, SQLITE_OPEN_READWRITE, nullptr);  // you can treat errors by throwing exceptions

		if (result == SQLITE_OK)
		{
			Connection = std::shared_ptr<sqlite3>(connection_buffer, sqlite3_close_v2);
		}
		else
		{
			if (connection_buffer)
				sqlite3_close_v2(connection_buffer);
			//connection_buffer = nullptr;
		}
	}
};



class MODEL_API DbCommand
{
public:

	//ʹ�����أ�����ascii��unicode��sql�ı���ͬ���ڱ�����
	DbCommand(DbConnection &_connection, char *sql) :connection_(_connection)
	{
		const char *tail = NULL;
		sqlite3_stmt *stmt_ptr = 0;

		//char *���䳤��ʹ��-1��string����string�ĳ��ȣ�wstring����Ҫ����2
		if (sqlite3_prepare_v2(connection_.Connection.get(), sql, -1, &stmt_ptr, &tail) != SQLITE_OK)
		{
			//�������ӣ����ظ����ӵ���������Ϣ��Ҳ�ɻ�ȡ������Ϣ�����ַ���
			throw DbException(connection_.Connection.get());
		}
		stmt = std::shared_ptr<sqlite3_stmt>(stmt_ptr, sqlite3_finalize);
		this->column_count = sqlite3_column_count(this->stmt.get());
	}
	DbCommand(DbConnection &connection, wchar_t *sql) :connection_(connection)
	{
		const wchar_t *tail = NULL;
		sqlite3_stmt *stmt_ptr = 0;
		if (sqlite3_prepare16_v2(connection_.Connection.get(), sql, -1, &stmt_ptr, (const void**)&tail) != SQLITE_OK)
		{
			throw DbException(connection.Connection.get());
		}
		stmt = std::shared_ptr<sqlite3_stmt>(stmt_ptr, sqlite3_finalize);
		this->column_count = sqlite3_column_count(this->stmt.get());
	}
	DbCommand(DbConnection &connection, std::string &sql) :connection_(connection)
	{
		const char *tail = NULL;
		sqlite3_stmt *stmt_ptr = 0;
		if (sqlite3_prepare_v2(connection.Connection.get(), sql.c_str(), (int)sql.length(), &stmt_ptr, &tail) != SQLITE_OK)
		{
			throw DbException(connection_.Connection.get());
		}
		stmt = std::shared_ptr<sqlite3_stmt>(stmt_ptr, sqlite3_finalize);
		this->column_count = sqlite3_column_count(this->stmt.get());
	}
	DbCommand(DbConnection &connection, std::wstring &sql) :connection_(connection)
	{
		const wchar_t *tail = NULL;
		sqlite3_stmt *stmt_ptr = 0;
		//ע�⣺wstring�ĳ���Ҫ����2
		if (sqlite3_prepare16_v2(connection_.Connection.get(), sql.c_str(), (int)sql.length() * 2, &stmt_ptr, (const void**)&tail) != SQLITE_OK)
		{
			throw DbException(connection_.Connection.get());
		}
		stmt = std::shared_ptr<sqlite3_stmt>(stmt_ptr, sqlite3_finalize);
		this->column_count = sqlite3_column_count(this->stmt.get());
	}


private:
	std::shared_ptr<sqlite3_stmt> stmt=nullptr;
	DbConnection &connection_;
	int column_count = 0;



};

#endif
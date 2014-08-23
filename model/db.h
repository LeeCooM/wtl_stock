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

//Blob��
//Thanks For https://github.com/catnapgames/NLDatabase
class Blob {
public:
	void *data;
	int length;

protected:
	Blob(void *data, int length) : data(data), length(length) {
	}
};


class StaticBlob : public Blob {
public:
	template<typename T>
	StaticBlob(T *data, int length) : Blob((void*)data, length) { }
};


class TransientBlob : public Blob {
public:
	template<typename T>
	TransientBlob(T *data, int length) : Blob((void*)data, length) { }
};


class DbCommand
{
public:

	//ʹ�����أ�����ascii��unicode��sql�ı���ͬ���ڱ�����
	DbCommand(std::shared_ptr<sqlite3> _connection, const char sql[]) :connection_(_connection)
	{
		const char *tail = NULL;
		sqlite3_stmt *stmt_ptr = 0;

		//char *���䳤��ʹ��-1��string����string�ĳ��ȣ�wstring����Ҫ����2
		if (sqlite3_prepare_v2(connection_.get(), sql, -1, &stmt_ptr, &tail) != SQLITE_OK)
		{
			//�������ӣ����ظ����ӵ���������Ϣ��Ҳ�ɻ�ȡ������Ϣ�����ַ���
			throw DbException(connection_.get());
		}
		stmt = std::shared_ptr<sqlite3_stmt>(stmt_ptr, sqlite3_finalize);
		this->column_count = sqlite3_column_count(this->stmt.get());
	}
	DbCommand(std::shared_ptr<sqlite3> _connection, const wchar_t sql[]) :connection_(_connection)
	{
		const wchar_t *tail = NULL;
		sqlite3_stmt *stmt_ptr = 0;
		if (sqlite3_prepare16_v2(connection_.get(), sql, -1, &stmt_ptr, (const void**)&tail) != SQLITE_OK)
		{
			throw DbException(connection_.get());
		}
		stmt = std::shared_ptr<sqlite3_stmt>(stmt_ptr, sqlite3_finalize);
		this->column_count = sqlite3_column_count(this->stmt.get());
	}
	DbCommand(std::shared_ptr<sqlite3> _connection,  std::string &sql) :connection_(_connection)
	{
		const char *tail = NULL;
		sqlite3_stmt *stmt_ptr = 0;
		if (sqlite3_prepare_v2(connection_.get(), sql.c_str(), (int)sql.length(), &stmt_ptr, &tail) != SQLITE_OK)
		{
			throw DbException(connection_.get());
		}
		stmt = std::shared_ptr<sqlite3_stmt>(stmt_ptr, sqlite3_finalize);
		this->column_count = sqlite3_column_count(this->stmt.get());
	}
	DbCommand(std::shared_ptr<sqlite3> _connection, std::wstring &sql) :connection_(_connection)
	{
		const wchar_t *tail = NULL;
		sqlite3_stmt *stmt_ptr = 0;
		//ע�⣺wstring�ĳ���Ҫ����2
		if (sqlite3_prepare16_v2(connection_.get(), sql.c_str(), (int)sql.length() * 2, &stmt_ptr, (const void**)&tail) != SQLITE_OK)
		{
			throw DbException(connection_.get());
		}
		stmt = std::shared_ptr<sqlite3_stmt>(stmt_ptr, sqlite3_finalize);
		this->column_count = sqlite3_column_count(this->stmt.get());
	}

	//Bind���֣�ʹ�ñ��ģ�崦��
	//sqlite3��bind��������9������
	//int sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
	//int sqlite3_bind_double(sqlite3_stmt*, int, double);
	//int sqlite3_bind_int(sqlite3_stmt*, int, int);
	//int sqlite3_bind_int64(sqlite3_stmt*, int, sqlite3_int64);
	//int sqlite3_bind_null(sqlite3_stmt*, int);
	//int sqlite3_bind_text(sqlite3_stmt*, int, const char*, int n, void(*)(void*));
	//int sqlite3_bind_text16(sqlite3_stmt*, int, const void*, int, void(*)(void*));
	//int sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);
	//int sqlite3_bind_zeroblob(sqlite3_stmt*, int, int n);

	//bind����������Ϻ����
	inline bool bind(const int) { return true; }


	//sqlite3_bind_int�����4λ���µ�����
	//���ʹ���������������������������������������汾����ͬʱ����int,unsigned int��
	//ͬʱ�����������أ����뱣֤ÿ�������޶��������������ز���ȷ�ı������
	//���ǽ����ں�������ֵ�У�����ԭ�������ͣ�����һ�����ƣ��Ա���������ɴ˰汾
	template <typename Tint, typename... Args>
	typename std::enable_if <std::is_integral<Tint>::value && sizeof(Tint)<8,bool>::type 
	bind(int current, Tint first, const Args &... args)
	{
		if (sqlite3_bind_int(stmt.get(), current, first) != SQLITE_OK)
		{
			return false;
		}
		return bind(current + 1, args...);
	}

	//sqlite3_bind_int64,���8λ���ϵ�����
	template <typename Tint, typename... Args>
	typename std::enable_if <std::is_integral<Tint>::value && sizeof(Tint)>=8, bool>::type
		bind(int current, Tint first, const Args &... args)
	{
			if (sqlite3_bind_int64(stmt.get(), current, first) != SQLITE_OK)
			{
				return false;
			}
			return bind(current + 1, args...);
	}

	//sqlite3_bind_double����Ը��ม����
	template <typename Tdouble, typename... Args>
	typename std::enable_if <std::is_floating_point<Tdouble>::value,bool>::type
	bind(int current,Tdouble first, const Args &... args)
	{
		if (sqlite3_bind_double(stmt.get(), current, first) != SQLITE_OK)
		{
			return false;
		}
		return bind(current + 1, args...);
	}

	//���������ض�����ʶ���ʱ��ת�����ַ���Ȼ��bind_text
	//д��Ͷ�ȡ����Ҫת��Ϊ�ַ���������stringstream��ʱ����������ܽϲ
	template <typename T,  typename... Args>
	typename std::enable_if <!std::is_floating_point<T>::value && !std::is_integral<T>::value, bool>::type
	bind(int current, T &first, const Args &... args)
	{
		std::stringstream ss;
		ss << first;
		if (sqlite3_bind_text(stmt.get(), current,ss.str().data(), ss.str().length(),SQLITE_TRANSIENT) != SQLITE_OK)
		{
			return false;
		}
		return bind(current + 1, args...);
	}

	//bind const char[]��char *
	//��ʹ��const char *������"first"֮��ĳ����ַ�����������ʶ��
	//���ڳ������⣬�е�sqlite��װ����size+1��������������0��β�ַ���Ŀǰ��δ�����кβ��ס�
	template <typename... Args>
	bool bind(int current, const char first[], const Args &... args)
	{
		if (sqlite3_bind_text(stmt.get(), current, first, strlen(first), SQLITE_TRANSIENT) != SQLITE_OK)
		{
			return false;
		}
		return bind(current + 1, args...);
	}

	//bind const wchar_t[]
	//����Ҫ����2
	template <typename... Args>
	bool bind(int current, const wchar_t first[], const Args &... args)
	{
		if (sqlite3_bind_text16(stmt.get(), current, first, wcslen(first)*2, SQLITE_TRANSIENT) != SQLITE_OK)
		{
			return false;
		}
		return bind(current + 1, args...);
	}

	//bind string
	//ʹ��const char *������"first"֮��ĳ����ַ�����������ʶ��
	//���ڳ������⣬�е�sqlite��װ����size+1��������������0��β�ַ���Ŀǰ��δ�����кβ��ס�
	template <typename... Args>
	bool bind(int current, std::string first, const Args &... args)
	{
		if (sqlite3_bind_text(stmt.get(), current, first.c_str(), first.length(), SQLITE_TRANSIENT) != SQLITE_OK)
		{
			return false;
		}
		return bind(current + 1, args...);
	}

	//bind const wchar_t[]
	//����Ҫ����2
	template <typename... Args>
	bool bind(int current, std::wstring first, const Args &... args)
	{
		if (sqlite3_bind_text16(stmt.get(), current, first.c_str(), first.length()*2, SQLITE_TRANSIENT) != SQLITE_OK)
		{
			return false;
		}
		return bind(current + 1, args...);
	}

	//SQLITE_STATIC��ʽ blob
	template <typename... Args>
	void bind(int current, StaticBlob value, const Args &... args) {
		if (sqlite3_bind_blob(stmt, index, value.data, value.length, SQLITE_STATIC) != SQLITE_OK)
		{
			return false;
		}
		return bind(current + 1, args...);
	}

	//bind SQLITE_TRANSIENT ��ʽ��Blob
	template <typename... Args>
	void bind(int current, TransientBlob value, const Args &... args) {
		if (sqlite3_bind_blob(stmt, index, value.data, value.length, SQLITE_TRANSIENT) != SQLITE_OK)
		{
			return false;
		}
		return bind(current + 1, args...);
	}

	//ִ���޷��ص�Sql����
	//step��Ӧ����Sqlite_done
	bool ExecuteNonQuery()
	{
		int rc = sqlite3_step(stmt.get());
		sqlite3_reset(stmt.get());
		return (rc == SQLITE_DONE);
	}

	//ȡ��һ�н��
	template <typename... Args>
	bool Execute(Args &... args)
	{
		int rc = sqlite3_step(stmt.get());
		sqlite3_reset(stmt.get());
		return (rc == SQLITE_DONE);
	}

	//֧�ֱ�����begin end��++����

private:
	std::shared_ptr<sqlite3_stmt> stmt=nullptr;
	std::shared_ptr<sqlite3> connection_;
	int column_count = 0;
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
	std::shared_ptr<sqlite3> Connection = nullptr;
	bool Conected = false;	//�������ݿ�


	DbConnection(const char *filename = nullptr)
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

	template <class T>
	DbCommand get_command(T &sql) {
		return DbCommand(this->Connection, sql);
	}

	std::string get_last_error() {
		return std::string((char *)sqlite3_errmsg(Connection.get()));
	}

	long long get_last_id() {
		if (!this->Connection.get()) throw DbException("Connection invalid");
		return sqlite3_last_insert_rowid(Connection.get());
	}

	void DbConnection::set_timeout(int ms_number) {
		if (!Connection.get()) throw DbException("Connection invalid");

		if (sqlite3_busy_timeout(Connection.get(), ms_number) != SQLITE_OK)
			throw DbException(Connection.get());
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
			throw DbException("unable to open database");
			//connection_buffer = nullptr;
		}
	}
};
#endif
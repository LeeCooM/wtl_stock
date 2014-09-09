#include <string>
#include <vector>
#include <codecvt>
#include <fstream>
#include <functional>
#include <memory>
#include <type_traits> //����ΪC++��
#include "sqlite/sqlite3.h" //����Ϊ������Ŀ��ͷ�ļ�
//����ΪC�⡢C++�⡢������Ŀ��ͷ�ļ�����Ŀ�ڵ�ͷ�ļ�

//��Ŀ����·������ͷ�ļ���,ȫ����д
//��#include�ĺ��棬��Visual Studio���﷨���ܸ�֪��������
#ifndef STOCK_MODEL_DB_H
#define STOCK_MODEL_DB_H

//Dll��Ŀ�У��ڴ˶����MODEL_API����Dll��Ŀ��ʹ��dll����Ŀ��ʹ��ͬһ��ͷ�ļ���
//#ifdef MODEL_EXPORTS
//#define MODEL_API __declspec(dllexport)
//#else
//#define MODEL_API __declspec(dllimport)
//#endif

//���ݿ��쳣
//����һ�������ʣ�ʹ������ĸ��д��ʽ�������»���
class DbException : public std::runtime_error {
public:
	//�������޹�
	DbException(const char *_error_message) : std::runtime_error(_error_message){}

	//�������й�
	DbException(sqlite3 *_sql_connection) : runtime_error(sqlite3_errmsg(_sql_connection))
	{
		const char *p=this->what();
	}
};

//Blob��
//Thanks For https://github.com/catnapgames/NLDatabase
struct Blob {
public:
	void *data; //�ṹ���Ա������������Ȼ��ȫ��Сд������ʹ��data_��ʽ
	int length;

	template<typename T>
	Blob(T *data, int length) : data((void*)data), length(length) {}
};

//sql������
//������ʹ�ã���Ҫ����һ��sql������Bind���������
//ע�⣺Query��֧�ֶ�������sqlite3_prepare_v2�е�tailΪʣ���������
class Query
{
public:

	//ʹ�����أ�����ascii��unicode��sql�ı���ͬ���ڱ�����
	Query(std::shared_ptr<sqlite3> _connection, const char sql[]) :connection_(_connection)
	{
		const char *tail = NULL;
		sqlite3_stmt *stmt_ptr = 0;

		//char *���䳤��ʹ��-1��string����string�ĳ��ȣ�wstring����Ҫ����2
		if (sqlite3_prepare_v2(connection_.get(), sql, -1, &stmt_ptr, &tail) != SQLITE_OK)
		{
			//�������ӣ����ظ����ӵ���������Ϣ��Ҳ�ɻ�ȡ������Ϣ�����ַ���
			throw DbException(connection_.get());
		}

		stmt_ = std::shared_ptr<sqlite3_stmt>(stmt_ptr, sqlite3_finalize);
		this->column_count_ = sqlite3_column_count(this->stmt_.get());
	}

	//���wchar_t[]�Ĺ��캯��
	Query(std::shared_ptr<sqlite3> _connection, const wchar_t sql[]) :connection_(_connection)
	{
		const wchar_t *tail = NULL;
		sqlite3_stmt *stmt_ptr = 0;
		if (sqlite3_prepare16_v2(connection_.get(), sql, -1, &stmt_ptr, (const void**)&tail) != SQLITE_OK)
		{
			throw DbException(connection_.get());
		}
		stmt_ = std::shared_ptr<sqlite3_stmt>(stmt_ptr, sqlite3_finalize);
		this->column_count_ = sqlite3_column_count(this->stmt_.get());
	}

	//���string�Ĺ��캯����ʵ����û��Ҫ��
	Query(std::shared_ptr<sqlite3> _connection, std::string &sql) :connection_(_connection)
	{
		const char *tail = NULL;
		sqlite3_stmt *stmt_ptr = 0;
		if (sqlite3_prepare_v2(connection_.get(), sql.c_str(), (int)sql.length(), &stmt_ptr, &tail) != SQLITE_OK)
		{
			throw DbException(connection_.get());
		}
		stmt_ = std::shared_ptr<sqlite3_stmt>(stmt_ptr, sqlite3_finalize);
		this->column_count_ = sqlite3_column_count(this->stmt_.get());
	}

	//���wstring�Ĺ��캯��
	Query(std::shared_ptr<sqlite3> _connection, std::wstring &sql) :connection_(_connection)
	{
		const wchar_t *tail = NULL;
		sqlite3_stmt *stmt_ptr = 0;
		//ע�⣺wstring�ĳ���Ҫ����2
		if (sqlite3_prepare16_v2(connection_.get(), sql.c_str(), (int)sql.length() * 2, &stmt_ptr, (const void**)&tail) != SQLITE_OK)
		{
			throw DbException(connection_.get());
		}
		stmt_ = std::shared_ptr<sqlite3_stmt>(stmt_ptr, sqlite3_finalize);
		this->column_count_ = sqlite3_column_count(this->stmt_.get());
	}

	//����...����Ҫ��ʹ��Excute��Ҫ������һ�����Excute�ڷ���SQLITE_DONEʱ�Զ����á�
	//��sql���Ϊselect��ʱ�򣬵ڶ���Bind��ִ�У���������
	//��˵ڶ���Bind֮ǰ��ӦReset���Ա�����ϴεĽ����
	//Insert֮�಻��Ҫ������
	//inline void Reset()
	//{
	//	sqlite3_reset(stmt_.get());
	//}

	//��������sql���
	bool Reset(const char *_sql)
	{
		const char *tail = NULL;
		sqlite3_stmt *stmt_ptr = 0;
		//����sqlites_reset,���°汾��step֮���Զ���reset��
		sqlite3_reset(stmt_.get());
		//sqlite3_clear_bindings(stmt_.get()); //��Ҫ��
		//sqlite3_finalize(stmt_.get()); //���ﲻ���ֹ�ִ�У���Ϊ����ָ��reset��ʱ��ִ�С�
		//char *���䳤��ʹ��-1��string����string�ĳ��ȣ�wstring����Ҫ����2
		if (sqlite3_prepare_v2(connection_.get(), _sql, -1, &stmt_ptr, &tail) != SQLITE_OK)
		{
			//�������ӣ����ظ����ӵ���������Ϣ��Ҳ�ɻ�ȡ������Ϣ�����ַ���
			throw DbException(connection_.get());
			return false;
		}
		stmt_.reset(stmt_ptr, sqlite3_finalize);
		this->column_count_ = sqlite3_column_count(stmt_.get());
		return true;
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
	inline bool  Bind(const int) { return true; }
	
	//sqlite3_bind_int�����4λ���µ�����
	//���ʹ���������������������������������������汾����ͬʱ����int,unsigned int��
	//ͬʱ�����������أ����뱣֤ÿ�������޶��������������ز���ȷ�ı������
	//���ǽ����ں�������ֵ�У�����ԭ�������ͣ�����һ�����ƣ��Ա���������ɴ˰汾
	template <typename Tint, typename... Args>
	inline typename std::enable_if < std::is_integral<Tint>::value && sizeof(Tint)<8, bool>::type
		Bind(int current, Tint first, const Args &... args)
	{
			if (sqlite3_bind_int(stmt_.get(), current, first) != SQLITE_OK)
			{
				return false;
			}
			return  Bind(current + 1, args...);
	}

	//sqlite3_bind_int64,���8λ���ϵ�����
	template <typename Tint, typename... Args>
	inline typename std::enable_if <std::is_integral<Tint>::value && sizeof(Tint) >= 8, bool>::type
		Bind(int current, Tint first, const Args &... args)
	{
			if (sqlite3_bind_int64(stmt_.get(), current, first) != SQLITE_OK)
			{
				return false;
			}
			return  Bind(current + 1, args...);
	}

	//sqlite3_bind_double����Ը��ม����
	template <typename Tdouble, typename... Args>
	inline typename std::enable_if <std::is_floating_point<Tdouble>::value, bool>::type
		Bind(int current, Tdouble first, const Args &... args)
	{
			if (sqlite3_bind_double(stmt_.get(), current, first) != SQLITE_OK)
			{
				return false;
			}
			return  Bind(current + 1, args...);
	}

	//���������ض�����ʶ���ʱ��ת�����ַ���Ȼ��bind_text
	//д��Ͷ�ȡ����Ҫת��Ϊ�ַ���������stringstream��ʱ����������ܽϲ
	template <typename T, typename... Args>
	inline typename std::enable_if <!std::is_floating_point<T>::value && !std::is_integral<T>::value, bool>::type
		Bind(int current, T &first, const Args &... args)
	{
			std::stringstream ss;
			ss << first;
			if (sqlite3_bind_text(stmt_.get(), current, ss.str().data(), ss.str().length(), SQLITE_TRANSIENT) != SQLITE_OK)
			{
				return false;
			}
			return  Bind(current + 1, args...);
	}

	//bind const char[]��char *
	//��ʹ��const char *������"first"֮��ĳ����ַ�����������ʶ��,��Ϊģ�庯���޷���char[]����Ϊchar *
	//���ڳ������⣬�е�sqlite��װ����size+1��������������0��β�ַ���Ŀǰ��δ�����кβ��ס�
	template <typename... Args>
	inline bool Bind(int current, const char first[], const Args &... args)
	{
		if (sqlite3_bind_text(stmt_.get(), current, first, strlen(first), SQLITE_TRANSIENT) != SQLITE_OK)
		{
			return false;
		}
		return  Bind(current + 1, args...);
	}

	//bind const wchar_t[]
	//����Ҫ����2
	template <typename... Args>
	inline bool Bind(int current, const wchar_t first[], const Args &... args)
	{
		if (sqlite3_bind_text16(stmt_.get(), current, first,-1, SQLITE_TRANSIENT) != SQLITE_OK)
		{
			return false;
		}
		return  Bind(current + 1, args...);
	}

	//bind string
	//ʹ��const char *������"first"֮��ĳ����ַ�����������ʶ��
	//���ڳ������⣬�е�sqlite��װ����size+1��������������0��β�ַ���Ŀǰ��δ�����кβ��ס�
	template <typename... Args>
	inline bool Bind(int current, std::string first, const Args &... args)
	{
		if (sqlite3_bind_text(stmt_.get(), current, first.c_str(), first.length(), SQLITE_TRANSIENT) != SQLITE_OK)
		{
			return false;
		}
		return  Bind(current + 1, args...);
	}

	//bind wstring
	//����Ҫ����2
	template <typename... Args>
	inline bool Bind(int current, std::wstring first, const Args &... args)
	{
		if (sqlite3_bind_text16(stmt_.get(), current, first.c_str(), first.length() * 2, SQLITE_TRANSIENT) != SQLITE_OK)
		{
			return false;
		}
		return  Bind(current + 1, args...);
	}

	//bind Blob
	template <typename... Args>
	inline bool Bind(int current, Blob value, const Args &... args) {
		if (sqlite3_bind_blob(stmt, index, value.data, value.length, SQLITE_STATIC) != SQLITE_OK)
		{
			return false;
		}
		return  Bind(current + 1, args...);
	}

	//��ȡ����ֹ����
	inline void ReadColumn(int idx){}

	//����
	template <typename Tint, typename... Args>
	inline typename std::enable_if < !std::is_enum<Tint>::value && std::is_integral<Tint>::value && sizeof(Tint)<8, void>::type
		ReadColumn(int idx, Tint &first, Args &... args)
	{
			first = sqlite3_column_int(stmt_.get(), idx);
			ReadColumn(idx + 1, args...);
	}

	template <typename Tint, typename... Args>
	inline typename std::enable_if <!std::is_enum<Tint>::value && std::is_integral<Tint>::value && sizeof(Tint) >= 8, void>::type
		ReadColumn(int idx, Tint &first, Args &... args)
	{
			first = sqlite3_column_int64(stmt.get(), idx);
			ReadColumn(idx + 1, args...);
	}

	//�����enum����Ҫת����enum
	template <typename Tenum, typename... Args>
	inline typename std::enable_if <std::is_enum<Tenum>::value, void>::type
		ReadColumn(int idx, Tenum &first, Args &... args)
	{
			first = (Tenum)sqlite3_column_int(stmt.get(), idx);
			ReadColumn(idx + 1, args...);
	}

	template <typename TDouble, typename... Args>
	inline typename std::enable_if <std::is_floating_point<TDouble>::value, void>::type
		ReadColumn(int idx, TDouble &first, Args &... args)
	{
			first = (TDouble)sqlite3_column_double(stmt_.get(), idx);
			ReadColumn(idx + 1, args...);
	}

	//ʹ��const char *������"first"֮��ĳ����ַ�����������ʶ��
	//���ڳ������⣬�е�sqlite��װ����size+1��������������0��β�ַ���Ŀǰ��δ�����кβ��ס�
	template <typename... Args>
	inline void ReadColumn(int idx, std::string& first, Args &... args)
	{
		const char *p = (const char*)sqlite3_column_text(stmt.get(), idx);
		first = string(p, strlen(p) + 1);
		ReadColumn(idx + 1, args...);
	}

	//bind const wchar_t[]
	//����Ҫ����2
	template <typename... Args>
	inline void ReadColumn(int idx, std::wstring &first, Args &... args)
	{
		const wchar_t *p = (const wchar_t*)sqlite3_column_text16(stmt.get(), idx);
		first = wstring((wchar_t *)p, wcslen(p) * 2 + 2);
		ReadColumn(idx + 1, args...);
	}

	//ʹ��const char *������"first"֮��ĳ����ַ�����������ʶ��
	//���ڳ������⣬�е�sqlite��װ����size+1��������������0��β�ַ���Ŀǰ��δ�����кβ��ס�
	template <typename... Args>
	inline void ReadColumn(int idx, char first[], Args &... args)
	{
		//int i = sizeof(first); //4�ֽ���ʵ��ָ�볤��
		const char *p = (const char*)sqlite3_column_text(stmt_.get(), idx);
		//int i = strlen(p);
		strcpy_s(first, strlen(p) + 1, p);
		//first = std::string(sqlite3_column_text(stmt.get(), idx), sqlite3_column_bytes(stmt.get(), idx));
		ReadColumn(idx + 1, args...);
	}

	//bind const wchar_t[]
	//����Ҫ����2
	template <typename... Args>
	inline void ReadColumn(int idx, wchar_t first[], Args &... args)
	{
		const wchar_t *p = (const wchar_t*)sqlite3_column_text16(stmt_.get(), idx);
		int i = wcslen(p);
		wcscpy_s(first, wcslen(p) + 1, p);
		ReadColumn(idx + 1, args...);
	}
	//SQLITE_STATIC��ʽ blob
	template <typename... Args>
	inline void ReadColumn(int idx, Blob &first, Args &... args) {
		first = Blob(sqlite3_column_blob(stmt.get(), idx), sqlite3_column_bytes(stmt.get(), idx));
		ReadColumn(idx + 1, args...);
	}

	//ִ���޷��ص�Sql����,��insert��update��delete��create table�ȡ�
	//step��Ӧ����Sqlite_done
	inline bool ExcuteNonQuery()
	{
		int rc = sqlite3_step(stmt_.get());
		sqlite3_reset(stmt_.get());
		return (rc == SQLITE_DONE);
	}

	//ȡ��һ�н��
	template <typename... Args>
	inline bool Excute(Args & ...args)
	{
		int rc = sqlite3_step(stmt_.get());
		if (rc == SQLITE_ROW)
			ReadColumn(0, args...);
		if (rc==SQLITE_DONE)
			sqlite3_reset(stmt_.get());
		return (rc == SQLITE_ROW);
	}

private:
	std::shared_ptr<sqlite3_stmt> stmt_ = nullptr;
	std::shared_ptr<sqlite3> connection_;
	int column_count_ = 0;
};

class Db
{
	//��̬����
public:
	//�����þ�̬������ָ��
	static std::string *default_path()
	{
		static std::string default_path;
		return &default_path;
	}

	static bool set_default_path(const char *_default, std::function<bool(const char *)> create_database = nullptr)
	{
		*default_path() = _default;
		//���ֱ���Թ��������ݿ��Ƿ������򿪣����жϡ�
		if (!CheckDatabaseExist(default_path()->c_str()))
		{
			if (create_database != nullptr)
				return create_database(default_path()->c_str());
			return false;
		}
		return true;
	};
	static bool set_default_path(const wchar_t *_default, std::function<bool(const char *)> create_database = nullptr)
	{
		//����һ��ת����
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		auto temp = conv.to_bytes(_default);//���������ת��:conv.from_bytes(narrowStr);
		return set_default_path(temp.c_str(), create_database);
	}
	
private:
	static bool CheckDatabaseExist(const char *_default)
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
	std::shared_ptr<sqlite3> connection_ = nullptr;

	Db(const char *filename = nullptr)
	{
		OpenDatabase(filename);
	}

	//���ﲻ��ʹ��=nullptr����������ظ�������
	Db(const wchar_t *filename)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		auto temp = conv.to_bytes(filename);//���������ת��:conv.from_bytes(narrowStr);
		OpenDatabase(temp.c_str());
	}

	//�Ƿ�������
	bool operator () ()
	{
		return (connection_.get() != nullptr);
	};

	template <class T>
	Query CreateQuery(T &sql) {
		return Query(this->connection_, sql);
	}

	std::string GetLastError() {
		return std::string((char *)sqlite3_errmsg(connection_.get()));
	}

	inline long long GetLastId() {
		if (!this->connection_.get()) throw DbException("connection_ invalid");
		return sqlite3_last_insert_rowid(connection_.get());
	}

	void SetTimeout(int ms_number) {
		if (!connection_.get()) throw DbException("connection_ invalid");

		if (sqlite3_busy_timeout(connection_.get(), ms_number) != SQLITE_OK)
			throw DbException(connection_.get());
	}

	//ִ��sql���������ֵ
	//֧���÷ֺŷָ��Ķ���sql���
	//��֧�ֲ���
	bool ExcuteScript(const char *sql)
	{
		sqlite3_stmt *stmt_ptr = 0;
		const char *tail = sql;
		int rc;
		while (tail &&strlen(tail)>0)
		{
			if (sqlite3_prepare_v2(connection_.get(), tail, strlen(tail), &stmt_ptr, &tail) != SQLITE_OK)
			{
				//�������ӣ����ظ����ӵ���������Ϣ��Ҳ�ɻ�ȡ������Ϣ�����ַ���
				throw DbException(connection_.get());
				return false;
			}
			rc = sqlite3_step(stmt_ptr);
			rc = sqlite3_finalize(stmt_ptr);
		}
		return true;
	}


	//���������lambda?
	//ִ���޷��صĵ���sql
	//��bind����
	template <typename... Args>
	bool ExcuteNonQuery(const char *sql, const Args &... args)
	{
		auto query = CreateQuery(sql);
		query.Bind(1, args...);
		return query.ExcuteNonQuery();
	}

	void Begin()
	{
		ExcuteScript("BEGIN");
	}

	void Commit()
	{
		ExcuteScript("COMMIT");
	}
	void Rollback()
	{
		ExcuteScript("ROLLBACK");
	}

private:
	void OpenDatabase(const char *filename = nullptr)
	{
		if (filename == nullptr)
			filename = Db::default_path()->c_str();
		sqlite3 *connection_buffer = nullptr;
		int result = sqlite3_open_v2(filename, &connection_buffer, SQLITE_OPEN_READWRITE, nullptr);  // you can treat errors by throwing exceptions
		if (result == SQLITE_OK)
			connection_ = std::shared_ptr<sqlite3>(connection_buffer, sqlite3_close_v2);
		else
		{
			if (connection_buffer)
				sqlite3_close_v2(connection_buffer);
			throw DbException("unable to open database");
		}
	}
};
#endif
#ifndef db_quote_h
#define db_quote_h

#include <algorithm>
#include "sqlite/sqlite3.h"
#include "db.h"
#include "dad_parse_iterator.h"
struct sqlite3_stmt;

class DbQuote
{
public:
	DbQuote()=default;
	~DbQuote()=default;

	//��������quote
	//����ص�����,�ص�������Ƶ��(N���㱨һ�Σ�N%�㱨һ��)
	template <typename T>
	//typedef const std::enable_if<std::is_base_of<std::iterator, T>::value, T>::type 
	size_t bulk_insert(T _begin, T _end)
	{
		//������static assert,�����������õ�ʱ��������ָ��ṹ��ָ�룬����ͨ������
		//��ͬ����ʾ�����ܱ�������Ϸ���ʵ�ʺ���...������enable_if���Ƶ�����
		static_assert(std::is_same<parse_of_dad, std::iterator_traits<T>::value_type>::value, "Incorrect usage!");
		
		//1.ȷ�����ݿ�򿪣�
		sqlite3 *default_db=nullptr;
		int rc = sqlite3_open_v2(db::default_.c_str(), &default_db, SQLITE_OPEN_READWRITE, nullptr);
		if (rc != SQLITE_OK)
		{
			sqlite3_close_v2(default_db);
			return 0;
		}
		//��ʱ���ݿ��Ѿ��򿪣�ʹ��default_db
		


		//2.����sql
		sqlite3_stmt *pStmt = NULL;
		const char *insert_sql = "insert into Quote values(?, ?, ?, ?, ?, ?, ?,?)";

		//3.prepare
		rc = sqlite3_prepare_v2(default_db, insert_sql, -1, &pStmt, 0);
		if (rc)
		{
			fprintf(stderr, "Error: %s\n", sqlite3_errmsg(default_db));
			if (pStmt) sqlite3_finalize(pStmt);
			sqlite3_close_v2(default_db);
			return 0;
		}//��ʱ�Ѿ�prepare�ɹ�

		//4.��������
		//�������Ҫ needCommit = sqlite3_get_autocommit(defult_db);
		int needCommit = 1;
		if (needCommit) sqlite3_exec(default_db, "BEGIN", 0, 0, 0);//��������
		
		//5.ѭ������
		int insert_nums = 0;
		for (auto current = _begin; current != _end; ++current)
		{
			sqlite3_bind_text(pStmt, 1, current->idOfDad->id, sizeof(current->idOfDad->id), SQLITE_TRANSIENT);
			sqlite3_bind_int(pStmt, 2, current->quoteOfDad->quoteTime);
			//ִ��
			sqlite3_step(pStmt);
			rc = sqlite3_reset(pStmt); //���ã��´�ѭ������ִ��
			if (rc != SQLITE_OK){
				return 0;
			}
			++insert_nums;			
		}
		//5.�ύ����
		
		if (needCommit) sqlite3_exec(default_db, "COMMIT", 0, 0, 0);

		//6.�����ֳ�
		sqlite3_finalize(pStmt);
		sqlite3_close_v2(default_db);
		//7.���ز��������
		return insert_nums;
	}
};


#endif
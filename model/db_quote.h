#include <algorithm>
#include "sqlite/sqlite3.h"
#include "db.h"
#include "dad_parse_iterator.h"
#include "global.h"
#include "db_code.h"
#ifndef db_quote_h
#define db_quote_h

struct sqlite3_stmt;

class DbQuote
{
public:
	DbQuote()=default;
	~DbQuote()=default;

	//һ�λ�ȡ��ȫ��
	static MODEL_API long start_date, end_date; //�ǳ����ľ�̬��Ա�����ܳ�ʼ��

	//��ȡ�Ѱ�װ���ݵ���ֹ����
	unsigned long &get_start_date()
	{
		static unsigned long start_date;
		return start_date;
	}

	//��ȡ�Ѱ�װ���ݵ���ֹ����
	unsigned long &get_end_date()
	{
		static unsigned long end_date ;
		return end_date;
	}

	//���ص���ΪΪnullptr������T *���ؽ��
	//���ص�Ϊnullptr����ֱ����T�����ã��ڲ����з��ؽ��
	//�������ƿ��Գ���һ��...
	void get_date_range(unsigned long &start,unsigned long &end, std::function<void(const char *, int)> func = nullptr)
	{
		//��ȡ����ָ�룬�ֱ�ָ��̬�ı���
		//unsigned long start = get_start_date();
		//unsigned long end = get_end_date();
		
		sqlite3 *default_db = nullptr;
		int rc = sqlite3_open_v2(DbConnection::get_default(), &default_db, SQLITE_OPEN_READWRITE, nullptr);
	
		//������ȻӦʹ���쳣���ƣ��׳������Ĵ�����Ϣ����Ϊ��ݡ�
		//���ָܻ����쳣����ʹ��...�����ݿⲻ���ڣ����Դ��������Ӳ����ڣ����Եȴ����������ӣ�������������ʵ���鷳
		if (rc != SQLITE_OK)
		{
		}

		sqlite3_stmt *pStmt = NULL;
		const char *find_sql = "Select Min(QuoteTime),Max(QuoteTime) From Quote Where Id=?";
		const char *code = "SH000001";
		//3.prepare
		rc = sqlite3_prepare_v2(default_db, find_sql, -1, &pStmt, 0);
		int len = sizeof(code);
		rc=sqlite3_bind_text(pStmt, 1, code,8 , SQLITE_TRANSIENT);
		rc=sqlite3_step(pStmt); //ִ��
		if (rc != SQLITE_DONE) //ֻҪ�����ݣ����صľ���SQLITE_ROW����������򷵻�DONE
		{
			//����
		}
		//SQLITE_NULL

		start= sqlite3_column_int(pStmt, 0);
		end = sqlite3_column_int(pStmt, 1);

		sqlite3_finalize(pStmt);
		sqlite3_close_v2(default_db);

	}

	//��������quote
	//����ص�����,�ص�������Ƶ��(N���㱨һ�Σ�N%�㱨һ��)
	template <typename T>
	//typedef const std::enable_if<std::is_base_of<std::iterator, T>::value, T>::type 
	size_t bulk_insert(T _begin, T _end, int period=2000, std::function<void(const char *,int)> func=nullptr)
	{
		//������static assert,�����������õ�ʱ��������ָ��ṹ��ָ�룬����ͨ������
		//��ͬ����ʾ�����ܱ�������Ϸ���ʵ�ʺ���...������enable_if���Ƶ�����
		static_assert(std::is_same<parse_of_dad, std::iterator_traits<T>::value_type>::value, "Incorrect usage!");
		
		//�����1.����һ��command

		//1.ȷ�����ݿ�򿪣�
		DbConnection connection;
		

		sqlite3 *default_db=connection.Connection.get(); //ʹ��Api����quote

		//int rc = sqlite3_open_v2(DbConnection::get_default(), &default_db, SQLITE_OPEN_READWRITE, nullptr);
		//if (rc != SQLITE_OK)
		//{
		//	auto p = sqlite3_errmsg(default_db);
		//	func(p, 0);
		//	if (default_db)
		//		sqlite3_close_v2(default_db);
		//	return 0;
		//}
		//��ʱ���ݿ��Ѿ��򿪣�ʹ��default_db
		


		//2.����sql
		sqlite3_stmt *pStmt = NULL;
		const char *insert_sql = "INSERT OR IGNORE INTO QUOTE VALUES(?, ?, ?, ?, ?, ?, ?,?)";

		//3.prepare
		int rc = sqlite3_prepare_v2(default_db, insert_sql, -1, &pStmt, 0);
		if (rc)
		{
			auto p = sqlite3_errmsg(default_db);
			func(p, 0);
			if (pStmt) sqlite3_finalize(pStmt);
			if (default_db)
				sqlite3_close_v2(default_db);
			return 0;
		}//��ʱ�Ѿ�prepare�ɹ�

		//4.��������
		//�������Ҫ needCommit = sqlite3_get_autocommit(defult_db);
		int needCommit = 1;
		if (needCommit) sqlite3_exec(default_db, "BEGIN", 0, 0, 0);//��������
		
		//5.ѭ������
		int insert_nums = 0;
		//command����L""
		auto cmd = connection.get_command("INSERT OR REPLACE INTO Stock(Id,Market,Catalog,Title,MiniCode) VALUES(?,?,?,?,?)"); //���ӻ���´��������
		int idNumber = 0;
		id_of_dad *oldId = nullptr;
		Stock stock;
		for (auto current = _begin; current != _end; ++current)
		{
			//�������仯
			if (current->idOfDad != oldId)
			{
				oldId = current->idOfDad;
				idNumber = DbCode::FindStock(current->idOfDad->id);
				
				if (idNumber < 0 || (strcmp(DbCode::get_stock_list()[idNumber].Title,current->idOfDad->title)!=0)) //���벻���ڻ�������ڵ����Ƹ���
				{
					memcpy(stock.Id,current->idOfDad->id,9);
					memcpy(stock.Title,current->idOfDad->title,9);
					//��Ҫinsert��update
					cmd.bind(1, stock.Id, stock.Market, stock.Catalog, stock.Title, stock.MiniCode);
					cmd.ExecuteNonQuery();
				}

			}
			//������16λ��ɴ��󣿣�
			//sqlite3_bind_text(pStmt, 1, current->idOfDad->id, sizeof(current->idOfDad->id), SQLITE_TRANSIENT);
			sqlite3_bind_text(pStmt, 1, current->idOfDad->id, 8, SQLITE_TRANSIENT);
			sqlite3_bind_int(pStmt, 2, current->quoteOfDad->quoteTime);
			sqlite3_bind_double(pStmt, 3, current->quoteOfDad->open);
			sqlite3_bind_double(pStmt, 4, current->quoteOfDad->high);
			sqlite3_bind_double(pStmt, 5, current->quoteOfDad->low);
			sqlite3_bind_double(pStmt, 6, current->quoteOfDad->close);
			sqlite3_bind_double(pStmt, 7, current->quoteOfDad->volume);
			sqlite3_bind_double(pStmt, 8, current->quoteOfDad->amount);
			//ִ��
			rc = sqlite3_step(pStmt);
			
			if (rc != SQLITE_DONE && rc!=SQLITE_OK){
				auto p = sqlite3_errmsg(default_db);
				func(p, 0);
				if (default_db)
					sqlite3_close_v2(default_db);
				return 0;
			}
			sqlite3_reset(pStmt); //���ã��´�ѭ������ִ��
			if ((insert_nums%period) == 0 && (func != nullptr))
				func(nullptr,insert_nums);
			++insert_nums;			
		}
		//5.�ύ����
		
		if (needCommit) sqlite3_exec(default_db, "COMMIT", 0, 0, 0);

		//6.�����ֳ�
		sqlite3_finalize(pStmt);
		//sqlite3_close_v2(default_db);
		//7.���ز��������
		if (func != nullptr)
			func(nullptr,insert_nums);
		return insert_nums;
	}


};


#endif
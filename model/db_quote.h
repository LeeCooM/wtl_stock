#include <algorithm>
#include "sqlite/sqlite3.h"
#include "db.h"
#include "dad_parse_iterator.h"
#include "global.h"

#ifndef STOCK_MODEL_DB_QUOTE_H
#define STOCK_MODEL_DB_QUOTE_H

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

	
	//��ȡ����
	//��Ҫÿ�δ����ӡ��ر����ӡ�����Query
	int GetQuote(int period = 20, std::function<void(const wchar_t *, int)> func = nullptr)
	{
		Db connection;
		Query query = connection.CreateQuery("select QuoteTime, Open, High, Low, Close, Volume, Amount from Quote where id = ? order by QuoteTime");
		Quote quote;
		for (int i = 0; i < g_stock.Data.size(); i++)
		{
			sqlite3_bind_text(query.stmt_.get(), 1, g_stock.Data[i].Id, 9, SQLITE_TRANSIENT);
			do 
			{
				sqlite3_reset(query.stmt_.get());
				quote.QuoteTime = sqlite3_column_int(query.stmt_.get(), 1);
				quote.Open = sqlite3_column_double(query.stmt_.get(), 2);
				quote.Low = sqlite3_column_double(query.stmt_.get(), 3);
				quote.High = sqlite3_column_double(query.stmt_.get(), 4);
				quote.Close = sqlite3_column_double(query.stmt_.get(), 5);
				quote.Volume = sqlite3_column_double(query.stmt_.get(), 6);
				quote.Amount = sqlite3_column_double(query.stmt_.get(), 7);
				g_stock.Data[i].QuoteSet.push_back(quote);
			} while (sqlite3_step(query.stmt_.get()) == SQLITE_ROW);
			if ((i%period) == 0 && (func != nullptr))
				func(nullptr, i * 100 / g_stock.Data.size() + 1);	
			return g_stock.Data.size();
		}		
	}
	//���ص���ΪΪnullptr������T *���ؽ��
	//���ص�Ϊnullptr����ֱ����T�����ã��ڲ����з��ؽ��
	//�������ƿ��Գ���һ��...
	void GetSavedDate(unsigned long &start,unsigned long &end, std::function<void(const char *, int)> func = nullptr)
	{
		//��ȡ����ָ�룬�ֱ�ָ��̬�ı���
		//unsigned long start = get_start_date();
		//unsigned long end = get_end_date();
		
		sqlite3 *default_db = nullptr;
		int rc = sqlite3_open_v2(Db::default_path()->c_str(), &default_db, SQLITE_OPEN_READWRITE, nullptr);
	
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

	void delete_all()
	{
		Db connection_;
		try
		{
			auto cmd = connection_.CreateQuery("delete from quote");
			cmd.ExcuteNonQuery();
			cmd.Reset("delete from stock");
			cmd.ExcuteNonQuery();
		}
		catch (DbException e)
		{
			e.what();
		}
		g_stock.Data.clear();
		g_stock.BeginDate = 0;
		g_stock.EndDate = 0;
	}

	//��������quote
	//����ص�����,�ص�������Ƶ��(N���㱨һ�Σ�N%�㱨һ��)
	template <typename T>
	//typedef const std::enable_if<std::is_base_of<std::iterator, T>::value, T>::type 
	size_t bulk_insert(T _begin, T _end, int totals = -1, int period = 2000, std::function<void(const wchar_t *, int)> func = nullptr)
	{
		//������static assert,�����������õ�ʱ��������ָ��ṹ��ָ�룬����ͨ������
		//��ͬ����ʾ�����ܱ�������Ϸ���ʵ�ʺ���...������enable_if���Ƶ�����
		static_assert(std::is_same<parse_of_dad, std::iterator_traits<T>::value_type>::value, "Incorrect usage!");
		
		//�����1.����һ��command

		//1.ȷ�����ݿ�򿪣�
		Db connection_;
		sqlite3 *default_db=connection_.connection_.get(); //ʹ��Api����quote

		//2.����sql
		sqlite3_stmt *pStmt = NULL;
		const char *insert_sql = "INSERT OR IGNORE INTO QUOTE VALUES(?, ?, ?, ?, ?, ?, ?,?)";

		//3.prepare
		int rc = sqlite3_prepare_v2(default_db, insert_sql, -1, &pStmt, 0);
		if (rc)
		{
			auto p = sqlite3_errmsg16(default_db);
			func((wchar_t *)p, 0);
			if (pStmt) sqlite3_finalize(pStmt);
			if (default_db)
				sqlite3_close_v2(default_db);
			throw DbException(default_db);
			return 0;
		}//��ʱ�Ѿ�prepare�ɹ�

		//4.��������
		//�������Ҫ needCommit = sqlite3_get_autocommit(defult_db);
		int needCommit = 1;
		if (needCommit) sqlite3_exec(default_db, "BEGIN", 0, 0, 0);//��������
		
		//5.ѭ������
		int insert_nums = 0;
		//command����L""
		auto cmd = connection_.CreateQuery("INSERT OR REPLACE INTO Stock(Id,Market,Catalog,Title,MiniCode) VALUES(?,?,?,?,?)"); //���ӻ���´��������
		int idNumber = 0;
		id_of_dad *oldId = nullptr;
		StockInfo stock;
		for (auto current = _begin; current != _end; ++current)
		{
			
			//�������仯
			if (oldId==nullptr || (oldId!=nullptr && oldId!=current->idOfDad))
			{
				oldId = current->idOfDad;
				//������˴���
				if (g_stock.GetCatalog(current->idOfDad->id) == -1) //����ʶ��
					continue;//����������Ͳ���ʶ�𣬼��Թ��������еĲ�����
				
				//ֻ�д�������ʶ�𣬲����ڴ�������������
				idNumber = g_stock.FindStock(current->idOfDad->id);
				

				if (idNumber < 0 || strcmp(g_stock.Data[idNumber].Title,current->idOfDad->title)!=0) //���벻���ڻ�������ڵ����Ƹ���
				{
					memcpy(stock.Id,current->idOfDad->id,9);
					memcpy(stock.Title,current->idOfDad->title,9);
					
					//���������ĸ��д����Ҫ�޸�
					memset(stock.MiniCode, '\0', 5); //ȫ����Ϊ0
					g_stock.GetMiniCode(stock.Title, stock.MiniCode);

					//����������ӵĹ�Ʊ����ʱ����Ҫ�޸Ľ�����������
					if (idNumber < 0)
					{
						stock.Market= g_stock.GetMarket(stock.Id);
						stock.Catalog = g_stock.GetCatalog(stock.Id);

						//�µĹ�ƱӦ��˳���������
						g_stock.Data.insert(g_stock.Data.begin()- (1 * idNumber + 1), stock);
					}
					else //��������Ǳ�����£��޸ļ���
					{
						memcpy(g_stock.Data[idNumber].Title, stock.Title, 9);
						
						memcpy(g_stock.Data[idNumber].MiniCode, stock.MiniCode, 5);
					}

					//��Ҫinsert��update
					cmd. Bind(1, stock.Id, stock.Market, stock.Catalog, stock.Title, stock.MiniCode);
					cmd.ExcuteNonQuery();
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
				auto p = sqlite3_errmsg16(default_db);
				func((wchar_t *)p, 0);
				if (default_db)
					sqlite3_close_v2(default_db);
				throw DbException(default_db);
				return 0;
			}
			sqlite3_reset(pStmt); //���ã��´�ѭ������ִ��
			if ((insert_nums%period) == 0 && (func != nullptr))
				func(nullptr, (totals == -1) ? insert_nums : (insert_nums*100 / totals + 1));
			++insert_nums;			
		}
		//5.�ύ����
		
		if (needCommit) sqlite3_exec(default_db, "COMMIT", 0, 0, 0);

		//6.�����ֳ�
		sqlite3_finalize(pStmt);
		//sqlite3_close_v2(default_db);

		//7.���ز��������
		if (func != nullptr)
			func(nullptr, (totals == -1) ? insert_nums : 100);
		return insert_nums;
	}


	//�Ƚ��������ߵĴ�С,����С�ڼ���
	//ע��ȽϺ������Ǿ�̬��
	static bool compare_quote(Quote first,Quote second)
	{
		return first.QuoteTime < second.QuoteTime;
	}
	//��Ȩ�㷨
	//������������ʾ�ҵ�������ֵΪ���
	//���ظ�������ʾδ�ҵ�������ֵΪ��һ�����ڸ����ڵ�������ţ�Ҳ���ǵǼ��ա�
	//���-1��Ϊǰ��Ȩ��Ҫ����ĵط���
	int GetExOrder(vector<Quote> &quotes,int _date)
	{
		Quote quote;
		quote.QuoteTime = _date;
		std::pair<std::vector<Quote>::iterator, std::vector<Quote>::iterator> bounds;
		bounds = std::equal_range(quotes.begin(), quotes.end(), quote,DbQuote::compare_quote);//����
		//if (bounds.first == bounds.second)//���Ǹ�������
		//	return -1 - std::distance(quotes.begin(), bounds.first); //û���ҵ�,���ظ�ֵ����ʾ�����λ�ã�ע�⣬��beginΪ0���򷵻�-1
		return (std::distance(quotes.begin(), bounds.first)); //�����ҵ��ĵ�һ�����һ���ľ��룬Ҳ�������
	}

	//����ŷŵ�ȨϢ����
	void GetExOrder(vector<Quote> &quotes, vector<ExRight> &exrights)
	{
		for (auto &exright : exrights)
		{
			exright.Start = GetExOrder(quotes, exright.QuoteTime);
		}
	}
		
	//float BackFactor(vector<Quote> &quotes, ExRight &exright)
	//{
	//  return	(quotes[exright.Start-1].Close*(1.0f + exright.AddStock + exright.BuyStock)) / (quotes[exright.Start-1].Close - exright.Bonus + exright.BuyStock*exright.Price);
	//}

	//wind��Ѷ���ǵ�����Ȩ�㷨��
	//�����Ȩ���ӣ����һ����Ȩ��Ϣ
	//ǰ��Ȩ����Ȩ��۸�[(��Ȩǰ�۸� - �ֽ����)����(��)�ɼ۸����ͨ�ɷݱ䶯����]��(1����ͨ�ɷݱ䶯����)
	float CaculateFactor(vector<Quote> &quotes, ExRight &exright)
	{
		float last_close_price = quotes[exright.Start - 1].Close; //ǰ���̼۸�


		//����ǰ���̣�����ǰ��Ȩ�㷨Ӧ��Ϊ����
		float adjusted_price = (last_close_price - exright.Bonus + exright.Price*(exright.BuyStock)) / (1.0f + exright.BuyStock + exright.AddStock);


		return last_close_price / adjusted_price; //���ذ���Ȩ�����ǰ���̼�����ʵǰ���̼۵ı�
		//����һ�£�ǰ���̼۳������ֵ���õ����Ǹ�Ȩ��

	}

	void CaculateFactor(vector<Quote> &quotes, vector<ExRight> &exrights)
	{
		float max_factor=-1;
		for (unsigned int i = 0; i < exrights.size(); i++)
		{
			exrights[i].Factor = CaculateFactor(quotes, exrights[i]);
			exrights[i].VolumeFactor = 1 + exrights[i].AddStock + exrights[i].BuyStock;
			if (i>0)
			{
				exrights[i].Factor = exrights[i].Factor *exrights[i - 1].Factor;
				exrights[i].VolumeFactor = exrights[i].VolumeFactor *exrights[i - 1].VolumeFactor;
			}
		}
	}

	//�ǵ�����Ȩ
	void AdjustPrice(vector<Quote> &source, vector<ExRight> &exrights)
	{
		GetExOrder(source, exrights);//�������
		CaculateFactor(source, exrights);
		int start=0;
		for (unsigned int i = 0;i<exrights.size();i++)
		{
			start = (i>exrights.size() - 1) ? exrights[i - 1].Start : 0;
			for ( int j = exrights[i].Start - 1; j >= start; j--)
			{
				source[j].Open = source[j].Open / exrights[i].Factor;
				source[j].High = source[j].High / exrights[i].Factor;
				source[j].Low = source[j].Low / exrights[i].Factor;
				source[j].Close = source[j].Close / exrights[i].Factor;
				source[j].Volume = source[j].Volume*exrights[i].VolumeFactor;
			}
		}
	}
};


#endif
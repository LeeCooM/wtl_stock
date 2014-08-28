//ͷ�ļ����ݿ������
#include "global.h"
#include "sqlite/sqlite3.h"
#include "db.h"
#include "dad_parse_iterator.h"
#include <vector>

#ifndef db_code_h
#define db_code_h

//Global���а����������ÿ������԰���ȨϢ���������ݣ��ڵ�һ��ʹ��ʱ���������ʱһ��������
//���ڲ������ݺ�ȨϢ�������岻��1M��Ҳ�ɿ�����Ϊ������Blob�ֶ�һ�����룬���ܻ����
struct code
{
	char id[9];
	char title[9];
	char index[5]; //��ƴ
	//1.��������ȡ����ļ�ƴ���г���������������
	//2.���ȨϢ���������ݵ�vector����Ҫʱ���룬���ʼ���������𲽵ĺ�̨����
};

class DbCode
{
public:
	DbCode() = default;
	~DbCode() = default;

	//��ȡȫ����Ʊ������Ϣ��������global::StockSet��
	//ʱ������Ӧ������ʱ����ÿֻ��Ʊ��ص����ߡ�ȨϢ���ݣ��������ϴ󣬸������ΰ�������
	//Ӧ�ó�������ʱ��Global::init����get_date_range����Ѱ�װ���ݵ���ֹ���ڣ������ô˷�����ô����
	//��װ��������ʱ���ɿ��Ƿ��´���StockSet����������ߣ���˳�����¶���
	//��ȡȫ�����ߣ���Ҫ2000-3000�ε����������ÿ��Լ2000�����ݣ�����Ϊ600��β�����
	static void GetAllCode(bool refresh=false)
	{
		if (DbCode::get_stock_list().size()>0)
		{
			if (!refresh)	return;
			else
				DbCode::get_stock_list().clear();
		}
		DbConnection connection;
		auto query = connection.get_command(L"select * from Stock order by Id");
		Stock stock;
		int market, catalog;
		while (query.Execute(stock.Id, market,catalog, stock.Title, stock.MiniCode))
		{
			stock.Market = (MarketEnum)market;
			stock.Catalog = (CatalogEnum)catalog;
			DbCode::get_stock_list().push_back(stock);
		}
		
	}

	static vector<Stock> &get_stock_list()
	{
		static vector<Stock> stock_list;
			return stock_list;
	}

	//����ĳֻ��Ʊ���룺�����ڴ����Ƿ��С���������ж��Ƿ�һ�£��پ����Ƿ����
	//���û�У�insert������У������Ʋ�һ�£���update�����в������޸��ڴ��еĴ�����ٸ������ݿ�
	//���ڴ�������...Ӧ��find�����û�м��ҵ�����λ�á�
	//�ڴ��а��մ���˳�����С�
	void Update(const char *id_)
	{

	}
	//static vector<Stock> StockList;
};

#endif
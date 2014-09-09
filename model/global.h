#pragma once
#include <string>
#include "stock_map.h"

#ifdef MODEL_EXPORTS
#define MODEL_API __declspec(dllexport)
#else
#define MODEL_API __declspec(dllimport)
#endif
#include <memory>
#include <vector>
#include <string>  
#include <hash_map>

#ifndef global_h
#define global_h

using namespace std;

//��Ʊ�г�Enum
enum MarketEnum
{
	ShangHai = 0,
	ShenZhen = 1,
	QiHuo = 2
};

//��Ʊ����Enum
enum CatalogEnum
{
	All = 0, //���У�ѡ���ʱ��ʹ�á�
	Ashares = 1,	//A��
	SmallAndMedium = 2, //������С��
	Growth = 3, //���ڴ�ҵ��
	Index = 4, //ָ����ֻ������֤����֤����֤180��ָֻ������������
	Bshares = 5, //B��
	Fund = 6, //����
	Bond = 7, //ծȯ
	Futures = 8, //�ڻ�
	Warrant = 9, //Ȩ֤
	Others = 10, //����	
};


//���һֻ��Ʊ�Ĵ�����Ϣ��ȨϢ���ݺ�����
//����б�Ҫ������һ������ָ�����������������������
struct Stock
{
	char Id[9]; //���룬8λ+0
	enum MarketEnum Market; //��Ʊ������г�
	enum CatalogEnum Catalog;  //��Ʊ����
	char Title[9]; //��Ʊ���ƣ�4������+0
	char MiniCode[5]; //ƴ������	

	bool ExRightLoaded = false; //ȨϢ�����Ƿ�����,��������
	bool QuoteLoaded = false; //�����Ƿ�����
	vector<Quote> QuoteSet;
	vector<ExRight> ExRightSet;


	//����С�ں͵��ڲ������������㷨��Ҫ
	bool operator < (const Stock& a) const
	{
		return (strcmp(Id,a.Id)<0);
	}
	bool operator == (const Stock& a)const
	{
		return (strcmp(Id, a.Id)==0);
	}

	//ǰ60�����ߡ���͡����ۡ�����
	//ǰ20�����ߡ���͡�����
	//���߷ֶ�
	//���߷ֶ�
};

namespace global
{
	MODEL_API bool create_default_database(const char  *str); //����Ĭ�����ݿ�
	MODEL_API const char * GetDefaultDb(); //��ȡĬ�����ݿ�·��

	extern MODEL_API unsigned long begin_date;
	extern MODEL_API unsigned long end_date; //�ѱ������ݵ���ֹ����
	extern MODEL_API vector<Stock> StockSet;
	extern MODEL_API StockMap g_stock;
};
#endif
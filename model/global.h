#pragma once
#include <string>

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

//���߽ṹ
struct Quote
{
	unsigned long QuoteTime; //ʹ��_mktime32תΪ__time32t�洢��ʹ��_localtime32��ȡ
	float Open, High, Low, Close, Volume, Amount;
};

//ȨϢ��ṹ
struct ExRight
{
	int QuoteTime;
	float AddStock, BuyStock, Price, Bonus;
};


//���һֻ��Ʊ�Ĵ�����Ϣ��ȨϢ���ݺ�����
//����б�Ҫ������һ������ָ�����������������������
struct Stock
{
	char Id[9]; //����
	enum MarketEnum Market; //��Ʊ������г�
	enum CatalogEnum Catalog;  //��Ʊ����
	char Title[9]; //��Ʊ���ƣ�10���ֽ�
	char MiniCode[5]; //ƴ������	

	bool ExRightLoaded = false; //ȨϢ�����Ƿ�����,��������
	bool QuoteLoaded = false; //�����Ƿ�����
	vector<Quote> QuoteSet;
	vector<ExRight> ExRightSet;


	//����С�ں͵��ڲ������������㷨��Ҫ
	bool operator < (const Stock& a) const
	{
		return (Id <a.Id);
	}
	bool operator == (const Stock& a)const
	{
		return (Id == a.Id);
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
};
#endif
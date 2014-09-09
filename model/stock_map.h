
#include <vector>
#include <map>
#ifndef STOCK_MODEL_STOCKAPI_H
#define STOCK_MODEL_STOCKAPI_H
#pragma once

//���߽ṹ
struct Quote
{
	unsigned long QuoteTime; //ʹ��_mktime32תΪ__time32t�洢��ʹ��_localtime32��ȡ
	float Open, High, Low, Close, Volume, Amount;
};
struct ExRight
{
	int QuoteTime;
	float AddStock, BuyStock, Price, Bonus;
};

struct StockInfo
{
	//������벿��
	char Id[9];
	short Market; //��Ʊ������г�
	short Catalog;  //��Ʊ����
	char Title[9]; //��Ʊ���ƣ�4������+0
	char MiniCode[5]; //ƴ������	

	//������벿��
	size_t TotalCirculation; //�ܹɱ�
	size_t Circulation; //��ͨ��
	float EarningsPerShare; //ÿ������earnings per share

	//ȨϢ���벿��
	std::vector<ExRight> ExRightSet;

	//����
	std::vector<Quote> QuoteSet;
};
class StockMap
{
public:
	StockMap();
	~StockMap();

	std::vector<StockInfo> Data;
	unsigned long BeginDate = 0, EndDate = 0;

	void SetConnection(); //�������Ӻ�cmd��������������
	//����ʱ��������
	void Load();

	//��������
	void LoadQuote(const char *id);

	//����������޸ģ�����������룬���浽���ݿ����д���
	void UpdateStock(const char *id,const char *title); //��������id�����ڣ����롣������������Ʋ�ͬ���޸����ơ��������������δ�䣬������
	void UpdateBase(const char *id, size_t toatal_circulation,size_t circulation, float earnings_per_share);//�����3�������������������¡����벻���ڲ�������
	void UpdateExright();//ȨϢ��������������ա����һ�յ�������Ȳ�������������¡�
	
};

#endif
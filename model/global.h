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

#ifndef global_h
#define global_h

using namespace std;

////��Ʊ�г�Enum
//enum MarketEnum
//{
//	ShangHai = 0,
//	ShenZhen = 1,
//	QiHuo = 2
//};
//
////��Ʊ����Enum
//enum CatalogEnum
//{
//	All = 0, //���У�ѡ���ʱ��ʹ�á�
//	Ashares = 1,	//A��
//	SmallAndMedium = 2, //������С��
//	Growth = 3, //���ڴ�ҵ��
//	Index = 4, //ָ����ֻ������֤����֤����֤180��ָֻ������������
//	Bshares = 5, //B��
//	Fund = 6, //����
//	Bond = 7, //ծȯ
//	Futures = 8, //�ڻ�
//	Warrant = 9, //Ȩ֤
//	Others = 10, //����	
//};



	MODEL_API bool create_default_database(const char  *str); //����Ĭ�����ݿ�
	MODEL_API const char * GetDefaultDb(); //��ȡĬ�����ݿ�·��

	extern MODEL_API StockMap g_stock;
#endif
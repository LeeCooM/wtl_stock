#pragma once
#include <algorithm>
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
	size_t bulk_insert(T begin, T end, sqlite3_stmt *pStmt)
	{
		//������static assert,�����������õ�ʱ��������ָ��ṹ��ָ�룬����ͨ������
		//��ͬ����ʾ�����ܱ�������Ϸ���ʵ�ʺ���...������enable_if���Ƶ�����
		static_assert(std::is_same<parse_of_dad, std::iterator_traits<T>::value_type>::value, "Incorrect usage!");

		//1.����sql
		//2.prepare
		//3.��������
		//4.ѭ������
		for (auto current = begin; current != end; ++current)
		{
			
		}
		//5.�ύ����
		//6.���ز��������
	}
};



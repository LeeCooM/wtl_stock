#include <functional>
#include "global.h"
#include "dad_file_parse.h"
#include "db_quote.h"
#ifndef STOCK_MODEL_IMPORT_MODEL_BASE_H
#define STOCK_MODEL_IMPORT_MODEL_BASE_H

//�������ݵ�model�����
//����ӿ���
class ImportModelBase
{
public:
	virtual void GetSavedDate(unsigned long &_start, unsigned long &_end)=0; //��ʼ������ȡ�Ѿ���װ����
	virtual bool CheckSourceFile(const wchar_t *_file, unsigned long &_start, unsigned long &_end)=0; //����ļ��Ƿ�Ϸ�
	virtual void ImportFile(std::function<void(const char *, int)> func)=0;
};

class ImportModelQuote:public ImportModelBase
{
public:
	DbQuote quote_;
	dad_file_parse parser_;

	virtual void GetSavedDate(unsigned long &_start, unsigned long &_end)
	{
		quote_.GetSavedDate(global::begin_date, global::end_date);
		_start = global::begin_date;
		_end = global::end_date;
	}
	virtual bool CheckSourceFile(const wchar_t *_file, unsigned long &_start, unsigned long &_end) //����ļ��Ƿ�Ϸ�
	{
		_start = 0;
		_end = 0;
		if (parser_.check(_file))
		{
			_start = parser_.m_start_date;
			_end = parser_.m_end_date;
			return true;
		}
		return false;
	}
	virtual void ImportFile(std::function<void(const char *, int)> func)
	{

	}
private:
	//�������ڻ��ʱ���ַ���
};

class ImportModelExright :public ImportModelBase
{
public:
	virtual void Init() //��ʼ������ȡ�Ѿ���װ����
	{

	}
	virtual bool CheckSourceFile(const wchar_t *_file) //����ļ��Ƿ�Ϸ�
	{

	}
	virtual void ImportFile(std::function<void(const char *, int)> func)
	{

	}

};

#endif
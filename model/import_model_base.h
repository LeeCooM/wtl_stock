#include <functional>
#include <sstream>
#include "global.h"
#include "dad_file_parse.h"
#include "db_quote.h"
#include "timer.h"
#ifndef STOCK_MODEL_IMPORT_MODEL_BASE_H
#define STOCK_MODEL_IMPORT_MODEL_BASE_H

//�������ݵ�model�����
//����ӿ���
class ImportModelBase
{
public:
	virtual void GetSavedDate(unsigned long &_start, unsigned long &_end)=0; //��ʼ������ȡ�Ѿ���װ����
	virtual bool CheckSourceFile(const wchar_t *_file, unsigned long &_start, unsigned long &_end, unsigned long &_count) = 0; //����ļ��Ƿ�Ϸ�
	virtual void ImportFile(const wchar_t *_file, std::function<void(const wchar_t *, int)> func) = 0;
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
	virtual bool CheckSourceFile(const wchar_t *_file, unsigned long &_start, unsigned long &_end, unsigned long &_count) //����ļ��Ƿ�Ϸ�
	{
		_start = 0;
		_end = 0;
		if (parser_.check(_file))
		{
			_start = parser_.m_start_date;
			_end = parser_.m_end_date;
			_count = parser_.m_quote_count;
			return true;
		}
		return false;
	}
	virtual void ImportFile(const wchar_t *_file,std::function<void(const wchar_t *, int)> func)
	{
		timer time_used; //��ʼ��ʱ
		time_used.reset();

		parser_.open(_file);

		wostringstream ss;
		ss << L"��" << parser_.m_quote_count << L"����¼�����ڰ�װ...";
		func(ss.str().c_str(), 0); 

		int inserted=quote_.bulk_insert(parser_.begin(), parser_.end(),parser_.m_quote_count, 2000, func);
		parser_.close(); //���������ִ̨�У�������˳��ִ�У��������رտ���

		ss.clear();
		ss << L"���,��ʱ";
		auto used = time_used.elapsed_seconds();
		if (used <= 0)
			ss << L"����1��!";
		else
			if (used >= 60)
				ss << used / 60 << L"��" << used % 60 << L"��!";
			else
				ss << used << L"��!";
		func(ss.str().c_str(), 100);
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
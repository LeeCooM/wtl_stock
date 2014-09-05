#include <functional>
#include <sstream>
#include "global.h"
#include "dad_file_parse.h"
#include "db_quote.h"
#include "timer.h"
#include "stdmore.h"
#ifndef STOCK_MODEL_IMPORT_MODEL_BASE_H
#define STOCK_MODEL_IMPORT_MODEL_BASE_H

//�������ݵ�model�����
//����ӿ���
class ImportModelBase
{
public:
	virtual void GetSavedInfo(wstring &_saved,bool _fetch=false)=0; //��ʼ������ȡ�Ѿ���װ����
	virtual void UpdateSavedInfo(wstring &_saved) = 0; //�Ѿ���װ���ݱ仯
	virtual bool CheckSourceFile(const wchar_t *_file, wstring &_selected) = 0; //����ļ��Ƿ�Ϸ�,�����ļ���˵��
	virtual void ImportFile(const wchar_t *_file, std::function<void(const wchar_t *, int)> func) = 0;
};

class ImportModelQuote:public ImportModelBase
{
public:
	DbQuote quote_;
	dad_file_parse parser_;

	virtual void GetSavedInfo(wstring &_saved, bool _fetch = false)
	{
		if (_fetch)
		  quote_.GetSavedDate(global::begin_date, global::end_date);
		if (global::begin_date == 0 || global::begin_date == 0)
			_saved = L"�Ѿ���װ��û������";
		else
		{
			_saved.clear();
			_saved= L"�Ѿ���װ��";
			stdmore::time_to_wstring((time_t)global::begin_date, L"%Y-%m-%d", _saved);
			_saved += L"��";
			stdmore::time_to_wstring((time_t)global::end_date, L"%Y-%m-%d", _saved);
		}
	}

	virtual void UpdateSavedInfo(wstring &_saved)
	{
		if (global::begin_date == 0)
		{
			if (parser_.m_start_date != 0)
				global::begin_date = parser_.m_start_date;
		}
		else
		{
			if (parser_.m_start_date != 0 && parser_.m_start_date< global::begin_date)
				global::begin_date = parser_.m_start_date;
		}

		if (global::end_date == 0)
		{
			if (parser_.m_end_date != 0)
				global::end_date = parser_.m_end_date;
		}
		else
		{
			if (parser_.m_end_date != 0 && parser_.m_end_date> global::end_date)
				global::end_date = parser_.m_end_date;
		}
		GetSavedInfo(_saved);
	}

	virtual bool CheckSourceFile(const wchar_t *_file, wstring &_selected) //����ļ��Ƿ�Ϸ�
	{
		if (parser_.check(_file))
		{
			_selected = L" ";
			_selected = L"׼����װ��";
			stdmore::time_to_wstring((time_t)parser_.m_start_date, L"%Y-%m-%d", _selected);
			_selected += L"��";
			stdmore::time_to_wstring((time_t)parser_.m_start_date, L"%Y-%m-%d", _selected);
			return true;
		}
		_selected = L"׼����װ����ѡ���ļ���ʽ����";
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
	virtual void GetSavedInfo(wstring &_saved, bool _fetch = false)  //��ʼ������ȡ�Ѿ���װ����
	{

	}
	virtual void UpdateSavedInfo(wstring &_saved)  //�Ѿ���װ���ݱ仯
	{
	
	}
	virtual bool CheckSourceFile(const wchar_t *_file, wstring &_selected)  //����ļ��Ƿ�Ϸ�,�����ļ���˵��
	{
	
	}
	virtual void ImportFile(const wchar_t *_file, std::function<void(const wchar_t *, int)> func) 
	{
	
	}
};

#endif
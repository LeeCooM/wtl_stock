#include <functional>

#ifndef STOCK_MODEL_IMPORT_MODEL_BASE_H
#define STOCK_MODEL_IMPORT_MODEL_BASE_H

//�������ݵ�model�����
//����ӿ���
class ImportModelBase
{
public:
	virtual void Init(); //��ʼ������ȡ�Ѿ���װ����
	virtual bool CheckSourceFile(const wchar_t *_file); //����ļ��Ƿ�Ϸ�
	virtual void ImportFile(std::function<void(const char *, int)> func);
};

class ImportModelQuote:public ImportModelBase
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
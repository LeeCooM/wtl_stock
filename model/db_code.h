//ͷ�ļ����ݿ������

#include "sqlite/sqlite3.h"
#include "db.h"
#include "dad_parse_iterator.h"
#include <vector>


#ifndef db_quote_h
#define db_quote_h

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

	//��ȡȫ����Ʊ����
	void GetAllCode()
	{

	}
	//����ĳֻ��Ʊ���룺�����ڴ����Ƿ��С���������ж��Ƿ�һ�£��پ����Ƿ����
	//���û�У�insert������У������Ʋ�һ�£���update�����в������޸��ڴ��еĴ�����ٸ������ݿ�
	//���ڴ�������...Ӧ��find�����û�м��ҵ�����λ�á�
	//�ڴ��а��մ���˳�����С�
	void Update(const char *id_)
	{

	}

};


#endif
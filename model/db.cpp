#include "stdafx.h"
#define MODEL_EXPORTS
#include "db.h"
#include <stdlib.h>
#include <stdio.h>

std::string db::default_="";

bool db::set_default(const char *_default, std::function<bool(const char *)> create_database)
{
	

	char exeFullPath[MAX_PATH]; // MAX_PATH��WINDEF.h�ж����ˣ�����260
	memset(exeFullPath, 0, MAX_PATH);

	GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
	char *p = strrchr(exeFullPath, '\\');
	*p = 0x00;

	default_.append(exeFullPath);
	default_.append(_default);
	//default_ =  _default;

	//�ж��ļ��Ƿ���ڣ�fstream�ķ�����Ȼ�Ǵ��Ƿ�����
	//���ֱ���Թ��������ݿ��Ƿ������򿪣����жϡ�
	if (!is_exist(default_.c_str()))
	{
		if (create_database != nullptr)
			return create_database(default_.c_str());
		return false;
	}
	return true;
};

bool db::set_default(const wchar_t *_default, std::function<bool(const char *)> create_database)
{
	//����һ��ת����
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	auto temp=conv.to_bytes(_default);//���������ת��:conv.from_bytes(narrowStr);
    auto rc = set_default(temp.c_str(), create_database);

	//defaultָ��ָ��temp.c_str()
	//Ȼ��temp��ʧ
	//Ȼ��default_ָ�������û��
	//Ȼ��default_������

	
	return rc;
}
const char *db::default()
{
	return default_.c_str();
}

//ע�⣬��ʹ����һ��0�ֽڵ�ͬ���ļ���checkҲ�᷵��false
bool db::is_exist(const char *_default)
{
	sqlite3 *pdb = NULL;
	int rc = 0;
	rc = sqlite3_open_v2(_default, &pdb, SQLITE_OPEN_READWRITE, NULL);
	if (pdb)
		sqlite3_close_v2(pdb);
	return (rc == SQLITE_OK);
}

#include <string>
#include "QuoteView.h"

#ifndef STOCK_STOCK_IMPORT_VIEW_MODEL_H
#define STOCK_STOCK_IMPORT_VIEW_MODEL_H
class QuoteViewModel
{
public:
	CQuoteView *view_;
	//BaseModel *model;

	std::wstring install_data_=L"";
	std::wstring selected_data_=L"";
	std::wstring selected_file_ = L"";
	std::wstring progress_info_ = L"";

	bool is_file_validate = false;
	int progress_total=100, progress_now=0;
	int import_catalog = 0;
	int ui_state = 0;

	//��ʼ������View��InitDialog����
	void Init()
	{

	}

	//ѡ���ļ��������Ϸ���
	void SelectFile()
	{

	}

	//��װ����ʹ�ûص����½����ı��ؼ��ͽ�����
	void ImportFile(std::function<void(const char *, int)> func)
	{

	}

	//�ı䵼���ļ������ͣ���״̬��Ϊ0
	void ChangeImportFileType()
	{

	}

	//�ı�View��״̬���ɴ��趨���ؼ��Ŀ����ԺͿɼ���
	void set_ui_state()
	{

	}

	//ɾ��ȫ�����ߺʹ����
	void RemoveInstalledQuote()
	{

	}
};
#endif
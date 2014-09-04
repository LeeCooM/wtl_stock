#include "stdafx.h"
#include "import_model_base.h"
#include "import_view.h"
#include "import_view_model.h"


ImportViewModel::ImportViewModel(ImportView *_view)
{
	view_ = _view;
}

void ImportViewModel::set_installed_info(unsigned long _begin, unsigned long _end) //��ʼ������ȡ�Ѿ���װ����
{
	if (_begin == 0 || _end == 0)
		installed_info_ = L"�Ѿ���װ��û������";
	else
	{
		installed_info_.clear();
		installed_info_ = L"�Ѿ���װ��";
		stdmore::time_to_wstring((time_t)_begin, L"%Y-%m-%d", installed_info_);
		installed_info_ += L"��";
		stdmore::time_to_wstring((time_t)_end, L"%Y-%m-%d", installed_info_);
	}
}
//��ʼ������View��InitDialog����
//��import_catalog��Ϊ0��ͬʱ����model����
void ImportViewModel::Init()
{
	switch (this->import_catalog_)
	{
	case 0:
		model_.reset(new ImportModelQuote);
		break;
	case 1:
		model_.reset(new ImportModelQuote);
		break;
	default:
		model_.reset(new ImportModelQuote);
		break;
	}
	unsigned long start_date = 0;
	unsigned long end_date = 0;
	model_->GetSavedDate(start_date,end_date);
	set_installed_info(start_date, end_date);//��ȡ"�Ѿ���װ����"
	view_->DoDataExchange(false, IDC_STATIC_SAVED); //�����ݵ�UI,false��ʾ�Ƿ��ȡui����

	set_ui_state(0);//״̬��Ϊ0
}

//ѡ���ļ��������Ϸ���
void ImportViewModel::SelectFile()
{

}
//��װ����ʹ�ûص����½����ı��ؼ��ͽ�����
void ImportViewModel::ImportFile(std::function<void(const char *, int)> func)
{

}

//�ı䵼���ļ������ͣ���״̬��Ϊ0
void ImportViewModel::ChangeImportFileType()
{

}

//�ı�View��״̬���ɴ��趨���ؼ��Ŀ����ԺͿɼ���
void ImportViewModel::set_ui_state(int _ui_state) 
{
	ui_state_ = _ui_state;
}

//ɾ��ȫ�����ߺʹ����
void ImportViewModel::RemoveInstalledQuote()
{

}
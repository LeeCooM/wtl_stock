#include "stdafx.h"
#include "import_model_base.h"
#include "import_view.h"
#include "import_view_model.h"
#include "ui_state.h"


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
	switch (_ui_state)
	{
	case 0:
		::SetEnable(view_, false, IDC_BUTTON_INSTALL);
		::SetEnable(view_, true, IDC_BUTTON_SELECT, IDC_EDIT_PATH, IDC_BUTTON_REMOVE, IDC_RADIO_QUOTE, IDC_RADIO_EXRIGHT, IDC_RADIO_BASE, IDC_RADIO_CATALOG);

		::SetVisble((CWindow *)view_, false, IDC_PROGRESS_IMPORT);
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;


	}
}

//ɾ��ȫ�����ߺʹ����
void ImportViewModel::RemoveInstalledQuote()
{

}
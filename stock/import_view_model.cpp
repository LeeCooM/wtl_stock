#include "stdafx.h"
#include "import_model_base.h"
#include "import_view.h"
#include "import_view_model.h"
#include "ui_state.h"

ImportViewModel::ImportViewModel(ImportView *_view)
{
	view_ = _view;
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

	model_->GetSavedInfo(installed_info_,true);
	view_->DoDataExchange(false, IDC_STATIC_SAVED); //�����ݵ�UI,false��ʾ�Ƿ��ȡui����
	set_ui_state(0);//״̬��Ϊ0
}

void ImportViewModel::CheckFile()
{
	wchar_t temp[MAX_PATH] = { 0 };
	view_->GetDlgItemText(IDC_EDIT_PATH, temp, MAX_PATH);
	if (temp != selected_file_)  //�Ի���ѡ���ļ���Ԥ���Ѿ�������һ�£�ֻ�б༭��ſ��ܵ���
	{
		selected_file_.clear();
		selected_file_.append(temp);
		if (this->model_->CheckSourceFile(selected_file_.c_str(), selected_info_))
		{
			set_ui_state(1);
			progress_info_ = L" ";
		}
		else
		{
			set_ui_state(0);
		}

		view_->DoDataExchange(FALSE, IDC_STATIC_OPENED);
		view_->DoDataExchange(FALSE, IDC_STATIC_INFO);
	}
}
//ѡ���ļ��������Ϸ���
void ImportViewModel::SelectFile()
{
	std::wstring sDest;
	LPCTSTR pfileName = selected_file_.empty() ? NULL : selected_file_.c_str();
	CFileDialog dlg(TRUE, NULL, pfileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("�����ļ� (*.*)\0*.*\0Dad�ļ� (*.Dad)\0*.exe\0Dat�ļ� (*.dat)\0\0"),view_->m_hWnd);
	if (dlg.DoModal() == IDOK)
	{
		wchar_t temp[MAX_PATH] = { 0 };
		view_->GetDlgItemText(IDC_EDIT_PATH, temp, MAX_PATH); //ȡ�ñ༭���·��
		if (temp != dlg.m_szFileName)
			view_->SetDlgItemTextW(IDC_EDIT_PATH, dlg.m_szFileName);
		CheckFile();
	}
}

//��װ����ʹ�ûص����½����ı��ؼ��ͽ�����
void ImportViewModel::RunImportFile()
{
	set_ui_state(2); //�������У���������

	timer time_used;
	time_used.reset();

	std::thread *t;
	//thread�Ĳ���
	//1.&ImportModelBase::ImportFile ��ʾ�ĸ�����ĸ�����
	//2.model_.get() ��ʾָ���ĸ�������ָ�룬��һ��������ָ���ʵ��
	//3��4��Ϊ�����ṩ�Ĳ���
	t = new thread(&ImportModelBase::ImportFile,model_.get(),
		selected_file_.c_str(),
		[this, t, time_used](const wchar_t *info, int now){
		if (info)
		{
			view_->SetDlgItemTextW(IDC_STATIC_INFO, info);
		}
		if (::IsWindow(view_->m_progressBar.m_hWnd))
		{
			view_->m_progressBar.SetPos(now);
			if ((now == 100) && info) //���һ��,info����Ϊ��
			{
				//�˴����¼���
				//model.set_saved_string();
				
				set_ui_state(3); //��ɣ���������
				
				model_->UpdateSavedInfo(installed_info_);
				view_->SetDlgItemTextW(IDC_STATIC_SAVED, installed_info_.c_str());
			
				//model.parser.close(); //��װ��Ϻ���������Dad�ļ�
				ThreadTask::Remove(import_id_);
			}
		}
	});
	import_id_ = t->get_id();
	ThreadTask::Add(import_id_, L"��������");
	t->detach(); //�����̷߳����ִ��
}

//�ı䵼���ļ������ͣ���״̬��Ϊ0
void ImportViewModel::ChangeImportFileType()
{

}

//�ı�View��״̬���ɴ��趨���ؼ��Ŀ����ԺͿɼ���
void ImportViewModel::set_ui_state(int _ui_state) 
{
	if (ui_state_ == _ui_state && ui_state_!=0)
		return;
	ui_state_ = _ui_state;
	switch (_ui_state)
	{
	case 0:
		::SetEnable(view_, false, IDC_BUTTON_INSTALL);
		::SetEnable(view_, true, IDC_BUTTON_SELECT, IDC_EDIT_PATH, IDC_BUTTON_REMOVE, IDC_RADIO_QUOTE, IDC_RADIO_EXRIGHT, IDC_RADIO_BASE, IDC_RADIO_CATALOG);
		::SetVisble((CWindow *)view_, false, IDC_PROGRESS_IMPORT);
		if (::IsWindow(view_->m_progressBar.m_hWnd)) 
			view_->m_progressBar.SetPos(0);
		break;
	case 1:
		::SetEnable(view_, true, IDC_BUTTON_INSTALL, IDC_BUTTON_SELECT, IDC_EDIT_PATH, IDC_BUTTON_REMOVE, IDC_RADIO_QUOTE, IDC_RADIO_EXRIGHT, IDC_RADIO_BASE, IDC_RADIO_CATALOG);
		::SetVisble((CWindow *)view_, false, IDC_PROGRESS_IMPORT);
		break;
	case 2:
		::SetEnable(view_, false, IDC_BUTTON_INSTALL,IDC_BUTTON_SELECT, IDC_EDIT_PATH, IDC_BUTTON_REMOVE, IDC_RADIO_QUOTE, IDC_RADIO_EXRIGHT, IDC_RADIO_BASE, IDC_RADIO_CATALOG);
		::SetVisble((CWindow *)view_, true, IDC_PROGRESS_IMPORT);
		break;
	case 3:
		::SetEnable(view_, true, IDC_BUTTON_SELECT, IDC_EDIT_PATH, IDC_BUTTON_REMOVE, IDC_RADIO_QUOTE, IDC_RADIO_EXRIGHT, IDC_RADIO_BASE, IDC_RADIO_CATALOG);
		break;
	}
}

//ɾ��ȫ�����ߺʹ����
void ImportViewModel::RemoveInstalledQuote()
{

}
#include "stdafx.h"
#include "import_view.h"
#include "import_view_model.h"


ImportViewModel::ImportViewModel(ImportView *_view)
{
	view_ = _view;
}

//��ʼ������View��InitDialog����
void ImportViewModel::Init()
{

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
void ImportViewModel::set_ui_state()
{

}

//ɾ��ȫ�����ߺʹ����
void ImportViewModel::RemoveInstalledQuote()
{

}
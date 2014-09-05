#include <string>
#include <memory>
#include <thread>

#ifndef STOCK_STOCK_IMPORT_VIEW_MODEL_H
#define STOCK_STOCK_IMPORT_VIEW_MODEL_H
class ImportView;
class ImportModelBase;
class ImportViewModel
{
public:

	ImportViewModel(ImportView *_view=nullptr);
	ImportView *view_;
	std::shared_ptr<ImportModelBase> model_;

	std::wstring installed_info_=L"";
	std::wstring selected_info_=L"";
	std::wstring selected_file_ = L"";
	std::wstring progress_info_ = L"";
	unsigned long file_count_ = 0; //��Ҫ��

	int progress_total_=100, progress_now_=0;
	int import_catalog_= 0;
	int ui_state_ = 0;
	
	std::thread::id  import_id_;

	//��ʼ������View��InitDialog����
	void Init();

	//ѡ���ļ��������Ϸ���
	void SelectFile();

	void CheckFile();
	//��װ����ʹ�ûص����½����ı��ؼ��ͽ�����
	void ImportFile(std::function<void(const char *, int)> func);

	void RunImportFile();

	//�ı䵼���ļ������ͣ���״̬��Ϊ0
	void ChangeImportFileType(WORD wID);

	//�ı�View��״̬���ɴ��趨���ؼ��Ŀ����ԺͿɼ���
	void set_ui_state(int _ui_state);

	//ɾ��ȫ�����ߺʹ����
	void RemoveInstalledQuote();

	//��ȡ�Ѱ�װ�����ַ���
	void set_installed_info(unsigned long _begin, unsigned long _end);
};
#endif
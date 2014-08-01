// stockView.h : interface of the CStockView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CCtlColor.h"
#include "atlddxEx.h"
#include <atlctrls.h>
#include <string>
#include <functional>
#include <chrono>
#include <thread>
#include <atlddx.h>
#include "db_quote.h"
#include "dad_file_parse.h"

class CQuoteView;
class CQuoteViewModel
{
public:
	
	std::wstring m_path; //ѡȡ���ļ�·��
	bool working = FALSE; //�Ƿ����ڹ���
	bool isValidFile = FALSE; //ѡ�е��ļ��Ƿ�Ϸ�

	//������ص�ҵ����
	DbQuote quote;
	dad_file_parse parser;

	void import(std::function<void(const char *,int)> func)
	{
		if (!parser.open(m_path.c_str()))
		{
			MessageBox(0,_T("��ѡ�е��ļ�������Dad�����ļ�"),L"�ļ���ʽ����",0);
			return;
		}
		if (parser.m_quote_count <= 0)
		{
			MessageBox(0, _T("��ѡ�е��ļ���û����������"), L"�ļ���ʽ����", 0);
			return;
		}
		quote.bulk_insert(parser.begin(), parser.end(), 2000, func);
		working = True;
		//for (int i = 0; i < 100; i++)
		//{
		//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
		//	func(i);
		//	//Sleep(100);//win32д��
		//}

	}

	void open(HWND hwnd)
	{
		std::wstring sDest;
		//GETDLGITEMTEXTCSTRING(IDC_EDITSOURCE, sDest);
		LPCTSTR pfileName = m_path.empty() ? NULL : m_path.c_str();

		CFileDialog dlg(TRUE, NULL, pfileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			_T("�����ļ� (*.*)\0*.*\0Dad�ļ� (*.Dad)\0*.exe\0Dat�ļ� (*.dat)\0\0"));
		if (dlg.DoModal() == IDOK)
		{
			m_path = dlg.m_szFileName;
			//m_view->DoDataExchange(false);//�����ݽ�����
		}
	}
};

class CQuoteView : public CDialogImpl<CQuoteView>
	, public CWinDataExchangeEx<CQuoteView>
	, public CCtlColored<CQuoteView>
{
public:
	
	CQuoteViewModel model;
	CProgressBarCtrl m_progressBar;
	CEdit m_pathctrl;
	
	enum { IDD = IDD_QUOTE_BOX };

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	BEGIN_DDX_MAP(CQuoteBox)
		DDX_TEXT(IDC_EDIT_PATH, model.m_path)
		//DDX_TEXT(IDC_STATIC_ALLQUOTE, model.m_path)
		//DDX_TEXT(IDC_EDIT_PATH, model.m_path)
	END_DDX_MAP()
	//init_dialog��show��ֻ����һ�Ρ�
	BEGIN_MSG_MAP(CQuoteView)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
		//NOTIFY_ID_HANDLER(IDC_EDIT_PATH,OnEditChanged)
		COMMAND_ID_HANDLER(IDC_EDIT_PATH,OnEditChanged)

		COMMAND_HANDLER(IDC_BUTTON_SELECT, BN_CLICKED, OnClickedButtonSelect)
		COMMAND_HANDLER(IDC_BUTTON_INSTALL, BN_CLICKED, OnClickedButtonInstall)
		CHAIN_MSG_MAP(CCtlColored<CQuoteView>)

	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/){

		
		//��ȡ����ɫ
		HBRUSH bk = (HBRUSH)GetClassLong(GetParent(), GCL_HBRBACKGROUND);
		LOGBRUSH brush;
		::GetObjectW(bk, sizeof(LOGBRUSH), &brush);
		DWORD bkColor = brush.lbColor;

		//SetTextColor(::GetSysColor(COLOR_WINDOWTEXT)); 
		//SetBkBrush(bk);
		
		SetTextBackGround(bkColor);  // Lightish kind of blue (Four)
		SetTextColor(RGB(0, 0, 0)); // Dark red 
		


		model.m_path = L"";
		m_progressBar = GetDlgItem(IDC_PROGRESS_IMPORT);
		m_pathctrl.Attach(GetDlgItem(IDC_EDIT_PATH));
		DoDataExchange(FALSE);
		//DeleteObject(bk);//delete��Ȼ���¿�ܴ�����ɫ����ʧЧ��
		return TRUE;
	}

	LRESULT OnForwardMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&	bHandled)
	{
		LPMSG pMsg = (LPMSG)lParam;
		return PreTranslateMessage(pMsg);
	}

	virtual void OnFinalMessage(HWND /*hWnd*/)
	{
		delete this;
	}

	LRESULT OnClickedButtonSelect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		model.open(0);
		DoDataExchange(false, IDC_EDIT_PATH);//�����ݽ�����
		return 0;
	}

	LRESULT OnClickedButtonInstall(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{

		std::thread t(&CQuoteViewModel::import, &model, [this](const char *err,int now){
			if (err)
			{
				MessageBoxA(m_hWnd, err, "������̳���", 0);
			}
			else if (::IsWindow(m_progressBar.m_hWnd))
			{			    	
				m_progressBar.SetPos(now*100/model.parser.m_quote_count + 1);
				if (now == model.parser.m_quote_count)
					model.working = False;
			}
		});
		t.detach(); //�����̷߳����ִ��
		//t.join();//�ȴ����߳�ִ�������ִ����һ�����

		//model.import([this](int now){
		//	CProgressBarCtrl bar = GetDlgItem(IDC_PROGRESS_IMPORT);//ע�⣬������ʹ�ý�����
		//	bar.SetPos(now + 1);
		//});
		return 0;
	}
	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnEditChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
	//LRESULT OnEditChanged(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		//512=EN_KILLFOCUS WID=1004���ؼ���id��hwnd�ǿؼ���hwnd
		//switch (pnmh->code)
		//{
		//case EN_CHANGE:
		//	DoDataExchange(TRUE, IDC_EDIT_PATH); //����ı䣬���ı�Ľ��ת������
		//	break;
		//default:
		//	break;
		//}
		if (wNotifyCode == EN_KILLFOCUS)
			DoDataExchange(true, wID);
		return 0;
	}

};


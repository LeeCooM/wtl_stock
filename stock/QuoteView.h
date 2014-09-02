// stockView.h : interface of the CStockView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CCtlColor.h"
#include <atldlgs.h>
#include "atlddxEx.h"
#include <atlctrls.h>
#include <string>
#include <functional>
#include <chrono>
#include <thread>
#include <atlddx.h>
#include "db_quote.h"
#include <sstream>
#include "dad_file_parse.h"
#include "ThreadTask.h"
#include "timer.h"
#include "stdmore.h"
#include <iomanip>

class CQuoteView;
class CQuoteViewModel
{
public:
	enum class State
	{
		init,
		selected,
		pending,
		complete,
	};

	int install_type= 0;
	State m_state = State::init;
	std::wstring m_path; //ѡȡ���ļ�·��
	std::wstring m_info=L"";
	std::wstring m_saved = L"";
	std::wstring m_opened = L"";
	

	bool isValidFile = FALSE; //ѡ�е��ļ��Ƿ�Ϸ�

	//������ص�ҵ����
	DbQuote quote;
	dad_file_parse parser;

	void get_date_range()
	{
		quote.get_date_range(global::begin_date,global::end_date);
		set_saved_string();

	}
	void set_saved_string()
	{
		if (global::begin_date == 0)
		{
			if (parser.m_start_date != 0)
				global::begin_date = parser.m_start_date;
		}
		else
		{
			if (parser.m_start_date != 0 && parser.m_start_date< global::begin_date)
				global::begin_date = parser.m_start_date;
		}

		if (global::end_date == 0)
		{
			if (parser.m_end_date != 0)
				global::end_date = parser.m_end_date;
		}
		else
		{
			if (parser.m_end_date != 0 && parser.m_end_date> global::end_date)
				global::end_date = parser.m_end_date;
		}

		if (global::begin_date == 0 || global::end_date == 0)
			m_saved = L"�Ѿ���װ��û������";
		else
		{
			m_saved.clear();
			m_saved = L"�Ѿ���װ��";
			stdmore::time_to_wstring((time_t)global::begin_date, L"%Y-%m-%d", m_saved);
			m_saved += L"��";
			stdmore::time_to_wstring((time_t)global::end_date, L"%Y-%m-%d", m_saved);
			//m_saved += L"������";
		}
	}

	

	void import(std::function<void(const char *,int)> func)
	{	
		m_state = CQuoteViewModel::State::pending;
		parser.open(m_path.c_str());
		quote.bulk_insert(parser.begin(), parser.end(), 2000, func);
		
		

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
			_T("�����ļ� (*.*)\0*.*\0Dad�ļ� (*.Dad)\0*.exe\0Dat�ļ� (*.dat)\0\0"),hwnd);
		if (dlg.DoModal() == IDOK)
		{
			m_path = dlg.m_szFileName;			
			//m_view->DoDataExchange(false);//�����ݽ�����
		}
	}

	bool is_state_changed()
	{
		auto oldState = m_state;
		//����ļ��Ϸ�
		if (parser.check(m_path.c_str()))
		{
			m_state = CQuoteViewModel::State::selected;
			m_info = L" ";
			m_opened = L"׼����װ��";
			stdmore::time_to_wstring((time_t)parser.m_start_date, L"%Y-%m-%d",m_opened);
			m_opened += L"��";
			stdmore::time_to_wstring((time_t)parser.m_end_date, L"%Y-%m-%d", m_opened);
			//m_opened += L"������";
		}
		else
		{
			m_state = CQuoteViewModel::State::init;
			m_opened = L"׼����װ����ѡ���ļ���ʽ����";
		}
		return (m_state != oldState);
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
	std::thread::id  import_id;
	
	enum { IDD = IDD_QUOTE_BOX };

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	BEGIN_DDX_MAP(CQuoteBox)
		DDX_TEXT(IDC_EDIT_PATH, model.m_path)
		DDX_TEXT(IDC_STATIC_INFO, model.m_info)
		DDX_TEXT(IDC_STATIC_SAVED, model.m_saved)
		DDX_TEXT(IDC_STATIC_OPENED, model.m_opened)

		
		//DDX_RADIO(IDC_RADIO_QUOTE, model.install_type_quote)
		//DDX_RADIO(IDC_RADIO_EXRIGHT, model.install_type_exright)
		//DDX_RADIO(IDC_RADIO_BASE, model.install_type_base)
		//DDX_RADIO(IDC_RADIO_CATALOG, model.install_type_catalog)

		
	END_DDX_MAP()
	//init_dialog��show��ֻ����һ�Ρ�
	BEGIN_MSG_MAP(CQuoteView)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
		//NOTIFY_ID_HANDLER(IDC_EDIT_PATH,OnEditChanged)
		COMMAND_ID_HANDLER(IDC_EDIT_PATH,OnEditChanged)
		COMMAND_RANGE_HANDLER(IDC_RADIO_QUOTE, IDC_RADIO_CATALOG, OnRadioClick)

		COMMAND_HANDLER(IDC_BUTTON_SELECT, BN_CLICKED, OnClickedButtonSelect)
		COMMAND_HANDLER(IDC_BUTTON_INSTALL, BN_CLICKED, OnClickedButtonInstall)
		CHAIN_MSG_MAP(CCtlColored<CQuoteView>)

	END_MSG_MAP()


	void SetVisible(CQuoteViewModel::State state)
	{
		switch (state)
		{
		case CQuoteViewModel::State::init:
		{
			::EnableWindow(GetDlgItem(IDC_BUTTON_SELECT), true);
			::EnableWindow(GetDlgItem(IDC_EDIT_PATH), true);

			::EnableWindow(GetDlgItem(IDC_BUTTON_INSTALL), FALSE);  //��װ��ť������

			
			::ShowWindow(GetDlgItem(IDC_PROGRESS_IMPORT), SW_HIDE);
			m_progressBar.SetPos(0);//����Ϊ0��������

		}
			break;

		case CQuoteViewModel::State::selected:
		{
			::EnableWindow(GetDlgItem(IDC_BUTTON_SELECT), true);
			::EnableWindow(GetDlgItem(IDC_EDIT_PATH), true);

			::EnableWindow(GetDlgItem(IDC_BUTTON_INSTALL), TRUE); 

			::ShowWindow(GetDlgItem(IDC_PROGRESS_IMPORT), SW_HIDE);
			m_progressBar.SetPos(0);//����Ϊ0��������

		}
			break;

		case CQuoteViewModel::State::pending:
		{
			::EnableWindow(GetDlgItem(IDC_BUTTON_SELECT), FALSE);
			::EnableWindow(GetDlgItem(IDC_EDIT_PATH),FALSE);

			::EnableWindow(GetDlgItem(IDC_BUTTON_INSTALL), FALSE);  //��װ��ť���ɼ�

			::ShowWindow(GetDlgItem(IDC_PROGRESS_IMPORT), SW_SHOW);

		}
			break;

		case CQuoteViewModel::State::complete:
		{
			::EnableWindow(GetDlgItem(IDC_BUTTON_SELECT), TRUE);
			::EnableWindow(GetDlgItem(IDC_EDIT_PATH), TRUE);

			//::EnableWindow(GetDlgItem(IDC_BUTTON_INSTALL), FALSE); //��װ��ť���ɼ�

			//::ShowWindow(GetDlgItem(IDC_PROGRESS_IMPORT), SW_SHOW);

		}
			break;
		}

	}
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/){

		
		//��ȡ����ɫ
		HBRUSH bk = (HBRUSH)GetClassLong(GetParent(), GCL_HBRBACKGROUND);
		LOGBRUSH brush;
		::GetObjectW(bk, sizeof(LOGBRUSH), &brush);
		DWORD bkColor = brush.lbColor;

		//SetTextColor(::GetSysColor(COLOR_WINDOWTEXT)); 
		//SetBkBrush(bk);		
		SetTextBackGround(bkColor); 
		SetTextColor(RGB(0, 0, 0)); 


		model.m_path = L"";
		m_progressBar = GetDlgItem(IDC_PROGRESS_IMPORT);
		m_pathctrl.Attach(GetDlgItem(IDC_EDIT_PATH));

		model.get_date_range();
		//model.set_saved_string();

		//��ʼ��Radio Button
		CheckRadioButton(IDC_RADIO_QUOTE, IDC_RADIO_CATALOG, IDC_RADIO_QUOTE);

		SetVisible(model.m_state);
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
		wchar_t temp[MAX_PATH] = { 0 };
		GetDlgItemText(IDC_EDIT_PATH, temp, MAX_PATH);
		model.open(m_hWnd);
		if (temp != model.m_path)
		{
			
			DoDataExchange(false, IDC_EDIT_PATH);
			//����init��selected״ִ̬��
			if (model.is_state_changed())
			{				
				SetVisible(model.m_state);
			}

			DoDataExchange(FALSE, IDC_STATIC_OPENED);
			DoDataExchange(FALSE, IDC_STATIC_INFO);
		}
		return 0;
	}
	LRESULT OnEditChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
	{
		//512=EN_KILLFOCUS WID=1004���ؼ���id��hwnd�ǿؼ���hwnd
		if (wNotifyCode == EN_KILLFOCUS)
		{
			wchar_t temp[MAX_PATH] = { 0 };
			GetDlgItemText(IDC_EDIT_PATH, temp,MAX_PATH);
			if (temp != model.m_path)
			{
				
				DoDataExchange(true, wID);
				//����init��selected״ִ̬��
				if (model.is_state_changed())
				{
					SetVisible(model.m_state);					
				}
				DoDataExchange(FALSE, IDC_STATIC_INFO);
			}
		}
		return 0;
	}


	LRESULT OnClickedButtonInstall(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		model.m_state = CQuoteViewModel::State::pending;
		SetVisible(CQuoteViewModel::State::pending);
		wostringstream info;
		info << L"��" << model.parser.m_quote_count << L"����¼�����ڰ�װ...";
		model.m_info.clear();
		model.m_info.append(info.str());
		SetDlgItemTextW(IDC_STATIC_INFO, model.m_info.c_str());

		timer time_used;
		time_used.reset();
		
		std::thread *t;
		t=new thread(&CQuoteViewModel::import, &model, [this,t,time_used](const char *err,int now){
			if (err)
			{
				MessageBoxA(m_hWnd, err, "������̳���", 0);
			}
			else if (::IsWindow(m_progressBar.m_hWnd))
			{	
				m_progressBar.SetPos(now*100/model.parser.m_quote_count + 1);
				if (now == model.parser.m_quote_count)
				{
					
					wstringstream ss;
					ss << L"���,��ʱ";
					auto used = time_used.elapsed_seconds();
					if (used <= 0)
						ss << L"����1��!";
					else
						if (used >=60)
							ss << used / 60 << L"��" << used % 60 << L"��!";
						else 
							ss << used << L"��!";
					model.m_info.append(ss.str());
					
					model.set_saved_string();

					model.m_state = CQuoteViewModel::State::complete;
					SetVisible(CQuoteViewModel::State::complete);

					SetDlgItemTextW(IDC_STATIC_SAVED, model.m_saved.c_str());

					SetDlgItemTextW(IDC_STATIC_INFO, model.m_info.c_str());
					model.parser.close(); //��װ��Ϻ���������Dad�ļ�
					ThreadTask::Remove(import_id);
				}
			}
		});
		import_id = t->get_id();
		ThreadTask::Add(import_id, L"��������");
		t->detach(); //�����̷߳����ִ��
		//t.join();//�ȴ����߳�ִ�������ִ����һ�����
		return 0;
	}

	LRESULT OnRadioClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CheckRadioButton(IDC_RADIO_QUOTE, IDC_RADIO_CATALOG, wID);
		switch (wID)
		{
		case IDC_RADIO_QUOTE:
			model.install_type = 0;
			break;
		case IDC_RADIO_EXRIGHT:
			model.install_type = 1;
			break;
		case IDC_RADIO_BASE:
			model.install_type = 2;
			break;
		case IDC_RADIO_CATALOG:
			model.install_type = 3;
			break;
		}
		return 0;
	}
	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
};


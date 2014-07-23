// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <atltypes.h>

class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CWindow *m_view=NULL;
	CCommandBarCtrl m_CmdBar;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		//����ͼ����WM_FORWARDMSG��Ϣ����ͼ����ػ���Ϣ������Ӧ����������Ӧ��
		if (m_hWndClient != NULL)
			return ::SendMessage(m_hWndClient, WM_FORWARDMSG, 0, (LPARAM)pMsg);
		return FALSE;
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		//UPDATE_ELEMENT(IDD_STOCK_FORM, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_SYSCOMMAND,OnSysCommand)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)		
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)

		MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterSizeMove)
		MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitSizeMove)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)

		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (wParam == SC_MAXIMIZE)
		{
			ATLTRACE("�����Ϣ�������ť��uMsg: %u, wParam LOWORD: %u, HIWORD: %u\n", uMsg, LOWORD
				(wParam), HIWORD(wParam));
			MessageBox(L"�������󻯰�ť");
		}
		else
		{
			ATLTRACE("uMsg: %u, wParam LOWORD: %u, HIWORD: %u\n lParam LOWORD: %u, HIWORD: %u\n", uMsg, LOWORD
				(wParam), HIWORD(wParam, LOWORD(lParam), HIWORD(lParam)));
			return DefWindowProc(uMsg, wParam, lParam);
			//return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		}
		//AtlTrace("�����Ϣ:����Ĭ�ϴ��� \n");
		m_view->ShowWindow(SW_HIDE);
		//::ShowWindow( m_CmdBar.m_hWnd, SW_HIDE);
		//::ShowWindow(m_hWndToolBar, SW_HIDE);
		//::ShowWindow(m_hWndStatusBar, SW_HIDE);
		//this->ShowWindow(SW_HIDE);
		MessageBox(L"�����Ϣ����ʼĬ�ϴ���");

		//LRESULT result = DefWindowProc(uMsg, wParam, lParam);
		LRESULT result = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		MessageBox(L"�����Ϣ������Ĭ�ϴ���");
		//UpdateLayout1();
		//AtlTrace("�����Ϣ������Ĭ�ϴ��� \n");
		
		return result;
	}

	LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		AtlTrace("WM_ERASEBKGND \n");
		return DefWindowProc(uMsg, wParam, lParam);
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		AtlTrace("WM_PAINT \n");
		//SetMsgHandled(FALSE);
		return DefWindowProc(uMsg, wParam, lParam);
	}
	LRESULT OnEnterSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		AtlTrace("WM_ENTERSIZEMOVE \n\r");
		return TRUE;
	}

	LRESULT OnExitSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		AtlTrace("WM_EXITSIZEMOVE \n\r");
		return True;
	}

	//��󻯡��ı��С���϶�����������������ᴥ������Ϣ
	//����ڴ�ʱ�����Ի������أ�����onsize֮ǰ����ػ�
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		MessageBox(L"����OnGetMinMaxInfo");
		//MINMAXINFO * info = (MINMAXINFO *)lParam;
		if (m_hWndClient && m_view->IsWindow())
		{
			//this->SetRedraw(False);
		}
		//LockWindowUpdate(true);
		
		bHandled = false; //������ã����������Ϣ�������ᴦ��
		return TRUE;
	}

	LRESULT OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		AtlTrace("WM_SIZING \n");
		return TRUE;
	}
	//��ʱ��С�ı���ɣ��Ҵ���ĸ��Ӵ���Ҳ�����Ļ���
	//�������������˶Ի�����˸öԻ���Ļ��Ʋ�δ���У��ڴ˸ı�λ��һ�λ��ƣ��Ͳ�����˸
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{	
		
		AtlTrace("WM_SIZE \n");
		if (m_hWndClient && m_view->IsWindow())
		{
			RECT rect;
			this->GetWindowRect(&rect);
			int x, y;
			//GetWindowPos(m_hWndClient, &x, &y);
			AtlTrace("����onsize x=%u,y=%u,right=%u,bottom=%u \n", rect.left,rect.top,rect.right,rect.bottom);
			MessageBox(L"����Onsize��");
			//m_view->LockWindowUpdate(FALSE);
			//m_view->CenterWindow(m_hWnd);
			//m_view->ShowWindow(SW_SHOW);
			UpdateLayout1();


			//m_view->SetRedraw(TRUE);
			//MessageBox(L"��ʼ�ػ�");
			//AtlTrace("Start Redraw x=%d,y=%d \n", x, y);
			//this->SetRedraw();
			//this->RedrawWindow();
			//AtlTrace("End Redraw x=%d,y=%d \n", x, y);
			//m_view->LockWindowUpdate(FALSE);
		}
		//LockWindowUpdate(FALSE);
		
		//UpdateWindow();

		bHandled = TRUE;
		return 1;
	}

	//�������϶����壬δ�ı��С������£�����OnGetMinMaxInfo�����˶Ի�����onsize��ʾ
	//�öԻ���Ĵ���û��ִ�У���ʱ�Ի������ʧ��
	//������ֻ��Ҫ������ʾ�öԻ��򼴿ɣ���Ϊ��Сδ�䣬����Ҳ�Ǿ��еģ����������ٴ���
	LRESULT OnWindowPosChanged(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if (m_hWndClient && m_view->IsWindow() && !m_view->IsWindowVisible())
		{
			//m_view->ShowWindow(SW_SHOW);
		}
		bHandled = False;
		return 1;
	}

	//Win32��ȡ���Ͻ���Ը����ڵ�����
	void GetWindowPos(HWND hWnd, int *x, int *y)
	{
		HWND hWndParent = ::GetParent(hWnd);
		POINT p = { 0 };

		::MapWindowPoints(hWnd, hWndParent, &p, 1);

		(*x) = p.x;
		(*y) = p.y;
	}


	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		//this->ModifyStyle(0, WS_CLIPCHILDREN); //�޸���ʽû��Ч��
		AtlTrace("WM_CREATE start \n");
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);

		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

		CreateSimpleStatusBar();

		m_view = ShowView<CStockView>();
		
		//m_hWndClient=m_view->Create(m_hWnd);

		//m_view->CenterWindow(m_hWnd);
		//���ʹ��CDialogResize����������DlgCtrlId
		//m_view->SetDlgCtrlID(m_view->IDD);
		UIAddToolBar(hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, 1);
		UISetCheck(ID_VIEW_STATUS_BAR, 1);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);
		AtlTrace("WM_CREATE end \n");
		return 0;
	}

	//��ִ��
	void UpdateLayout(BOOL bResizeBars = TRUE)
	{

	}

	void UpdateBarsPosition(RECT& rect, BOOL bResizeBars = TRUE)
	{
		// resize toolbar
		if (m_hWndToolBar != NULL && ((DWORD)::GetWindowLong(m_hWndToolBar, GWL_STYLE) & WS_VISIBLE))
		{
			if (bResizeBars != FALSE)
			{
				::SendMessage(m_hWndToolBar, WM_SIZE, 0, 0);
				::InvalidateRect(m_hWndToolBar, NULL, TRUE);
			}
			RECT rectTB = { 0 };
			::GetWindowRect(m_hWndToolBar, &rectTB);
			rect.top += rectTB.bottom - rectTB.top;
		}

		// resize status bar
		if (m_hWndStatusBar != NULL && ((DWORD)::GetWindowLong(m_hWndStatusBar, GWL_STYLE) & WS_VISIBLE))
		{
			if (bResizeBars != FALSE)
				::SendMessage(m_hWndStatusBar, WM_SIZE, 0, 0);
			RECT rectSB = { 0 };
			::GetWindowRect(m_hWndStatusBar, &rectSB);
			rect.bottom -= rectSB.bottom - rectSB.top;
		}
	}

	//m_hWndClientδռ��������ܿͻ�������ܴ�С�ı�ʱ״̬������ֲк�
	//�򵥵ķ�ʽ�ǣ�ʼ����m_hWndClentΪ�գ���ʹ����
	//����UpdateLayout�ǽϺõķ�ʽ
	void UpdateLayout1(BOOL bResizeBars = TRUE)
	{
		if (m_view)
		{
		AtlTrace("UpdateLayout start \n");
		RECT rect = { 0 };
		GetClientRect(&rect); //��ȡ����Ӧ�õĿͻ���rect����ֻ�ǳ�ȥ���ڵı��⡢�߿�֮��ʣ�µĴ��幤������

		// position bars and offset their dimensions
		UpdateBarsPosition(rect, bResizeBars);  //��rect��ȥ�˵�����������״̬����ռ����
		//�˴��õ���rect��ȫ���ͻ����������������Χ�ھ�����ʾ

		//�������ǻ�ȡ
		RECT clientRect = { 0 };

			m_view->GetClientRect(&clientRect); //�������ô�С

			int left = rect.left + ((rect.right - rect.left) - (clientRect.right - clientRect.left)) / 2;
			int top = rect.top + ((rect.bottom - rect.top) - (clientRect.bottom - clientRect.top)) / 2;
			int nowLeft, nowTop;
			GetWindowPos(m_hWndClient, &nowLeft, &nowTop);

			//������������ʱ��ִ��һ�Σ�ÿ�δ�С�����ִ��һ�Ρ�
			//��ֵ��ǣ��˳���ʱ��...Ҳִ��һ�Ρ�
			if (m_hWndClient != NULL && (nowLeft != left || nowTop != top))
			{
				AtlTrace("UpdateLayout:SetWindowPos \n");
				MessageBox(L"UpdateLayout:׼��SetWindowPos \n");

				if (!m_view->IsWindowVisible())
				{
					m_view->ShowWindow(SW_SHOW);
					//::ShowWindow(m_CmdBar.m_hWnd, SW_SHOW);
					//::ShowWindow(m_hWndToolBar, SW_SHOW);
					//::ShowWindow(m_hWndStatusBar, SW_SHOW);
				}

				m_view->SetWindowPos(NULL, left, top, -1, -1,
					SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

				
				MessageBox(L"UpdateLayout:����SetWindowPos��׼����ʾ��ͼ�͹����� \n");


				//this->LockWindowUpdate(False);
				MessageBox(L"UpdateLayout:SetWindowPos���� \n");
				//::SetWindowPos(m_hWndClient, NULL, left, top, -1, -1,
				//	SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
		}
		AtlTrace("UpdateLayout end \n");
	}



	//��ʾ��ͼ
	template <class T>
	CWindow * ShowView()
	{
		if (::IsWindow(m_hWndClient))
		{
			if (::GetDlgCtrlID(m_hWndClient) == T::IDD)
				return m_view;
			::DestroyWindow(m_hWndClient);
			m_hWndClient = NULL;
		}

		T* pView = new T;
		m_hWndClient = pView->Create(m_hWnd);
		pView->SetDlgCtrlID(pView->IDD);
	   
		//�����Ҫ֧��CWindow��ͼ��
		//m_hWndClient = pView->Create(m_hWnd, NULL, NULL, 0, 0, IDD);
		return (CWindow *)pView;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: add code to initialize document

		return 0;
	}

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		AtlTrace("OnViewToolBar \n");
		static BOOL bVisible = TRUE;	// initially visible
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		//UpdateLayout();
		return 0;
	}

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		AtlTrace("OnViewStatusBar \n");
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		//UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}
};

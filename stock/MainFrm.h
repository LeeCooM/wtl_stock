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

		return FALSE;
		//return m_view->PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)		
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	//��󻯡��ı��С���϶�����������������ᴥ������Ϣ
	//����ڴ�ʱ�����Ի������أ�����onsize֮ǰ����ػ�
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//MINMAXINFO * info = (MINMAXINFO *)lParam;
		if (m_hWndClient && m_view->IsWindow())
		{
			m_view->ShowWindow(SW_HIDE);
		}
		bHandled = false; //������ã����������Ϣ�������ᴦ��
		return TRUE;
	}

	//��ʱ��С�ı���ɣ��Ҵ���ĸ��Ӵ���Ҳ�����Ļ���
	//�������������˶Ի�����˸öԻ���Ļ��Ʋ�δ���У��ڴ˸ı�λ��һ�λ��ƣ��Ͳ�����˸
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_hWndClient && m_view->IsWindow())
		{
			//m_view->LockWindowUpdate(FALSE);
			m_view->CenterWindow(m_hWnd);
			m_view->ShowWindow(SW_SHOW);
			//UpdateLayout();
		}
		bHandled = False;
		return 1;
	}

	//�������϶����壬δ�ı��С������£�����OnGetMinMaxInfo�����˶Ի�����onsize��ʾ
	//�öԻ���Ĵ���û��ִ�У���ʱ�Ի������ʧ��
	//������ֻ��Ҫ������ʾ�öԻ��򼴿ɣ���Ϊ��Сδ�䣬����Ҳ�Ǿ��еģ����������ٴ���
	LRESULT OnWindowPosChanged(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if (m_hWndClient && m_view->IsWindow() && !m_view->IsWindowVisible())
		{
			m_view->ShowWindow(SW_SHOW);
		}
		bHandled = False;
		return 1;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		//this->ModifyStyle(0, WS_CLIPCHILDREN); //�޸���ʽû��Ч��

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

		m_view->CenterWindow(m_hWnd);
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
		
		return 0;
	}

	//m_hWndClientδռ��������ܿͻ�������ܴ�С�ı�ʱ״̬������ֲк�
	//�򵥵ķ�ʽ�ǣ�ʼ����m_hWndClentΪ�գ���ʹ����
	//����UpdateLayout�ǽϺõķ�ʽ
	void UpdateLayout(BOOL bResizeBars = TRUE)
	{
		RECT rect = { 0 };
		GetClientRect(&rect); //��ȡ����Ӧ�õĿͻ���rect����ֻ�ǳ�ȥ���ڵı��⡢�߿�֮��ʣ�µĴ��幤������

		// position bars and offset their dimensions
		UpdateBarsPosition(rect, bResizeBars);  //��rect��ȥ�˵�����������״̬����ռ����
		//�˴��õ���rect��ȫ���ͻ����������������Χ�ھ�����ʾ
	}

	LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return DefWindowProc(uMsg, wParam, lParam);
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
		static BOOL bVisible = TRUE;	// initially visible
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}
};

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
	RECT m_rateRect ;

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
		MESSAGE_HANDLER(WM_NCCALCSIZE, OnNCCalcSize)
		MESSAGE_HANDLER(WM_NCPAINT, OnPaint)
		
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, OnWindowPosChanging)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		MESSAGE_HANDLER(WM_MOVE, OnMove)

		MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterSizeMove)
		MESSAGE_HANDLER(WM_EXITSIZEMOVE, OnExitSizeMove)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)

		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	HMENU m_menu =NULL;

	//rcArea����������
	//left,top�����Ͻ����꣬����0Ϊ�̶���ֵ��-1��-100��ʾ��rcArea��Ⱥ͸߶ȵİٷֱȡ�
	//width,height����Ⱥ͸߶ȣ�����0Ϊ�̶���ֵ������0��ά�ֲ���
	//�����ȼ����С����ȷ�����Ͻǵ�λ��
	void CalcRect(RECT rcArea, RECT rcRate, RECT &rcChild, bool mode = TRUE)
	{
		//��ʼ������Ⱥ͸߶�
		int widthArea = rcArea.right - rcArea.left + 1;
		int heightArea = rcArea.bottom - rcArea.top + 1;
		int width = rcChild.right - rcChild.left + 1;
		int height = rcChild.bottom - rcChild.top + 1;

		//���ȼ����С
		//mode���������Ͻ�����������
		//rcRate.right=0,����ά��ԭ״
		//rcRate.right>0,������ΪrcRate��rightֵ
		//rcRate.right<0�������ģʽ���趨ΪrcArea���Ϊ���������Ȳ�Ϊ���������ֵ��-1����1%��-100Ϊ100%
		if (rcRate.right > 0) width = rcRate.right;
		if (rcRate.right < 0) width = -widthArea*rcRate.right / 100;

		//rcRate.right=0,��߶�ά��ԭ״
		//rcRate.bottom>0,��߶���ΪrcRate��bottomֵ
		//rcRate.bottom<0�������ģʽ���趨ΪrcAre�߶�ΪrcArea�߶ȵ����ֵ��-1����1%��-100Ϊ100%
		if (rcRate.bottom > 0) height = rcRate.bottom;
		if (rcRate.bottom < 0) height = -heightArea*rcRate.bottom / 100;


		int widthBase = mode ? (widthArea - width) : widthArea;
		int heightBase = mode ? (heightArea - height) : heightArea;

		//Ȼ��ȷ��λ��
		if (rcRate.left >= 0)
			rcChild.left = rcArea.left + rcRate.left; //rcRate����Ը�RECT��ֵ��������Ҫ�õ���������
		else
			rcChild.left = rcArea.left - widthBase*rcRate.left / 100;//���������꣬��Ӧ��ȥ1

		//������rcAreaΪ������ʼ�ջ����������˲�����С��0
		//if (rcChild.left < 0) rcChild.left = 0;

		if (rcRate.top >= 0)
			rcChild.top = rcArea.top + rcRate.top;
		else
			rcChild.top = rcArea.top - heightBase*rcRate.top / 100;

		//���ˣ�λ���Ѿ���ȷ����ʱ�޸���������
		rcChild.right = rcChild.left + width - 1;
		rcChild.bottom = rcChild.top + height - 1;
	}

	void ShowChild(bool bShow=true)
	{
		if (m_menu == NULL && this->IsWindow())
			m_menu = GetMenu();

		//SetRedraw(False)����������еĸ��¶�������
		//LockWindowUpdate,����ĸ�����Ч����������ʾ
		if (bShow) LockWindowUpdate();
		::ShowWindow(m_hWndClient, bShow?SW_SHOW:SW_HIDE);
		::ShowWindow(m_hWndToolBar, bShow ? SW_SHOW : SW_HIDE);
		::ShowWindow(m_hWndStatusBar, bShow ? SW_SHOW : SW_HIDE);
		::SetMenu(m_hWnd,bShow?m_menu:NULL);
		if (bShow){
			::SendMessage(m_hWndToolBar, WM_SIZE, 0, 0);
			::InvalidateRect(m_hWndToolBar, NULL, TRUE);
			::SendMessage(m_hWndStatusBar, WM_SIZE, 0, 0);
			::InvalidateRect(m_hWndToolBar, NULL, TRUE);
			LockWindowUpdate(false);

			//���������£�û�б�Ҫ��ʾ��paint
			//this->UpdateWindow();
		}
	}

	//��ʾ��ͼ
	template <class T>
	CWindow * ShowView(RECT rcRate = {0,0,0,0})
	{
		if (::IsWindow(m_hWndClient))
		{
			if (::GetDlgCtrlID(m_hWndClient) == T::IDD)
				return m_view;
			::DestroyWindow(m_hWndClient);
			m_hWndClient = NULL;
		}

		m_rateRect = rcRate;
		T* pView = new T;
		m_hWndClient = pView->Create(m_hWnd);
		pView->SetDlgCtrlID(pView->IDD);

		//�����Ҫ֧��CWindow��ͼ��
		//m_hWndClient = pView->Create(m_hWnd, NULL, NULL, 0, 0, IDD);
		return (CWindow *)pView;
	}

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{		
		if (wParam == SC_MAXIMIZE || wParam==SC_RESTORE)
		{
			ATLTRACE("��󻯻�ָ���Ϣ:wParam %u,%u,lParam: %u,%u \n",
				LOWORD(wParam), HIWORD(wParam), LOWORD(lParam), HIWORD(lParam));
			//MessageBox(L"�������󻯻�ָ���ť");
			ShowChild(FALSE);
			//MessageBox(L"��󻯻�ָ���Ϣ����ʼĬ�ϴ���");
			//LRESULT result = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
			LRESULT result = DefWindowProc(uMsg, wParam, lParam);

			
			AtlTrace("�����Ϣ������Ĭ�ϴ��� \n");
			//MessageBox(L"�����Ϣ��Ĭ�ϴ������,���ڿ����ܷ���ʾ�Ӵ���");
			
			//������࣬��Ϊ�ض���ִ��OnSize���Ӷ���ִ��UpdateChild
			//ShowChild(TRUE);			
		}
		else
		{
			return DefWindowProc(uMsg, wParam, lParam);
		}
	}

	LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{

		AtlTrace("WM_ERASEBKGND \n");
		return DefWindowProc(uMsg, wParam, lParam);
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		AtlTrace("WM_PAINT \n");
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
		//MessageBox(L"����OnGetMinMaxInfo");
		//MINMAXINFO * info = (MINMAXINFO *)lParam;
		bHandled = false; //������ã����������Ϣ�������ᴦ��
		return TRUE;
	}

	LRESULT OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		////MessageBox(L"λ�����ڸı䣺WM_WINDOWPOSCHANGING");
		ATLTRACE("OnWindowPosChanging:wParam %u(%u,%u),lParam: %u(%u,%u) \n",
			wParam, LOWORD(wParam), HIWORD(wParam), lParam, LOWORD(lParam), HIWORD(lParam));
		bHandled = false; //������ã����������Ϣ�������ᴦ��
		return TRUE;
	}
	

	LRESULT OnNCCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
			////MessageBox(L"����ϵͳ����С��OnNCCalcSize");
			bHandled = false; //������ã����������Ϣ�������ᴦ��
			return TRUE;
	}

	LRESULT OnNCPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		ATLTRACE("WM_NCPAINT:wParam %u(%u,%u),lParam: %u(%u,%u) \n",
			wParam, LOWORD(wParam), HIWORD(wParam), lParam, LOWORD(lParam), HIWORD(lParam));
		return DefWindowProc(uMsg, wParam, lParam);
	}

	LRESULT OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		ATLTRACE("OnWindowPosChanged:wParam %u(%u,%u),lParam: %u(%u,%u) \n",
			wParam, LOWORD(wParam), HIWORD(wParam), lParam, LOWORD(lParam), HIWORD(lParam));
		//MessageBox(L"λ���Ѹı䣺WM_WINDOWPOSCHANGED");
		bHandled = false; //������ã����������Ϣ�������ᴦ��
		return TRUE;
	}

	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		////MessageBox(L"�ƶ�:WM_MOVE");
		ATLTRACE("WM_MOVE:wParam %u(%u,%u),lParam: %u(%u,%u) \n",
			wParam,LOWORD(wParam), HIWORD(wParam),lParam,LOWORD(lParam), HIWORD(lParam));
		bHandled = false; //������ã����������Ϣ�������ᴦ��
		return TRUE;
	}


	LRESULT OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		ATLTRACE("WM_SIZING:wParam %u(%u,%u),lParam: %u(%u,%u) \n",
			wParam, LOWORD(wParam), HIWORD(wParam), lParam, LOWORD(lParam), HIWORD(lParam));
		return TRUE;
	}
	//��ʱ��С�ı���ɣ��Ҵ���ĸ��Ӵ���Ҳ�����Ļ���
	//�������������˶Ի�����˸öԻ���Ļ��Ʋ�δ���У��ڴ˸ı�λ��һ�λ��ƣ��Ͳ�����˸
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{	
		ATLTRACE("WM_SIZE:wParam %u(%u,%u),lParam: %u(%u,%u) \n",
			wParam, LOWORD(wParam), HIWORD(wParam), lParam, LOWORD(lParam), HIWORD(lParam));
		if (m_hWndClient && m_view->IsWindow())
		{
			RECT rect;
			this->GetWindowRect(&rect);
			int x, y;
			//GetWindowPos(m_hWndClient, &x, &y);
			AtlTrace("����onsize x=%u,y=%u,right=%u,bottom=%u \n", rect.left,rect.top,rect.right,rect.bottom);
			//MessageBox(L"����Onsize��");
			//m_view->LockWindowUpdate(FALSE);
			//m_view->CenterWindow(m_hWnd);
			//m_view->ShowWindow(SW_SHOW);
			UpdateChild();
		}

		bHandled = TRUE;
		return 1;
	}

	//�������϶����壬δ�ı��С������£�����OnGetMinMaxInfo�����˶Ի�����onsize��ʾ
	//�öԻ���Ĵ���û��ִ�У���ʱ�Ի������ʧ��
	//������ֻ��Ҫ������ʾ�öԻ��򼴿ɣ���Ϊ��Сδ�䣬����Ҳ�Ǿ��еģ����������ٴ���


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

		m_view = ShowView<CStockView>({-50,-50,0,0});
		
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

	//m_hWndClientδռ��������ܿͻ�������ܴ�С�ı�ʱ״̬������ֲк�
	//�򵥵ķ�ʽ�ǣ�ʼ����m_hWndClentΪ�գ���ʹ����
	//����UpdateLayout�ǽϺõķ�ʽ
	void UpdateChild(BOOL bResizeBars = TRUE)
	{
		if (m_view)
		{
			////MessageBox(L"����UpdateLayout");
			


			int nowLeft, nowTop;
			GetWindowPos(m_hWndClient, &nowLeft, &nowTop); //�õ����ڵ�����
			

			//������������ʱ��ִ��һ�Σ�ÿ�δ�С�����ִ��һ�Ρ�
			//��ֵ��ǣ��˳���ʱ��...Ҳִ��һ�Ρ�
			if (m_hWndClient != NULL)
			{
				AtlTrace("UpdateLayout:SetWindowPos \n");
				////MessageBox(L"UpdateLayout:׼��SetWindowPos \n");

				if (!m_view->IsWindowVisible())
					ShowChild(TRUE);

				AtlTrace("UpdateLayout start \n");
				RECT rect = { 0 };
				GetClientRect(&rect); //��ȡ����Ӧ�õĿͻ���rect����ֻ�ǳ�ȥ���ڵı��⡢�߿�֮��ʣ�µĴ��幤������

				UpdateBarsPosition(rect, bResizeBars);  //��rect��ȥ�˵�����������״̬����ռ����
				//�˴��õ���rect��ȫ���ͻ����������������Χ�ھ�����ʾ

				//�������ǻ�ȡ
				RECT clientRect = { 0 };
				m_view->GetClientRect(&clientRect); //�������ô�С

				//��ʾ�˹�����״̬�����ټ��㣬�����©��
				CalcRect(rect, m_rateRect, clientRect);

				m_view->SetWindowPos(NULL,clientRect.left,clientRect.top,
					clientRect.right-clientRect.left+1,clientRect.bottom-clientRect.top+1, SWP_NOZORDER | SWP_NOACTIVATE );

			}
		}
		//MessageBox(L"UpdateLayout:����\n");
		AtlTrace("UpdateLayout end \n");
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

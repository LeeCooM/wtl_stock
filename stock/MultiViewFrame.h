#ifndef multi_view_frame_h
#define multi_view_frame_h
#pragma once
#include <map>
/*
1.�л���ͼ
2.������������λ����ʾ��ͼ
3.���������������ͼ��С
4.���ÿ����С�ĳߴ�
5.���ÿ�ܱ���ɫ
6.������󻯡���ԭ��������˸
7.��������ť��ʾ����
*/
template <class T>
class CMultiViewFrame :public CFrameWindowImpl<T>
{
public:
	CWindow *m_view = NULL; //��ǰ��ͼָ��
	HBRUSH m_hDialogBrush;  //��ܱ�����ˢ
	HMENU m_menu = NULL;

	RECT m_rateRect; //���λ�úʹ�С�ı�������
	bool m_rateMode = TRUE;//Ĭ������ͼ�Ϳ�ܴ�С֮��Ϊ��׼���������λ�á�

	BEGIN_MSG_MAP(CMainFrame)		
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		//ɾ��m_views�ﱣ�����ͼ
		for (auto p : m_views)
		{
			if ((p.second) && ::IsWindow(p.second->m_hWnd))
				::DestroyWindow(p.second->m_hWnd);
		}
		bHandled = FALSE;
		return 1;
	}
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 800; // min width
		lpMMI->ptMinTrackSize.y = 600; // min height
		//lpMMI->ptMaxTrackSize.x = 600; // max width
		//lpMMI->ptMaxTrackSize.y = 450; // max height
		bHandled = false; //������ã����������Ϣ�������ᴦ��
		return 0;
	}

	LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return DefWindowProc(uMsg, wParam, lParam);
	}

	//��ʱ��С�ı���ɣ��Ҵ���ĸ��Ӵ���Ҳ�����Ļ���
	//�������������˶Ի�����˸öԻ���Ļ��Ʋ�δ���У��ڴ˸ı�λ��һ�λ��ƣ��Ͳ�����˸
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_hWndClient &&  m_view->IsWindow())
			UpdateChild();
		return 1;
	}

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (wParam == SC_MAXIMIZE || wParam == SC_RESTORE)
		{
			ShowChild(FALSE);
		}
		return  DefWindowProc(uMsg, wParam, lParam);		
	}

	//��ִ��
	void UpdateLayout(BOOL bResizeBars = TRUE){}

	//m_hWndClientδռ��������ܿͻ�������ܴ�С�ı�ʱ״̬������ֲк�
	//�򵥵ķ�ʽ�ǣ�ʼ����m_hWndClentΪ�գ���ʹ����
	//����UpdateLayout�ǽϺõķ�ʽ
	void UpdateChild(BOOL bResizeBars = TRUE)
	{
		if (m_hWndClient != NULL && m_view)
		{
			if (!m_view->IsWindowVisible())
				ShowChild(TRUE);
			RECT rect = { 0 };
			GetClientRect(&rect); //��ȡ����Ӧ�õĿͻ���rect����ֻ�ǳ�ȥ���ڵı��⡢�߿�֮��ʣ�µĴ��幤������
			UpdateBarsPosition(rect, bResizeBars);  //��rect��ȥ�˵�����������״̬����ռ����

			RECT clientRect = { 0 };
			m_view->GetClientRect(&clientRect); //�������ô�С
			CalcRect(rect, m_rateRect, clientRect, m_rateMode);
			m_view->SetWindowPos(NULL, clientRect.left, clientRect.top,
					clientRect.right - clientRect.left + 1, clientRect.bottom - clientRect.top + 1, SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

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

	void ShowChild(bool bShow = true)
	{
		if (m_menu == NULL && this->IsWindow())
			m_menu = GetMenu();
		if (bShow) LockWindowUpdate();
		::ShowWindow(m_hWndClient, bShow ? SW_SHOW : SW_HIDE);
		::ShowWindow(m_hWndToolBar, bShow ? SW_SHOW : SW_HIDE);
		::ShowWindow(m_hWndStatusBar, bShow ? SW_SHOW : SW_HIDE);
		::SetMenu(m_hWnd, bShow ? m_menu : NULL);
		if (bShow){
			::SendMessage(m_hWndToolBar, WM_SIZE, 0, 0);
			::InvalidateRect(m_hWndToolBar, NULL, TRUE);
			::SendMessage(m_hWndStatusBar, WM_SIZE, 0, 0);
			::InvalidateRect(m_hWndToolBar, NULL, TRUE);
			LockWindowUpdate(false);
		}
	}


	std::map<int, CWindow *> m_views;
	//��ʾ��ͼ
	template <class T>
	CWindow * ShowView(RECT rcRate = { 0, 0, 0, 0 },bool rateMode=TRUE,bool autoDelete=true)
	{
		if (::IsWindow(m_hWndClient) )
		{
			if (::GetDlgCtrlID(m_hWndClient) == T::IDD)
				return m_view;
			if (m_views.find(::GetDlgCtrlID(m_hWndClient))==m_views.end()) //���ص�������Ϊ�ձ�ʾδ�ҵ���
			//if (!m_views[::GetDlgCtrlID(m_hWndClient)]) //�����ǰ��ͼ���б��в�����
			{
				::DestroyWindow(m_hWndClient);
				m_hWndClient = NULL;
			}
			else
				::ShowWindow(m_hWndClient, SW_HIDE);
		}//��ʱ��ǰһ���ڻ����ٻ�����

		m_rateRect = rcRate;
		m_rateMode = rateMode;

		if (m_views.find(T::IDD)!=m_views.end()) //�������ͼ�Ѿ�����
		{
			m_view = m_views[T::IDD];
			m_hWndClient = m_view->m_hWnd;
			::ShowWindow(m_view->m_hWnd, SW_SHOW);
			UpdateChild();
			return m_view;
		}

		T* pView = new T;
		m_hWndClient = pView->Create(m_hWnd);
		pView->SetDlgCtrlID(pView->IDD);
		m_view = pView;
		//�����׼���Զ�ɾ��������뵽��ͼ�б�
		if (!autoDelete)
			m_views[T::IDD] = pView;
		//�����Ҫ֧��CWindow��ͼ��
		//m_hWndClient = pView->Create(m_hWnd, NULL, NULL, 0, 0, IDD);
		UpdateChild();
		return (CWindow *)pView;
	}

	void AddToolbarButtonText(HWND hWndToolBar, UINT nID, LPCTSTR lpsz)
	{
		CToolBarCtrl toolbar(hWndToolBar);
		// Set extended style
		if ((toolbar.GetExtendedStyle() & TBSTYLE_EX_MIXEDBUTTONS) !=
			TBSTYLE_EX_MIXEDBUTTONS)
			toolbar.SetExtendedStyle(toolbar.GetExtendedStyle() |
			TBSTYLE_EX_MIXEDBUTTONS);
		// Get the button index
		int nIndex = toolbar.CommandToIndex(nID);
		TBBUTTON tb;
		memset(&tb, 0, sizeof(tb));
		toolbar.GetButton(nIndex, &tb);
		// Add the string to the main toolbar list
		//std::wstring str(lpsz, _tcslen(lpsz) + 1);
		int nStringID = toolbar.AddStrings(lpsz);
		// Alter the button style
		tb.iString = nStringID;
		tb.fsStyle |= TBSTYLE_AUTOSIZE | BTNS_SHOWTEXT;
		// Delete and re-insert the button
		toolbar.DeleteButton(nIndex);
		toolbar.InsertButton(nIndex, &tb);
	}

	void SetBkColor(COLORREF cl = RGB(204, 232, 207))
	{
		m_hDialogBrush = CreateSolidBrush(cl);
		SetClassLong(m_hWnd, GCL_HBRBACKGROUND, (LONG)m_hDialogBrush);
	}

	COLORREF GetBkColor()
	{
		//���ñ���ɫ����ܻ�ȡ...��Ϊ֮ǰ��δ����
		HBRUSH bk = (HBRUSH)GetClassLong(m_hWnd, GCL_HBRBACKGROUND);
		LOGBRUSH brush;
		::GetObjectW(bk, sizeof(LOGBRUSH), &brush);
		return brush.lbColor;
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
};
#endif
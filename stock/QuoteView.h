// stockView.h : interface of the CStockView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CCtlColor.h"

class CQuoteView : public CDialogImpl<CQuoteView>
	, public CCtlColored<CQuoteView>
{
public:
	enum { IDD = IDD_QUOTE_BOX };

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	//init_dialog��show��ֻ����һ�Ρ�
	BEGIN_MSG_MAP(CQuoteView)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		CHAIN_MSG_MAP(CCtlColored<CQuoteView>)
		//MESSAGE_HANDLER(WM_SHOWWINDOW, OnShow)
		//MESSAGE_HANDLER(WM_PAINT,OnPaint)
		//MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/){

		//��ȡ����ɫ
		HBRUSH bk = (HBRUSH)GetClassLong(GetParent(), GCL_HBRBACKGROUND);
		LOGBRUSH brush;
		::GetObjectW(bk, sizeof(LOGBRUSH), &brush);
		DWORD bkColor = brush.lbColor;
		SetTextBackGround(bkColor);  // Lightish kind of blue (Four)
		SetTextColor(RGB(0X60, 0, 0)); // Dark red   


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
	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//AtlTrace("View:WM_ERASEBKGND \n");
		bHandled = FALSE;
		return this->DefWindowProcW();
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		AtlTrace("View:WM_PAINT \n");
		return this->DefWindowProcW();
		//SetMsgHandled(FALSE);
		//return ::DefWindowProc(m_hWnd,uMsg,wParam,lParam);
	}
};

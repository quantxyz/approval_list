// approval_listDlg.h : 头文件
//
#include "AClass/HtmlElementCollection.h"
#pragma once


// Capproval_listDlg 对话框
class Capproval_listDlg : public CDialog
{
// 构造
public:
	Capproval_listDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_APPROVAL_LIST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString CleanHtml(const char* input);
	CString CleanHtml(CString input);
	CDWordArray m_dwarrTagStart;
	CDWordArray m_dwarrTagLen;
private:
	CString m_szHtmlPage;
private:
	void SetOutputHtml(CString szTag, CString szAtrib, CString szAtribVal);
	void FillList(HtmlTree hTree);
	BOOL AddTagToList(HtmlNode node);
	afx_msg void OnEnSetfocusEditOutput();
};

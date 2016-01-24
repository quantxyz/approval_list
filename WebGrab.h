#if !defined(AFX_WEBGRAB_H__D31F39A4_1C63_11D3_AA11_AC48B5000000__INCLUDED_)
#define AFX_WEBGRAB_H__D31F39A4_1C63_11D3_AA11_AC48B5000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebGrab.h : header file
//

#include <afxinet.h>

/////////////////////////////////////////////////////////////////////////////
// CWebGrabSession 

class CWebGrabSession : public CInternetSession
{
// Construction
public:
	CWebGrabSession(LPCTSTR szAgentName);
	CWebGrabSession(LPCTSTR szAgentName, CWnd* pStatusWnd);
	virtual ~CWebGrabSession();
    void CommonConstruct();

// Operations
public:
    void SetStatus(LPCTSTR fmt, ...);
    void SetStatusWnd(CWnd* pWnd)     { m_pStatusWnd = pWnd; }

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebGrabSession)
	//}}AFX_VIRTUAL
    virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus, 
                                  LPVOID lpvStatusInformation, 
                                  DWORD dwStatusInformationLength);

	// Generated message map functions
	//{{AFX_MSG(CWebGrabSession)
	//}}AFX_MSG

// Attributes
protected:
    CWnd* m_pStatusWnd;
};


/////////////////////////////////////////////////////////////////////////////
// CWebGrab 

class CWebGrab : public CObject
{
public:
	CWebGrab();
	virtual ~CWebGrab();

// Operations
public:
    BOOL Initialise(LPCTSTR szAgentName = NULL, CWnd* pWnd = NULL);
    void Close();

    BOOL GetFile(LPCTSTR szURL, CString& strBuffer, LPCTSTR szAgentName = NULL, CWnd* pWnd = NULL);
	BOOL GetFileInfo(LPCTSTR  szURL,
		CString& strLastModified,
		DWORD&   dwSize,
		DWORD&   dwServerError,
		LPCTSTR  szAgentName = NULL,
		CWnd*    pWnd = NULL);
	
	void SetTimeOut(DWORD timeOut);
	double  GetRate();
	
	BOOL GetForceReload() { return m_bForceReload; }
	void SetForceReload (bool bForceReload) { m_bForceReload = bForceReload; }

	void    SetUseProxy(bool use);
	void    SetProxy(LPCSTR proxy, WORD port,bool use=true);
	void    SetProxyPort(UINT port);
	void    SetProxyServer(LPCSTR server);
	void	SetProgressCtrl(CProgressCtrl* pProgressCtrl);
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebGrab)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CWebGrab))
	//}}AFX_MSG

protected:
    CWebGrabSession* m_pSession;

private:
	DWORD   m_timeOut;
	BOOL	m_bForceReload;
	bool    m_useProxy;
	UINT    m_Port;
	CString m_Proxy;
	double  m_transferRate;
	DWORD	m_dwFileSz;
	CProgressCtrl* m_pProgressCtrl;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBGRAB_H__D31F39A4_1C63_11D3_AA11_AC48B5000000__INCLUDED_)

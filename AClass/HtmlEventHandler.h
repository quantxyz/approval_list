// HtmlEventHandler.h: interface for the CHtmlEventHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTMLEVENTHANDLER_H__FA3A808A_146C_4941_A4D8_53BA07701900__INCLUDED_)
#define AFX_HTMLEVENTHANDLER_H__FA3A808A_146C_4941_A4D8_53BA07701900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LiteHTMLReader.h"

struct TAG_HTML
{
	DWORD dwTagStart;
	DWORD dwTagStop;
	BOOL b_IsValid;
};
class CHtmlEventHandler : public ILiteHTMLReaderEvents  
{
public:
	CString GetInnerTagText();
	CString GetOuterTagText();
	void InitWantedTag(CString szTag, CString szTagParam, CString szTagParamValue, int iSkipNo);
	CHtmlEventHandler();
	virtual ~CHtmlEventHandler();
private:
    void BeginParse(DWORD dwAppData, bool &bAbort);
    void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort);
    void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort);
    void Characters(const CString &rText, DWORD dwAppData, bool &bAbort);
    void Comment(const CString &rComment, DWORD dwAppData, bool &bAbort);
    void EndParse(DWORD dwAppData, bool bIsAborted);

private:
	void Init();
	BOOL	m_bStartTraceEndTag;
	CString m_szEnclosedTagArray;
	BOOL	m_bEndTagFound;
	LPCTSTR	m_szTag1Start;
	LPCTSTR	m_szTag2Start;
	LPCTSTR	m_szTag1End;
	LPCTSTR	m_szTag2End;

	CString m_szTagName;
	CString m_szTagParam;
	CString m_szTagParamValue;
	int m_iSkipNo;

	CObArray m_arTags;

	int m_iTagsAlreadySkipped;
};

#endif // !defined(AFX_HTMLEVENTHANDLER_H__FA3A808A_146C_4941_A4D8_53BA07701900__INCLUDED_)

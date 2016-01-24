/**
 *	PROJECT - HTML Reader Class Library
 *
 *	HtmlElementCollection.h - Defines CHtmlElementCollection
 *
 *	Written By dchris_med  <christig2k2@yahoo.com> www.cgsoftlabs.ro
 *	Copyright (c) 2013. All rights reserved.
 *
 *	This code may be used in compiled form in any way you desire 
 *	(including commercial use). The code may be redistributed 
 *	unmodified by any means PROVIDING it is not sold for profit 
 *	without the authors written consent, and providing that this 
 *	notice and the authors name and all copyright notices remains 
 *	intact. However, this file and the accompanying source code may 
 *	not be hosted on a website or bulletin board without the authors 
 *	written permission.
 *
 *	This file is provided "AS IS" with no expressed or implied warranty.
 *	The author accepts no liability for any damage/loss of business that
 *	this product may cause.
 *
 *	Although it is not necessary, but if you use this code in any of 
 *	your application (commercial or non-commercial), please INFORM me 
 *	so that I may know how useful this library is. This will encourage 
 *	me to keep updating it.
 *
 *	HISTORY:
 *
 *	Version 1.0				dchris_med (www.cgsoftlabs.ro)
 *	Oct 4, 2013				First release version.
 */

#pragma once
#include "litehtmlreader.h"
#include "Tree.h"

using namespace std;

class HTMLElement
{
public:
	HTMLElement(){
		lpszStartStart = lpszStartStop = lpszStopStart = lpszStopStop = 0;
		iComplete = iFiltered = 0;
	}
	~HTMLElement(){;};

	CString	szName;
	LPCTSTR lpszStartStart;
	LPCTSTR lpszStartStop;
	LPCTSTR lpszStopStart;
	LPCTSTR lpszStopStop;
	BOOL iComplete;
	BOOL iFiltered;
};

typedef Tree<HTMLElement> HtmlTree;
typedef HtmlTree::Node HtmlNode; //

class CHtmlElementCollection :
	public ILiteHTMLReaderEvents
{
public:
	CHtmlElementCollection(void);
	~CHtmlElementCollection(void);

	void InitWantedTag(CString szTag, CString szAtrib=_T(""), CString szAtribVal=_T(""));
	int	GetNumElements() {return m_iElements;};
	int GetNumElementsFiltered() {return m_iElementsFilter;};
	BOOL GetInnerHtml(int iNoElement, CString &szTag, BOOL bFilter=0);
	BOOL GetOuterHtml(int iNoElement, CString &szTag, BOOL bFilter=0);
	HtmlTree GetTree(){ return m_tree;};

private:
	void Init();
	void BeginParse(DWORD dwAppData, bool &bAbort);
	void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort);
	void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort);
	void Characters(const CString &rText, DWORD dwAppData, bool &bAbort);
	void Comment(const CString &rComment, DWORD dwAppData, bool &bAbort);
	void EndParse(DWORD dwAppData, bool bIsAborted);
	//
	BOOL CloseLastTag(CLiteHTMLTag *pTag, HtmlNode node);
	int CountTreeChildren(HtmlNode node);
	BOOL GetElementAt(HtmlNode node,int &iCur, int iAt, HtmlNode &nodeO, BOOL bFiltered=0);
	BOOL GetElementTxt(int iAt, CString &szTagOut, BOOL bInner, BOOL bFilter = 0);
	BOOL FilterTag(CLiteHTMLTag *pTag);

private:
	CString m_szTagName;
	CString m_szAtrib;
	CString m_szAtribVal;
	HtmlTree m_tree;
	HtmlNode m_node;
	int m_iElements;
	int m_iElementsFilter;
};

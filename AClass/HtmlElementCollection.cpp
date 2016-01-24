#include "stdafx.h"
#include "HtmlElementCollection.h"

#include <stdio.h>
#include <string>


CHtmlElementCollection::CHtmlElementCollection(void)
{
	Init();
}
void CHtmlElementCollection::Init()
{
	//m_tree.Delete();

	HTMLElement tg;
	tg.szName = _T("Root");
	((HTMLElement&)m_tree)=tg;
	m_node = m_tree;
	m_iElements = 0;
	m_iElementsFilter = 0;
}
CHtmlElementCollection::~CHtmlElementCollection(void)
{
}

void CHtmlElementCollection::InitWantedTag(CString szTag, CString szAtrib, CString szAtribVal)
{
	m_szTagName = szTag;
	m_szAtrib = szAtrib;
	m_szAtribVal = szAtribVal;
	while(m_tree.Count)
		m_tree.DeleteNode(0);
	Init();
}

void CHtmlElementCollection::BeginParse(DWORD dwAppData, bool &bAbort)
{

}
//return 1 if we skip the tag from our dom
BOOL CHtmlElementCollection::FilterTag(CLiteHTMLTag *pTag)
{
	CString szTag = pTag->getTagName();

	if(!m_szTagName.IsEmpty()){
		if(!szTag.IsEmpty()&&(0!=m_szTagName.CompareNoCase(szTag)))
			return 1;

		if(!m_szAtrib.IsEmpty()){
			CLiteHTMLAttributes* ptagAtr = (CLiteHTMLAttributes*)pTag->getAttributes();
			if(!ptagAtr)
				return 1;

			int iAttrCount = !ptagAtr->getCount();
			if(iAttrCount)
				return 1;

			if(0!=ptagAtr->getName(0).CompareNoCase(m_szAtrib))
				return 1;

			if(!m_szAtribVal.IsEmpty()){
				CString szParamVal = ptagAtr->getValue(0);
				if(0!=szParamVal.CompareNoCase(m_szAtribVal))
					return 1;
			}		
		}
	}
	return 0;
}
void CHtmlElementCollection::StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	if(!pTag)
		return;

	CString szTag = pTag->getTagName();


	TRACE(_T("Start tag detected : -> %s\r\n"),szTag);

	HTMLElement tg;
	tg.szName = szTag;
	tg.lpszStartStart = pTag->GetTagStartPos();
	tg.lpszStartStop = pTag->GetTagEndPos();
	//filter
	tg.iFiltered = FilterTag(pTag);

	m_node = m_node.AddNode(tg);
}
BOOL CHtmlElementCollection::CloseLastTag(CLiteHTMLTag *pTag, HtmlNode node)
{
	CString szTagToClose = pTag->getTagName();

	node=node.Parent();

	for(int n = node.Count-1; n>=0; n--) {
		CString szTag = node.Nodes[n]->szName;
		if(!node.Nodes[n]->iComplete){
			if(!node.Nodes[n]->szName.CompareNoCase(szTagToClose)){
				node.Nodes[n]->lpszStopStart = pTag->GetTagStartPos();
				node.Nodes[n]->lpszStopStop = pTag->GetTagEndPos();
				node.Nodes[n]->iComplete = 1;
				m_node = node;
				return 1;
			}
		}
	}

	//fail if we're already root
	if(node.IsRoot())
		return 0;

	CloseLastTag(pTag, node);

	return 0;
}
void CHtmlElementCollection::EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	if(!pTag)
		return;


	CString szTag = pTag->getTagName();

	//filter
// 	if(!m_szTagName.IsEmpty())
// 		if(!szTag.IsEmpty()&&m_szTagName.CompareNoCase(szTag))
// 			return;

	TRACE(_T("End tag detected : -> %s\r\n"),szTag);

	if(!CloseLastTag(pTag, m_node)){
		TRACE(_T("End tag closing fail : -> %s\r\n"),szTag);
		//bAbort = true;
	}
}

void CHtmlElementCollection::Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
{

}

void CHtmlElementCollection::Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
{

}

BOOL CHtmlElementCollection::GetElementTxt(int iAt, CString &szTagOut, BOOL bInner, BOOL bFilter)
{
	if(!bFilter)
		if(iAt>=m_iElements)
			return FALSE;
	if(bFilter)
		if(iAt>=m_iElementsFilter)
			return FALSE;

	HtmlNode nodeO;
	int iCur=0;
	if(!GetElementAt(m_tree, iCur, iAt, nodeO, bFilter))
		return FALSE;

	if(!nodeO->iComplete){
		szTagOut = CString(nodeO->lpszStartStop,  nodeO->lpszStartStop - nodeO->lpszStartStart);
	}
	else{
		if(bInner)
			szTagOut = CString(nodeO->lpszStartStop,  nodeO->lpszStopStart - nodeO->lpszStartStop );
		else
			szTagOut = CString(nodeO->lpszStartStart, nodeO->lpszStopStop - nodeO->lpszStartStart);
	}

	return TRUE;
}
BOOL CHtmlElementCollection::GetOuterHtml(int iNoElement, CString &szTag, BOOL bFilter)
{
	return GetElementTxt( iNoElement, szTag, 0, bFilter);
}

BOOL CHtmlElementCollection::GetInnerHtml(int iNoElement, CString &szTag, BOOL bFilter)
{
	return GetElementTxt( iNoElement, szTag, 1, bFilter);
}

BOOL CHtmlElementCollection::GetElementAt(HtmlNode node,int &iCur, int iAt, HtmlNode &nodeO, BOOL bFiltered)
{
	static int ui=1;
	int iElements = node.Count;
	for(int n=0;n<iElements;n++) {

		if(!bFiltered){
			if(iCur+n==iAt){
				nodeO = node.Nodes[n];
				return TRUE;
			}
			iCur++;
		}
		else {
			if(!node.Nodes[n]->iFiltered){
				TRACE(_T("%i Filtered elem : -> %s iCur: %i\r\n"),ui++,node.Nodes[n]->szName, iCur);
				if(iCur==iAt){
					nodeO = node.Nodes[n];
					return TRUE;
				}
				iCur++;
			}
		}

		if(GetElementAt(node.Nodes[n], iCur, iAt, nodeO, bFiltered))
			return TRUE;
	}

	return FALSE;
}

int CHtmlElementCollection::CountTreeChildren(HtmlNode node)
{
	int iElements = 0;
	for(int n=0;n<node.Count;n++) {
		if(!node.Nodes[n]->iFiltered)
			m_iElementsFilter+=1;
		iElements+=1;
		iElements+=CountTreeChildren(node.Nodes[n]);
	}
	return iElements;
}
void CHtmlElementCollection::EndParse(DWORD dwAppData, bool bIsAborted)
{
	m_iElements = CountTreeChildren(m_tree);
}
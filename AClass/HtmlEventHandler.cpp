// HtmlEventHandler.cpp: implementation of the CHtmlEventHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "dweather.h"
#include "HtmlEventHandler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHtmlEventHandler::CHtmlEventHandler()
{
	Init();
}
void CHtmlEventHandler::Init()
{
	m_bStartTraceEndTag = 0;
	m_bEndTagFound = 0;
	m_szTag1Start = 0;
	m_szTag1End = 0;
	m_szTag2Start = 0;
	m_szTag2End = 0;
	
	m_iSkipNo = 0;
	m_iTagsAlreadySkipped = 0;	
}
CHtmlEventHandler::~CHtmlEventHandler()
{

}
void CHtmlEventHandler::BeginParse(DWORD dwAppData, bool &bAbort)
{

}
void CHtmlEventHandler::StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	if(!pTag)
		return;

	//TRACE(_T("Start tag detected : -> %s\r\n"),pTag->getTagName());

	if(!m_bEndTagFound){
		if (!pTag->getTagName().CompareNoCase(m_szTagName)&&!m_bStartTraceEndTag){
			CLiteHTMLAttributes* ptagAtr = (CLiteHTMLAttributes*)pTag->getAttributes();
			if(!ptagAtr)
				return;

			if(!ptagAtr->getCount())
				return;

			if(!ptagAtr->getName(0).CompareNoCase(m_szTagParam)){
				CString szParamVal = ptagAtr->getValue(0);
				if(!szParamVal.CompareNoCase(m_szTagParamValue)){
					if(m_iSkipNo!=m_iTagsAlreadySkipped){
						m_iTagsAlreadySkipped++;
					}
					else{
						m_bStartTraceEndTag = 1;
						m_szTag1Start = pTag->GetTagStartPos();
						m_szTag1End = pTag->GetTagEndPos();
					}
				}
			}

		}

		while(m_bStartTraceEndTag){
			static int iParsedTags = 0;
			iParsedTags++;
			//if (iParsedTags==118)
			//	int o=0;
			//TRACE("ParsedTags : %i %s\r\n",iParsedTags,pTag->getTagName());
			// don't add to our stack tags for which we won't receive EndTag;
			// <br> often is not inline (it doesn't finish in the same brackets)
			// and it doesn't have </br>
			// also <img> often doesn't finsish with />..like <img/>
			// http://www.freesoft.org/CIE/RFC/Orig/rfc1866.txt [tags]
			if(!pTag->getTagName().CompareNoCase(_T("img"))||
				!pTag->getTagName().CompareNoCase(_T("br"))){
				if(!pTag->IsTagInline())
					break;
			}

			m_szEnclosedTagArray =_T("/")+pTag->getTagName()+m_szEnclosedTagArray;
			//g_Global.LOG(m_szEnclosedTagArray+"\r\n","tags.txt",1);
			break;
		}
	}
}
void CHtmlEventHandler::EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	if(!pTag)
		return;

	//TRACE(_T("End tag detected : -> %s\r\n"),pTag->getTagName());


	if(m_bStartTraceEndTag){
		static int iRemovedTags = 0;
		iRemovedTags++;

#ifdef _DEBUG
// 	char pBuff[50];
// 	memcpy(&pBuff[0],pTag->GetTagStartPos(),49);
// 	pBuff[49]=0;
// 	TRACE("RemovedTags : %i %s \r\n",iRemovedTags,pBuff);
#endif
		CString szTagName = pTag->getTagName();
		if(!szTagName.IsEmpty()){
			szTagName = _T("/")+szTagName;
			CString szTempArr = m_szEnclosedTagArray;
			//||szTempArr.Replace("/"+m_szTagName,"")==1
			if(szTagName == m_szEnclosedTagArray){
				m_bEndTagFound = 1;
				m_bStartTraceEndTag = 0;
				m_szTag2End = pTag->GetTagEndPos();
				m_szTag2Start = pTag->GetTagStartPos();
			}
			if(m_szEnclosedTagArray.Find(szTagName)==0){
				m_szEnclosedTagArray = m_szEnclosedTagArray.Right(m_szEnclosedTagArray.GetLength()-szTagName.GetLength());
				//g_Global.LOG(m_szEnclosedTagArray+"\r\n","tags.txt",1);
			}
		}
	}
}

void CHtmlEventHandler::Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
{

}
void CHtmlEventHandler::Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
{

}
void CHtmlEventHandler::EndParse(DWORD dwAppData, bool bIsAborted)
{

}

void CHtmlEventHandler::InitWantedTag(CString szTag, CString szTagParam, CString szTagParamValue, int iSkipNo)
{
	Init();

	m_szTagName = szTag;
	m_szTagParam = szTagParam;
	m_szTagParamValue = szTagParamValue;
	m_iSkipNo = iSkipNo;
}

CString CHtmlEventHandler::GetOuterTagText()
{
	CString szRet;
	if(!m_bEndTagFound){
		return _T("");
	}

	szRet = CString(m_szTag1Start, m_szTag2End - m_szTag1Start);

	return szRet;
}

CString CHtmlEventHandler::GetInnerTagText()
{
	CString szRet;
	if(!m_bEndTagFound)
		return _T("");
	
	szRet = CString(m_szTag1End, m_szTag2Start - m_szTag1End);
	
	return szRet;
}


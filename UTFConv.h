#ifndef STR_H
#define STR_H
//#include <string>
//#include <iostream>
// 
// typedef std::string Str;
// typedef std::wstring WStr;
// 
// std::ostream& operator<<(std::ostream& f, const WStr& s);
// std::istream& operator>>(std::istream& f, WStr& s);
// void utf8toWStr(WStr& dest, const Str& src);
// void wstrToUtf8(Str& dest, const WStr& src);


namespace UTF8Util 
{
	CStringW ConvertUTF8ToUTF16( __in const CHAR * pszTextUTF8 );
	CStringA ConvertUTF16ToUTF8( __in const WCHAR * pszTextUTF16 );
	wchar_t *CodePageToUnicode(int codePage, const char *src);
	char *UnicodeToCodePage(int codePage, const wchar_t *src);
}; // namespace UTF8Util

#endif
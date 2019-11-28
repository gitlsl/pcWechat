#include <Windows.h>
#include <string>
#include <vector>
DWORD weChatWinAddress = 0;

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 获取WeChatWin.dll地址
 * @param  
 * @return DWORD
 */
DWORD getWeChatWinAddr()
{
	if (weChatWinAddress == 0)
	{
		weChatWinAddress = (DWORD)LoadLibrary(L"WeChatWin.dll");
	}
	return weChatWinAddress;
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 编码转换
 * @param const wchar_t* unicode
 * @return char*
 */
char* UnicodeToUtf8(const wchar_t* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	char* szUtf8 = (char*)malloc(len + 1);
	if (szUtf8 != 0) {
		memset(szUtf8, 0, len + 1);
	}
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 编码转换
 * @param const char* str
 * @return wchar_t*
 */
wchar_t* UTF8ToUnicode(const char* str)
{
	int    textlen = 0;
	wchar_t* result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t*)malloc((textlen + 1) * sizeof(wchar_t));
	if (result != 0)
	{
		memset(result, 0, (textlen + 1) * sizeof(wchar_t));
	}
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
	return    result;
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 编码转换
 * @param std::string sInfo, INT sourceEncode, INT targetEncode
 * @return std::string
 */
std::string EncodeConvert(std::string sInfo, INT sourceEncode, INT targetEncode)
{
	const char* buf = sInfo.c_str();
	int len = MultiByteToWideChar(sourceEncode, 0, buf, -1, NULL, 0);
	std::vector<wchar_t> unicode(len);
	MultiByteToWideChar(sourceEncode, 0, buf, -1, &unicode[0], len);
	std::wstring wstr(&unicode[0]);

	const wchar_t* wbuf = wstr.c_str();
	len = WideCharToMultiByte(targetEncode, 0, wbuf, -1, NULL, 0, NULL, NULL);
	std::vector<char> multi(len);
	WideCharToMultiByte(targetEncode, 0, wbuf, -1, &multi[0], len, NULL, NULL);
	std::string sMulti(&multi[0]);
	return sMulti;
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 编码转换
 * @param std::string str
 * @return wchar_t*
 */
wchar_t* StrToWchar(std::string str)
{
	int strSize = (int)(str.length() + 1);
	wchar_t* wStr = new wchar_t[strSize];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wStr, strSize);
	return wStr;
	delete[] wStr;
}
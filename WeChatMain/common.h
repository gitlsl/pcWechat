#include <Windows.h>
#include <string>

DWORD getWeChatWinAddr();
VOID setGlobalHwnd(HWND hwnd);
HWND getGlobalHwnd();
char* UnicodeToUtf8(const wchar_t* unicode);
wchar_t* UTF8ToUnicode(const char* str);
std::string EncodeConvert(std::string sInfo, INT sourceEncode, INT targetEncode);
wchar_t* StrToWchar(std::string str);
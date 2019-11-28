#include <Windows.h>
#include "common.h"
#include "offset.h"
#include "resource.h"

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 获取微信登录状态
 * @param  
 * @return int
 */
int isLogin()
{
	return (int) * (int*)(getWeChatWinAddr() + ISLOGIN);
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 线程持续获取登录状态
 * @param  HWND hwnd
 * @return void
 */
void getLoginStatus(HWND hwnd)
{
	if (getWeChatWinAddr() != 0)
	{
		SetDlgItemText(hwnd, LOGIN_STATUS, isLogin() == 0 ? L"未登录" : L"已登录");
	}
	Sleep(500);
	HANDLE lThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)getLoginStatus, hwnd, NULL, 0);
	if (lThread != 0) {
		CloseHandle(lThread);
	}
}
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include "resource.h"
#include "struct.h"
#include "common.h"
#include "MyInfo.h"
#include "Login.h"
using namespace std;

HWND hwnd;
DWORD WINAPI ShowDialog(
	_In_ HMODULE hModule
);
INT_PTR CALLBACK DialogProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 入口函数
 * @param HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved
 * @return BOOL
 */
BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		HANDLE lThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShowDialog, hModule, NULL, 0);
		if (lThread != 0) {
			CloseHandle(lThread);
		}
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 线程启动窗口，防止微信卡顿
 * @param _In_ HMODULE hModule
 * @return DWORD
 */
DWORD WINAPI ShowDialog(
	_In_ HMODULE hModule
)
{
	DialogBox(hModule, MAKEINTRESOURCE(WECHAT_MAIN), NULL, &DialogProc);
	return TRUE;
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 显示窗口
 * @param _In_ HWND   hwndDlg,_In_ UINT   uMsg,_In_ WPARAM wParam,_In_ LPARAM lParam
 * @return INT_PTR
 */
INT_PTR CALLBACK DialogProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	if (uMsg == WM_INITDIALOG)
	{
		hwnd = hwndDlg;
		HANDLE lThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)getLoginStatus, hwnd, NULL, 0);
		if (lThread != 0) {
			CloseHandle(lThread);
		}
	}
	if (uMsg == WM_COMMAND)
	{
		if (wParam == GET_MY_INFO)
		{
			if (isLogin() == 0)
			{
				SetDlgItemText(hwnd, MY_INFO_TEXT, L"请先登录微信");
			}
			else
			{
				Information* myInfo = GetMyInfo();
				wchar_t str[0x1500] = { 0 };
				swprintf_s(str,
					L"微信ID：%s\r\n账号：%s\r\n昵称：%s\r\n设备：%s\r\n手机号：%s\r\n邮箱：%s\r\n性别：%s\r\n国籍：%s\r\n省份：%s\r\n城市：%s\r\n签名：%s\r\n头像：%s",
					myInfo->wxid,
					myInfo->account,
					myInfo->nickname,
					myInfo->device,
					myInfo->phone,
					myInfo->email,
					myInfo->sex,
					myInfo->nation,
					myInfo->province,
					myInfo->city,
					myInfo->signName,
					myInfo->header);
				SetDlgItemText(hwndDlg, MY_INFO_TEXT, str);
			}
		}
	}
	else if (uMsg == WM_CLOSE)
	{
		EndDialog(hwndDlg, 0);
	}
	return FALSE;
}
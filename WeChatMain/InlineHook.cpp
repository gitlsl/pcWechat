#include <Windows.h>
#include <string>
#include <list>
#include "common.h"
#include "offset.h"
#include "struct.h"
#include "resource.h"
#include <CommCtrl.h>
#include "Login.h"
using namespace std;

list<DbHandle> DbHandleList;
list<friendStruct> friendList;
DWORD recieveMsgParam, recieveMsgJmpAddr, databaseJumpAddr, friendHookCall, friendJmpAddr;

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 处理拦截到的消息内容
 * @param DWORD esp
 * @return VOID
 */
VOID recieveMessageJump(DWORD esp)
{
	DWORD** msgAddress = (DWORD**)(esp + 0x20);
	recieveMsgStruct* msg = new recieveMsgStruct;
	msg->type = (int)(*((DWORD*)(**msgAddress + 0x30)));
	msg->isSelf = (int)(*((DWORD*)(**msgAddress + 0x34)));
	msg->fromWxid = (wchar_t*)(*((LPVOID*)(**msgAddress + 0x40)));
	msg->content = (wchar_t*)(*((LPVOID*)(**msgAddress + 0x68)));
	msg->senderWxid = (wchar_t*)(*((LPVOID*)(**msgAddress + 0x114)));
	msg->unkonwStr = (wchar_t*)(*((LPVOID*)(**msgAddress + 0x128)));

	HWND listHwnd = GetDlgItem(getGlobalHwnd(), RECIEVE_MSG_LIST);
	LVITEM item = { 0 };
	item.mask = LVIF_TEXT;

	wchar_t type[0x100] = { 0 };
	swprintf_s(type, L"%d", msg->type);
	item.iSubItem = 0;
	item.pszText = type;
	ListView_InsertItem(listHwnd, &item);

	wchar_t isSelf[0x100] = { 0 };
	swprintf_s(isSelf, L"%d", msg->isSelf);
	item.iSubItem = 1;
	item.pszText = isSelf;
	ListView_SetItem(listHwnd, &item);

	item.iSubItem = 2;
	item.pszText = msg->fromWxid;
	ListView_SetItem(listHwnd, &item);

	item.iSubItem = 3;
	item.pszText = msg->senderWxid;
	ListView_SetItem(listHwnd, &item);

	item.iSubItem = 4;
	item.pszText = msg->unkonwStr;
	ListView_SetItem(listHwnd, &item);

	item.iSubItem = 5;
	item.pszText = msg->content;
	ListView_SetItem(listHwnd, &item);
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 将获取到的数据库句柄压入list内
 * @param int dbAddress, int dbHandle
 * @return VOID
 */
VOID databaseHwnd(int dbAddress, int dbHandle)
{
	DbHandle Db = { 0 };
	Db.handler = dbHandle;
	sprintf_s(Db.path, "%s", (char*)dbAddress);
	DbHandleList.push_back(Db);

	HWND listHwnd = GetDlgItem(getGlobalHwnd(), DATABSE_HANDLE_LIST);
	LVITEM item = { 0 };
	item.mask = LVIF_TEXT;

	wchar_t handle[0x100] = { 0 };
	swprintf_s(handle, L"0x%08X", dbHandle);
	item.iSubItem = 0;
	item.pszText = handle;
	ListView_InsertItem(listHwnd, &item);

	wchar_t address[0x100] = { 0 };
	swprintf_s(address, L"%S", (char*)dbAddress);
	item.iSubItem = 1;
	item.pszText = address;
	ListView_SetItem(listHwnd, &item);
}

VOID showFriendList()
{
	HWND listHwnd = GetDlgItem(getGlobalHwnd(), FRIEND_LIST);
	ListView_DeleteAllItems(listHwnd);
	for (auto& fData : friendList)
	{
		LVITEM item = { 0 };
		item.mask = LVIF_TEXT;

		item.iSubItem = 0;
		item.pszText = fData.wxid;
		ListView_InsertItem(listHwnd, &item);

		item.iSubItem = 1;
		item.pszText = fData.head;
		ListView_SetItem(listHwnd, &item);

		item.iSubItem = 2;
		item.pszText = fData.account;
		ListView_SetItem(listHwnd, &item);

		item.iSubItem = 3;
		item.pszText = fData.v1;
		ListView_SetItem(listHwnd, &item);

		item.iSubItem = 4;
		item.pszText = fData.reMark;
		ListView_SetItem(listHwnd, &item);

		item.iSubItem = 5;
		item.pszText = fData.nickname;
		ListView_SetItem(listHwnd, &item);
	}
}

VOID insertFriendList(friendStruct* friendInfo)
{
	BOOL hasData = FALSE;
	for (auto& fData : friendList)
	{
		if (wcscmp(fData.wxid, friendInfo->wxid) == 0)
		{
			swprintf_s(fData.account, L"%s", friendInfo->account);
			swprintf_s(fData.nickname, L"%s", friendInfo->nickname);
			swprintf_s(fData.head, L"%s", friendInfo->head);
			swprintf_s(fData.v1, L"%s", friendInfo->v1);
			swprintf_s(fData.reMark, L"%s", friendInfo->reMark);
			hasData = TRUE;
			break;
		}
	}
	if (hasData == FALSE)
	{
		friendStruct one = { 0 };
		swprintf_s(one.wxid, L"%s", friendInfo->wxid);
		swprintf_s(one.account, L"%s", friendInfo->account);
		swprintf_s(one.nickname, L"%s", friendInfo->nickname);
		swprintf_s(one.head, L"%s", friendInfo->head);
		swprintf_s(one.v1, L"%s", friendInfo->v1);
		swprintf_s(one.reMark, L"%s", friendInfo->reMark);
		friendList.push_back(one);
	}
	showFriendList();
}

VOID parseFriendJump(DWORD esi)
{
	friendStruct* friendInfo = new friendStruct;
	swprintf_s(friendInfo->wxid, L"%s", (wchar_t*)(*((LPVOID*)(esi + 0x10))));
	swprintf_s(friendInfo->head, L"%s", (wchar_t*)(esi + 0x24));//24可能是头像
	swprintf_s(friendInfo->account, L"%s", (wchar_t*)(*((LPVOID*)(esi + 0x44))));
	swprintf_s(friendInfo->v1, L"%s", (wchar_t*)(*((LPVOID*)(esi + 0x58))));
	swprintf_s(friendInfo->reMark, L"%s", (wchar_t*)(esi + 0x78));
	if (IsBadReadPtr((wchar_t*)(*((LPVOID*)(esi + 0x8C))), sizeof(wchar_t*)))
	{
		swprintf_s(friendInfo->nickname, L"%s", (wchar_t*)(esi + 0x8C));
	}
	else
	{
		swprintf_s(friendInfo->nickname, L"%s", (wchar_t*)(*((LPVOID*)(esi + 0x8C))));
	}

	HANDLE lThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)insertFriendList, friendInfo, NULL, 0);
	if (lThread != 0) {
		CloseHandle(lThread);
	}

}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 被写入到hook点的接收消息裸函数
 */
__declspec(naked) void recieveMsgDeclspec()
{
	__asm
	{
		mov ecx, recieveMsgParam
		pushad
		push esp
		call recieveMessageJump
		add esp, 4
		popad
		jmp recieveMsgJmpAddr
	}
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 被写入到hook点的数据库句柄获取裸函数
 */
__declspec(naked) void databaseDeclspec()
{
	__asm
	{
		mov esi, dword ptr ss : [ebp - 0x14]
		add esp, 0x8
		pushad
		push[ebp - 0x14]
		push[ebp - 0x24]
		call databaseHwnd
		add esp, 8
		popad
		jmp databaseJumpAddr
	}
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 被写入到hook点的好友列表裸函数
 */
__declspec(naked) void friendDeclspec()
{
	__asm
	{
		pushad
		push esi
		call parseFriendJump
		add esp, 4
		popad
		call friendHookCall
		jmp friendJmpAddr
	}

	//DWORD r_esi;
	//__asm
	//{
	//	mov r_esi, esi
	//	pushad
	//	pushf
	//}
	//parseFriendJump(r_esi);
	//__asm
	//{
	//	popf
	//	popad
	//	call friendHookCall
	//	jmp friendJmpAddr
	//}
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 使用inLineHook拦截消息和数据库
 * @param
 * @return void
 */
void inLineHook()
{
	//获取模块地址，直到获取为止
	while (true)
	{
		if (getWeChatWinAddr() != 0)
		{
			break;
		}
		Sleep(300);
	}

	//HOOK接收消息
	DWORD recieveMsgHookAddr = getWeChatWinAddr() + RECEIVEMSGHOOKADDR;
	recieveMsgParam = getWeChatWinAddr() + RECEIVEMSGHOOKPARM;
	recieveMsgJmpAddr = recieveMsgHookAddr + 5;
	BYTE msgJmpCode[5] = { 0xE9 };
	*(DWORD*)&msgJmpCode[1] = (DWORD)recieveMsgDeclspec - recieveMsgHookAddr - 5;
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)recieveMsgHookAddr, msgJmpCode, 5, NULL);
	SetDlgItemText(getGlobalHwnd(), HOOK_MSG_STATUS, L"消息拦截已开启");

	//HOOK数据库句柄
	DWORD hookAddress = getWeChatWinAddr() + DATABASEHOOKADDR;
	databaseJumpAddr = hookAddress + 6;
	BYTE dbJmpCode[6] = { 0xE9 };
	dbJmpCode[6 - 1] = 0x90;
	*(DWORD*)&dbJmpCode[1] = (DWORD)databaseDeclspec - hookAddress - 5;
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)hookAddress, dbJmpCode, 6, NULL);
	SetDlgItemText(getGlobalHwnd(), HOOK_DATABASE_STATUS, L"数据库句柄拦截已开启");


	DWORD friendHookAddr = getWeChatWinAddr() + FRIENDHOOKADDR;
	friendHookCall = getWeChatWinAddr() + FRIENDHOOKCALL;
	friendJmpAddr = friendHookAddr + 5;
	BYTE friendJmpCode[5] = { 0xE9 };
	*(DWORD*)&friendJmpCode[1] = (DWORD)friendDeclspec - friendHookAddr - 5;
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)friendHookAddr, friendJmpCode, 5, NULL);
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 返回数据库句柄list
 * @param
 * @return list<DbHandle>
 */
list<DbHandle> getDbHandleList()
{
	return DbHandleList;
}
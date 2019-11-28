#include "common.h"
#include <Shlwapi.h>
#include "struct.h"
#pragma comment(lib,"Shlwapi.lib")

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 获取当前登录用户的个人信息
 * @param
 * @return Information* (结构体)
 */
Information* GetMyInfo()
{
	Information* info = new Information;
	//微信ID
	char wxid[0x200] = { 0 };
	sprintf_s(wxid, "%s", (char*)(*(DWORD*)(getWeChatWinAddr() + 0x1397264)));
	wcscpy_s(info->wxid, wcslen(UTF8ToUnicode(wxid)) + 1, UTF8ToUnicode(wxid));
	DWORD isOld = 0x0;
	if (!StrStrW(info->wxid, L"wxid_"))
	{
		sprintf_s(wxid, "%s", (char*)(getWeChatWinAddr() + 0x1397284));
		wcscpy_s(info->wxid, wcslen(UTF8ToUnicode(wxid)) + 1, UTF8ToUnicode(wxid));
		isOld = 0x20;
	}

	//自定义账号
	char account[100] = { 0 };
	sprintf_s(account, "%s", (char*)(getWeChatWinAddr() + 0x1397440 + isOld));
	wcscpy_s(info->account, wcslen(UTF8ToUnicode(account)) + 1, UTF8ToUnicode(account));

	//昵称
	char nickname[200] = { 0 };
	if (IsBadReadPtr((char*)(*(DWORD*)(getWeChatWinAddr() + 0x13972DC + isOld)), sizeof(char*)))
	{
		sprintf_s(nickname, "%s", (char*)(getWeChatWinAddr() + 0x13972DC + isOld));
		wcscpy_s(info->nickname, wcslen(UTF8ToUnicode(nickname)) + 1, UTF8ToUnicode(nickname));
	}
	else
	{
		sprintf_s(nickname, "%s", (char*)(*(DWORD*)(getWeChatWinAddr() + 0x13972DC + isOld)));
		wcscpy_s(info->nickname, wcslen(UTF8ToUnicode(nickname)) + 1, UTF8ToUnicode(nickname));
	}

	//登陆设备
	char device[15] = { 0 };
	sprintf_s(device, "%s", (char*)(getWeChatWinAddr() + 0x1397718 + isOld));
	wcscpy_s(info->device, wcslen(UTF8ToUnicode(device)) + 1, UTF8ToUnicode(device));

	//手机号
	char phone[40] = { 0 };
	sprintf_s(phone, "%s", (char*)(getWeChatWinAddr() + 0x1397310 + isOld));
	wcscpy_s(info->phone, wcslen(UTF8ToUnicode(phone)) + 1, UTF8ToUnicode(phone));

	//邮箱
	char email[40] = { 0 };
	sprintf_s(email, "%s", (char*)(getWeChatWinAddr() + 0x13972F8 + isOld));
	wcscpy_s(info->email, wcslen(UTF8ToUnicode(email)) + 1, UTF8ToUnicode(email));

	//性别
	char sex[8] = { 0 };
	memcpy(sex, (void*)(getWeChatWinAddr() + 0x13973C4 + isOld), 1);
	if (sex[0] == 1)
	{
		wcscpy_s(info->sex, wcslen(L"男") + 1, L"男");
	}
	else if (sex[0] == 2)
	{
		wcscpy_s(info->sex, wcslen(L"女") + 1, L"女");
	}
	else
	{
		wcscpy_s(info->sex, wcslen(L"未知") + 1, L"未知");
	}

	//国籍
	char nation[40] = { 0 };
	sprintf_s(nation, "%s", (char*)(getWeChatWinAddr() + 0x13974B8 + isOld));
	wcscpy_s(info->nation, wcslen(UTF8ToUnicode(nation)) + 1, UTF8ToUnicode(nation));

	//省份
	char province[40] = { 0 };
	sprintf_s(province, "%s", (char*)(getWeChatWinAddr() + 0x13973C8 + isOld));
	wcscpy_s(info->province, wcslen(UTF8ToUnicode(province)) + 1, UTF8ToUnicode(province));

	//城市
	char city[40] = { 0 };
	sprintf_s(city, "%s", (char*)(getWeChatWinAddr() + 0x13973E0 + isOld));
	wcscpy_s(info->city, wcslen(UTF8ToUnicode(city)) + 1, UTF8ToUnicode(city));

	//个性签名
	char signName[40] = { 0 };
	sprintf_s(signName, "%s", (char*)(*(DWORD*)(getWeChatWinAddr() + 0x13973F8 + isOld)));
	wcscpy_s(info->signName, wcslen(UTF8ToUnicode(signName)) + 1, UTF8ToUnicode(signName));

	//头像
	char header[0x200] = { 0 };
	sprintf_s(header, "%s", (char*)(*(DWORD*)(getWeChatWinAddr() + 0x13975A4 + isOld)));
	wcscpy_s(info->header, wcslen(UTF8ToUnicode(header)) + 1, UTF8ToUnicode(header));

	return info;
}
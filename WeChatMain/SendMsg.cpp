#include <Windows.h>
#include <string>
#include "common.h"
#include "offset.h"
using namespace std;

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 发送文本消息
 * @param wchar_t* wxid, string content
 * @return void
 */
void SendTextMsg(wchar_t* wxid, wchar_t* msg)
{
	struct wxMsg
	{
		wchar_t* pMsg;		//微信ID
		int msgLen;			//ID长度
		int buffLen;		//缓冲区长度
		int fill1 = 0;
		int fill2 = 0;
	};
	//拿到发送消息的call的地址
	DWORD dwSendCallAddr = getWeChatWinAddr() + SENDTEXTADDR;

	//组装微信ID/群ID的结构体
	wxMsg id = { 0 };
	id.pMsg = wxid;
	id.msgLen = wcslen(wxid);
	id.buffLen = wcslen(wxid) * 2;

	//消息内容
	wxMsg text = { 0 };
	text.pMsg = msg;
	text.msgLen = wcslen(msg);
	text.buffLen = wcslen(msg) * 2;

	//取出微信ID和消息的地址
	char* pWxid = (char*)&id.pMsg;
	char* pWxmsg = (char*)&text.pMsg;

	char buff[0x81C] = { 0 };

	//调用微信发送消息call
	__asm {
		mov edx, pWxid;
		push 0x1;
		push 0;
		mov ebx, pWxmsg;
		push ebx;
		lea ecx, buff;
		call dwSendCallAddr;
		add esp, 0xC;
	}
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 发送图片文件
 * @param wchar_t* wxid, string content
 * @return void
 */
void SendImageMsg(wchar_t* wxid, wchar_t* filepath)
{
	struct SendImageStruct
	{
		wchar_t* pWxid = nullptr;
		DWORD length = 0;
		DWORD maxLength = 0;
		DWORD fill1 = 0;
		DWORD fill2 = 0;
		wchar_t wxid[1024] = { 0 };
	};

	//组装微信ID的数据结构
	SendImageStruct imagewxid;
	memcpy(imagewxid.wxid, wxid, wcslen(wxid) + 1);
	imagewxid.length = wcslen(wxid) + 1;
	imagewxid.maxLength = wcslen(wxid) * 2;
	imagewxid.pWxid = wxid;

	//组装文件路径的数据结构
	SendImageStruct imagefilepath;
	memcpy(imagefilepath.wxid, filepath, wcslen(filepath) + 1);
	imagefilepath.length = wcslen(filepath) + 1;
	imagefilepath.maxLength = MAX_PATH;
	imagefilepath.pWxid = filepath;

	char buff[0x45C] = { 0 };

	DWORD dwCall1 = getWeChatWinAddr() + SENDIMGADDR1;
	DWORD dwCall2 = getWeChatWinAddr() + SENDIMGADDR2;
	DWORD myEsp = 0;


	//微信发送图片调用了GidCreateBimapFileCM 之后图片一直被占用 无法删除 patch掉下面的代码
	unsigned char oldcode[5] = { 0 };
	unsigned char fix[5] = { 0x31,0xC0,0x90,0x90,0x90 };
	DWORD dwPathcAddr = getWeChatWinAddr() + SENDIMGADDR3;
	//修改代码段属性
	DWORD dwOldAttr = 0;
	VirtualProtect((LPVOID)dwPathcAddr, 5, PAGE_EXECUTE_READWRITE, &dwOldAttr);
	//先保存原有指令
	memcpy(oldcode, (LPVOID)dwPathcAddr, 5);
	//再Patch掉
	memcpy((LPVOID)dwPathcAddr, fix, 5);
	__asm
	{
		//保存esp
		mov myEsp, esp;

		lea ebx, imagefilepath;
		push ebx;
		lea eax, imagewxid;
		push eax;
		lea eax, buff;
		push eax;
		push 0;
		call dwCall1;
		add esp, 0x4;
		mov ecx, eax;
		call dwCall2;
		//恢复堆栈
		mov eax, myEsp;
		mov esp, eax;
	}
	//调用完之后恢复
	memcpy((LPVOID)dwPathcAddr, oldcode, 5);
	//恢复属性
	VirtualProtect((LPVOID)dwPathcAddr, 5, dwOldAttr, &dwOldAttr);
}

/**
 * 作者QQ：50728123
 * 交流群：810420984
 * 作者微信：codeByDog
 * 发送附件文件
 * @param wchar_t* wxid, string content
 * @return void
 */
void SendAttachMsg(wchar_t* wxid, wchar_t* filepath)
{
	//微信ID的结构体
	struct Wxid
	{
		wchar_t* str;
		int strLen = 0;
		int maxLen = 0;
		char file[0x8] = { 0 };
	};

	//文件路径的结构体
	struct filePath
	{
		wchar_t* str;
		int strLen = 0;
		int maxLen = 0;
		char file[0x18] = { 0 };
	};

	//构造需要的地址
	DWORD dwCall1 = getWeChatWinAddr() + SENDATTACHADDR1;
	DWORD dwCall2 = getWeChatWinAddr() + SENDATTACHADDR2;
	DWORD dwCall3 = getWeChatWinAddr() + SENDATTACHADDR3;	//组合数据
	DWORD dwCall4 = getWeChatWinAddr() + SENDATTACHADDR4;	//发送消息
	DWORD dwParams = getWeChatWinAddr() + SENDATTACHPARAM;

	char buff[0x45C] = { 0 };

	//构造需要的数据
	Wxid wxidStruct = { 0 };
	wxidStruct.str = wxid;
	wxidStruct.strLen = wcslen(wxid);
	wxidStruct.maxLen = wcslen(wxid) * 2;

	filePath filePathStruct = { 0 };
	filePathStruct.str = filepath;
	filePathStruct.strLen = wcslen(filepath);
	filePathStruct.maxLen = wcslen(filepath) * 2;

	//取出需要的数据的地址
	char* pFilePath = (char*)&filePathStruct.str;
	char* pWxid = (char*)&wxidStruct.str;

	__asm {
		push dword ptr ss : [ebp - 0x5C] ;
		sub esp, 0x14;
		mov ecx, esp;
		push - 0x1;
		push dwParams;
		call dwCall1;
		sub esp, 0x14;
		mov ecx, esp;
		push pFilePath;
		call dwCall2;
		sub esp, 0x14;
		mov ecx, esp;
		push pWxid;
		call dwCall2;
		lea eax, buff;
		push eax;
		call dwCall3;
		mov ecx, eax;
		call dwCall4;
	}
}
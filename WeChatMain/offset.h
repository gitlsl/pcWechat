#pragma once
#define ISLOGIN 0x13973B4					//是否登录标识，0-未登录|1-已登录

#define RECEIVEMSGHOOKADDR 0x325373			//HOOK消息的内存地址偏移
#define RECEIVEMSGHOOKPARM 0x13971B8		//HOOK消息需要PUSH的参数偏移

#define SENDTEXTADDR 0x2FA7F0				//发送文本消息ecx参数偏移

#define SENDIMGADDR1 0x85710				//发送图片文件call地址偏移
#define SENDIMGADDR2 0x2FA1E0				//发送图片文件call地址偏移
#define SENDIMGADDR3 0x2F6FB5				//因微信发送图片会占用图片，该偏移是为了释放图片占用

#define DATABASEHOOKADDR 0x4497F3			//数据库句柄HOOK偏移

#define FRIENDHOOKADDR 0x4418B5				//好友列表HOOK点偏移
#define FRIENDHOOKCALL 0x4A4B70				//好友列表HOOK点CALL偏移

#define SENDATTACHADDR1 0x4A4510
#define SENDATTACHADDR2 0x4A4550
#define SENDATTACHADDR3 0x6DFA0
#define SENDATTACHADDR4 0x242EC0
#define SENDATTACHPARAM 0x10FD310
/******************************************************************************

                  版权所有 (C), 2001-2012, 华为技术有限公司

 ******************************************************************************
  文 件 名   : bsp_mailbox.h
  版 本 号   : 初稿
  作    者   : 苏庄銮 59026
  生成日期   : 2012年12月17日
  最近修改   :
  功能描述   : mailbox 底软子系统内部头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年12月17日
    作    者   : 苏庄銮 59026
    修改内容   : 创建文件

******************************************************************************/
#ifndef __BSP_MAILBOX_H__
#define __BSP_MAILBOX_H__


/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "drv_mailbox.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 宏定义
*****************************************************************************/
/*生成远程调用发起函数*/
#define IFC_GEN_CALL0(id, name)       _IFC_GEN_CALLx(0, id, name)
#define IFC_GEN_CALL1(id, name, ...)  _IFC_GEN_CALLx(1, id, name, __VA_ARGS__)
#define IFC_GEN_CALL2(id, name, ...)  _IFC_GEN_CALLx(2, id, name, __VA_ARGS__)
#define IFC_GEN_CALL3(id, name, ...)  _IFC_GEN_CALLx(3, id, name, __VA_ARGS__)
#define IFC_GEN_CALL4(id, name, ...)  _IFC_GEN_CALLx(4, id, name, __VA_ARGS__)
#define IFC_GEN_CALL5(id, name, ...)  _IFC_GEN_CALLx(5, id, name, __VA_ARGS__)
#define IFC_GEN_CALL6(id, name, ...)  _IFC_GEN_CALLx(6, id, name, __VA_ARGS__)

/*生成带各个参数的远程调用执行函数*/
/*注意执行函数name必须是有返回值的(不能是void型，否则编译不过)*/
#define IFC_GEN_EXEC0(name)      __IFC_GEN_EXECx(0, name)
#define IFC_GEN_EXEC1(name, ...) __IFC_GEN_EXECx(1, name, __VA_ARGS__)
#define IFC_GEN_EXEC2(name, ...) __IFC_GEN_EXECx(2, name, __VA_ARGS__)
#define IFC_GEN_EXEC3(name, ...) __IFC_GEN_EXECx(3, name, __VA_ARGS__)
#define IFC_GEN_EXEC4(name, ...) __IFC_GEN_EXECx(4, name, __VA_ARGS__)
#define IFC_GEN_EXEC5(name, ...) __IFC_GEN_EXECx(5, name, __VA_ARGS__)
#define IFC_GEN_EXEC6(name, ...) __IFC_GEN_EXECx(6, name, __VA_ARGS__)

/*****************************************************************************
  3 枚举定义
*****************************************************************************/


/*****************************************************************************
  4 消息头定义
*****************************************************************************/


/*****************************************************************************
  5 消息定义
*****************************************************************************/


/*****************************************************************************
  6 STRUCT定义
*****************************************************************************/


/*****************************************************************************
  7 UNION定义
*****************************************************************************/

/*****************************************************************************
  8 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  9 全局变量声明
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/

/*注册远程调用执行函数*/
#define mailbox_ifc_register(id, name) mailbox_ifc_register_exec(id ,IFC_GEN_EXEC_NAME(name))

#define IFC_REG_EXEC(id, name) mailbox_ifc_register(id, name)

/*****************************************************************************
 函 数 名  : mailbox_init
 功能描述  : 初始化跨核邮箱, 由系统初始化时调用
 输入参数  : 无
 输出参数  : 无
 返 回 值  : MAILBOX_OK / MAILBOX_ERR
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月24日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/
unsigned int mailbox_init(void);


/*****************************************************************************
 函 数 名  : mailbox_send_msg
 功能描述  : 邮箱用户发送邮件数据
 输入参数  : MailCode   -- 邮编, 指定邮件发送目标
             pData      -- 指向邮件数据的指针
             Length     -- 待发送数据长度, 单位byte
 输出参数  : 无
 返 回 值  : 正确:MAILBOX_OK / 错误:MAILBOX_ERRO或者其他错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月28日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/
unsigned int mailbox_send_msg(
                unsigned int            mailcode,
                void                    *data,
                unsigned int            length);

/*****************************************************************************
 函 数 名  : mailbox_reg_msg_cb
 功能描述  : 注册邮件数据接收函数
 输入参数  : mailcode   -- 数据接收函数处理的对象邮编
             func       -- 邮件数据接收处理函数指针
             data -- 用户定义句柄, 数据接收函数被调用时会作为入参, 无则填空
 输出参数  : 无
 返 回 值  : 正确:MAILBOX_OK / 错误:MAILBOX_ERRO或者其他错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年9月29日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/
unsigned int  mailbox_reg_msg_cb(
                unsigned int             mailcode,
                mb_msg_cb                 func,
                void                     *data);

/*****************************************************************************
 函 数 名  : mailbox_read_msg_data
 功能描述  : 邮箱用户在数据接收回调函数中调用, 从邮箱中读取一封最先到达的邮件
 输入参数  : MailHandle -- 邮箱句柄, 数据接收回调函数入参
             pData      -- 保存待读出数据的缓存地址
             *pSize     -- 缓存长度, 单位byte, 需要大于待读取邮件长度
 输出参数  : *pSize     -- 实际读取长度, 单位byte
 返 回 值  : 正确:MAILBOX_OK / 错误:MAILBOX_ERRO或者其他错误码
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年10月26日
    作    者   : 莫南 00176101
    修改内容   : 新生成函数

*****************************************************************************/
unsigned int mailbox_read_msg_data(
                void                   *MailHandle,
                 char                 *pData,
                unsigned int          *pSize);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of bsp_mailbox.h */


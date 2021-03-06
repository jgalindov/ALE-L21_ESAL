#include "gtest/gtest.h"
#include "llt_mockcpp.h"

#include "uttest_AtCmdMsgProc.h"


//建议这样引用，避免下面用关键字时需要加前缀 testing::
using namespace testing;

//说明：经过改造后的gtest+，测试套和测试用例名字不允许为空
//      请避免用和gtest相同的关键字，避免不必要的麻烦
//      gtest提供 EXPECT 和 ASSERT 两套,具体含义可以看胶片，两套宏都是一一对应的，下面以EXPECT为例

//1、比对布尔类型 EXPECT_TRUE　EXPECT_FALSE/ASSERT_TRUE
//2、比较整形 EXPECT_EQ EXPECT_NE
//3、比较字符串 EXPECT_STREQ
//4、设置失败输出信息 ADD_FAILUREd
//5、设置成功和失败 SUCCEED FAIL
//6、自定义格式 EXPECT_PRED_FORMAT3
//7、期望带入参数返回TRUE EXPECT_PRED1
//8、浮点型比较 EXPECT_FLOAT_EQ EXPECT_PRED_FORMAT2 ASSERT_PRED_FORMAT2
//9、类型检查 StaticAssertTypeEq
//10、设置3种测试级别事件
//11、获取测试用例名字 test_case_name()
//12、比较异常 EXPECT_THROW
//13、结构体类型比较ASSERT_SAME_DATA ASSERT_SAME_MEMORY

typedef struct
{
    VOS_MSG_HEADER
    TAF_UINT8   ucType;
    VOS_UINT8   ucUserType;
    TAF_UINT8   ucIndex;
    VOS_UINT8   aucReserved[3];
    TAF_UINT16 usLen;
    TAF_UINT8 aucValue[100];
}TEST_AT_MSG_STRU;

extern AT_PARSE_CONTEXT_STRU g_stParseContext[AT_MAX_CLIENT_NUM];
AT_PAR_CMD_ELEMENT_STRU             gstCcfcCmdInfo = {AT_CMD_CCFC,
    At_SetCcfcPara,     AT_SET_PARA_TIME, TAF_NULL_PTR,     AT_NOT_SET_TIME,    At_TestCcfcPara, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
    (VOS_UINT8*)"+CCFC", (VOS_UINT8*)"(0-5),(0-4),(number),(0-255),(1-255),(subaddr),(0-255),(1-30)"};

AT_PAR_CMD_ELEMENT_STRU             gstCmgdCmdInfo = {AT_CMD_CMGD,
    At_SetCmgdPara,     AT_SET_PARA_TIME, TAF_NULL_PTR,     AT_NOT_SET_TIME ,   At_QryCmgdPara, AT_SET_PARA_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CMS_OPERATION_NOT_ALLOWED, CMD_TBL_LIMITED_NULL,
    (VOS_UINT8*)"+CMGD",    (VOS_UINT8*)"(0-254),(0-4)"};


/* Modified by f62575 for B050 Project, 2012-2-3, Begin   */

AT_PAR_CMD_ELEMENT_STRU g_astAtDeviceCmdCsdFltTbl = {AT_CMD_CSDFLT,
At_SetCsdfltPara,    AT_NOT_SET_TIME,    At_QryCsdfltPara,      AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
VOS_NULL_PTR,   AT_NOT_SET_TIME,
AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
(VOS_UINT8*)"^CSDFLT",   (VOS_UINT8*)"(0,1)"};

extern "C" VOS_UINT32 AT_ProcRabmReleaseRrcCnf(RABM_AT_RELEASE_RRC_CNF_STRU *pstMsg);

/*****************************************************************************
类名     : Test_AT_ProcRabmSetFastDormParaCnf
功能描述 : Test_AT_ProcRabmSetFastDormParaCnf UT工程类
修改历史     :
#  1.日    期: 2012年03月21日
#    作    者: l60609
#    修改内容: 新建CASE
*****************************************************************************/
class Test_AT_ProcRabmSetFastDormParaCnf: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }
    void TearDown()
    {

    }
};

/*******************************************************************
测试用例编号      : Test_AT_ProcRabmSetFastDormParaCnf_001
测试用例标题      : AT处理ID_RABM_AT_SET_FASTDORM_PARA_CNF消息
预期结果          : 输出AT_OK
修改历史          :
#  1.日    期: 2012年03月21日
#    作    者: l60609
#    修改内容: 新建CASE
*******************************************************************/
TEST_F(Test_AT_ProcRabmSetFastDormParaCnf, Test_AT_ProcRabmSetFastDormParaCnf_001)
{
    RABM_AT_SET_FASTDORM_PARA_CNF_STRU *pstMsg = VOS_NULL_PTR;                  /* 定义原语类型指针 */

    /* 申请内存  */
    pstMsg = (RABM_AT_SET_FASTDORM_PARA_CNF_STRU *)PS_ALLOC_MSG(WUEPS_PID_RABM,
                                               sizeof(RABM_AT_SET_FASTDORM_PARA_CNF_STRU) - VOS_MSG_HEAD_LENGTH);
    if ( VOS_NULL_PTR == pstMsg )
    {
        return ;
    }

    /* 填写相关参数 */
    pstMsg->stMsgHeader.ulSenderPid       = WUEPS_PID_RABM;
    pstMsg->stMsgHeader.ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulReceiverPid     = WUEPS_PID_AT;
    pstMsg->stMsgHeader.ulLength          = sizeof(RABM_AT_SET_FASTDORM_PARA_CNF_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg->stMsgHeader.ulMsgName         = ID_RABM_AT_SET_FASTDORM_PARA_CNF;
    pstMsg->enRslt                        = AT_RABM_PARA_SET_RSLT_SUCC;
    pstMsg->usClientId                    = 0;
    pstMsg->ucOpId                        = 0;

    gastAtClientTab[0].usClientId = 0;
    gastAtClientTab[0].ucUsed = AT_CLIENT_USED;

    At_MsgProc((MsgBlock *)pstMsg);

    //检查调用时, 需要在用例结束位置加上这句
    GlobalMockObject::verify();
}

/*****************************************************************************
类名     : Test_AT_ProcRabmGetFastDormParaCnf
功能描述 : Test_AT_ProcRabmGetFastDormParaCnf UT工程类
修改历史     :
#  1.日    期: 2012年03月21日
#    作    者: l60609
#    修改内容: 新建CASE
*****************************************************************************/
class Test_AT_ProcRabmGetFastDormParaCnf: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }
    void TearDown()
    {

    }
};

/*******************************************************************
测试用例编号      : Test_AT_ProcRabmGetFastDormParaCnf_001
测试用例标题      : AT处理ID_RABM_AT_GET_FASTDORM_PARA_CNF消息，结果为VOS_TRUE
预期结果          : 输出AT_OK
修改历史          :
#  1.日    期: 2012年03月21日
#    作    者: l60609
#    修改内容: 新建CASE
*******************************************************************/
TEST_F(Test_AT_ProcRabmGetFastDormParaCnf, Test_AT_ProcRabmGetFastDormParaCnf_001)
{
    RABM_AT_QRY_FASTDORM_PARA_CNF_STRU *pstMsg = VOS_NULL_PTR;                  /* 定义原语类型指针 */

    /* 申请内存  */
    pstMsg = (RABM_AT_QRY_FASTDORM_PARA_CNF_STRU *)PS_ALLOC_MSG(WUEPS_PID_RABM,
                                               sizeof(RABM_AT_QRY_FASTDORM_PARA_CNF_STRU) - VOS_MSG_HEAD_LENGTH);
    if ( VOS_NULL_PTR == pstMsg )
    {
        /* 内存申请失败 */
        return ;
    }

    /* 填写相关参数 */
    pstMsg->stMsgHeader.ulSenderPid       = WUEPS_PID_RABM;
    pstMsg->stMsgHeader.ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulReceiverPid     = WUEPS_PID_AT;
    pstMsg->stMsgHeader.ulLength          = sizeof(RABM_AT_QRY_FASTDORM_PARA_CNF_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg->stMsgHeader.ulMsgName         = ID_RABM_AT_QRY_FASTDORM_PARA_CNF;
    pstMsg->usClientId                    = 0;
    pstMsg->ucOpId                        = 0;
    pstMsg->ucRslt                        = VOS_TRUE;
    pstMsg->stFastDormPara.enFastDormOperationType = 3;
    pstMsg->stFastDormPara.ulTimeLen = 3;


    gastAtClientTab[0].usClientId = 0;
    gastAtClientTab[0].ucUsed = AT_CLIENT_USED;

    At_MsgProc((MsgBlock *)pstMsg);

    //检查调用时, 需要在用例结束位置加上这句
    GlobalMockObject::verify();
}

/*******************************************************************
测试用例编号      : Test_AT_ProcRabmGetFastDormParaCnf_002
测试用例标题      : AT处理ID_RABM_AT_GET_FASTDORM_PARA_CNF消息，结果为VOS_FALSE
预期结果          : 输出AT_ERROR
修改历史          :
#  1.日    期: 2012年03月21日
#    作    者: l60609
#    修改内容: 新建CASE
*******************************************************************/
TEST_F(Test_AT_ProcRabmGetFastDormParaCnf, Test_AT_ProcRabmGetFastDormParaCnf_002)
{
    RABM_AT_QRY_FASTDORM_PARA_CNF_STRU *pstMsg = VOS_NULL_PTR;                  /* 定义原语类型指针 */

    /* 申请内存  */
    pstMsg = (RABM_AT_QRY_FASTDORM_PARA_CNF_STRU *)PS_ALLOC_MSG(WUEPS_PID_RABM,
                                               sizeof(RABM_AT_QRY_FASTDORM_PARA_CNF_STRU) - VOS_MSG_HEAD_LENGTH);
    if ( VOS_NULL_PTR == pstMsg )
    {
        /* 内存申请失败 */
        return ;
    }

    /* 填写相关参数 */
    pstMsg->stMsgHeader.ulSenderPid       = WUEPS_PID_RABM;
    pstMsg->stMsgHeader.ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulReceiverPid     = WUEPS_PID_AT;
    pstMsg->stMsgHeader.ulLength          = sizeof(RABM_AT_QRY_FASTDORM_PARA_CNF_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg->stMsgHeader.ulMsgName         = ID_RABM_AT_QRY_FASTDORM_PARA_CNF;
    pstMsg->usClientId                    = 0;
    pstMsg->ucOpId                        = 0;
    pstMsg->ucRslt                        = VOS_FALSE;
    pstMsg->stFastDormPara.enFastDormOperationType = 3;
    pstMsg->stFastDormPara.ulTimeLen = 3;


    gastAtClientTab[0].usClientId = 0;
    gastAtClientTab[0].ucUsed = AT_CLIENT_USED;

    At_MsgProc((MsgBlock *)pstMsg);

    //检查调用时, 需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
/*****************************************************************************
类名     : Test_AT_RcvDrvAgentSetSecuBootRsp
功能描述 : Test_AT_RcvDrvAgentSetSecuBootRsp UT工程类
修改历史     :
#  1.日    期: 2012年02月09日
#    作    者: f62575
#    修改内容: 新建CASE
*****************************************************************************/
class Test_AT_RcvDrvAgentSetSecuBootRsp: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }
    void TearDown()
    {

    }
};

/*******************************************************************
测试用例编号      : Test_AT_RcvDrvAgentSetSecuBootRsp_001
测试用例标题      : 硬件加密启用成功
预期结果          : 输出AT_OK
修改历史          :
#  1.日    期: 2012年02月09日
#    作    者: f62575
#    修改内容: 新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentSetSecuBootRsp, Test_AT_RcvDrvAgentSetSecuBootRsp_001)
{
    DRV_AGENT_SECUBOOT_SET_CNF_STRU     *pstSecuBootCnf;
    DRV_AGENT_MSG_STRU                  *pstRcvMsg;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulRet;

    //参数初始化
    ucIndex = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_SECUBOOT_SET;

    pstRcvMsg = (DRV_AGENT_MSG_STRU *)malloc(sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_SECUBOOT_SET_CNF_STRU));
    if (VOS_NULL_PTR == pstRcvMsg)
    {
        return;
    }

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_OK));

    pstSecuBootCnf = (DRV_AGENT_SECUBOOT_SET_CNF_STRU *)pstRcvMsg->aucContent;
    pstSecuBootCnf->bFail = VOS_FALSE;

    ulRet = AT_RcvDrvAgentSetSecuBootRsp(pstRcvMsg);


    // 执行检查
    ASSERT_EQ(VOS_OK, ulRet);
    free(pstRcvMsg);

    GlobalMockObject::verify();

}

/*******************************************************************
测试用例编号      : Test_AT_RcvDrvAgentSetSecuBootRsp_002
测试用例标题      : 硬件加密启用失败
预期结果          : 输出AT_ERROR
修改历史          :
#  1.日    期: 2012年02月09日
#    作    者: f62575
#    修改内容: 新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentSetSecuBootRsp, Test_AT_RcvDrvAgentSetSecuBootRsp_002)
{
    DRV_AGENT_SECUBOOT_SET_CNF_STRU     *pstSecuBootCnf;
    DRV_AGENT_MSG_STRU                  *pstRcvMsg;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulRet;

    //参数初始化
    ucIndex = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_SECUBOOT_SET;

    pstRcvMsg = (DRV_AGENT_MSG_STRU *)malloc(sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_SECUBOOT_SET_CNF_STRU));
    if (VOS_NULL_PTR == pstRcvMsg)
    {
        return;
    }

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), eq((VOS_UINT32)AT_ERROR));

    pstSecuBootCnf = (DRV_AGENT_SECUBOOT_SET_CNF_STRU *)pstRcvMsg->aucContent;
    pstSecuBootCnf->bFail = VOS_TRUE;

    ulRet = AT_RcvDrvAgentSetSecuBootRsp(pstRcvMsg);


    // 执行检查
    ASSERT_EQ(VOS_OK, ulRet);
    free(pstRcvMsg);

    GlobalMockObject::verify();

}

/*******************************************************************
测试用例编号      : Test_AT_RcvDrvAgentSetSecuBootRsp_003
测试用例标题      : 获取ucIndex失败
预期结果          : 输出打印告警
修改历史          :
#  1.日    期: 2012年02月09日
#    作    者: f62575
#    修改内容: 新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentSetSecuBootRsp, Test_AT_RcvDrvAgentSetSecuBootRsp_003)
{
    DRV_AGENT_SECUBOOT_SET_CNF_STRU     *pstSecuBootCnf;
    DRV_AGENT_MSG_STRU                  *pstRcvMsg;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulRet;

    //参数初始化

    ucIndex = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_SECUBOOT_SET;

    pstRcvMsg = (DRV_AGENT_MSG_STRU *)malloc(sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_SECUBOOT_SET_CNF_STRU));
    if (VOS_NULL_PTR == pstRcvMsg)
    {
        return;
    }

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    pstSecuBootCnf = (DRV_AGENT_SECUBOOT_SET_CNF_STRU *)pstRcvMsg->aucContent;
    pstSecuBootCnf->bFail = VOS_TRUE;

    ulRet = AT_RcvDrvAgentSetSecuBootRsp(pstRcvMsg);


    // 执行检查
    ASSERT_EQ(VOS_ERR, ulRet);
    free(pstRcvMsg);

    GlobalMockObject::verify();

}

/*******************************************************************
测试用例编号      : Test_AT_RcvDrvAgentSetSecuBootRsp_004
测试用例标题      : 获取ucIndex为AT_BROADCAST_CLIENT_INDEX_MODEM_0
预期结果          : 输出打印告警
修改历史          :
#  1.日    期: 2012年02月09日
#    作    者: f62575
#    修改内容: 新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentSetSecuBootRsp, Test_AT_RcvDrvAgentSetSecuBootRsp_004)
{
    DRV_AGENT_SECUBOOT_SET_CNF_STRU     *pstSecuBootCnf;
    DRV_AGENT_MSG_STRU                  *pstRcvMsg;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulRet;

    //参数初始化

    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    //gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_SECUBOOT_SET;

    pstRcvMsg = (DRV_AGENT_MSG_STRU *)malloc(sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_SECUBOOT_SET_CNF_STRU));
    if (VOS_NULL_PTR == pstRcvMsg)
    {
        return;
    }

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    pstSecuBootCnf = (DRV_AGENT_SECUBOOT_SET_CNF_STRU *)pstRcvMsg->aucContent;
    pstSecuBootCnf->bFail = VOS_TRUE;

    ulRet = AT_RcvDrvAgentSetSecuBootRsp(pstRcvMsg);


    // 执行检查
    ASSERT_EQ(VOS_ERR, ulRet);
    free(pstRcvMsg);

    GlobalMockObject::verify();

}

/*******************************************************************
测试用例编号      : Test_AT_RcvDrvAgentSetSecuBootRsp_005
测试用例标题      : AT模块等待状态不是AT_CMD_SECUBOOT_SET
预期结果          : 输出打印告警
修改历史          :
#  1.日    期: 2012年02月09日
#    作    者: f62575
#    修改内容: 新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentSetSecuBootRsp, Test_AT_RcvDrvAgentSetSecuBootRsp_005)
{
    DRV_AGENT_SECUBOOT_SET_CNF_STRU     *pstSecuBootCnf;
    DRV_AGENT_MSG_STRU                  *pstRcvMsg;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulRet;

    //参数初始化


    ucIndex = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_SECUBOOTFEATURE_QRY;

    pstRcvMsg = (DRV_AGENT_MSG_STRU *)malloc(sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_SECUBOOT_SET_CNF_STRU));
    if (VOS_NULL_PTR == pstRcvMsg)
    {
        return;
    }

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(At_FormatResultData)
        .expects(never());

    pstSecuBootCnf = (DRV_AGENT_SECUBOOT_SET_CNF_STRU *)pstRcvMsg->aucContent;
    pstSecuBootCnf->bFail = VOS_TRUE;

    ulRet = AT_RcvDrvAgentSetSecuBootRsp(pstRcvMsg);


    // 执行检查
    ASSERT_EQ(VOS_ERR, ulRet);
    free(pstRcvMsg);

    GlobalMockObject::verify();

}
/* Modified by f62575 for B050 Project, 2012-2-3, End   */

class Test_At_MsgProc: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();
    }

    void TearDown()
    {
    }
};


TEST_F(Test_At_MsgProc,Test_At_MsgProc_001)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = VOS_PID_TIMER;

    // MOCKER操作;
    MOCKER(At_TimeOutProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_003)
{
    // 参数定义
    AT_MSG_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = WUEPS_PID_AT;

    // MOCKER操作;
    MOCKER(At_CmdMsgDistr)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_004)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I0_WUEPS_PID_TAF;

    // MOCKER操作;
    MOCKER(At_EventMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_005)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I1_WUEPS_PID_TAF;

    // MOCKER操作;
    MOCKER(At_EventMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}

TEST_F(Test_At_MsgProc,Test_At_MsgProc_015)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  *pMsg   = VOS_NULL_PTR;
    TAF_SS_CALL_INDEPENDENT_EVENT_STRU  *pEvent = VOS_NULL_PTR;
    VOS_UINT8                            ucIndex;
    VOS_UINT32                           ulLength;
    VOS_CHAR                            *pcExpactedStr3;

    ulLength = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(TAF_SS_CALL_INDEPENDENT_EVENT_STRU);
    pMsg = (MN_AT_IND_EVT_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_TAF, ulLength);

    PS_MEM_SET(pMsg, 0, ulLength);
    pMsg->ulSenderCpuId               = VOS_LOCAL_CPUID;
    pMsg->ulSenderPid                 = WUEPS_PID_TAF;
    pMsg->ulReceiverCpuId             = VOS_LOCAL_CPUID;
    pMsg->ulReceiverPid               = WUEPS_PID_AT;
    pMsg->ulLength                    = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(TAF_SS_CALL_INDEPENDENT_EVENT_STRU) - VOS_MSG_HEAD_LENGTH;
    pMsg->clientId                    = 0;
    pMsg->usLen                       = sizeof(TAF_SS_CALL_INDEPENDENT_EVENT_STRU);
    pMsg->usMsgName                   = MN_CALLBACK_SS;

    pEvent                            = (TAF_SS_CALL_INDEPENDENT_EVENT_STRU *)&(pMsg->aucContent[0]);
    pEvent->ClientId                  = 0;
    pEvent->OpId                      = 0;
    pEvent->SsEvent                   = TAF_SS_EVT_INTERROGATESS_CNF;
    pEvent->OP_FwdFeaturelist         = VOS_TRUE;
    pEvent->OP_UnParsePara            = VOS_TRUE;

    pEvent->FwdFeaturelist.ucCnt      = 0x01;
    pEvent->FwdFeaturelist.astFwdFtr[0].OP_BsService = VOS_TRUE;
    pEvent->FwdFeaturelist.astFwdFtr[0].OP_SsStatus  = VOS_TRUE;
    pEvent->FwdFeaturelist.astFwdFtr[0].OP_NumType   = VOS_TRUE;
    pEvent->FwdFeaturelist.astFwdFtr[0].OP_FwdToNum  = VOS_TRUE;
    pEvent->FwdFeaturelist.astFwdFtr[0].OP_NoRepCondTime = VOS_TRUE;
    pEvent->FwdFeaturelist.astFwdFtr[0].NumType      = 0x91;
    pEvent->FwdFeaturelist.astFwdFtr[0].BsService.BsType = TAF_SS_TELE_SERVICE;
    pEvent->FwdFeaturelist.astFwdFtr[0].BsService.BsServiceCode = TAF_ALL_SPEECH_TRANSMISSION_SERVICES_TSCODE;
    pEvent->FwdFeaturelist.astFwdFtr[0].SsStatus     = TAF_SS_PROVISIONED_STATUS_MASK | TAF_SS_REGISTERED_STATUS_MASK | TAF_SS_ACTIVE_STATUS_MASK;
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[0] = '+';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[1] = '8';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[2] = '6';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[3] = '2';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[4] = '1';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[5] = '6';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[6] = '1';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[7] = '7';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[8] = '5';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[9] = '6';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[10] = '3';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[11] = '8';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[12] = '6';

    pEvent->FwdFeaturelist.astFwdFtr[0].NoRepCondTime   = 0x14;

    /* 初始化变量 */
    ucIndex = 5;
    pcExpactedStr3 = "+CCFC: 1,1,\"+862161756386\",145,,,20";
    g_stParseContext[ucIndex].pstCmdElement = &gstCcfcCmdInfo;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(), eq((VOS_UINT32)AT_OK));

    // 注册了无应答转移情况下，查询无应答转移业务
    At_MsgProc((MsgBlock *)pMsg);
    ASSERT_STREQ(pcExpactedStr3, (TAF_CHAR *)pgucAtSndCodeAddr);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}

/*******************************************************************
测试用例编号      : Test_At_MsgProc_013
测试用例标题      : AT模块处理来自TAF的消息
预期结果          : 无
修改历史     :
1.日   期  : 2011-10-18
  作   者  : f62575
  修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_At_MsgProc,Test_At_MsgProc_013)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  *pMsg;
    VOS_CHAR                            *pcExpactedStr;
    VOS_UINT8                           ucIndex;
    VOS_UINT8                           aucMsgCcfc0[] = {0x00,0x00,0x00,0x00,0x98,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0d,0x01,0x00,0x00,0x3c,0x11,0x00,0x00,
                                                         0x03,0x00,0x46,0x86,0x01,0x00,0x34,0x11,0x80,0x00,0x04,0x00,0x09,0x00,0x00,0x05,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x01,0x04,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    VOS_CHAR                            *pcExpactedStr1;
    VOS_UINT8                           *pPidValue;
    pPidValue = aucMsgCcfc0 + 4*sizeof(VOS_UINT8);
    *pPidValue = I0_WUEPS_PID_TAF;
    /* 初始化变量 */
    ucIndex = 5;
    pcExpactedStr1 = "+CCFC: 0,255";
    g_stParseContext[ucIndex].pstCmdElement = &gstCcfcCmdInfo;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(), eq((VOS_UINT32)AT_OK));

    // 未注册无条件转移情况下，查询无条件转移业务
    At_MsgProc((MsgBlock *)aucMsgCcfc0);
    ASSERT_STREQ(pcExpactedStr1, (TAF_CHAR *)pgucAtSndCodeAddr);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_At_MsgProc,Test_At_MsgProc_014)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  *pMsg   = VOS_NULL_PTR;
    TAF_SS_CALL_INDEPENDENT_EVENT_STRU  *pEvent = VOS_NULL_PTR;
    VOS_UINT32                          ulLength;
    VOS_UINT8                           ucIndex;

    VOS_CHAR                            *pcExpactedStr2;

    ulLength = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(TAF_SS_CALL_INDEPENDENT_EVENT_STRU);
    pMsg = (MN_AT_IND_EVT_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_TAF, ulLength);

    PS_MEM_SET(pMsg, 0, ulLength);
    pMsg->ulSenderCpuId               = VOS_LOCAL_CPUID;
    pMsg->ulSenderPid                 = WUEPS_PID_TAF;
    pMsg->ulReceiverCpuId             = VOS_LOCAL_CPUID;
    pMsg->ulReceiverPid               = WUEPS_PID_AT;
    pMsg->ulLength                    = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(TAF_SS_CALL_INDEPENDENT_EVENT_STRU) - VOS_MSG_HEAD_LENGTH;
    pMsg->clientId                    = 0;
    pMsg->usLen                       = sizeof(TAF_SS_CALL_INDEPENDENT_EVENT_STRU);
    pMsg->usMsgName                   = MN_CALLBACK_SS;

    pEvent                            = (TAF_SS_CALL_INDEPENDENT_EVENT_STRU *)&(pMsg->aucContent[0]);
    pEvent->ClientId                  = 0;
    pEvent->OpId                      = 0;
    pEvent->SsEvent                   = TAF_SS_EVT_INTERROGATESS_CNF;
    pEvent->OP_FwdFeaturelist         = VOS_TRUE;
    pEvent->OP_UnParsePara            = VOS_TRUE;

    pEvent->FwdFeaturelist.ucCnt      = 0x01;
    pEvent->FwdFeaturelist.astFwdFtr[0].OP_BsService = VOS_TRUE;
    pEvent->FwdFeaturelist.astFwdFtr[0].OP_SsStatus  = VOS_TRUE;
    pEvent->FwdFeaturelist.astFwdFtr[0].OP_NumType   = VOS_TRUE;
    pEvent->FwdFeaturelist.astFwdFtr[0].OP_FwdToNum  = VOS_TRUE;
    pEvent->FwdFeaturelist.astFwdFtr[0].NumType      = 0x91;
    pEvent->FwdFeaturelist.astFwdFtr[0].BsService.BsType = TAF_SS_TELE_SERVICE;
    pEvent->FwdFeaturelist.astFwdFtr[0].BsService.BsServiceCode = TAF_ALL_SPEECH_TRANSMISSION_SERVICES_TSCODE;
    pEvent->FwdFeaturelist.astFwdFtr[0].SsStatus     = TAF_SS_PROVISIONED_STATUS_MASK | TAF_SS_REGISTERED_STATUS_MASK | TAF_SS_ACTIVE_STATUS_MASK;
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[0] = '+';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[1] = '8';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[2] = '6';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[3] = '2';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[4] = '1';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[5] = '6';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[6] = '1';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[7] = '7';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[8] = '5';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[9] = '6';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[10] = '3';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[11] = '8';
    pEvent->FwdFeaturelist.astFwdFtr[0].aucFwdToNum[12] = '6';

    /* 初始化变量 */
    ucIndex = 5;
    pcExpactedStr2 = "+CCFC: 1,1,\"+862161756386\",145,,,";
    g_stParseContext[ucIndex].pstCmdElement = &gstCcfcCmdInfo;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(), eq((VOS_UINT32)AT_OK));

    // 注册了无条件转移情况下，查询无条件转移业务
    At_MsgProc((MsgBlock *)pMsg);
    ASSERT_STREQ(pcExpactedStr2, (TAF_CHAR *)pgucAtSndCodeAddr);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_At_MsgProc,Test_At_MsgProc_007)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I0_WUEPS_PID_MMA;

    // MOCKER操作;
    MOCKER(At_MmaMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_008)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I1_WUEPS_PID_MMA;

    // MOCKER操作;
    MOCKER(At_MmaMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_009)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I0_DSP_PID_GPHY;

    // MOCKER操作;
    MOCKER(At_GHPAMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_010)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I1_DSP_PID_GPHY;

    // MOCKER操作;
    MOCKER(At_GHPAMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_011)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I0_WUEPS_PID_VC;

    // MOCKER操作;
    MOCKER(At_ProcMsgFromVc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_012)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I1_WUEPS_PID_VC;

    // MOCKER操作;
    MOCKER(At_ProcMsgFromVc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}

TEST_F(Test_At_MsgProc,Test_At_MsgProc_016)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I0_WUEPS_PID_DRV_AGENT;

    // MOCKER操作;
    MOCKER(At_ProcMsgFromDrvAgent)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_017)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I1_WUEPS_PID_DRV_AGENT;

    // MOCKER操作;
    MOCKER(At_ProcMsgFromDrvAgent)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_018)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I0_WUEPS_PID_CC;

    // MOCKER操作;
    MOCKER(At_ProcMsgFromCc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_019)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I1_WUEPS_PID_CC;

    // MOCKER操作;
    MOCKER(At_ProcMsgFromCc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_020)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I0_MAPS_STK_PID;

    // MOCKER操作;
    MOCKER(At_STKMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_021)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I1_MAPS_STK_PID;

    // MOCKER操作;
    MOCKER(At_STKMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_022)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I0_MAPS_PB_PID;

    // MOCKER操作;
    MOCKER(At_PbMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_023)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I1_MAPS_PB_PID;

    // MOCKER操作;
    MOCKER(At_PbMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_024)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I0_MAPS_PIH_PID;

    // MOCKER操作;
    MOCKER(At_PIHMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_025)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I1_MAPS_PIH_PID;

    // MOCKER操作;
    MOCKER(At_PIHMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_026)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I0_WUEPS_PID_RABM;

    // MOCKER操作;
    MOCKER(AT_RabmMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_027)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I1_WUEPS_PID_RABM;

    // MOCKER操作;
    MOCKER(AT_RabmMsgProc)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MsgProc,Test_At_MsgProc_028)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I1_UEPS_PID_MTA;

    // MOCKER操作;
    MOCKER(At_ProcMtaMsg)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}

TEST_F(Test_At_MsgProc,Test_At_MsgProc_029)
{
    // 参数定义
    MSG_HEADER_STRU stMsg;

    // 参数初始化
    stMsg.ulSenderPid = I1_UEPS_PID_MTA;

    // MOCKER操作;
    MOCKER(At_ProcMtaMsg)
    .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      : Test_At_MsgProc_030
测试用例标题      : 网络无响应超时事件上报
预期结果          : 输出打印+CUSD: 5
修改历史     :
1.日   期  : 2012-04-25
  作   者  : f62575
  修改内容 : 新生成类
*******************************************************************/
TEST_F(Test_At_MsgProc,Test_At_MsgProc_030)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  *pMsg;

    VOS_UINT8      aucMsg[] = {0x01,0x00,0x00,0x7f,0x98,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x0d,0x01,0x00,0x00,0x3c,0x11,0x00,0x00,0x03,0x00,0x00,0x00,0xff,
                             0xff,0x34,0x11,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x27,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,
                             0x18,0x00,0x01,0x01,0x05,0x00,0x10,0x00,0x03,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x14,0x00,0x30,0x00,0x8e,0x00,0x00,0x00,0xaa,
                             0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x34,0x00,
                             0x01,0x01,0x01,0x1c,0x2c,0x00,0x04,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x5e,0x00,0x01,0x00,0x00,0x7f,0x8e,0x00,0x00,
                             0x00,0x01,0x00,0x00,0x7f,0x01,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x0a,0x00,0x00,0x00,0x10,0x27,0x00,0x00,0x00,0x00,0x00,0x00,
                             0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x30,
                             0x00,0x01,0x01,0x01,0x49,0x28,0x00,0x05,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0xb5,0x05,0x10,0x56,0x51,0x50,0x8d,0x00,
                             0x00,0x00,0x01,0x00,0x00,0x7f,0x8d,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x04,0x10,0x00,0x00,0x01,0x00,0x00,0x00,0xaa,0xaa,0x55,
                             0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x54,0x00,0x01,0x01,
                             0x01,0x4c,0x4c,0x00,0x06,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0xff,0xff,0x01,0x00,0x00,0x7f,0x8e,0x00,0x00,0x00,0x01,
                             0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x2c,0x00,0x00,0x00,0x04,0x00,0x0b,0x00,0x00,0x00,0x00,0x00,0x10,0x55,0x55,0x55,0x55,0x55,
                             0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x10,0x56,0x51,0x50,0x53,0x52,0x5d,0x00,0x00,0x55,0x54,0x57,0x56,0x51,
                             0x50,0x53,0x52,0x00,0x00,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x55,0x55,0xaa,0xaa,0x2d,0x00,0x01,0x01,0x01,0x00,0x25,0x00,0x07,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,
                             0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0xa4,0x00,0x04,0x02,0x10,
                             0x03,0x80,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,
                             0xaa,0x29,0x00,0x01,0x01,0x01,0x00,0x21,0x00,0x08,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,
                             0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0xa4,0xa4,0x00,0x04,0xaa,0xaa,0x55,0x55,0x07,
                             0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x2a,0x00,0x01,0x01,0x01,0x00,
                             0x22,0x00,0x09,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,
                             0x7f,0x65,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x7f,0xff,0x00,0x04,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x2a,0x00,0x01,0x01,0x01,0x00,0x22,0x00,0x0a,0x08,0x00,0x00,0xa9,
                             0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x02,0x00,
                             0x00,0x00,0x61,0x3e,0x00,0x04,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x55,0x55,0xaa,0xaa,0x2d,0x00,0x01,0x01,0x01,0x00,0x25,0x00,0x0b,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,
                             0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0xc0,0x00,0x00,0x3e,
                             0x10,0x03,0x80,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,
                             0xaa,0xaa,0x29,0x00,0x01,0x01,0x01,0x00,0x21,0x00,0x0c,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,
                             0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0xc0,0xc0,0x00,0x00,0xaa,0xaa,0x55,0x55,
                             0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x66,0x00,0x01,0x01,0x01,
                             0x00,0x5e,0x00,0x0d,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,
                             0x00,0x7f,0x65,0x00,0x00,0x00,0x3e,0x00,0x00,0x00,0x62,0x3c,0x82,0x02,0x38,0x21,0x84,0x10,0xa0,0x00,0x00,0x00,0x87,0x10,0x02,
                             0xff,0xff,0xff,0xff,0x89,0x03,0x02,0x00,0x00,0xa5,0x11,0x80,0x01,0x71,0x81,0x03,0x01,0x0a,0x32,0x82,0x01,0x1e,0x83,0x04,0x00,
                             0x00,0x17,0x48,0x8a,0x01,0x05,0x8b,0x03,0x2f,0x06,0x03,0xc6,0x09,0x90,0x01,0x40,0x83,0x01,0x01,0x83,0x01,0x81,0x00,0x00,0xaa,
                             0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x2a,0x00,
                             0x01,0x01,0x01,0x00,0x22,0x00,0x0e,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,
                             0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x90,0x00,0x00,0x00,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x2d,0x00,0x01,0x01,0x01,0x00,0x25,0x00,0x0f,
                             0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,
                             0x00,0x00,0x05,0x00,0x00,0x00,0x00,0x88,0x00,0x81,0x22,0x10,0x03,0x80,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x29,0x00,0x01,0x01,0x01,0x00,0x21,0x00,0x10,0x08,0x00,0x00,
                             0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,
                             0x00,0x00,0x00,0x88,0x88,0x00,0x81,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x55,0x55,0xaa,0xaa,0x4a,0x00,0x01,0x01,0x01,0x00,0x42,0x00,0x11,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,
                             0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x22,0x00,0x00,0x00,0x10,0x55,0x55,0x55,
                             0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x10,0x56,0x51,0x50,0x53,0x52,0x5d,0x00,0x00,0x55,0x54,0x57,
                             0x56,0x51,0x50,0x53,0x52,0x82,0x01,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x55,0x55,0xaa,0xaa,0x2a,0x00,0x01,0x01,0x01,0x00,0x22,0x00,0x12,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,
                             0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x61,0x2c,0x00,0x81,
                             0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x2d,
                             0x00,0x01,0x01,0x01,0x00,0x25,0x00,0x13,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,
                             0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0xc0,0x00,0x00,0x2c,0x10,0x03,0x80,0xaa,0xaa,0x55,
                             0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x29,0x00,0x01,0x01,
                             0x01,0x00,0x21,0x00,0x14,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,
                             0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0xc0,0xc0,0x00,0x00,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x54,0x00,0x01,0x01,0x01,0x00,0x4c,0x00,0x15,0x08,0x00,
                             0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,
                             0x2c,0x00,0x00,0x00,0xdb,0x08,0x0b,0xc7,0xe1,0x33,0x19,0x60,0x9f,0x42,0x10,0x07,0xb6,0x68,0x65,0x25,0xca,0xac,0x9f,0xff,0x73,
                             0x9d,0x7a,0xe9,0x4b,0xde,0xd2,0x10,0xac,0x59,0xba,0x8b,0xd0,0xef,0xd2,0x39,0xa3,0xa0,0xa2,0xb2,0x64,0xc5,0x8c,0x32,0xaa,0xaa,
                             0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x2a,0x00,0x01,
                             0x01,0x01,0x00,0x22,0x00,0x16,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,
                             0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x90,0x00,0x00,0x00,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x45,0x00,0x01,0x01,0x02,0x00,0x3d,0x00,0x17,0x08,
                             0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0xc1,0x00,0x00,0x04,0x01,0x01,0x00,0x29,0x00,0x00,0x00,0x25,0x00,0x00,0x00,0x05,0x12,0x03,
                             0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x20,0x10,0x56,0x51,0x50,0x53,0x52,0x5d,0x00,
                             0x00,0x55,0x54,0x57,0x56,0x51,0x50,0x53,0x52,0x00,0x00,0x00,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x70,0x00,0x01,0x01,0x01,0x4c,0x68,0x00,0x18,0x08,0x00,0x00,0xa9,0x1f,
                             0x00,0x00,0x03,0x80,0xff,0xff,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x8e,0x00,0x00,0x00,0x48,0x00,0x00,
                             0x00,0x09,0x00,0x00,0x00,0x00,0x00,0xac,0x59,0xba,0x8b,0xd0,0xef,0xd2,0x39,0xa3,0xa0,0xa2,0xb2,0x64,0xc5,0x8c,0x32,0x07,0xb6,
                             0x68,0x65,0x25,0xca,0xac,0x9f,0xff,0x73,0x9d,0x7a,0xe9,0x4b,0xde,0xd2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x0b,0xc7,
                             0xe1,0x33,0x19,0x60,0x9f,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xaa,0xaa,
                             0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x18,0x00,0x01,
                             0x01,0x05,0x00,0x10,0x00,0x19,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x15,0x00,0x30,0x00,0x8e,0x00,0x00,0x00,0xaa,0xaa,0x55,0x55,
                             0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x34,0x00,0x01,0x01,0x01,
                             0xab,0x2c,0x00,0x1a,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x8e,0x00,0x00,0x00,0x01,0x00,
                             0x00,0x7f,0x01,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0xaa,0xaa,0x55,
                             0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0xd0,0x00,0x01,0x01,
                             0x01,0x4c,0xc8,0x00,0x1b,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0xff,0xff,0x01,0x00,0x00,0x7f,0x8e,0x00,0x00,0x00,0x01,
                             0x00,0x00,0x7f,0x85,0x00,0x00,0x00,0xa8,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xb6,0x68,0x65,0x25,0xca,0xac,0x9f,0xff,0x73,0x9d,0x7a,0xe9,0x4b,0xde,0xd2,0xac,0x59,0xba,
                             0x8b,0xd0,0xef,0xd2,0x39,0xa3,0xa0,0xa2,0xb2,0x64,0xc5,0x8c,0x32,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,
                             0x55,0xaa,0xaa,0x59,0x00,0x01,0x01,0x01,0x4c,0x51,0x00,0x1c,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0xff,0xff,0x01,0x00,
                             0x00,0x7f,0x8e,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x31,0x00,0x00,0x00,0x02,0x00,0x0b,0x00,0x08,0x6f,0x21,
                             0x00,0x03,0x00,0x00,0x00,0x00,0x03,0x07,0xb6,0x68,0x65,0x25,0xca,0xac,0x9f,0xff,0x73,0x9d,0x7a,0xe9,0x4b,0xde,0xd2,0xac,0x59,
                             0xba,0x8b,0xd0,0xef,0xd2,0x39,0xa3,0xa0,0xa2,0xb2,0x64,0xc5,0x8c,0x32,0x55,0x55,0x55,0x55,0x55,0x20,0xaa,0xaa,0x55,0x55,0x07,
                             0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x2d,0x00,0x01,0x01,0x01,0x00,
                             0x25,0x00,0x1d,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,
                             0x7f,0x65,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0xa4,0x08,0x04,0x04,0x10,0x03,0x80,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x29,0x00,0x01,0x01,0x01,0x00,0x21,0x00,0x1e,
                             0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,
                             0x00,0x00,0x01,0x00,0x00,0x00,0xa4,0xa4,0x08,0x04,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x2c,0x00,0x01,0x01,0x01,0x00,0x24,0x00,0x1f,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,
                             0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x7f,
                             0xff,0x6f,0x08,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,
                             0xaa,0xaa,0x2a,0x00,0x01,0x01,0x01,0x00,0x22,0x00,0x20,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,
                             0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x61,0x1d,0x6f,0x08,0xaa,0xaa,0x55,0x55,
                             0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x2d,0x00,0x01,0x01,0x01,
                             0x00,0x25,0x00,0x21,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,
                             0x00,0x7f,0x65,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0xc0,0x00,0x00,0x1d,0x10,0x03,0x80,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x29,0x00,0x01,0x01,0x01,0x00,0x21,0x00,
                             0x22,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,
                             0x00,0x00,0x00,0x01,0x00,0x00,0x00,0xc0,0xc0,0x00,0x00,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x45,0x00,0x01,0x01,0x01,0x00,0x3d,0x00,0x23,0x08,0x00,0x00,0xa9,0x1f,0x00,
                             0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x1d,0x00,0x00,0x00,
                             0x62,0x1b,0x82,0x02,0x01,0x21,0x83,0x02,0x6f,0x08,0x8a,0x01,0x05,0x8b,0x03,0x6f,0x06,0x04,0x80,0x02,0x00,0x21,0x81,0x02,0x00,
                             0x35,0x88,0x01,0x40,0x53,0x52,0x5d,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x55,0x55,0xaa,0xaa,0x2a,0x00,0x01,0x01,0x01,0x00,0x22,0x00,0x24,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,
                             0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x90,0x00,0x00,0x00,
                             0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x2d,
                             0x00,0x01,0x01,0x01,0x00,0x25,0x00,0x25,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,
                             0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0xd6,0x00,0x00,0x21,0x10,0x03,0x80,0xaa,0xaa,0x55,
                             0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x29,0x00,0x01,0x01,
                             0x01,0x00,0x21,0x00,0x26,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,
                             0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0xd6,0xd6,0x00,0x00,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x49,0x00,0x01,0x01,0x01,0x00,0x41,0x00,0x27,0x08,0x00,
                             0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,
                             0x21,0x00,0x00,0x00,0x03,0x07,0xb6,0x68,0x65,0x25,0xca,0xac,0x9f,0xff,0x73,0x9d,0x7a,0xe9,0x4b,0xde,0xd2,0xac,0x59,0xba,0x8b,
                             0xd0,0xef,0xd2,0x39,0xa3,0xa0,0xa2,0xb2,0x64,0xc5,0x8c,0x32,0x00,0x55,0x54,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x2a,0x00,0x01,0x01,0x01,0x00,0x22,0x00,0x28,0x08,0x00,
                             0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,
                             0x02,0x00,0x00,0x00,0x90,0x00,0x00,0x00,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x34,0x00,0x01,0x01,0x01,0xfc,0x2c,0x00,0x29,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,
                             0x5e,0x00,0x01,0x00,0x00,0x7f,0x8e,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x01,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x06,0x00,0x00,
                             0x00,0x20,0x4e,0x00,0x00,0x00,0x00,0x00,0x00,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x44,0x00,0x01,0x01,0x01,0x4c,0x3c,0x00,0x2a,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,
                             0x80,0xff,0xff,0x01,0x00,0x00,0x7f,0x8e,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x85,0x00,0x00,0x00,0x1c,0x00,0x00,0x00,0x0a,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x05,0x54,0x0b,0xc7,0xe1,0x33,0x21,0x04,0x19,0x60,0x9f,
                             0x42,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,
                             0x30,0x00,0x01,0x01,0x01,0x49,0x28,0x00,0x2b,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0xb5,0x05,0x05,0x54,0x0b,0xc7,0x8d,
                             0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x8d,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x04,0x10,0x00,0x00,0x01,0x00,0xa0,0xa2,0xaa,0xaa,
                             0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x2c,0x00,0x01,
                             0x01,0x02,0x6f,0x24,0x00,0x2c,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0xc1,0x25,0xca,0x05,0x01,0x00,0x73,0x10,0x00,0x00,0x00,
                             0x0c,0x00,0x00,0x00,0x05,0x14,0x0b,0xc7,0xe1,0x33,0x21,0x04,0x19,0x60,0x9f,0x42,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x3c,0x00,0x01,0x01,0x01,0x4c,0x34,0x00,0x2d,0x08,
                             0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0xff,0xff,0x01,0x00,0x00,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x8e,0x00,0x00,
                             0x00,0x14,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x6f,0x21,0x00,0x00,0xac,0x59,0xba,
                             0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x30,
                             0x00,0x01,0x01,0x01,0x49,0x28,0x00,0x2e,0x08,0x00,0x00,0xa9,0x1f,0x00,0x00,0x03,0x80,0xb5,0x05,0x05,0x54,0x0b,0xc7,0x8d,0x00,
                             0x00,0x00,0x01,0x00,0x00,0x7f,0x8d,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x04,0x10,0x00,0x00,0x01,0x00,0xa0,0xa2,0xaa,0xaa,0x55,
                             0x55,0x1b,0x9e,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xaa,0xaa,0x55,0x55,0xeb,0x99,0x7e,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xaa,0xaa,0x55,0x55,0x07,0x00,
                             0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0x55,0xaa,0xaa,0x21,0x00,0x01,0x01,0x05,0x00,0x19,
                             0x00,0xdc,0x07,0x00,0x00,0x4b,0x1e,0x00,0x00,0x2d,0x00,0x00,0x00,0x8d,0x00,0x00,0x00,0x00,0xf1,0x10,0x64,0xf0,0x60,0x82,0xf0,
                             0x20,0x00,0x00,0x00,0xaa,0xaa,0x55,0x55,0x07,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,
                             0x55,0xaa,0xaa,0x30,0x00,0x01,0x01,0x01,0x49,0x28,0x00,0xdd,0x07,0x00,0x00,0x4b,0x1e,0x00,0x00,0x03,0x80,0xb5,0x05,0x00,0xf1,
                             0x10,0x64,0x8d,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x8d,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x04,0x10,0x00,0x00,0x01,0x00,0x00,
                             0x00,0xaa,0xaa,0x55,0x55,0x7f,0x65,0x00,0x00,0x00,0x01,0x00,0x00,0x7f,0x8f,0x00,0x00,0x00,0x14,0x00,0x00,0x00,0x03,0x00,0xaa,
                             0xaa,0x55,0x55,0x55,0x20,0xf5,0x7e,0x55,0xc3,0x19,0x7e,0xcb,0xba,0x7e,0x5d,0x02,0x00,0xaa,0xaa,0x55,0x55,0x74,0xee,0x7e,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                             0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    VOS_CHAR                            *pcExpactedStr;
    VOS_UINT8                           ucIndex;

    /* 初始化变量 */
    ucIndex = 0;
    pcExpactedStr = "\r\n+CUSD: 5\r\n";
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CURRENT_OPT_BUTT;

    PS_MEM_SET(&gstAtSendData, 0, sizeof(gstAtSendData));

    // 参数初始化
    pMsg = (MN_AT_IND_EVT_STRU *)aucMsg;

    // MOCKER操作

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

	MOCKER(At_SendResultData)
		.expects(once());

    // 调用被测函数
    At_EventMsgProc(pMsg);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}

/*******************************************************************
测试用例编号      : Test_At_MsgProc_031
测试用例标题      : AT模块处理来自TAF的消息
预期结果          : 无
修改历史     :
1.日   期  : 2011-10-18
  作   者  : f62575
  修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_At_MsgProc,Test_At_MsgProc_031)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  *pMsg;
    VOS_CHAR                            *pcExpactedStr;
    VOS_UINT8                           ucIndex;
    VOS_UINT8                           aucMsg[] = {0x00,0x00,0x00,0x00,0x98,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0d,0x01,0x00,0x00,0x3c,0x0b,0x00,0x00,0x02,0x00,
                                                    0x00,0x00,0x05,0x00,0x3c,0x0b,0x0c,0x00,0x00,0x00,0x05,0x00,0x11,0x00,0x14,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    /* 初始化变量 */
    ucIndex = 5;
    pcExpactedStr = "\r\n+CMGD: (0),(0-4)\r\n";
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CMGD_TEST;
    gastAtClientTab[ucIndex].opId          = 17;
    g_stParseContext[ucIndex].pstCmdElement = &gstCmgdCmdInfo;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

	MOCKER(At_SendResultData)
		.expects(once());

    // 调用被测函数
    At_MsgProc((MsgBlock *)aucMsg);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}

/*******************************************************************
测试用例编号      : Test_At_MsgProc_032
测试用例标题      : AT模块接收到MMA回复的CPOL命令查询响应消息调用函数AT_QryParaRspPrefPlmnProc处理
预期结果          : 无
修改历史     :
1.日   期  : 2014-1-17
  作   者  : f62575
  修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_At_MsgProc,Test_At_MsgProc_032)
{
    // 参数定义
    TAF_MMA_CPOL_INFO_QUERY_CNF_STRU   stCpolQryCnf;

    // 参数初始化
    memset(&stCpolQryCnf, 0, sizeof(stCpolQryCnf));
    stCpolQryCnf.ulMsgName      = AT_MMA_CPOL_INFO_QUERY_CNF;
    stCpolQryCnf.ulSenderPid    = WUEPS_PID_MMA;
    stCpolQryCnf.ulReceiverPid  = WUEPS_PID_AT;

    // MOCKER操作;
    MOCKER(AT_QryParaRspPrefPlmnProc)
    .expects(exactly(1))
	.will(returnValue((VOS_UINT32)VOS_OK));

    // 调用被测函数
    At_MsgProc((MsgBlock *)&stCpolQryCnf);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


class Test_At_ProcMsgFromVc: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();
    }

    void TearDown()
    {
    }
};


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_001)
{
    // 参数定义
    MN_AT_IND_EVT_STRU          pMsg;

    // 参数初始化
    pMsg.usMsgName              = APP_VC_MSG_CNF_QRY_MODE;

    // 相关全局变量初始化

    // MOCKER操作;
    MOCKER(At_RcvVcMsgQryModeCnfProc)
        .expects(exactly(1));

    // 调用被测函数
    At_ProcMsgFromVc(&pMsg);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_002)
{
    // 参数定义
    MN_AT_IND_EVT_STRU          pMsg;

    // 参数初始化
    pMsg.usMsgName              = APP_VC_MSG_CNF_SET_PORT;

    // 相关全局变量初始化

    // MOCKER操作;
    MOCKER(At_RcvVcMsgSetPortCnfProc)
        .expects(exactly(1));

    // 调用被测函数
    At_ProcMsgFromVc(&pMsg);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_003)
{
    // 参数定义
    MN_AT_IND_EVT_STRU          pMsg;

    // 参数初始化
    pMsg.usMsgName              = APP_VC_MSG_CNF_QRY_PORT;

    // 相关全局变量初始化

    // MOCKER操作;
    MOCKER(At_RcvVcMsgQryPortCnfProc)
        .expects(exactly(1));

    // 调用被测函数
    At_ProcMsgFromVc(&pMsg);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_004)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  stMsg;

    // 参数初始化
    stMsg.usMsgName              = APP_VC_MSG_SET_FOREGROUND_CNF;
    stMsg.clientId              = AT_CLIENT_ID_BUTT;

    // 相关全局变量初始化

    // MOCKER操作
    MOCKER(AT_StopRelTimer).expects(never());
    MOCKER(At_FormatResultData).expects(never());

    // 调用被测函数
    At_ProcMsgFromVc(&stMsg);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_005)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  stMsg;
    VOS_UINT8                           ucIndex;

    // 参数初始化
    stMsg.usMsgName              = APP_VC_MSG_SET_FOREGROUND_CNF;
    ucIndex                      = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 相关全局变量初始化

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));
    MOCKER(AT_StopRelTimer).expects(never());
    MOCKER(At_FormatResultData).expects(never());

    // 调用被测函数
    At_ProcMsgFromVc(&stMsg);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_006)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  stMsg;
    VOS_UINT8                           ucIndex;

    // 参数初始化
    stMsg.usMsgName              = APP_VC_MSG_SET_BACKGROUND_CNF;
    ucIndex                      = AT_CLIENT_TAB_APP_INDEX;

    // 相关全局变量初始化
    gastAtClientTab[AT_CLIENT_TAB_APP_INDEX].CmdCurrentOpt = AT_CMD_CBG_READ;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));
    MOCKER(AT_StopRelTimer).expects(never());
    MOCKER(At_FormatResultData).expects(never());

    // 调用被测函数
    At_ProcMsgFromVc(&stMsg);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_007)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  stMsg;
    VOS_UINT8                           ucIndex;

    // 参数初始化
    stMsg.usMsgName              = APP_VC_MSG_SET_BACKGROUND_CNF;
    stMsg.aucContent[0]          = VOS_OK;
    ucIndex                      = AT_CLIENT_TAB_APP_INDEX;

    // 相关全局变量初始化
    gastAtClientTab[AT_CLIENT_TAB_APP_INDEX].CmdCurrentOpt = AT_CMD_CBG_SET;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));
    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));
    MOCKER(At_FormatResultData).expects(exactly(1));

    // 调用被测函数
    At_ProcMsgFromVc(&stMsg);

    // 执行检查
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_008)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  stMsg;
    VOS_UINT8                           ucIndex;

    // 参数初始化
    stMsg.usMsgName              = APP_VC_MSG_SET_BACKGROUND_CNF;
    stMsg.aucContent[0]          = VOS_ERR;
    ucIndex                      = AT_CLIENT_TAB_APP_INDEX;

    // 相关全局变量初始化
    gastAtClientTab[AT_CLIENT_TAB_APP_INDEX].CmdCurrentOpt = AT_CMD_CBG_SET;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));
    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));
    MOCKER(At_FormatResultData).expects(exactly(1));

    // 调用被测函数
    At_ProcMsgFromVc(&stMsg);

    // 执行检查
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_009)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  stMsg;

    // 参数初始化
    stMsg.usMsgName              = APP_VC_MSG_FOREGROUND_RSP;
    stMsg.clientId               = AT_CLIENT_ID_BUTT;

    // 相关全局变量初始化

    // MOCKER操作
    MOCKER(AT_StopRelTimer).expects(never());
    MOCKER(At_FormatResultData).expects(never());

    // 调用被测函数
    At_ProcMsgFromVc(&stMsg);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_010)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  stMsg;
    VOS_UINT8                           ucIndex;

    // 参数初始化
    stMsg.usMsgName              = APP_VC_MSG_FOREGROUND_RSP;
    ucIndex                      = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 相关全局变量初始化

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));
    MOCKER(AT_StopRelTimer).expects(never());
    MOCKER(At_FormatResultData).expects(never());

    // 调用被测函数
    At_ProcMsgFromVc(&stMsg);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_011)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  stMsg;
    VOS_UINT8                           ucIndex;

    // 参数初始化
    stMsg.usMsgName              = APP_VC_MSG_FOREGROUND_RSP;
    ucIndex                      = AT_CLIENT_TAB_APP_INDEX;

    // 相关全局变量初始化
    gastAtClientTab[AT_CLIENT_TAB_APP_INDEX].CmdCurrentOpt = AT_CMD_CBG_SET;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));
    MOCKER(AT_StopRelTimer).expects(never());
    MOCKER(At_FormatResultData).expects(never());

    // 调用被测函数
    At_ProcMsgFromVc(&stMsg);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_012)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  stMsg;
    VOS_UINT8                           ucIndex;
    APP_VC_QRY_GROUNG_RSP_STRU         *pstQryRslt;
    VOS_CHAR                           *pStr = "^CBG:1";

    // 参数初始化
    stMsg.usMsgName              = APP_VC_MSG_FOREGROUND_RSP;
    ucIndex                      = AT_CLIENT_TAB_APP_INDEX;
    pstQryRslt                   = (APP_VC_QRY_GROUNG_RSP_STRU *)stMsg.aucContent;
    pstQryRslt->ucQryRslt        = VOS_OK;
    pstQryRslt->enGround         = APP_VC_BACKGROUND;

    // 相关全局变量初始化
    gastAtClientTab[AT_CLIENT_TAB_APP_INDEX].CmdCurrentOpt = AT_CMD_CBG_READ;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));
    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));
    MOCKER(At_FormatResultData).expects(exactly(1));

    // 调用被测函数
    At_ProcMsgFromVc(&stMsg);

    // 执行检查
    ASSERT_STREQ(pStr, (VOS_CHAR *)pgucAtSndCodeAddr);
    EXPECT_EQ(gstAtSendData.usBufLen, VOS_StrLen("^CBG:1"));

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_013)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  stMsg;
    VOS_UINT8                           ucIndex;
    APP_VC_QRY_GROUNG_RSP_STRU         *pstQryRslt;;

    // 参数初始化
    stMsg.usMsgName              = APP_VC_MSG_FOREGROUND_RSP;
    ucIndex                      = AT_CLIENT_TAB_APP_INDEX;
    pstQryRslt                   = (APP_VC_QRY_GROUNG_RSP_STRU *)stMsg.aucContent;
    pstQryRslt->ucQryRslt        = VOS_ERR;

    // 相关全局变量初始化
    gastAtClientTab[AT_CLIENT_TAB_APP_INDEX].CmdCurrentOpt = AT_CMD_CBG_READ;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));
    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));
    MOCKER(At_FormatResultData).expects(exactly(1));

    // 调用被测函数
    At_ProcMsgFromVc(&stMsg);

    // 执行检查
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_ProcMsgFromVc,Test_At_ProcMsgFromVc_014)
{
    // 参数定义
    MN_AT_IND_EVT_STRU                  stMsg;

    // 参数初始化
    stMsg.usMsgName              = APP_VC_MSG_BUTT;

    // 相关全局变量初始化

    // MOCKER操作

    // 调用被测函数
    At_ProcMsgFromVc(&stMsg);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}

#if (NAS_FEATURE_CS_VC == FEATURE_ON)

class Test_At_RcvVcMsgQryModeCnfProc: public ::testing::Test
{
public:
    VOS_UINT8                            ucIndex;
    MN_AT_IND_EVT_STRU                  *pstData;
    APP_VC_EVENT_INFO_STRU              *pstEvent;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                                 = 0;

        gstAtSendData.usBufLen                  = 0;

        pstData = (MN_AT_IND_EVT_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(MN_AT_IND_EVT_STRU) + sizeof(APP_VC_EVENT_INFO_STRU));

        if (VOS_NULL_PTR == pstData)
        {
            return;
        }
        pstEvent = (APP_VC_EVENT_INFO_STRU *)(pstData->aucContent);

        PS_MEM_SET(pstData, 0, sizeof(MN_AT_IND_EVT_STRU));
        PS_MEM_SET(pstEvent, 0, sizeof(APP_VC_EVENT_INFO_STRU));
    }

    void TearDown()
    {
        gstAtSendData.usBufLen                  = 0;

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstData)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstData);
        }
    }
};
TEST_F(Test_At_RcvVcMsgQryModeCnfProc,Test_At_RcvVcMsgQryModeCnfProc_001)
{
    // 参数定义

    // 参数初始化

    // 相关全局变量初始化

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    At_RcvVcMsgQryModeCnfProc(pstData);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}


TEST_F(Test_At_RcvVcMsgQryModeCnfProc,Test_At_RcvVcMsgQryModeCnfProc_002)
{
    // 参数定义
    // 参数初始化
    pstEvent->enDevMode                     = VC_PHY_DEVICE_MODE_BUTT;
    // 相关全局变量初始化
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_VMSET_READ;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(),eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数
    At_RcvVcMsgQryModeCnfProc(pstData);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}


TEST_F(Test_At_RcvVcMsgQryModeCnfProc,Test_At_RcvVcMsgQryModeCnfProc_003)
{
   // 参数定义
    char                                        acExpectedResult[] = "^VMSET: 5";
    AT_PAR_CMD_ELEMENT_STRU                     stCmdInfo = {AT_CMD_VMSET,
                                                             At_SetVMSETPara, AT_SET_PARA_TIME,
                                                             At_QryVMSETPara, AT_QRY_PARA_TIME,
                                                             VOS_NULL_PTR, AT_NOT_SET_TIME,
                                                             VOS_NULL_PTR, AT_NOT_SET_TIME,
                                                             AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
                                                             (VOS_UINT8*)"^VMSET",(VOS_UINT8*)"(0-2)"};




    // 参数初始化
    pstEvent->enDevMode                     = VC_PHY_DEVICE_MODE_PCVOICE;
    // 相关全局变量初始化
    g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_VMSET_READ;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(),eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    At_RcvVcMsgQryModeCnfProc(pstData);

    // 执行检查
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}


TEST_F(Test_At_RcvVcMsgQryModeCnfProc,Test_At_RcvVcMsgQryModeCnfProc_004)
{
   // 参数定义

    // 参数初始化
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CVOICE_READ;

    // 相关全局变量初始化

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(APP_VC_VcPhyVoiceMode2AppVcVoiceMode)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT16)APP_VC_VOICE_MODE_BUTT));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(),eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数
    At_RcvVcMsgQryModeCnfProc(pstData);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}


TEST_F(Test_At_RcvVcMsgQryModeCnfProc,Test_At_RcvVcMsgQryModeCnfProc_005)
{
   // 参数定义
    char                                        acExpectedResult[] = "^CVOICE: 0, 8000, 16, 20";
    AT_PAR_CMD_ELEMENT_STRU                     stCmdInfo = {AT_CMD_CVOICE,
                                                             At_SetCvoicePara, AT_SET_PARA_TIME,
                                                             At_QryCvoicePara, AT_QRY_PARA_TIME,
                                                             VOS_NULL_PTR, AT_NOT_SET_TIME,
                                                             VOS_NULL_PTR, AT_NOT_SET_TIME,
                                                             AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
                                                             (VOS_UINT8*)"^CVOICE",VOS_NULL_PTR};


    // 参数初始化

    // 相关全局变量初始化
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CVOICE_READ;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(APP_VC_VcPhyVoiceMode2AppVcVoiceMode)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT16)APP_VC_VOICE_MODE_PCVOICE));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(),eq((VOS_UINT32)AT_OK));


    // 调用被测函数
    At_RcvVcMsgQryModeCnfProc(pstData);

    // 执行检查
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}

TEST_F(Test_At_RcvVcMsgQryModeCnfProc,Test_At_RcvVcMsgQryModeCnfProc_006)
{
    // 参数定义

    // 参数初始化
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CURRENT_OPT_BUTT;

    // 相关全局变量初始化

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(0));
    MOCKER(At_FormatResultData)
        .expects(exactly(0));

    // 调用被测函数
    At_RcvVcMsgQryModeCnfProc(pstData);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
#endif

#if (FEATURE_ON == FEATURE_PC_VOICE)

class Test_At_RcvVcMsgSetPortCnfProc: public ::testing::Test
{
public:
    VOS_UINT8                            ucIndex;
    MN_AT_IND_EVT_STRU                  *pstData;
    APP_VC_EVENT_INFO_STRU              *pstEvent;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                                = 0;
        pstData = (MN_AT_IND_EVT_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(MN_AT_IND_EVT_STRU) + sizeof(APP_VC_EVENT_INFO_STRU));

        if (VOS_NULL_PTR == pstData)
        {
            return;
        }
        pstEvent = (APP_VC_EVENT_INFO_STRU *)(pstData->aucContent);

        PS_MEM_SET(pstData, 0, sizeof(MN_AT_IND_EVT_STRU));
        PS_MEM_SET(pstEvent, 0, sizeof(APP_VC_EVENT_INFO_STRU));
    }

    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstData)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstData);
        }
    }};
TEST_F(Test_At_RcvVcMsgSetPortCnfProc,Test_At_RcvVcMsgSetPortCnfProc_001)
{
    // 参数定义

    // 参数初始化

    // 相关全局变量初始化

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(0));
    MOCKER(At_FormatResultData)
        .expects(exactly(0));

    // 调用被测函数
    At_RcvVcMsgSetPortCnfProc(pstData);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}


TEST_F(Test_At_RcvVcMsgSetPortCnfProc,Test_At_RcvVcMsgSetPortCnfProc_002)
{
    // 参数定义

    // 参数初始化
    pstEvent->bSuccess                          = VOS_FALSE;
    // 相关全局变量初始化

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(),eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数
    At_RcvVcMsgSetPortCnfProc(pstData);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();


}


TEST_F(Test_At_RcvVcMsgSetPortCnfProc,Test_At_RcvVcMsgSetPortCnfProc_003)
{
    // 参数定义

    // 参数初始化
    pstEvent->bSuccess                          = VOS_TRUE;
    // 相关全局变量初始化

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(),eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    At_RcvVcMsgSetPortCnfProc(pstData);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();


}

#endif


class Test_At_RcvVcMsgQryPortCnfProc: public ::testing::Test
{
public:
    VOS_UINT8                            ucIndex;
    MN_AT_IND_EVT_STRU                  *pstData;
    APP_VC_EVENT_INFO_STRU              *pstEvent;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                                 = 0;

        gstAtSendData.usBufLen                  = 0;

        pstData = (MN_AT_IND_EVT_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(MN_AT_IND_EVT_STRU) + sizeof(APP_VC_EVENT_INFO_STRU));

        if (VOS_NULL_PTR == pstData)
        {
            return;
        }
        pstEvent = (APP_VC_EVENT_INFO_STRU *)(pstData->aucContent);

        PS_MEM_SET(pstData, 0, sizeof(MN_AT_IND_EVT_STRU));
        PS_MEM_SET(pstEvent, 0, sizeof(APP_VC_EVENT_INFO_STRU));
    }

    void TearDown()
    {
        gstAtSendData.usBufLen          = 0;

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstData)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstData);
        }
    }
};
TEST_F(Test_At_RcvVcMsgQryPortCnfProc,Test_At_RcvVcMsgQryPortCnfProc_001)
{
    // 参数定义

    // 参数初始化

    // 相关全局变量初始化

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(0));

    MOCKER(At_FormatResultData)
        .expects(exactly(0));

    // 调用被测函数
    At_RcvVcMsgQryPortCnfProc(pstData);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}


TEST_F(Test_At_RcvVcMsgQryPortCnfProc,Test_At_RcvVcMsgQryPortCnfProc_002)
{
   // 参数定义
    // 参数初始化
    pstEvent->enVoicePort                     = APP_VC_VOICE_PORT_BUTT;
    // 相关全局变量初始化

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(),eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数
    At_RcvVcMsgQryPortCnfProc(pstData);

    // 执行检查

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}

#if (FEATURE_ON == NAS_FEATURE_CS_VC)

TEST_F(Test_At_RcvVcMsgQryPortCnfProc,Test_At_RcvVcMsgQryPortCnfProc_003)
{
   // 参数定义
    char                                        acExpectedResult[] = "^DDSETEX: 2";
    AT_PAR_CMD_ELEMENT_STRU                     stCmdInfo = {AT_CMD_DDSETEX,
                                                             At_SetDdsetexPara, AT_SET_PARA_TIME,
                                                             At_QryDdsetexPara, AT_QRY_PARA_TIME,
                                                             VOS_NULL_PTR, AT_NOT_SET_TIME,
                                                             VOS_NULL_PTR, AT_NOT_SET_TIME,
                                                             AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
                                                             (VOS_UINT8*)"^DDSETEX",VOS_NULL_PTR};



    // 参数初始化
    pstEvent->enVoicePort                     = APP_VC_VOICE_PORT_DIAG;
    // 相关全局变量初始化
    g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(),eq((VOS_UINT32)AT_OK));


    // 调用被测函数
    At_RcvVcMsgQryPortCnfProc(pstData);

    // 执行检查
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
#endif

/*****************************************************************************
类名 : Test_At_ProcMsgFromDrvAgent
功能描述 : At_ProcMsgFromDrvAgent UT工程类
修改历史     :
1.日   期  : 2011-10-13
作   者  :   l60609
修改内容 : 新生成类
*****************************************************************************/
class Test_At_ProcMsgFromDrvAgent: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }
    void TearDown()
    {

}
};

/*******************************************************************
测试用例编号      : Test_At_ProcMsgFromDrvAgent_001
测试用例标题      : 处理DRV_AGENT_MSID_QRY_CNF消息成功
预期结果          : AT_RcvDrvAgentMsidQryCnf被调用
修改历史     :
1.日   期  : 2011-10-13
作   者  :   l60609
修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_At_ProcMsgFromDrvAgent,Test_At_ProcMsgFromDrvAgent_001)
{
    DRV_AGENT_MSG_STRU                  stMsidQryRsp;

    /*构造DRV_AGENT_MSID_QRY_RSP消息包*/
    stMsidQryRsp.ulSenderPid = WUEPS_PID_DRV_AGENT;
    stMsidQryRsp.ulMsgId   = DRV_AGENT_MSID_QRY_CNF;

    MOCKER(AT_RcvDrvAgentMsidQryCnf)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    /* 调用测试函数 */
    At_ProcMsgFromDrvAgent(&stMsidQryRsp);

    GlobalMockObject::verify();
}

/*******************************************************************
测试用例编号      : Test_At_ProcMsgFromDrvAgent_002
测试用例标题      : 处理DRV_AGENT_MSID_QRY_CNF消息失败
预期结果          : AT_RcvDrvAgentMsidQryCnf被调用，异常打印
修改历史     :
1.日   期  : 2011-10-13
作   者  :   l60609
修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_At_ProcMsgFromDrvAgent,Test_At_ProcMsgFromDrvAgent_002)
{
    DRV_AGENT_MSG_STRU                  stMsidQryRsp;

    /*构造DRV_AGENT_MSID_QRY_RSP消息包*/
    stMsidQryRsp.ulSenderPid = WUEPS_PID_DRV_AGENT;
    stMsidQryRsp.ulMsgId   = DRV_AGENT_MSID_QRY_CNF;

    MOCKER(AT_RcvDrvAgentMsidQryCnf)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_ERR));

    /* 调用测试函数 */
    At_ProcMsgFromDrvAgent(&stMsidQryRsp);

    GlobalMockObject::verify();
}

/*******************************************************************
测试用例编号      : Test_At_ProcMsgFromDrvAgent_003
测试用例标题      : 处理非法消息
预期结果          : AT_RcvDrvAgentMsidQryCnf不被调用，异常打印
修改历史     :
1.日   期  : 2011-10-13
作   者  :   l60609
修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_At_ProcMsgFromDrvAgent,Test_At_ProcMsgFromDrvAgent_003)
{
    DRV_AGENT_MSG_STRU                  stMsidQryRsp;

    /*构造DRV_AGENT_MSID_QRY_RSP消息包*/
    stMsidQryRsp.ulSenderPid = WUEPS_PID_DRV_AGENT;
    stMsidQryRsp.ulMsgId   = DRV_AGENT_MSG_TYPE_BUTT;

    MOCKER(AT_RcvDrvAgentMsidQryCnf)
        .expects(never());

    /* 调用测试函数 */
    At_ProcMsgFromDrvAgent(&stMsidQryRsp);

    GlobalMockObject::verify();
}



class Test_At_PppProtocolRelIndProc: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();
    }
    void TearDown()
    {
    }
};


TEST(Test_At_PppProtocolRelIndProc, Test_At_PppProtocolRelIndProc_001)
{
    // 参数初始化
    AT_PPP_PROTOCOL_REL_IND_MSG_STRU Msg;

    Msg.usPppId = 0;

    gastAtPppIndexTab[0] = AT_CLIENT_TAB_MODEM_INDEX;

    /* 相关全局变量初始化 */
    PS_MEM_SET(gastAtClientTab, 0, sizeof(AT_CLIENT_MANAGE_STRU) * AT_MAX_CLIENT_NUM);

    gastAtClientTab[AT_CLIENT_TAB_MODEM_INDEX].UserType = AT_MODEM_USER;
    gastAtClientTab[AT_CLIENT_TAB_MODEM_INDEX].Mode     = AT_DATA_MODE;
    gastAtClientTab[AT_CLIENT_TAB_MODEM_INDEX].DataMode = AT_DATA_BUTT_MODE;

    // MOCKER操作;
    MOCKER(At_FormatResultData)
    .expects(never());

    // 调用被测函数
    At_PppProtocolRelIndProc(&Msg);

    // 执行检查
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST(Test_At_PppProtocolRelIndProc, Test_At_PppProtocolRelIndProc_002)
{
    // 参数初始化
    AT_PPP_PROTOCOL_REL_IND_MSG_STRU Msg;

    Msg.usPppId = 0;

    gastAtPppIndexTab[0] = AT_CLIENT_TAB_MODEM_INDEX;

    /* 相关全局变量初始化 */
    PS_MEM_SET(gastAtClientTab, 0, sizeof(AT_CLIENT_MANAGE_STRU) * AT_MAX_CLIENT_NUM);

    gastAtClientTab[AT_CLIENT_TAB_MODEM_INDEX].UserType         = AT_MODEM_USER;
    gastAtClientTab[AT_CLIENT_TAB_MODEM_INDEX].Mode             = AT_DATA_MODE;
    gastAtClientTab[AT_CLIENT_TAB_MODEM_INDEX].DataMode         = AT_PPP_DATA_MODE;
    gastAtClientTab[AT_CLIENT_TAB_MODEM_INDEX].CmdCurrentOpt    = AT_CMD_WAIT_PPP_PROTOCOL_REL_SET;

    // MOCKER操作;
    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
    .expects(exactly(1))
    .with(any(), eq((VOS_UINT32)AT_NO_CARRIER));


    // 调用被测函数
    At_PppProtocolRelIndProc(&Msg);

    // 执行检查
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST(Test_At_PppProtocolRelIndProc, Test_At_PppProtocolRelIndProc_003)
{
    // 参数初始化
    AT_PPP_PROTOCOL_REL_IND_MSG_STRU Msg;

    Msg.usPppId = 0;
    VOS_UINT8 ucIndex = AT_CLIENT_TAB_HSUART_INDEX;

    gastAtPppIndexTab[0] = ucIndex;

    /* 相关全局变量初始化 */
    PS_MEM_SET(gastAtClientTab, 0, sizeof(AT_CLIENT_MANAGE_STRU) * AT_MAX_CLIENT_NUM);

    gastAtClientTab[ucIndex].UserType      = AT_HSUART_USER;
    gastAtClientTab[ucIndex].Mode          = AT_DATA_MODE;
    gastAtClientTab[ucIndex].DataMode      = AT_PPP_DATA_MODE;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_WAIT_PPP_PROTOCOL_REL_SET;

    // MOCKER操作;
    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(), eq((VOS_UINT32)AT_NO_CARRIER));


    // 调用被测函数
    At_PppProtocolRelIndProc(&Msg);

    // 执行检查
    EXPECT_EQ(AT_CMD_MODE,gastAtClientTab[ucIndex].Mode);
    EXPECT_EQ(AT_NORMAL_MODE,g_stParseContext[ucIndex].ucMode);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}

/*******************************************************************
*测试项:              AT_ModemMscIndProc
*被测函数功能描述:    MODEM未使用
*预期结果：           warning打印
************************* 修改记录 *************************
#  1.日    期: 2010年03月10日
#    作    者: L47619
#    修改内容: 新建CASE
*******************************************************************/
TEST(Test_At_ModemMscIndProc, Test_At_ModemMscIndProc_001)
{
    // 参数初始化
    AT_PPP_MODEM_MSC_IND_MSG_STRU Msg;

    Msg.ucIndex    = AT_CLIENT_TAB_MODEM_INDEX;
    Msg.ucDlci     = AT_MODEM_USER_DLCI;

    /* 相关全局变量初始化 */
    gastAtClientTab[AT_CLIENT_TAB_MODEM_INDEX].UserType = AT_CTR_USER;

    // MOCKER操作;
	MOCKER(AT_MNTN_TraceInputMsc)
		.expects(once());

    MOCKER(AT_ModemStatusPreProc)
    .expects(never());

    // 调用被测函数
    AT_ModemMscIndProc(&Msg);

    // 执行检查
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}


/*******************************************************************
*测试项:              AT_ModemMscIndProc
*被测函数功能描述:    找到匹配index
*预期结果：           warning打印
************************* 修改记录 *************************
#  1.日    期: 2010年03月10日
#    作    者: L47619
#    修改内容: 新建CASE
*******************************************************************/
TEST(Test_At_ModemMscIndProc,Test_At_ModemMscIndProc_002)
{
    // 参数初始化
    AT_PPP_MODEM_MSC_IND_MSG_STRU Msg;

    Msg.ucIndex    = AT_CLIENT_TAB_MODEM_INDEX;
    Msg.ucDlci     = AT_MODEM_USER_DLCI;

    /* 相关全局变量初始化 */
    /* 相关全局变量初始化 */
    gastAtClientTab[AT_CLIENT_TAB_MODEM_INDEX].UserType = AT_MODEM_USER;
    gastAtClientTab[AT_CLIENT_TAB_MODEM_INDEX].ucUsed = AT_CLIENT_USED;

    // MOCKER操作;
	MOCKER(AT_MNTN_TraceInputMsc)
		.expects(once());

    MOCKER(AT_ModemStatusPreProc)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    AT_ModemMscIndProc(&Msg);

    // 执行检查
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}


TEST(Test_At_ModemMscIndProc, Test_At_ModemMscIndProc_003)
{
    // 参数初始化
    AT_PPP_MODEM_MSC_IND_MSG_STRU       stMsg;
    VOS_UINT8                           ucIndex;

    ucIndex     = AT_CLIENT_TAB_MODEM_INDEX;
    stMsg.ucIndex = ucIndex;

    /* 相关全局变量初始化 */
    gastAtClientTab[ucIndex].UserType = AT_HSUART_USER;
    gastAtClientTab[ucIndex].ucUsed = AT_CLIENT_USED;

    // MOCKER操作;
	MOCKER(AT_MNTN_TraceInputMsc)
		.expects(once());

    MOCKER(AT_ModemStatusPreProc)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    AT_ModemMscIndProc(&stMsg);

    // 执行检查
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}


/*******************************************************************
*测试项:              At_PppMsgProc
*被测函数功能描述:    AT_PPP_RELEASE_IND_MSG消息
*预期结果：
************************* 修改记录 *************************
#  1.日    期: 2010年03月10日
#    作    者: L47619
#    修改内容: 新建CASE
*******************************************************************/
TEST(Test_At_PppMsgProc, AT_PPP_RELEASE_IND_MSG)
{
    // 参数初始化
	AT_PPP_RELEASE_IND_MSG_STRU Msg;

	Msg.MsgHeader.ulMsgName = AT_PPP_RELEASE_IND_MSG;

    /* 相关全局变量初始化 */


    // MOCKER操作;
	MOCKER(At_PppReleaseIndProc)
		.expects(exactly(1));

    // 调用被测函数
    At_PppMsgProc((MSG_HEADER_STRU *)&Msg);

    // 执行检查
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}



/*******************************************************************
*测试项:              At_PppMsgProc
*被测函数功能描述:    AT_PPP_MODEM_MSC_IND_MSG消息
*预期结果：
************************* 修改记录 *************************
#  1.日    期: 2010年03月10日
#    作    者: L47619
#    修改内容: 新建CASE
*******************************************************************/
TEST(Test_At_PppMsgProc, AT_PPP_MODEM_MSC_IND_MSG)
{
    // 参数初始化
	AT_PPP_MODEM_MSC_IND_MSG_STRU Msg;

	Msg.MsgHeader.ulMsgName = AT_PPP_MODEM_MSC_IND_MSG;

    /* 相关全局变量初始化 */


    // MOCKER操作;
    MOCKER(AT_ModemMscIndProc)
        .expects(exactly(1));

    // 调用被测函数
    At_PppMsgProc((MSG_HEADER_STRU *)&Msg);

    // 执行检查
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}



/*******************************************************************
*测试项:              At_PppMsgProc
*被测函数功能描述:    AT_PPP_PROTOCOL_REL_IND_MSG消息
*预期结果：
************************* 修改记录 *************************
#  1.日    期: 2010年03月10日
#    作    者: L47619
#    修改内容: 新建CASE
*******************************************************************/
TEST(Test_At_PppMsgProc, AT_PPP_PROTOCOL_REL_IND_MSG)
{
    // 参数初始化
	AT_PPP_PROTOCOL_REL_IND_MSG_STRU Msg;

	Msg.MsgHeader.ulMsgName = AT_PPP_PROTOCOL_REL_IND_MSG;

    /* 相关全局变量初始化 */


    // MOCKER操作;
	MOCKER(At_PppProtocolRelIndProc)
		.expects(exactly(1));

    // 调用被测函数
    At_PppMsgProc((MSG_HEADER_STRU *)&Msg);

    // 执行检查
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}

#if (FEATURE_ON == NAS_FEATURE_SD_PWD)
class Test_AT_SetSdrebootCmd: public ::testing::Test
{
public:

    void SetUp()
	{
		UT_STUB_INIT();
    }

    void TearDown()
    {
    }

};

TEST_F(Test_AT_SetSdrebootCmd,Test_AT_SetSdrebootCmd_01)
{
    VOS_UINT8                           ucData[100]="AT^SDREBOOT=\"HWGP012011";
    VOS_UINT16                          usLength;

    usLength = VOS_StrLen((VOS_CHAR *) ucData);

    EXPECT_EQ( VOS_ERR, AT_SetSdrebootCmd(ucData, usLength) );
}


TEST_F(Test_AT_SetSdrebootCmd,Test_AT_SetSdrebootCmd_02)
{
    VOS_UINT8                           ucData[100]="AT^SDREBOOT=HWGP012011\"";
    VOS_UINT16                          usLength;

    usLength = VOS_StrLen((VOS_CHAR *) ucData);

    EXPECT_EQ( VOS_ERR, AT_SetSdrebootCmd(ucData, usLength) );
}


TEST_F(Test_AT_SetSdrebootCmd,Test_AT_SetSdrebootCmd_03)
{
    VOS_UINT8                           ucData[100]="AT^SDREBOOT=\"HWGP012011\"";
    VOS_UINT16                          usLength;

    usLength = VOS_StrLen((VOS_CHAR *) ucData);

    VOS_StrCpy((VOS_CHAR *)g_acATE5DissdPwd, "ERROR");

    EXPECT_EQ( VOS_ERR, AT_SetSdrebootCmd(ucData, usLength) );
}


TEST_F(Test_AT_SetSdrebootCmd,Test_AT_SetSdrebootCmd_04)
{
    VOS_UINT8                           ucData[100]="AT^SDREBOOT=\"HWGP012011\"";
    VOS_UINT16                          usLength;

    usLength = VOS_StrLen((VOS_CHAR *) ucData);

    VOS_StrCpy((VOS_CHAR *)g_acATE5DissdPwd, "HWGP012011");

    EXPECT_EQ( VOS_OK, AT_SetSdrebootCmd(ucData, usLength) );
}
#endif

class Test_AT_RcvDrvAgentHardwareQryRsp: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                 *pRcvMsg;
    DRV_AGENT_HARDWARE_QRY_CNF_STRU    *pstEvent;
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stElement;
    VOS_UINT8                           szCmdName[32];

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                                 = 0;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_INVALID;

        VOS_StrCpy((VOS_CHAR*)szCmdName,"^HVER");
        g_stParseContext[ucIndex].pstCmdElement = &stElement;
        stElement.pszCmdName                    = szCmdName;

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_HARDWARE_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstEvent = (DRV_AGENT_HARDWARE_QRY_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(DRV_AGENT_MSG_STRU));
        PS_MEM_SET(pstEvent, 0, sizeof(DRV_AGENT_HARDWARE_QRY_CNF_STRU));
    }

    void TearDown()
    {
        ucIndex                                 = 0;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_INVALID;

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentHardwareQryRsp_001
测试用例标题      :无效的CLIENT_ID值
预期结果          :返回错误
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentHardwareQryRsp,Test_AT_RcvDrvAgentHardwareQryRsp_001)
{
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    EXPECT_EQ(VOS_ERR,AT_RcvDrvAgentHardwareQryRsp(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentHardwareQryRsp_002
测试用例标题      :接收到回复消息指示查询结果正确
预期结果          :返回OK
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentHardwareQryRsp,Test_AT_RcvDrvAgentHardwareQryRsp_002)
{
    pstEvent->ulResult                      = DRV_AGENT_NO_ERROR;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_DRV_AGENT_HARDWARE_QRY;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_OK,AT_RcvDrvAgentHardwareQryRsp(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentHardwareQryRsp_003
测试用例标题      :非等待的消息
预期结果          :返回错误
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentHardwareQryRsp,Test_AT_RcvDrvAgentHardwareQryRsp_003)
{
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    EXPECT_EQ(VOS_ERR,AT_RcvDrvAgentHardwareQryRsp(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentHardwareQryRsp_004
测试用例标题      :接收到回复消息指示查询结果错误
预期结果          :返回OK
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentHardwareQryRsp,Test_AT_RcvDrvAgentHardwareQryRsp_004)
{
    pstEvent->ulResult                      = DRV_AGENT_ERROR;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_DRV_AGENT_HARDWARE_QRY;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_OK,AT_RcvDrvAgentHardwareQryRsp(pRcvMsg));

    GlobalMockObject::verify();
}

class Test_AT_RcvDrvAgentQryRxdivCnf: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                 *pRcvMsg;
    DRV_AGENT_QRY_RXDIV_CNF_STRU       *pstEvent;
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stElement;
    VOS_UINT8                           szCmdName[32];

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        VOS_StrCpy((VOS_CHAR*)szCmdName,"^rxdiv");
        g_stParseContext[ucIndex].pstCmdElement = &stElement;
        stElement.pszCmdName                    = szCmdName;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_QRY_RXDIV_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstEvent = (DRV_AGENT_QRY_RXDIV_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(DRV_AGENT_MSG_STRU));
        PS_MEM_SET(pstEvent, 0, sizeof(DRV_AGENT_QRY_RXDIV_CNF_STRU));
    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentQryRxdivCnf_001
测试用例标题      :无效的CLIENT_ID值
预期结果          :返回AT_ERROR
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentQryRxdivCnf,Test_AT_RcvDrvAgentQryRxdivCnf_001)
{
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    EXPECT_EQ(VOS_ERR,AT_RcvDrvAgentQryRxdivCnf(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentQryRxdivCnf_002
测试用例标题      :非等待的消息
预期结果          :返回错误
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentQryRxdivCnf,Test_AT_RcvDrvAgentQryRxdivCnf_002)
{
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    EXPECT_EQ(VOS_ERR,AT_RcvDrvAgentQryRxdivCnf(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentQryRxdivCnf_003
测试用例标题      :回复消息指示为正确
预期结果          :返回VOS_OK
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentQryRxdivCnf,Test_AT_RcvDrvAgentQryRxdivCnf_003)
{
    VOS_UINT32                                       ulDivBandsLow;
    VOS_UINT32                                       ulDivBandsHigh;

    ulDivBandsLow       = 1;
    ulDivBandsHigh      = 1;

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_DRV_AGENT_RXDIV_QRY_REQ;
    pstEvent->ulResult                     = DRV_AGENT_NO_ERROR;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_CovertMsInternalRxDivParaToUserSet)
        .stubs()
        .with(any(),outBoundP(&ulDivBandsLow),outBoundP(&ulDivBandsHigh));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_OK,AT_RcvDrvAgentQryRxdivCnf(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentQryRxdivCnf_004
测试用例标题      :回复消息指示为其他错误
预期结果          :返回错误
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentQryRxdivCnf,Test_AT_RcvDrvAgentQryRxdivCnf_004)
{
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_DRV_AGENT_RXDIV_QRY_REQ;
    pstEvent->ulResult                     = DRV_AGENT_ERROR;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_OK,AT_RcvDrvAgentQryRxdivCnf(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentQryRxdivCnf_005
测试用例标题      :回复消息指示为正确但主集和分集都为0
预期结果          :返回VOS_OK
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentQryRxdivCnf,Test_AT_RcvDrvAgentQryRxdivCnf_005)
{
    VOS_UINT32                                       ulDivBandsLow;
    VOS_UINT32                                       ulDivBandsHigh;

    ulDivBandsLow       = 0;
    ulDivBandsHigh      = 0;

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_DRV_AGENT_RXDIV_QRY_REQ;
    pstEvent->ulResult                     = DRV_AGENT_NO_ERROR;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_CovertMsInternalRxDivParaToUserSet)
        .stubs()
        .with(any(),outBoundP(&ulDivBandsLow, (VOS_UINT32)sizeof(ulDivBandsLow)),outBoundP(&ulDivBandsHigh, (VOS_UINT32)sizeof(ulDivBandsHigh)));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_OK,AT_RcvDrvAgentQryRxdivCnf(pRcvMsg));

    GlobalMockObject::verify();
}

class Test_AT_RcvDrvAgentSetRxdivCnf: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                 *pRcvMsg;
    DRV_AGENT_AT_RXDIV_CNF_STRU        *pstEvent;
    VOS_UINT8                           ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_AT_RXDIV_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstEvent = (DRV_AGENT_AT_RXDIV_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(DRV_AGENT_MSG_STRU));
        PS_MEM_SET(pstEvent, 0, sizeof(DRV_AGENT_AT_RXDIV_CNF_STRU));
    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentSetRxdivCnf_001
测试用例标题      :无效的CLIENT_ID值
预期结果          :返回AT_ERROR
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentSetRxdivCnf,Test_AT_RcvDrvAgentSetRxdivCnf_001)
{
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    EXPECT_EQ(VOS_ERR,AT_RcvDrvAgentSetRxdivCnf(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentSetRxdivCnf_002
测试用例标题      :非等待的消息
预期结果          :返回错误
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentSetRxdivCnf,Test_AT_RcvDrvAgentSetRxdivCnf_002)
{
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    EXPECT_EQ(VOS_ERR,AT_RcvDrvAgentSetRxdivCnf(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentSetRxdivCnf_003
测试用例标题      :回复消息指示为其他错误
预期结果          :返回错误
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentSetRxdivCnf,Test_AT_RcvDrvAgentSetRxdivCnf_003)
{
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_DRV_AGENT_RXDIV_SET_REQ;
    pstEvent->ulResult                     = DRV_AGENT_CME_RX_DIV_OTHER_ERR;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

	MOCKER(AT_StopRelTimer)
		.expects(exactly(1))
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_ERR,AT_RcvDrvAgentSetRxdivCnf(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentSetRxdivCnf_004
测试用例标题      :回复消息指示为分集不支持
预期结果          :返回错误
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentSetRxdivCnf,Test_AT_RcvDrvAgentSetRxdivCnf_004)
{
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_DRV_AGENT_RXDIV_SET_REQ;
    pstEvent->ulResult                     = DRV_AGENT_CME_RX_DIV_NOT_SUPPORTED;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

	MOCKER(AT_StopRelTimer)
		.expects(exactly(1))
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_ERR,AT_RcvDrvAgentSetRxdivCnf(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentSetRxdivCnf_006
测试用例标题      :RX打开下保存到NV中
预期结果          :返回错误
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentSetRxdivCnf,Test_AT_RcvDrvAgentSetRxdivCnf_006)
{
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_DRV_AGENT_RXDIV_SET_REQ;
    pstEvent->ulResult                      = DRV_AGENT_NO_ERROR;
    pstEvent->ucRxOnOff                     = AT_DSP_RF_SWITCH_ON-1;
    g_stAtDevCmdCtrl.ucCurrentTMode         = AT_TMODE_FTM;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(NV_WriteEx)
        .stubs()
        .will(returnValue((VOS_UINT32)NV_OK));

	MOCKER(AT_StopRelTimer)
		.expects(exactly(1))
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_OK,AT_RcvDrvAgentSetRxdivCnf(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentSetRxdivCnf_005
测试用例标题      :设置的频段不是当前使用的频段
预期结果          :返回错误
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentSetRxdivCnf,Test_AT_RcvDrvAgentSetRxdivCnf_005)
{
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_DRV_AGENT_RXDIV_SET_REQ;
    pstEvent->ulResult                      = DRV_AGENT_CME_RX_DIV_BAND_ERR;
    pstEvent->ucRxOnOff                     = AT_DSP_RF_SWITCH_ON;
    g_stAtDevCmdCtrl.ucCurrentTMode         = AT_TMODE_FTM;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

	MOCKER(AT_StopRelTimer)
		.expects(exactly(1))
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_ERR,AT_RcvDrvAgentSetRxdivCnf(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentSetRxdivCnf_008
测试用例标题      :发送消息HPA失败
预期结果          :返回错误
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentSetRxdivCnf,Test_AT_RcvDrvAgentSetRxdivCnf_008)
{
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_DRV_AGENT_RXDIV_SET_REQ;
    pstEvent->ulResult                      = DRV_AGENT_NO_ERROR;
    pstEvent->ucRxOnOff                     = AT_DSP_RF_SWITCH_ON;
    g_stAtDevCmdCtrl.ucCurrentTMode         = AT_TMODE_FTM;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_SendRfCfgAntSelToHPA)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

	MOCKER(AT_StopRelTimer)
		.expects(exactly(1))
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_ERR,AT_RcvDrvAgentSetRxdivCnf(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentSetRxdivCnf_007
测试用例标题      :发送消息HPA成功
预期结果          :返回错误
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentSetRxdivCnf,Test_AT_RcvDrvAgentSetRxdivCnf_007)
{
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_DRV_AGENT_RXDIV_SET_REQ;
    pstEvent->ulResult                      = DRV_AGENT_NO_ERROR;
    pstEvent->ucRxOnOff                     = AT_DSP_RF_SWITCH_ON;
    g_stAtDevCmdCtrl.ucCurrentTMode         = AT_TMODE_FTM;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_SendRfCfgAntSelToHPA)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    EXPECT_EQ(AT_WAIT_ASYNC_RETURN,AT_RcvDrvAgentSetRxdivCnf(pRcvMsg));

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentFullHardwareQryRsp: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                      *pRcvMsg;
    DRV_AGENT_FULL_HARDWARE_QRY_CNF_STRU    *pstEvent;
    VOS_UINT8                                ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                  stElement;
    VOS_UINT8                                szCmdName[32];

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                                 = 0;

        VOS_StrCpy((VOS_CHAR*)szCmdName,"^FHVER");
        g_stParseContext[ucIndex].pstCmdElement = &stElement;
        stElement.pszCmdName                    = szCmdName;

        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_INVALID;

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_FULL_HARDWARE_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstEvent = (DRV_AGENT_FULL_HARDWARE_QRY_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(DRV_AGENT_MSG_STRU));
        PS_MEM_SET(pstEvent, 0, sizeof(DRV_AGENT_FULL_HARDWARE_QRY_CNF_STRU));
    }

    void TearDown()
    {
        ucIndex                                 = 0;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_INVALID;

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }

    }
};
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentFullHardwareQryRsp_001
测试用例标题      :无效的CLIENT_ID值
预期结果          :返回AT_ERROR
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentFullHardwareQryRsp,Test_AT_RcvDrvAgentFullHardwareQryRsp_001)
{
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    EXPECT_EQ(VOS_ERR,AT_RcvDrvAgentFullHardwareQryRsp(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentFullHardwareQryRsp_002
测试用例标题      :查询完整的软硬件版本信息失败
预期结果          :返回AT_ERROR
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentFullHardwareQryRsp,Test_AT_RcvDrvAgentFullHardwareQryRsp_002)
{
    pstEvent->ulResult                      = DRV_AGENT_ERROR;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_DRV_AGENT_FULL_HARDWARE_QRY;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_OK,AT_RcvDrvAgentFullHardwareQryRsp(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentFullHardwareQryRsp_003
测试用例标题      :查询完整的软硬件版本信息成功
预期结果          :返回AT_OK
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentFullHardwareQryRsp,Test_AT_RcvDrvAgentFullHardwareQryRsp_003)
{
    pstEvent->ulResult                      = DRV_AGENT_NO_ERROR;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_DRV_AGENT_FULL_HARDWARE_QRY;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

	MOCKER(AT_StopRelTimer)
		.expects(exactly(1))
		.will(returnValue((VOS_UINT32)VOS_OK));

    /*MOCKER(At_sprintf)
        .expects(exactly(4));*/

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_OK,AT_RcvDrvAgentFullHardwareQryRsp(pRcvMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvDrvAgentFullHardwareQryRsp_004
测试用例标题      :非等待的消息
预期结果          :返回错误
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentFullHardwareQryRsp,Test_AT_RcvDrvAgentFullHardwareQryRsp_004)
{
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    EXPECT_EQ(VOS_ERR,AT_RcvDrvAgentFullHardwareQryRsp(pRcvMsg));

    GlobalMockObject::verify();
}



class Test_AT_FormatAtiCmdQryString: public ::testing::Test
{
public:

    void SetUp()
	{
		UT_STUB_INIT();
        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
    }

    void TearDown()
    {
        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
    }
};


TEST_F(Test_AT_FormatAtiCmdQryString, Test_AT_FormatAtiCmdQryString_001)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;

    /* 变量初始化 */


    /* 函数打桩 */


    /* 调用被测函数 */
    ulRst = AT_FormatAtiCmdQryString(MODEM_ID_0, VOS_NULL_PTR);

    /* 结果验证 */
    ASSERT_EQ(AT_ERROR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_FormatAtiCmdQryString, Test_AT_FormatAtiCmdQryString_002)
{
    /* 变量定义 */
    DRV_AGENT_MSID_QRY_CNF_STRU         stDrvAgentMsidQryCnf;
    VOS_UINT32                          ulRst;

    /* 变量初始化 */
    PS_MEM_SET(&stDrvAgentMsidQryCnf, 0, sizeof(DRV_AGENT_MSID_QRY_CNF_STRU));
    stDrvAgentMsidQryCnf.ulResult = DRV_AGENT_MSID_QRY_READ_PRODUCT_ID_ERROR;

    /* 函数打桩 */

    /* 调用被测函数 */
    ulRst = AT_FormatAtiCmdQryString(MODEM_ID_0, &stDrvAgentMsidQryCnf);

    /* 结果验证 */
    ASSERT_EQ(AT_ERROR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_FormatAtiCmdQryString, Test_AT_FormatAtiCmdQryString_003)
{
    /* 变量定义 */
    DRV_AGENT_MSID_QRY_CNF_STRU         stDrvAgentMsidQryCnf;
    VOS_UINT32                          ulRst;

    /* 变量初始化 */
    PS_MEM_SET(&stDrvAgentMsidQryCnf, 0, sizeof(DRV_AGENT_MSID_QRY_CNF_STRU));
    stDrvAgentMsidQryCnf.ulResult = DRV_AGENT_MSID_QRY_NO_ERROR;

    /* 函数打桩 */
    MOCKER(NV_ReadEx)
        .stubs()
        .with(any(),eq((VOS_UINT16)en_NV_Item_FMRID), any(), any())
        .will(returnValue(NV_BUFFER_NOT_EXIST));

    /* 调用被测函数 */
    ulRst = AT_FormatAtiCmdQryString(MODEM_ID_0, &stDrvAgentMsidQryCnf);

    /* 结果验证 */
    ASSERT_EQ(AT_ERROR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_FormatAtiCmdQryString, Test_AT_FormatAtiCmdQryString_005)
{
    /* 变量定义 */
    DRV_AGENT_MSID_QRY_CNF_STRU         stDrvAgentMsidQryCnf;
    VOS_UINT32                          ulRst;
    VOS_UINT8                           aucFmrId[AT_MAX_VERSION_LEN] = "Balong V3R2";
    VOS_UINT8                           aucImei[TAF_PH_IMEI_LEN]     = {0x01, 0x02, 0x03, 0x04, 0x05};

    VOS_INT8                            acModelId[TAF_MAX_MODEL_ID_LEN + 1]          = "Model Id Info";      /* 模块信息 */
    VOS_INT8                            acSoftwareVerId[TAF_MAX_REVISION_ID_LEN + 1] = "Soft Version Info";  /* 软件版本信息 */

    char                                acExpectedResult[] = "Manufacturer: Balong V3R2\r\nModel: Model Id Info\r\nRevision: Soft Version Info\r\nIMEI: 123450000000009\r\n+GCAP: +CGSM,+DS,+ES";

    /* 变量初始化 */
    PS_MEM_SET(&stDrvAgentMsidQryCnf, 0, sizeof(DRV_AGENT_MSID_QRY_CNF_STRU));

    stDrvAgentMsidQryCnf.ulResult = DRV_AGENT_MSID_QRY_NO_ERROR;
    PS_MEM_CPY(stDrvAgentMsidQryCnf.acModelId, acModelId, sizeof(acModelId));
    PS_MEM_CPY(stDrvAgentMsidQryCnf.acSoftwareVerId, acSoftwareVerId, sizeof(acSoftwareVerId));
    PS_MEM_CPY(stDrvAgentMsidQryCnf.aucImei, aucImei, TAF_PH_IMEI_LEN);

    /* 函数打桩 */
    MOCKER(NV_ReadEx)
        .stubs()
        .with(any(), eq((VOS_UINT16)en_NV_Item_FMRID), outBoundP((VOS_VOID *)aucFmrId, (VOS_UINT8)sizeof(aucFmrId)), any())
        .will(returnValue((VOS_UINT32)NV_OK));

    /* 调用被测函数 */
    ulRst = AT_FormatAtiCmdQryString(MODEM_ID_0, &stDrvAgentMsidQryCnf);

    /* 结果验证 */
    ASSERT_EQ(AT_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);

  //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}

#if (FEATURE_ON == FEATURE_MULTI_MODEM)
TEST_F(Test_AT_FormatAtiCmdQryString, Test_AT_FormatAtiCmdQryString_006)
{
    /* 变量定义 */
    DRV_AGENT_MSID_QRY_CNF_STRU         stDrvAgentMsidQryCnf;
    VOS_UINT32                          ulRst;
    VOS_UINT8                           aucFmrId[AT_MAX_VERSION_LEN] = "Balong V3R2";
    VOS_UINT8                           aucImei[TAF_PH_IMEI_LEN]     = {0x01, 0x02, 0x03, 0x04, 0x05};

    VOS_INT8                            acModelId[TAF_MAX_MODEL_ID_LEN + 1]          = "Model Id Info";      /* 模块信息 */
    VOS_INT8                            acSoftwareVerId[TAF_MAX_REVISION_ID_LEN + 1] = "Soft Version Info";  /* 软件版本信息 */

    char                                acExpectedResult[] = "Manufacturer: Balong V3R2\r\nModel: Model Id Info\r\nRevision: Soft Version Info\r\nIMEI: 123450000000009\r\n+GCAP: +CGSM,+DS,+ES";

    /* 变量初始化 */
    PS_MEM_SET(&stDrvAgentMsidQryCnf, 0, sizeof(DRV_AGENT_MSID_QRY_CNF_STRU));

    stDrvAgentMsidQryCnf.ulResult = DRV_AGENT_MSID_QRY_NO_ERROR;
    stDrvAgentMsidQryCnf.stAtAppCtrl.usClientId = AT_CLIENT_ID_CTRL;
    PS_MEM_CPY(stDrvAgentMsidQryCnf.acModelId, acModelId, sizeof(acModelId));
    PS_MEM_CPY(stDrvAgentMsidQryCnf.acSoftwareVerId, acSoftwareVerId, sizeof(acSoftwareVerId));
    PS_MEM_CPY(stDrvAgentMsidQryCnf.aucImei, aucImei, TAF_PH_IMEI_LEN);

    /* 函数打桩 */
    MOCKER(NV_ReadEx)
        .stubs()
        .with(any(), eq((VOS_UINT16)en_NV_Item_FMRID), outBoundP((VOS_VOID *)aucFmrId, (VOS_UINT8)sizeof(aucFmrId)), any())
        .will(returnValue((VOS_UINT32)NV_OK));

    /* 调用被测函数 */
    ulRst = AT_FormatAtiCmdQryString(MODEM_ID_1, &stDrvAgentMsidQryCnf);

    /* 结果验证 */
    ASSERT_EQ(AT_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);

  //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
#endif
class Test_AT_RcvDrvAgentMsidQryCnf: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                 *pRcvMsg;
    DRV_AGENT_MSID_QRY_CNF_STRU        *pstDrvAgentMsidQryCnf;
    VOS_UINT8                           ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_MSID_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstDrvAgentMsidQryCnf = (DRV_AGENT_MSID_QRY_CNF_STRU *)(pRcvMsg->aucContent);

        At_PidInit(VOS_IP_INITIAL);

    }
    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentMsidQryCnf, Test_AT_RcvDrvAgentMsidQryCnf_001)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;

    /* 变量初始化 */


    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentMsidQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvDrvAgentMsidQryCnf, Test_AT_RcvDrvAgentMsidQryCnf_002)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_MSID_READ;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(AT_FormatAtiCmdQryString)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_ERROR));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(),eq((VOS_UINT32)AT_ERROR));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentMsidQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvDrvAgentMsidQryCnf, Test_AT_RcvDrvAgentMsidQryCnf_003)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_MSID_READ;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(AT_FormatAtiCmdQryString)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(),eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentMsidQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvDrvAgentMsidQryCnf, Test_AT_RcvDrvAgentMsidQryCnf_004)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentMsidQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}

#if (FEATURE_ON == FEATURE_MULTI_MODEM)
TEST_F(Test_AT_RcvDrvAgentMsidQryCnf, Test_AT_RcvDrvAgentMsidQryCnf_005)
{
    /* 变量定义 */
    DRV_AGENT_MSID_QRY_CNF_STRU         stDrvAgentMsidQryCnf;
    VOS_UINT32                          ulRst;
    VOS_UINT8                           aucFmrId[AT_MAX_VERSION_LEN] = "Balong V3R2";
    VOS_UINT8                           aucImei[TAF_PH_IMEI_LEN]     = {0x01, 0x02, 0x03, 0x04, 0x05};

    VOS_INT8                            acModelId[TAF_MAX_MODEL_ID_LEN + 1]          = "Model Id Info";      /* 模块信息 */
    VOS_INT8                            acSoftwareVerId[TAF_MAX_REVISION_ID_LEN + 1] = "Soft Version Info";  /* 软件版本信息 */

   // char                                acExpectedResult[] = "Manufacturer: Balong V3R2\r\nModel: Model Id Info\r\nRevision: Soft Version Info\r\nIMEI: 123450000000009\r\n+GCAP: +CGSM,+DS,+ES\r\n\r\nOK\r\n";
    VOS_UINT8                           ucIndex;
    AT_CLIENT_CTX_STRU                 *pstClientCtx;

    ucIndex = AT_CLIENT_TAB_CTRL_INDEX;

    pstClientCtx = AT_GetClientCtxAddr(ucIndex);

    pstClientCtx->stClientConfiguration.enModemId = MODEM_ID_1;

    /* 变量初始化 */
    PS_MEM_SET(&stDrvAgentMsidQryCnf, 0, sizeof(DRV_AGENT_MSID_QRY_CNF_STRU));

    stDrvAgentMsidQryCnf.ulResult = DRV_AGENT_MSID_QRY_NO_ERROR;
    stDrvAgentMsidQryCnf.stAtAppCtrl.usClientId = AT_CLIENT_ID_CTRL;
    PS_MEM_CPY(stDrvAgentMsidQryCnf.acModelId, acModelId, sizeof(acModelId));
    PS_MEM_CPY(stDrvAgentMsidQryCnf.acSoftwareVerId, acSoftwareVerId, sizeof(acSoftwareVerId));
    PS_MEM_CPY(stDrvAgentMsidQryCnf.aucImei, aucImei, TAF_PH_IMEI_LEN);

    PS_MEM_CPY(pRcvMsg->aucContent, &stDrvAgentMsidQryCnf, sizeof(stDrvAgentMsidQryCnf));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_MSID_READ;

    /* 函数打桩 */
    MOCKER(NV_ReadEx)
        .stubs()
        .with(any(), eq((VOS_UINT16)en_NV_Item_FMRID), outBoundP((VOS_VOID *)aucFmrId, (VOS_UINT8)sizeof(aucFmrId)), any())
        .will(returnValue((VOS_UINT32)NV_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentMsidQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
  //  EXPECT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
#endif
class Test_AT_RcvDrvAgentGasMntnCmdRsp: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                 *pRcvMsg;
    DRV_AGENT_GAS_MNTN_CMD_CNF_STRU    *pstDrvAgentGasMntnCmdCnf;
    VOS_UINT8                           ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_GAS_MNTN_CMD_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstDrvAgentGasMntnCmdCnf = (DRV_AGENT_GAS_MNTN_CMD_CNF_STRU *)(pRcvMsg->aucContent);

    }
    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentGasMntnCmdRsp, Test_AT_RcvDrvAgentGasMntnCmdRsp_001)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;

    /* 变量初始化 */


    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentGasMntnCmdRsp(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvDrvAgentGasMntnCmdRsp, Test_AT_RcvDrvAgentGasMntnCmdRsp_002)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CGAS_QUERY;

    pstDrvAgentGasMntnCmdCnf->ulResult               = VOS_ERR;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.ulRsltNum  = 1;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[0] = 2;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(),eq(AT_CME_UNKNOWN));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentGasMntnCmdRsp(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvDrvAgentGasMntnCmdRsp, Test_AT_RcvDrvAgentGasMntnCmdRsp_003)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CGAS_QUERY;

    pstDrvAgentGasMntnCmdCnf->ulResult               = VOS_OK;

    pstDrvAgentGasMntnCmdCnf->ucCmd                  = GAS_AT_CMD_NCELL;

    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.ulRsltNum  = 3;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[0] = 0;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[1] = 1;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[2] = 2;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[3] = 3;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[4] = 4;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[5] = 5;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[6] = 6;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[7] = 7;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[8] = 8;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[9] = 9;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[10] = 10;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[11] = 11;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[12] = 12;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[13] = 13;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[14] = 14;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(),eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentGasMntnCmdRsp(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvDrvAgentGasMntnCmdRsp, Test_AT_RcvDrvAgentGasMntnCmdRsp_004)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CGAS_QUERY;

    pstDrvAgentGasMntnCmdCnf->ulResult               = VOS_OK;

    pstDrvAgentGasMntnCmdCnf->ucCmd                  = GAS_AT_CMD_NCELL;

    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.ulRsltNum  = 0;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[0] = 0;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[1] = 1;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[2] = 2;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[3] = 3;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[4] = 4;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[5] = 5;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[6] = 6;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[7] = 7;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[8] = 8;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[9] = 9;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[10] = 10;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[11] = 11;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[12] = 12;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[13] = 13;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[14] = 14;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(),eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentGasMntnCmdRsp(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvDrvAgentGasMntnCmdRsp, Test_AT_RcvDrvAgentGasMntnCmdRsp_005)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CGAS_QUERY;

    pstDrvAgentGasMntnCmdCnf->ulResult               = VOS_OK;

    pstDrvAgentGasMntnCmdCnf->ucCmd                  = GAS_AT_CMD_NCELL;

    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.ulRsltNum  = 1;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[0] = 0;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[1] = 1;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[2] = 2;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[3] = 3;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[4] = 4;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[5] = 5;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[6] = 6;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[7] = 7;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[8] = 8;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[9] = 9;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[10] = 10;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[11] = 11;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[12] = 12;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[13] = 13;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[14] = 14;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(),eq((VOS_UINT32)AT_ERROR));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentGasMntnCmdRsp(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvDrvAgentGasMntnCmdRsp, Test_AT_RcvDrvAgentGasMntnCmdRsp_006)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentGasMntnCmdRsp(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvDrvAgentGasMntnCmdRsp, Test_AT_RcvDrvAgentGasMntnCmdRsp_007)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CGAS_QUERY;

    pstDrvAgentGasMntnCmdCnf->ulResult               = VOS_OK;

    pstDrvAgentGasMntnCmdCnf->ucCmd                  = GAS_AT_CMD_SCELL;

    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.ulRsltNum  = 1;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[0] = 0;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[1] = 1;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[2] = 2;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[3] = 3;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[4] = 4;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[5] = 5;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[6] = 6;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[7] = 7;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[8] = 8;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[9] = 9;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[10] = 10;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[11] = 11;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[12] = 12;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[13] = 13;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[14] = 14;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(),eq((VOS_UINT32)AT_ERROR));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentGasMntnCmdRsp(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvDrvAgentGasMntnCmdRsp, Test_AT_RcvDrvAgentGasMntnCmdRsp_008)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CGAS_QUERY;

    pstDrvAgentGasMntnCmdCnf->ulResult               = VOS_OK;

    pstDrvAgentGasMntnCmdCnf->ucCmd                  = GAS_AT_CMD_SCELL;

    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.ulRsltNum  = 3;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[0] = 0;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[1] = 1;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[2] = 2;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[3] = 3;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[4] = 4;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[5] = 5;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[6] = 6;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[7] = 7;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[8] = 8;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[9] = 9;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[10] = 10;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[11] = 11;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[12] = 12;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[13] = 13;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[14] = 14;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(),eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentGasMntnCmdRsp(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvDrvAgentGasMntnCmdRsp, Test_AT_RcvDrvAgentGasMntnCmdRsp_009)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CGAS_QUERY;

    pstDrvAgentGasMntnCmdCnf->ulResult               = VOS_OK;

    pstDrvAgentGasMntnCmdCnf->ucCmd                  = GAS_AT_CMD_MEASURE_UP;

    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.ulRsltNum  = 3;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[0] = 0;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[1] = 1;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[2] = 2;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[3] = 3;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[4] = 4;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[5] = 5;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[6] = 6;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[7] = 7;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[8] = 8;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[9] = 9;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[10] = 10;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[11] = 11;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[12] = 12;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[13] = 13;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[14] = 14;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(),eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentGasMntnCmdRsp(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvDrvAgentGasMntnCmdRsp, Test_AT_RcvDrvAgentGasMntnCmdRsp_010)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CGAS_QUERY;

    pstDrvAgentGasMntnCmdCnf->ulResult               = VOS_OK;

    pstDrvAgentGasMntnCmdCnf->ucCmd                  = GAS_AT_CMD_MEASURE_UP;

    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.ulRsltNum  = 0;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[0] = 0;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[1] = 1;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[2] = 2;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[3] = 3;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[4] = 4;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[5] = 5;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[6] = 6;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[7] = 7;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[8] = 8;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[9] = 9;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[10] = 10;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[11] = 11;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[12] = 12;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[13] = 13;
    pstDrvAgentGasMntnCmdCnf->stAtCmdRslt.aulRslt[14] = 14;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(),eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentGasMntnCmdRsp(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
class Test_AT_RcvDrvAgentVertimeQryRsp: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                 *pRcvMsg;
    DRV_AGENT_VERSION_TIME_STRU        *pstDrvAgentVersionTime;
    VOS_UINT8                           ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_VERSION_TIME_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstDrvAgentVersionTime = (DRV_AGENT_VERSION_TIME_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(DRV_AGENT_MSG_STRU));
        PS_MEM_SET(pstDrvAgentVersionTime, 0, sizeof(DRV_AGENT_VERSION_TIME_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentVertimeQryRsp, Test_AT_RcvDrvAgentVertimeQryRsp_001)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;

    /* 变量初始化 */


    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentVertimeQryRsp(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvDrvAgentVertimeQryRsp, Test_AT_RcvDrvAgentVertimeQryRsp_002)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentVertimeQryRsp(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvDrvAgentVertimeQryRsp, Test_AT_RcvDrvAgentVertimeQryRsp_003)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    char                                acExpectedResult[] = "^VERTIME:2011,01,02";
    VOS_CHAR                            acVerTime[] = "2011,01,02";
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       =   {AT_CMD_VERTIME,
                                                                AT_SetVertime,              AT_SET_PARA_TIME,
                                                                VOS_NULL_PTR,               AT_NOT_SET_TIME,
                                                                VOS_NULL_PTR,               AT_NOT_SET_TIME,
                                                                VOS_NULL_PTR,               AT_NOT_SET_TIME,
                                                                AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
                                                                (VOS_UINT8*)"^VERTIME",     VOS_NULL_PTR};


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_VERSIONTIME_READ;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstDrvAgentVersionTime->ucLen = (VOS_UINT8)VOS_StrNLen(acVerTime, AT_AGENT_DRV_VERSION_TIME_LEN);
    PS_MEM_CPY(pstDrvAgentVersionTime->aucData, acVerTime, pstDrvAgentVersionTime->ucLen);

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentVertimeQryRsp(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvDrvAgentYjcxSetCnf: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                      *pRcvMsg;
    DRV_AGENT_YJCX_SET_CNF_STRU             *pstYjcxSetCnf;
    VOS_UINT8                                ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_YJCX_SET_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstYjcxSetCnf = (DRV_AGENT_YJCX_SET_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(DRV_AGENT_MSG_STRU));
        PS_MEM_SET(pstYjcxSetCnf, 0, sizeof(DRV_AGENT_YJCX_SET_CNF_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentYjcxSetCnf, Test_AT_RcvDrvAgentYjcxSetCnf_001)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;

    /* 变量初始化 */


    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentYjcxSetCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvDrvAgentYjcxSetCnf, Test_AT_RcvDrvAgentYjcxSetCnf_002)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentYjcxSetCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvDrvAgentYjcxSetCnf, Test_AT_RcvDrvAgentYjcxSetCnf_003)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;

    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_YJCX_SET;
    pstYjcxSetCnf->ulResult                 = VOS_ERR;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_ERROR));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentYjcxSetCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvDrvAgentYjcxSetCnf, Test_AT_RcvDrvAgentYjcxSetCnf_004)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    char                                acExpectedResult[] = "^YJCX: abcd";
    TAF_UCHAR                           aucflashInfo[TAF_MAX_FLAFH_INFO_LEN + 1] = "abcd";
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       = {AT_CMD_YJCX,
                                                              At_SetYjcxPara,           AT_SET_PARA_TIME,
                                                              At_QryYjcxPara,           AT_QRY_PARA_TIME,
                                                              VOS_NULL_PTR,             AT_NOT_SET_TIME,
                                                              VOS_NULL_PTR,             AT_NOT_SET_TIME,
                                                              AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE,
                                                              (VOS_UINT8*)"^YJCX",      (VOS_UINT8*)"(0,1)"};


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_YJCX_SET;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstYjcxSetCnf->ulResult = VOS_OK;
    PS_MEM_CPY(pstYjcxSetCnf->aucflashInfo, aucflashInfo, sizeof(aucflashInfo));

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentYjcxSetCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvDrvAgentYjcxQryCnf: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                      *pRcvMsg;
    DRV_AGENT_YJCX_QRY_CNF_STRU             *pstYjcxQryCnf;
    VOS_UINT8                                ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_YJCX_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstYjcxQryCnf = (DRV_AGENT_YJCX_QRY_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(DRV_AGENT_MSG_STRU));
        PS_MEM_SET(pstYjcxQryCnf, 0, sizeof(DRV_AGENT_YJCX_QRY_CNF_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentYjcxQryCnf, Test_AT_RcvDrvAgentYjcxQryCnf_001)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;

    /* 变量初始化 */


    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentYjcxQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvDrvAgentYjcxQryCnf, Test_AT_RcvDrvAgentYjcxQryCnf_002)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentYjcxQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvDrvAgentYjcxQryCnf, Test_AT_RcvDrvAgentYjcxQryCnf_003)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;

    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_YJCX_QUERY;
    pstYjcxQryCnf->ulResult                 = VOS_ERR;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_ERROR));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentYjcxQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvDrvAgentYjcxQryCnf, Test_AT_RcvDrvAgentYjcxQryCnf_004)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    char                                acExpectedResult[] = "^YJCX: abcd";
    TAF_UCHAR                           aucgpioInfo[TAF_MAX_GPIO_INFO_LEN + 1] = "abcd";
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       = {AT_CMD_YJCX,
                                                              At_SetYjcxPara,           AT_SET_PARA_TIME,
                                                              At_QryYjcxPara,           AT_QRY_PARA_TIME,
                                                              VOS_NULL_PTR,             AT_NOT_SET_TIME,
                                                              VOS_NULL_PTR,             AT_NOT_SET_TIME,
                                                              AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE,
                                                              (VOS_UINT8*)"^YJCX",      (VOS_UINT8*)"(0,1)"};



    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_YJCX_QUERY;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstYjcxQryCnf->ulResult = VOS_OK;
    PS_MEM_CPY(pstYjcxQryCnf->aucgpioInfo, aucgpioInfo, sizeof(aucgpioInfo));

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvDrvAgentYjcxQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvDrvAgentQrySfeatureRsp: public ::testing::Test
{
public:

    DRV_AGENT_MSG_STRU                 *pstDrvAgentMsg;     /* 接受消息结构 */
    DRV_AGENT_SFEATURE_QRY_CNF_STRU    *pstEvent;           /* 发送消息结构 */
    VOS_UINT8                           ucIndex;
    VOS_UINT16                          usLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        /* 生成输入消息 */
        usLen                               = sizeof(DRV_AGENT_MSG_STRU)
                                            + sizeof(DRV_AGENT_SFEATURE_QRY_CNF_STRU)
                                            - sizeof(pstDrvAgentMsg->aucContent);
        pstDrvAgentMsg                      = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, usLen);
        PS_MEM_SET(pstDrvAgentMsg, 0x00, usLen);

        if (VOS_NULL_PTR == pstDrvAgentMsg)
        {
            return;
        }

        pstEvent                            = (DRV_AGENT_SFEATURE_QRY_CNF_STRU *)(pstDrvAgentMsg->aucContent);
    }
    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstDrvAgentMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstDrvAgentMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentQrySfeatureRsp, Test_AT_RcvDrvAgentQrySfeatureRsp_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 初始化全局变量;

    //MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentQrySfeatureRsp(pstDrvAgentMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQrySfeatureRsp, Test_AT_RcvDrvAgentQrySfeatureRsp_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    //参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_PRODTYPE_QRY;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(eq(pstEvent->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentQrySfeatureRsp(pstDrvAgentMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQrySfeatureRsp, Test_AT_RcvDrvAgentQrySfeatureRsp_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo = {AT_CMD_FEATURE,            VOS_NULL_PTR,
                                                     AT_NOT_SET_TIME,           AT_QryFeaturePara,
                                                     AT_QRY_PARA_TIME,          At_CmdTestProcERROR,
                                                     AT_NOT_SET_TIME,
                                                     VOS_NULL_PTR,              AT_NOT_SET_TIME,
                                                     AT_CME_INCORRECT_PARAMETERS,
                                                     CMD_TBL_PIN_IS_LOCKED,
                                                     (VOS_UINT8*)"^SFEATURE",   VOS_NULL_PTR};

    //参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_SFEATURE_QRY;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(eq(pstEvent->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(V_MemAlloc)
        .stubs()
        .will(returnValue((VOS_VOID *)VOS_NULL_PTR));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentQrySfeatureRsp(pstDrvAgentMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(AT_FW_CLIENT_STATUS_READY, g_stParseContext[ucIndex].ucClientStatus);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);
    EXPECT_EQ(0, gastAtClientTab[ucIndex].opId);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQrySfeatureRsp, Test_AT_RcvDrvAgentQrySfeatureRsp_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo = {AT_CMD_FEATURE,            VOS_NULL_PTR,
                                                     AT_NOT_SET_TIME,           AT_QryFeaturePara,
                                                     AT_QRY_PARA_TIME,          At_CmdTestProcERROR,
                                                     AT_NOT_SET_TIME,
                                                     VOS_NULL_PTR,              AT_NOT_SET_TIME,
                                                     AT_CME_INCORRECT_PARAMETERS,
                                                     CMD_TBL_PIN_IS_LOCKED,
                                                     (VOS_UINT8*)"^SFEATURE",   VOS_NULL_PTR};

    //参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_SFEATURE_QRY;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(eq(pstEvent->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_GetWFeatureInfo)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_ERR));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentQrySfeatureRsp(pstDrvAgentMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(AT_FW_CLIENT_STATUS_READY, g_stParseContext[ucIndex].ucClientStatus);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);
    EXPECT_EQ(0, gastAtClientTab[ucIndex].opId);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQrySfeatureRsp, Test_AT_RcvDrvAgentQrySfeatureRsp_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo = {AT_CMD_FEATURE,            VOS_NULL_PTR,
                                                     AT_NOT_SET_TIME,           AT_QryFeaturePara,
                                                     AT_QRY_PARA_TIME,          At_CmdTestProcERROR,
                                                     AT_NOT_SET_TIME,
                                                     VOS_NULL_PTR,              AT_NOT_SET_TIME,
                                                     AT_CME_INCORRECT_PARAMETERS,
                                                     CMD_TBL_PIN_IS_LOCKED,
                                                     (VOS_UINT8*)"^SFEATURE",   VOS_NULL_PTR};

    //参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_SFEATURE_QRY;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(eq(pstEvent->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentQrySfeatureRsp(pstDrvAgentMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(AT_FW_CLIENT_STATUS_READY, g_stParseContext[ucIndex].ucClientStatus);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);
    EXPECT_EQ(0, gastAtClientTab[ucIndex].opId);

    GlobalMockObject::verify();
}



class Test_AT_RcvDrvAgentQryProdtypeRsp: public ::testing::Test
{
public:

    DRV_AGENT_MSG_STRU                 *pstDrvAgentMsg;     /* 接受消息结构 */
    DRV_AGENT_PRODTYPE_QRY_CNF_STRU    *pstEvent;           /* 发送消息结构 */
    VOS_UINT8                           ucIndex;
    VOS_UINT16                          usLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        /* 生成输入消息 */
        usLen                               = sizeof(DRV_AGENT_MSG_STRU)
                                            + sizeof(DRV_AGENT_PRODTYPE_QRY_CNF_STRU)
                                            - sizeof(pstDrvAgentMsg->aucContent);
        pstDrvAgentMsg                      = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, usLen);
        PS_MEM_SET(pstDrvAgentMsg, 0x00, usLen);

        if (VOS_NULL_PTR == pstDrvAgentMsg)
        {
            return;
        }

        pstEvent                            = (DRV_AGENT_PRODTYPE_QRY_CNF_STRU *)(pstDrvAgentMsg->aucContent);
    }
    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstDrvAgentMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstDrvAgentMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentQryProdtypeRsp, Test_AT_RcvDrvAgentQryProdtypeRsp_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentQryProdtypeRsp(pstDrvAgentMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQryProdtypeRsp, Test_AT_RcvDrvAgentQryProdtypeRsp_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    //参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_SFEATURE_QRY;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(eq(pstEvent->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentQryProdtypeRsp(pstDrvAgentMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQryProdtypeRsp, Test_AT_RcvDrvAgentQryProdtypeRsp_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo = {AT_CMD_PRODTYPE,       VOS_NULL_PTR,
                                                     AT_NOT_SET_TIME,       AT_QryProdTypePara,
                                                     AT_QRY_PARA_TIME,      At_CmdTestProcERROR,
                                                     AT_NOT_SET_TIME,
                                                     VOS_NULL_PTR,          AT_NOT_SET_TIME,
                                                     AT_CME_INCORRECT_PARAMETERS,
                                                     CMD_TBL_PIN_IS_LOCKED, (VOS_UINT8*)"^PRODTYPE",  VOS_NULL_PTR};

    //参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_PRODTYPE_QRY;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(eq(pstEvent->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentQryProdtypeRsp(pstDrvAgentMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(AT_FW_CLIENT_STATUS_READY, g_stParseContext[ucIndex].ucClientStatus);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);
    EXPECT_EQ(0, gastAtClientTab[ucIndex].opId);

    GlobalMockObject::verify();
}



class Test_AT_RcvDrvAgentQrySecuBootRsp: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                           *pRcvMsg;
    DRV_AGENT_SECUBOOT_QRY_CNF_STRU              *pstSecubootQryCnfInfo;
    VOS_UINT8                                     ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_SECUBOOT_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstSecubootQryCnfInfo = (DRV_AGENT_SECUBOOT_QRY_CNF_STRU *)(pRcvMsg->aucContent);
    }
    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentQrySecuBootRsp, Test_AT_RcvDrvAgentQrySecuBootRsp_001)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstSecubootQryCnfInfo->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQrySecuBootRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQrySecuBootRsp, Test_AT_RcvDrvAgentQrySecuBootRsp_002)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstSecubootQryCnfInfo->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /*全局变量初始化*/
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_SECUBOOTFEATURE_QRY;

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQrySecuBootRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvDrvAgentQrySecuBootRsp, Test_AT_RcvDrvAgentQrySecuBootRsp_003)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstSecubootQryCnfInfo->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /*全局变量初始化*/
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_SECUBOOT_QRY;
    pstSecubootQryCnfInfo->bFail = VOS_TRUE;

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_ERROR));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQrySecuBootRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvDrvAgentQrySecuBootRsp, Test_AT_RcvDrvAgentQrySecuBootRsp_004)
{
    /*变量定义*/
    VOS_UINT32                           ulRet;
    AT_PAR_CMD_ELEMENT_STRU              stCmdInfo = {AT_CMD_SECUBOOT,
                                                     At_SetSecuBootPara,  AT_NOT_SET_TIME,
                                                     At_QrySecuBootPara,    AT_QRY_PARA_TIME,
                                                     VOS_NULL_PTR ,    AT_NOT_SET_TIME,
                                                     VOS_NULL_PTR ,    AT_NOT_SET_TIME,
                                                     AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
                                                     (VOS_UINT8*)"^SECUBOOT", (VOS_UINT8*)"(0,1)"};

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstSecubootQryCnfInfo->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /*全局变量初始化*/
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_SECUBOOT_QRY;
    g_stParseContext[0].pstCmdElement   = &stCmdInfo;

    pstSecubootQryCnfInfo->bFail = VOS_FALSE;

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQrySecuBootRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    GlobalMockObject::verify();
}
class Test_AT_RcvDrvAgentQryVersionRsp: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                           *pRcvMsg;
    DRV_AGENT_VERSION_QRY_CNF_STRU               *pstVersionQryCnfInfo;
    VOS_UINT8                                     ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_VERSION_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstVersionQryCnfInfo = (DRV_AGENT_VERSION_QRY_CNF_STRU *)(pRcvMsg->aucContent);
    }
    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentQryVersionRsp, Test_AT_RcvDrvAgentQryVersionRsp_001)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstVersionQryCnfInfo->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQryVersionRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQryVersionRsp, Test_AT_RcvDrvAgentQryVersionRsp_002)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstVersionQryCnfInfo->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_FCHAN_SET;

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQryVersionRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQryVersionRsp, Test_AT_RcvDrvAgentQryVersionRsp_003)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstVersionQryCnfInfo->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_VERSION_QRY;

    pstVersionQryCnfInfo->enResult = DRV_AGENT_VERSION_QRY_VERTIME_ERROR;

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_ERROR));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQryVersionRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQryVersionRsp, Test_AT_RcvDrvAgentQryVersionRsp_004)
{
    /*变量定义*/
    VOS_UINT32                           ulRet;
    VOS_BOOL                             bPhyNumIsNull;
    AT_PAR_CMD_ELEMENT_STRU              stCmdInfo = {AT_CMD_VERSION,
                                                      VOS_NULL_PTR,        AT_NOT_SET_TIME,
                                                      At_QryVersion,         AT_QRY_PARA_TIME,
                                                      VOS_NULL_PTR ,    AT_NOT_SET_TIME,
                                                      VOS_NULL_PTR ,    AT_NOT_SET_TIME,
                                                      AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
                                                      (VOS_UINT8*)"^VERSION",  VOS_NULL_PTR};
    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstVersionQryCnfInfo->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_VERSION_QRY;
    g_stParseContext[0].pstCmdElement   = &stCmdInfo;

    pstVersionQryCnfInfo->enResult = DRV_AGENT_VERSION_QRY_NO_ERROR;

    PS_MEM_SET(pstVersionQryCnfInfo->acVerTime, 0x00, AT_AGENT_DRV_VERSION_TIME_LEN);

    pstVersionQryCnfInfo->acVerTime[AT_AGENT_DRV_VERSION_TIME_LEN-1] = '\0';

    MOCKER(AT_PhyNumIsNull)
        .stubs()
        .with(any(), eq(AT_PHYNUM_TYPE_IMEI), outBoundP(&bPhyNumIsNull))
        .will(returnValue((VOS_UINT32)AT_OK));

    g_bAtDataLocked = VOS_FALSE;

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQryVersionRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQryVersionRsp, Test_AT_RcvDrvAgentQryVersionRsp_005)
{
    /*变量定义*/
    VOS_UINT32                           ulRet;
    VOS_BOOL                             bPhyNumIsNull;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstVersionQryCnfInfo->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_VERSION_QRY;

    pstVersionQryCnfInfo->enResult = DRV_AGENT_VERSION_QRY_NO_ERROR;

    PS_MEM_SET(pstVersionQryCnfInfo->acVerTime, 0x00, AT_AGENT_DRV_VERSION_TIME_LEN);

    pstVersionQryCnfInfo->acVerTime[AT_AGENT_DRV_VERSION_TIME_LEN-1] = '\0';

    MOCKER(AT_PhyNumIsNull)
        .stubs()
        .with(any(), eq(AT_PHYNUM_TYPE_IMEI), outBoundP(&bPhyNumIsNull))
        .will(returnValue((VOS_UINT32)AT_ERROR));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_ERROR));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQryVersionRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQryVersionRsp, Test_AT_RcvDrvAgentQryVersionRsp_006)
{
    /*变量定义*/
    VOS_UINT32                           ulRet;
    VOS_BOOL                             bPhyNumIsNull;
    AT_PAR_CMD_ELEMENT_STRU              stCmdInfo = {AT_CMD_VERSION,
                                                      VOS_NULL_PTR,        AT_NOT_SET_TIME,
                                                      At_QryVersion,         AT_QRY_PARA_TIME,
                                                      VOS_NULL_PTR ,    AT_NOT_SET_TIME,
                                                      VOS_NULL_PTR ,    AT_NOT_SET_TIME,
                                                      AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
                                                      (VOS_UINT8*)"^VERSION",  VOS_NULL_PTR};
    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstVersionQryCnfInfo->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_VERSION_QRY;
    g_stParseContext[0].pstCmdElement   = &stCmdInfo;

    pstVersionQryCnfInfo->enResult = DRV_AGENT_VERSION_QRY_NO_ERROR;

    PS_MEM_SET(pstVersionQryCnfInfo->acVerTime, 0x00, AT_AGENT_DRV_VERSION_TIME_LEN);

    pstVersionQryCnfInfo->acVerTime[AT_AGENT_DRV_VERSION_TIME_LEN-1] = '\0';

    MOCKER(AT_PhyNumIsNull)
        .stubs()
        .with(any(), eq(AT_PHYNUM_TYPE_IMEI), outBoundP(&bPhyNumIsNull))
        .will(returnValue((VOS_UINT32)AT_OK));

    g_bAtDataLocked = VOS_TRUE;
    bPhyNumIsNull   = VOS_FALSE;

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));


    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQryVersionRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentQryTbatvoltRsp: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                           *pRcvMsg;
    DRV_AGENT_TBATVOLT_QRY_CNF_STRU              *pstTbatvoltQryCnf;
    VOS_UINT8                                     ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_TBATVOLT_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstTbatvoltQryCnf = (DRV_AGENT_TBATVOLT_QRY_CNF_STRU *)(pRcvMsg->aucContent);
    }
    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


class Test_AT_RcvDrvAgentSetDatalockRsp: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                           *pRcvMsg;
    DRV_AGENT_DATALOCK_SET_CNF_STRU              *pstDatalockSetCnf;
    VOS_UINT8                                     ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_DATALOCK_SET_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstDatalockSetCnf = (DRV_AGENT_DATALOCK_SET_CNF_STRU *)(pRcvMsg->aucContent);
    }
    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentSetDatalockRsp, Test_AT_RcvDrvAgentSetDatalockRsp_001)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstDatalockSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentSetDatalockRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSetDatalockRsp, Test_AT_RcvDrvAgentSetDatalockRsp_002)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstDatalockSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_TMODE_SET;

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentSetDatalockRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSetDatalockRsp, Test_AT_RcvDrvAgentSetDatalockRsp_003)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstDatalockSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_DATALOCK_SET;
    pstDatalockSetCnf->bFail = VOS_TRUE;

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_ERROR));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentSetDatalockRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSetDatalockRsp, Test_AT_RcvDrvAgentSetDatalockRsp_004)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstDatalockSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_DATALOCK_SET;
    pstDatalockSetCnf->bFail = VOS_FALSE;

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentSetDatalockRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentSetGpioplRsp: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                           *pRcvMsg;
    DRV_AGENT_GPIOPL_SET_CNF_STRU                *pstDrvAgentGpioSetCnf;
    VOS_UINT8                                     ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_GPIOPL_SET_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstDrvAgentGpioSetCnf = (DRV_AGENT_GPIOPL_SET_CNF_STRU *)(pRcvMsg->aucContent);
    }
    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentSetGpioplRsp, Test_AT_RcvDrvAgentSetGpioplRsp_001)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstDrvAgentGpioSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentSetGpioplRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSetGpioplRsp, Test_AT_RcvDrvAgentSetGpioplRsp_002)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstDrvAgentGpioSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_GPIOPL_QRY;

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentSetGpioplRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSetGpioplRsp, Test_AT_RcvDrvAgentSetGpioplRsp_003)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstDrvAgentGpioSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_GPIOPL_SET;
    pstDrvAgentGpioSetCnf->bFail = VOS_TRUE;

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_ERROR));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentSetGpioplRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSetGpioplRsp, Test_AT_RcvDrvAgentSetGpioplRsp_004)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstDrvAgentGpioSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_GPIOPL_SET;
    pstDrvAgentGpioSetCnf->bFail = VOS_FALSE;

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentSetGpioplRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentQryGpioplRsp: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                           *pRcvMsg;
    DRV_AGENT_GPIOPL_QRY_CNF_STRU                *pstGpioQryCnf;
    VOS_UINT8                                     ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_GPIOPL_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstGpioQryCnf = (DRV_AGENT_GPIOPL_QRY_CNF_STRU *)(pRcvMsg->aucContent);
    }
    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentQryGpioplRsp, Test_AT_RcvDrvAgentQryGpioplRsp_001)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstGpioQryCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQryGpioplRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQryGpioplRsp, Test_AT_RcvDrvAgentQryGpioplRsp_002)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstGpioQryCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_GPIOPL_SET;

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQryGpioplRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQryGpioplRsp, Test_AT_RcvDrvAgentQryGpioplRsp_003)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstGpioQryCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_GPIOPL_QRY;
    pstGpioQryCnf->bFail = VOS_TRUE;

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_ERROR));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQryGpioplRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentQryGpioplRsp, Test_AT_RcvDrvAgentQryGpioplRsp_004)
{
    /*变量定义*/
    VOS_UINT32                          ulRet;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo = {AT_CMD_GPIOPL,
                                                     At_SetGPIOPL,        AT_SET_PARA_TIME,
                                                     At_QryGPIOPL,          AT_QRY_PARA_TIME,
                                                     VOS_NULL_PTR ,    AT_NOT_SET_TIME,
                                                     VOS_NULL_PTR ,    AT_NOT_SET_TIME,
                                                     AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
                                                     (VOS_UINT8*)"^GPIOPL",   (VOS_UINT8*)"(@GPIOPL)"};
    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstGpioQryCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_GPIOPL_QRY;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
    pstGpioQryCnf->bFail = VOS_FALSE;

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentQryGpioplRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentSetFchanRsp: public ::testing::Test
{
public:
        DRV_AGENT_MSG_STRU                 *pRcvMsg;
        DRV_AGENT_FCHAN_SET_CNF_STRU       *pstFchanSetCnf;
        VOS_UINT8                           ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        /* 生成输入消息 */
        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_FCHAN_SET_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstFchanSetCnf = (DRV_AGENT_FCHAN_SET_CNF_STRU *)(pRcvMsg->aucContent);
    }
    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentSetFchanRsp, Test_AT_RcvDrvAgentSetFchanRsp_001)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstFchanSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentSetFchanRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSetFchanRsp, Test_AT_RcvDrvAgentSetFchanRsp_002)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstFchanSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_FCHAN_READ;

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentSetFchanRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSetFchanRsp, Test_AT_RcvDrvAgentSetFchanRsp_003)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstFchanSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_FCHAN_SET;

    pstFchanSetCnf->enResult = DRV_AGENT_FCHAN_OTHER_ERR;

    MOCKER(AT_SetFchanRspErr)
        .stubs()
        .with(eq(pstFchanSetCnf->enResult))
        .will(returnValue((VOS_UINT32)AT_FCHAN_OTHER_ERR));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq(AT_FCHAN_OTHER_ERR));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentSetFchanRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    ASSERT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}




TEST_F(Test_AT_RcvDrvAgentSetFchanRsp, Test_AT_RcvDrvAgentSetFchanRsp_004)
{
    /*变量定义*/
    VOS_UINT32                      ulRet;

    /*函数打桩*/
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstFchanSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_FCHAN_SET;

    pstFchanSetCnf->enResult = DRV_AGENT_FCHAN_SET_NO_ERROR;

    MOCKER(AT_SetFchanRspErr)
        .stubs()
        .with(eq(pstFchanSetCnf->enResult))
        .will(returnValue((VOS_UINT32)DRV_AGENT_FCHAN_SET_NO_ERROR));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /*调用被测函数 */
    ulRet = AT_RcvDrvAgentSetFchanRsp(pRcvMsg);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulRet);
    ASSERT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSetFchanRsp, Test_AT_RcvDrvAgentSetFchanRsp_005)
{
    /*变量定义*/
    VOS_UINT32                             ulRet;
    DRV_AGENT_FCHAN_SET_ERROR_ENUM_UINT32  enResult;

    enResult = DRV_AGENT_FCHAN_OTHER_ERR;

    /*调用被测函数 */
    ulRet = AT_SetFchanRspErr(enResult);

    /* 结果验证*/
    EXPECT_EQ(AT_FCHAN_OTHER_ERR, ulRet);
    ASSERT_EQ(VOS_FALSE, g_stAtDevCmdCtrl.bDspLoadFlag);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvDrvAgentSetFchanRsp, Test_AT_RcvDrvAgentSetFchanRsp_006)
{
    /*变量定义*/
    VOS_UINT32                             ulRet;
    DRV_AGENT_FCHAN_SET_ERROR_ENUM_UINT32  enResult;

    enResult = DRV_AGENT_FCHAN_SET_NO_ERROR;

    /*调用被测函数 */
    ulRet = AT_SetFchanRspErr(enResult);

    /* 结果验证*/
    EXPECT_EQ(DRV_AGENT_FCHAN_SET_NO_ERROR, ulRet);
    GlobalMockObject::verify();
}
class Test_AT_RcvDrvAgentSetSimlockCnf: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                 *pRcvMsg;
    DRV_AGENT_SET_SIMLOCK_CNF_STRU     *pstSimlockCnf;
    VOS_UINT8                           ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_SET_SIMLOCK_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstSimlockCnf = (DRV_AGENT_SET_SIMLOCK_CNF_STRU *)(pRcvMsg->aucContent);

        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;
    }
    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};

TEST_F(Test_AT_RcvDrvAgentSetSimlockCnf,Test_AT_RcvDrvAgentSetSimlockCnf_001)
{
    VOS_UINT32                      ulRet;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    ulRet = AT_RcvDrvAgentSetSimlockCnf(pRcvMsg);

    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();

}

TEST_F(Test_AT_RcvDrvAgentSetSimlockCnf,Test_AT_RcvDrvAgentSetSimlockCnf_002)
{
    VOS_UINT32                      ulRet;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstSimlockCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    ulRet = AT_RcvDrvAgentSetSimlockCnf(pRcvMsg);

    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();

}

TEST_F(Test_AT_RcvDrvAgentSetSimlockCnf,Test_AT_RcvDrvAgentSetSimlockCnf_003)
{
    VOS_UINT32                      ulRet;

    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_DRV_AGENT_SIMLOCK_SET_REQ;
    pstSimlockCnf->ulResult                 = DRV_AGENT_NO_ERROR;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstSimlockCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    ulRet = AT_RcvDrvAgentSetSimlockCnf(pRcvMsg);

    EXPECT_EQ(VOS_OK, ulRet);
    GlobalMockObject::verify();
}

TEST_F(Test_AT_RcvDrvAgentSetSimlockCnf,Test_AT_RcvDrvAgentSetSimlockCnf_004)
{
    VOS_UINT32                      ulRet;

    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_DRV_AGENT_SIMLOCK_SET_REQ;
    pstSimlockCnf->ulResult                 = DRV_AGENT_NO_ERROR;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstSimlockCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    ulRet = AT_RcvDrvAgentSetSimlockCnf(pRcvMsg);

    GlobalMockObject::verify();
}

TEST_F(Test_AT_RcvDrvAgentSetSimlockCnf,Test_AT_RcvDrvAgentSetSimlockCnf_005)
{
    VOS_UINT32                      ulRet;

    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_DRV_AGENT_SIMLOCK_SET_REQ;
    pstSimlockCnf->ulResult                 = DRV_AGENT_BUTT;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstSimlockCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    ulRet = AT_RcvDrvAgentSetSimlockCnf(pRcvMsg);

    GlobalMockObject::verify();
}
#ifdef FEATURE_SECURITY_SHELL

class Test_AT_RcvDrvAgentSetSpwordRsp: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                 *pRcvMsg;
    DRV_AGENT_SPWORD_SET_CNF_STRU      *pstSpwordCnf;
    VOS_UINT8                           ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_SPWORD_SET_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstSpwordCnf = (DRV_AGENT_SPWORD_SET_CNF_STRU *)(pRcvMsg->aucContent);

        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;
    }
    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentSetSpwordRsp,Test_AT_RcvDrvAgentSetSpwordRsp_001)
{
    VOS_UINT32                      ulRet;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    ulRet = AT_RcvDrvAgentSetSpwordRsp(pRcvMsg);

    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();

}

TEST_F(Test_AT_RcvDrvAgentSetSpwordRsp,Test_AT_RcvDrvAgentSetSpwordRsp_002)
{
    VOS_UINT32                      ulRet;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstSpwordCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    ulRet = AT_RcvDrvAgentSetSpwordRsp(pRcvMsg);

    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();

}

TEST_F(Test_AT_RcvDrvAgentSetSpwordRsp,Test_AT_RcvDrvAgentSetSpwordRsp_003)
{
    VOS_UINT32                      ulRet;

    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_SPWORD_SET;
    pstSpwordCnf->ulResult                  = VOS_OK;

    g_stSpWordCtx.ucShellPwdCheckFlag      = VOS_FALSE;

    g_stSpWordCtx.ucErrTimes               = 5;


    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstSpwordCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));


    ulRet = AT_RcvDrvAgentSetSpwordRsp(pRcvMsg);

    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(VOS_TRUE, g_stSpWordCtx.ucShellPwdCheckFlag);
    EXPECT_EQ(0, g_stSpWordCtx.ucErrTimes);

    GlobalMockObject::verify();
}

TEST_F(Test_AT_RcvDrvAgentSetSpwordRsp,Test_AT_RcvDrvAgentSetSpwordRsp_004)
{
    VOS_UINT32                      ulRet;

    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_SPWORD_SET;
    pstSpwordCnf->ulResult                  = VOS_ERR;

    g_stSpWordCtx.ucShellPwdCheckFlag      = VOS_FALSE;

    g_stSpWordCtx.ucErrTimes               = 5;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstSpwordCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));


    ulRet = AT_RcvDrvAgentSetSpwordRsp(pRcvMsg);


    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(VOS_FALSE, g_stSpWordCtx.ucShellPwdCheckFlag);
    EXPECT_EQ(6, g_stSpWordCtx.ucErrTimes);

    GlobalMockObject::verify();
}


#endif

/* Modified by f62575 for B050 Project, 2012-2-3, Begin   */
/*****************************************************************************
类名     : Test_AT_RcvSimLockQryRsp
功能描述 : Test_AT_RcvSimLockQryRsp UT工程类
修改历史     :
1.日   期  : 2012-02-17
作   者  :   f62575
修改内容 : 新生成类
*****************************************************************************/
class Test_AT_RcvSimLockQryRsp: public ::testing::Test
{
public:

    void SetUp()
	{
		UT_STUB_INIT();
    }
    void TearDown()
    {
    }
};

/*******************************************************************
测试用例编号      : Test_AT_RcvSimLockQryRsp_001
测试用例标题      : 获取INDEX失败
预期结果          :
修改历史     :
1.日   期  : 2012-02-17
作   者  :   f62575
修改内容 : 新生成类
*******************************************************************/
TEST_F(Test_AT_RcvSimLockQryRsp,Test_AT_RcvSimLockQryRsp_001)
{
    VOS_UINT32                      ulRet;
    VOS_UINT8                       ucIndex;
    AT_MMA_SIMLOCK_STATUS_STRU      stEvent;
    const char                      cWarning[] = "AT_RcvSimLockQryRsp: AT INDEX NOT FOUND!";

    MOCKER(OM_Log)
        .stubs()
        .with(any(), any(), any(), any(), any(), mirror(cWarning,strlen(cWarning) + 1));

    ucIndex = 0;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(At_FormatResultData)
        .expects(never());

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_SIMLOCKSTATUS_READ;

    AT_RcvSimLockQryRsp(&stEvent);

    GlobalMockObject::verify();

}

/*******************************************************************
测试用例编号      : Test_AT_RcvSimLockQryRsp_002
测试用例标题      : AT模块没有在AT_CMD_SIMLOCKSTATUS_READ或AT_CMD_CSDFLT_READ
预期结果          :
修改历史     :
1.日   期  : 2012-02-17
作   者  :   f62575
修改内容 : 新生成类
*******************************************************************/
TEST_F(Test_AT_RcvSimLockQryRsp,Test_AT_RcvSimLockQryRsp_002)
{
    VOS_UINT32                      ulRet;
    VOS_UINT8                       ucIndex;
    AT_MMA_SIMLOCK_STATUS_STRU      stEvent;

    ucIndex = 0;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(never());

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        stEvent.bSimlockEnableFlg = VOS_TRUE;
    AT_RcvSimLockQryRsp(&stEvent);

    GlobalMockObject::verify();

}

/*******************************************************************
测试用例编号      : Test_AT_RcvSimLockQryRsp_003
测试用例标题      : AT模块在AT_CMD_SIMLOCKSTATUS_READ状态
预期结果          :
修改历史     :
1.日   期  : 2012-02-17
作   者  :   f62575
修改内容 : 新生成类
*******************************************************************/
TEST_F(Test_AT_RcvSimLockQryRsp,Test_AT_RcvSimLockQryRsp_003)
{
    VOS_UINT32                      ulRet;
    VOS_UINT8                       ucIndex;
    AT_MMA_SIMLOCK_STATUS_STRU      stEvent;
    VOS_CHAR                           *pcPrintStr;

    pcPrintStr                          = "SIMLOCK:1";

    ucIndex = 0;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_SIMLOCKSTATUS_READ;

    stEvent.bSimlockEnableFlg = VOS_TRUE;
    AT_RcvSimLockQryRsp(&stEvent);

    EXPECT_EQ(VOS_StrLen(pcPrintStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcPrintStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();

}

/*******************************************************************
测试用例编号      : Test_AT_RcvSimLockQryRsp_004
测试用例标题      : AT模块在AT_CMD_CSDFLT_READ状态
预期结果          :
修改历史     :
1.日   期  : 2012-02-17
作   者  :   f62575
修改内容 : 新生成类
*******************************************************************/
TEST_F(Test_AT_RcvSimLockQryRsp,Test_AT_RcvSimLockQryRsp_004)
{
    VOS_UINT32                      ulRet;
    VOS_UINT8                       ucIndex;
    AT_MMA_SIMLOCK_STATUS_STRU      stEvent;
    AT_TRAFFIC_CLASS_CUSTOMIZE_STRU     stNvTrafficClass;
    NAS_NVIM_CUSTOMIZE_SERVICE_STRU     stCustSrv;
    VOS_UINT32                          ulGprsActiveTimerLen;
    AT_UE_CAPA_STRU                     stUECapa;
    TAF_PH_MS_CLASS_TYPE                ucMsClass;
    AT_USB_ENUM_STATUS_STRU             stUsbEnumStatus;
    VOS_UINT16                          usLength;
    VOS_UINT16                          usValueInfo;
    VOS_UINT8                           ucDpaRate;
    VOS_UINT8                           ucSearchMode;
    VOS_CHAR                           *pcPrintStr;

    //pcPrintStr                          = "^CSDFLT:2\r\n\^CSDFLT:SIM Lock Status,1\r\n\^CSDFLT:QOS,0\r\n\^CSDFLT:Call Enable,0\r\n\^CSDFLT:GPRS Active Timer,0\r\n\^CSDFLT:HSDPA State,0\r\n\^CSDFLT:HSUPA State,0\r\n\^CSDFLT:HSDPA Rate,0\r\n\^CSDFLT:Domain Prefer,0\r\n\^CSDFLT:Search Mode,0\r\n\^CSDFLT:Device State,0";
    pcPrintStr                          = "^CSDFLT:1";

    ucIndex = 0;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

      usValueInfo = AT_CUSTOMIZE_ITEM_DEFAULT_VAL_CHANGED;
      stCustSrv.ulCustomizeService = 0;
      MOCKER(At_ReadCustomizeServiceNV)
          .stubs()
        .with(outBoundP(&stCustSrv), outBoundP(&usValueInfo))
        .will(returnValue((VOS_UINT32)VOS_OK));

      usValueInfo = AT_CUSTOMIZE_ITEM_DEFAULT_VAL_CHANGED;
      ulGprsActiveTimerLen = 0;
      MOCKER(At_ReadGprsActiveTimerLenNV)
          .stubs()
        .with(outBoundP(&ulGprsActiveTimerLen), outBoundP(&usValueInfo))
        .will(returnValue((VOS_UINT32)VOS_OK));

      MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

      MOCKER(At_FormatResultData)
          .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CSDFLT_READ;
    g_stParseContext[ucIndex].pstCmdElement = &g_astAtDeviceCmdCsdFltTbl;

    stEvent.bSimlockEnableFlg = VOS_TRUE;
    AT_RcvSimLockQryRsp(&stEvent);

    EXPECT_EQ(VOS_StrLen(pcPrintStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcPrintStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();

}
/* Modified by f62575 for B050 Project, 2012-2-3, End   */

class Test_AT_RcvDrvAgentNvBackupStatQryRsp: public ::testing::Test
{
public:
    VOS_UINT8                               ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                 stCmdInfo;
    DRV_AGENT_MSG_STRU                     *pMsg;
    DRV_AGENT_NVBACKUPSTAT_QRY_CNF_STRU    *pstEvent;
    VOS_UINT32                              ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex     = 0;
        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_NVBACKUPSTAT_QRY_CNF_STRU);

        stCmdInfo.ulCmdIndex        = AT_CMD_NVBACKUPSTAT;
        stCmdInfo.pfnSetProc        = VOS_NULL_PTR;
        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = AT_QryNvBackupStatusPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = At_CmdTestProcOK;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^NVBACKUPSTAT";
        stCmdInfo.pszParam          = VOS_NULL_PTR;

        pMsg     = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent = (DRV_AGENT_NVBACKUPSTAT_QRY_CNF_STRU *)pMsg->aucContent;
        pstEvent->ulResult               = DRV_AGENT_NO_ERROR;
        pstEvent->ulNvBackupStat         = NV_OK;

        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_NVBACKUPSTAT_READ;

    }
    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};
TEST_F(Test_AT_RcvDrvAgentNvBackupStatQryRsp, Test_AT_RcvDrvAgentNvBackupStatQryRsp_001)
{
    VOS_UINT32                          ulResult;
    VOS_CHAR                           *pcExpactedStr;

    // 参数初始化
    pcExpactedStr   = "^NVBACKUPSTAT: 1";

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    ulResult = AT_RcvDrvAgentNvBackupStatQryRsp((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulResult);
    EXPECT_EQ(VOS_StrLen(pcExpactedStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcExpactedStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentNvBackupStatQryRsp, Test_AT_RcvDrvAgentNvBackupStatQryRsp_002)
{
    VOS_UINT32                              ulResult;

    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    ulResult = AT_RcvDrvAgentNvBackupStatQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_ERR, ulResult);
    GlobalMockObject::verify();

}


TEST_F(Test_AT_RcvDrvAgentNvBackupStatQryRsp, Test_AT_RcvDrvAgentNvBackupStatQryRsp_003)
{
    VOS_UINT32                              ulResult;

    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    ulResult = AT_RcvDrvAgentNvBackupStatQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_ERR, ulResult);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentNvBackupStatQryRsp, Test_AT_RcvDrvAgentNvBackupStatQryRsp_004)
{
    VOS_UINT32                              ulResult;

    gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;


    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    ulResult = AT_RcvDrvAgentNvBackupStatQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_ERR, ulResult);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentNvBackupStatQryRsp, Test_AT_RcvDrvAgentNvBackupStatQryRsp_005)
{
    VOS_UINT32                          ulResult;
    VOS_CHAR                           *pcExpactedStr;

    // 参数初始化
    pstEvent->ulNvBackupStat    = NV_NO_BACKUP;
    pcExpactedStr               = "^NVBACKUPSTAT: 0";

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    ulResult = AT_RcvDrvAgentNvBackupStatQryRsp((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulResult);
    EXPECT_EQ(VOS_StrLen(pcExpactedStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcExpactedStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentNandBadBlockQryRsp: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;
    DRV_AGENT_MSG_STRU                 *pMsg;
    DRV_AGENT_NANDBBC_QRY_CNF_STRU     *pstEvent;
    VOS_UINT32                          ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex     = 0;
        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4
                      + sizeof(DRV_AGENT_NANDBBC_QRY_CNF_STRU);

        stCmdInfo.ulCmdIndex        = AT_CMD_NANDBBC;
        stCmdInfo.pfnSetProc        = VOS_NULL_PTR;
        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = AT_QryNandBadBlockPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = At_CmdTestProcOK;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^NANDBBC";
        stCmdInfo.pszParam          = VOS_NULL_PTR;

        pMsg     = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent = (DRV_AGENT_NANDBBC_QRY_CNF_STRU *)pMsg->aucContent;
        PS_MEM_SET(pstEvent,
                   0x00,
                   sizeof(DRV_AGENT_NANDBBC_QRY_CNF_STRU));
        pstEvent->ulResult          = DRV_AGENT_NO_ERROR;
        pstEvent->ulBadBlockNum     = 0;

        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_NANDBBC_READ;
    }
    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};
TEST_F(Test_AT_RcvDrvAgentNandBadBlockQryRsp, Test_AT_RcvDrvAgentNandBadBlockQryRsp_001)
{
    VOS_UINT32                          ulResult;
    VOS_CHAR                           *pcExpactedStr;

    // 参数初始化
    pstEvent->ulBadBlockNum             = 0;
    pcExpactedStr                       = "^NANDBBC: 0";

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    ulResult = AT_RcvDrvAgentNandBadBlockQryRsp((VOS_VOID *)pMsg);
    EXPECT_EQ(VOS_OK, ulResult);
    EXPECT_EQ(VOS_StrLen(pcExpactedStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcExpactedStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentNandBadBlockQryRsp, Test_AT_RcvDrvAgentNandBadBlockQryRsp_002)
{
    VOS_UINT32                              ulResult;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    ulResult = AT_RcvDrvAgentNandBadBlockQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_ERR, ulResult);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentNandBadBlockQryRsp, Test_AT_RcvDrvAgentNandBadBlockQryRsp_003)
{
    VOS_UINT32                              ulResult;

    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    ulResult = AT_RcvDrvAgentNandBadBlockQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_ERR, ulResult);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentNandBadBlockQryRsp, Test_AT_RcvDrvAgentNandBadBlockQryRsp_004)
{
    VOS_UINT32                              ulResult;

    gastAtClientTab[0].CmdCurrentOpt = AT_CMD_INVALID;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    ulResult = AT_RcvDrvAgentNandBadBlockQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_ERR, ulResult);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentNandBadBlockQryRsp, Test_AT_RcvDrvAgentNandBadBlockQryRsp_005)
{
    VOS_UINT32                              ulResult;

    pstEvent->ulResult                      = DRV_AGENT_ERROR;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_ERROR));

    ulResult = AT_RcvDrvAgentNandBadBlockQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_OK, ulResult);
    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentNandDevInfoQryRsp: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;
    DRV_AGENT_MSG_STRU                 *pMsg;
    DRV_AGENT_NANDVER_QRY_CNF_STRU     *pstEvent;
    VOS_UINT32                          ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex     = 0;
        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4
                      + sizeof(DRV_AGENT_NANDVER_QRY_CNF_STRU);

        stCmdInfo.ulCmdIndex        = AT_CMD_NANDVER;
        stCmdInfo.pfnSetProc        = VOS_NULL_PTR;
        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = AT_QryNandDevInfoPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = At_CmdTestProcOK;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^NANDVER";
        stCmdInfo.pszParam          = VOS_NULL_PTR;

        pMsg     = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent = (DRV_AGENT_NANDVER_QRY_CNF_STRU *)pMsg->aucContent;
        PS_MEM_SET(pstEvent,    0x00,   sizeof(DRV_AGENT_NANDVER_QRY_CNF_STRU));
        pstEvent->ulResult          = DRV_AGENT_NO_ERROR;
        pstEvent->stNandDevInfo.ulMufId = 236;
        pstEvent->stNandDevInfo.ulDevId = 218;
        PS_MEM_CPY(pstEvent->stNandDevInfo.aucMufName, "Samsung", VOS_StrLen("Samsung"));
        PS_MEM_CPY(pstEvent->stNandDevInfo.aucDevSpec,
                   "NAND 256MiB 3,3V 8-bit",
                   VOS_StrLen("NAND 256MiB 3,3V 8-bit"));

        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_NANDVER_READ;
    }
    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};
TEST_F(Test_AT_RcvDrvAgentNandDevInfoQryRsp, Test_AT_RcvDrvAgentNandDevInfoQryRsp_001)
{
    VOS_UINT32                          ulResult;
    VOS_CHAR                           *pcExpactedStr;

    // 参数初始化
    pcExpactedStr   = "^NANDVER: 236,\"Samsung\",218,\"NAND 256MiB 3,3V 8-bit\"";

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    ulResult = AT_RcvDrvAgentNandDevInfoQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_OK, ulResult);
    EXPECT_EQ(VOS_StrLen(pcExpactedStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcExpactedStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentNandDevInfoQryRsp, Test_AT_RcvDrvAgentNandDevInfoQryRsp_002)
{
    VOS_UINT32                              ulResult;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    ulResult = AT_RcvDrvAgentNandDevInfoQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_ERR, ulResult);
    GlobalMockObject::verify();


}


TEST_F(Test_AT_RcvDrvAgentNandDevInfoQryRsp, Test_AT_RcvDrvAgentNandDevInfoQryRsp_003)
{
    VOS_UINT32                              ulResult;

    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    ulResult = AT_RcvDrvAgentNandDevInfoQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_ERR, ulResult);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentNandDevInfoQryRsp, Test_AT_RcvDrvAgentNandDevInfoQryRsp_004)
{
    VOS_UINT32                              ulResult;

    gastAtClientTab[0].CmdCurrentOpt = AT_CMD_INVALID;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    ulResult = AT_RcvDrvAgentNandDevInfoQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_ERR, ulResult);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentNandDevInfoQryRsp, Test_AT_RcvDrvAgentNandDevInfoQryRsp_005)
{
    VOS_UINT32                              ulResult;

    pstEvent->ulResult                      = DRV_AGENT_ERROR;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_ERROR));

    ulResult = AT_RcvDrvAgentNandDevInfoQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_OK, ulResult);
    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentChipTempQryRsp: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;
    DRV_AGENT_MSG_STRU                 *pMsg;
    DRV_AGENT_CHIPTEMP_QRY_CNF_STRU    *pstEvent;
    VOS_UINT32                          ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex     = 0;
        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4
                      + sizeof(DRV_AGENT_NANDVER_QRY_CNF_STRU);

        stCmdInfo.ulCmdIndex        = AT_CMD_CHIPTEMP;
        stCmdInfo.pfnSetProc        = VOS_NULL_PTR;
        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = AT_QryChipTempPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = At_CmdTestProcOK;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^CHIPTEMP";
        stCmdInfo.pszParam          = VOS_NULL_PTR;

        pMsg     = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent = (DRV_AGENT_CHIPTEMP_QRY_CNF_STRU *)pMsg->aucContent;
        PS_MEM_SET(pstEvent,    0x00,   sizeof(DRV_AGENT_CHIPTEMP_QRY_CNF_STRU));
        pstEvent->ulResult          = DRV_AGENT_NO_ERROR;
        pstEvent->lGpaTemp          = 500;
        pstEvent->lWpaTemp          = 500;
        pstEvent->lLpaTemp          = 500;
        pstEvent->lSimTemp          = 80;
        pstEvent->lBatTemp          = 250;

        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CHIPTEMP_READ;

    }
    void TearDown()
    {

    }
};


TEST_F(Test_AT_RcvDrvAgentChipTempQryRsp, Test_AT_RcvDrvAgentChipTempQryRsp_001)
{
    VOS_UINT32                          ulResult;
    VOS_CHAR                           *pcExpactedStr;

    // 参数初始化
    pcExpactedStr   = "^CHIPTEMP: 500,500,500,80,250";

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    ulResult = AT_RcvDrvAgentChipTempQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_OK, ulResult);
    EXPECT_EQ(VOS_StrLen(pcExpactedStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcExpactedStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentChipTempQryRsp, Test_AT_RcvDrvAgentChipTempQryRsp_002)
{
    VOS_UINT32                              ulResult;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    ulResult = AT_RcvDrvAgentChipTempQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_ERR, ulResult);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentChipTempQryRsp, Test_AT_RcvDrvAgentChipTempQryRsp_003)
{
    VOS_UINT32                              ulResult;

    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    ulResult = AT_RcvDrvAgentChipTempQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_ERR, ulResult);
    GlobalMockObject::verify();

}


TEST_F(Test_AT_RcvDrvAgentChipTempQryRsp, Test_AT_RcvDrvAgentChipTempQryRsp_004)
{
    VOS_UINT32                              ulResult;

    gastAtClientTab[0].CmdCurrentOpt = AT_CMD_INVALID;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    ulResult = AT_RcvDrvAgentChipTempQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_ERR, ulResult);
    GlobalMockObject::verify();

}


TEST_F(Test_AT_RcvDrvAgentChipTempQryRsp, Test_AT_RcvDrvAgentChipTempQryRsp_005)
{
    VOS_UINT32                              ulResult;

    pstEvent->ulResult                      = DRV_AGENT_ERROR;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_ERROR));

    ulResult = AT_RcvDrvAgentChipTempQryRsp((VOS_VOID *)pMsg);

    EXPECT_EQ(VOS_OK, ulResult);
    GlobalMockObject::verify();
}

/*****************************************************************************
类名     : Test_AT_RcvDrvAgentAntStateIndRsp
功能描述 : AT_RcvDrvAgentAntStateIndRsp UT工程类
修改历史     :
1.日   期  : 2012-04-26
  作   者  : h59254
  修改内容 : 新生成类
*****************************************************************************/
class Test_AT_RcvDrvAgentAntStateIndRsp: public ::testing::Test
{
public:
    TAF_UINT8                           ucIndex;
	DRV_AGENT_MSG_STRU                  *pMsg;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = 0;
        gstAtSendData.usBufLen      = 0;

		pMsg = (DRV_AGENT_MSG_STRU *)PS_ALLOC_MSG(WUEPS_PID_DRV_AGENT, sizeof(VOS_UINT32)+ sizeof(DRV_AGENT_ANT_STATE_IND_STRU));

		pMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
		pMsg->ulSenderPid     = WUEPS_PID_DRV_AGENT;
		pMsg->ulReceiverPid   = WUEPS_PID_AT;
		pMsg->ulMsgId         = DRV_AGENT_ANTSTATE_QRY_IND;
		PS_MEM_SET(pMsg->aucContent, 0, sizeof(DRV_AGENT_ANT_STATE_IND_STRU));
    }
    void TearDown()
    {
        ucIndex                 = 1;
        gstAtSendData.usBufLen  = 0;

		free(pMsg);
    }
};

/*******************************************************************
测试用例编号      : Test_AT_RcvDrvAgentAntStateIndRsp_001
测试用例标题      : ^ANTSTATE 查询回复处理函数
预期结果          : 返回VOS_OK
修改历史     :
1.日   期  : 2012-04-26
  作   者  :   h59254
  修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvDrvAgentAntStateIndRsp, Test_AT_RcvDrvAgentAntStateIndRsp_001)
{
    /* 变量初始化 */
    VOS_UINT32                          ulRslt;
	VOS_UINT8							ucIndex;

    /* 函数打桩 */

	ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

	MOCKER(At_ClientIdToUserId)
		.stubs()
		.with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
		.will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_SendResultData)
        .expects(exactly(1));

    /* 调用被测函数并结果验证 */
    ulRslt = AT_RcvDrvAgentAntStateIndRsp((VOS_VOID*)(pMsg));

    EXPECT_EQ(VOS_OK, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvDrvAgentAntStateIndRsp, Test_AT_RcvDrvAgentAntStateIndRsp_002)
{
    /* 变量初始化 */
    VOS_UINT32                          ulRslt;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /* 调用被测函数并结果验证 */
    ulRslt = AT_RcvDrvAgentAntStateIndRsp((VOS_VOID*)(pMsg));

    EXPECT_EQ(VOS_ERR, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvDrvAgentSetMaxLockTmsRsp: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                     *pRcvMsg;
    DRV_AGENT_MAX_LOCK_TMS_SET_CNF_STRU    *pstMaxlockTmsSetCnf;
    VOS_UINT8                               ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_MAX_LOCK_TMS_SET_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstMaxlockTmsSetCnf = (DRV_AGENT_MAX_LOCK_TMS_SET_CNF_STRU *)(pRcvMsg->aucContent);

        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;
    }
    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentSetMaxLockTmsRsp,Test_AT_RcvDrvAgentSetMaxLockTmsRsp_001)
{
    VOS_UINT32                      ulRet;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    ulRet = AT_RcvDrvAgentSetMaxLockTmsRsp(pRcvMsg);

    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();

}


TEST_F(Test_AT_RcvDrvAgentSetMaxLockTmsRsp,Test_AT_RcvDrvAgentSetMaxLockTmsRsp_002)
{
    VOS_UINT32                      ulRet;
    VOS_UINT8                       ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstMaxlockTmsSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    ulRet = AT_RcvDrvAgentSetMaxLockTmsRsp(pRcvMsg);

    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();

}


TEST_F(Test_AT_RcvDrvAgentSetMaxLockTmsRsp,Test_AT_RcvDrvAgentSetMaxLockTmsRsp_003)
{
    VOS_UINT32                      ulRet;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstMaxlockTmsSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    ulRet = AT_RcvDrvAgentSetMaxLockTmsRsp(pRcvMsg);

    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();

}



TEST_F(Test_AT_RcvDrvAgentSetMaxLockTmsRsp,Test_AT_RcvDrvAgentSetMaxLockTmsRsp_004)
{
    VOS_UINT32                      ulRet;

    gastAtClientTab[ucIndex].CmdCurrentOpt         = AT_CMD_MAXLCKTMS_SET;
    pstMaxlockTmsSetCnf->ulResult                  = VOS_OK;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstMaxlockTmsSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    ulRet = AT_RcvDrvAgentSetMaxLockTmsRsp(pRcvMsg);

    EXPECT_EQ(VOS_OK, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSetMaxLockTmsRsp,Test_AT_RcvDrvAgentSetMaxLockTmsRsp_005)
{
    VOS_UINT32                      ulRet;

    gastAtClientTab[ucIndex].CmdCurrentOpt         = AT_CMD_MAXLCKTMS_SET;
    pstMaxlockTmsSetCnf->ulResult                  = VOS_ERR;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstMaxlockTmsSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(ucIndex), eq(AT_DEVICE_OTHER_ERROR));

    ulRet = AT_RcvDrvAgentSetMaxLockTmsRsp(pRcvMsg);

    EXPECT_EQ(VOS_OK, ulRet);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentSetApSimstRsp: public ::testing::Test
{
public:
    DRV_AGENT_MSG_STRU                     *pRcvMsg;
    DRV_AGENT_AP_SIMST_SET_CNF_STRU        *pstApSimStSetCnf;
    VOS_UINT8                               ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        pRcvMsg = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(DRV_AGENT_MSG_STRU) + sizeof(DRV_AGENT_AP_SIMST_SET_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }

        pstApSimStSetCnf = (DRV_AGENT_AP_SIMST_SET_CNF_STRU *)(pRcvMsg->aucContent);

        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;
    }
    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentSetApSimstRsp,Test_AT_RcvDrvAgentSetApSimstRsp_001)
{
    VOS_UINT32                      ulRet;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    ulRet = AT_RcvDrvAgentSetApSimstRsp(pRcvMsg);

    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();

}


TEST_F(Test_AT_RcvDrvAgentSetApSimstRsp,Test_AT_RcvDrvAgentSetApSimstRsp_002)
{
    VOS_UINT32                      ulRet;
    VOS_UINT8                       ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstApSimStSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    ulRet = AT_RcvDrvAgentSetApSimstRsp(pRcvMsg);

    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();

}


TEST_F(Test_AT_RcvDrvAgentSetApSimstRsp,Test_AT_RcvDrvAgentSetApSimstRsp_003)
{
    VOS_UINT32                      ulRet;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstApSimStSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    ulRet = AT_RcvDrvAgentSetApSimstRsp(pRcvMsg);

    EXPECT_EQ(VOS_ERR, ulRet);
    GlobalMockObject::verify();

}


TEST_F(Test_AT_RcvDrvAgentSetApSimstRsp,Test_AT_RcvDrvAgentSetApSimstRsp_004)
{
    VOS_UINT32                      ulRet;

    gastAtClientTab[ucIndex].CmdCurrentOpt         = AT_CMD_APSIMST_SET;
    pstApSimStSetCnf->ulResult                     = VOS_OK;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstApSimStSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    ulRet = AT_RcvDrvAgentSetApSimstRsp(pRcvMsg);

    EXPECT_EQ(VOS_OK, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSetApSimstRsp,Test_AT_RcvDrvAgentSetApSimstRsp_005)
{
    VOS_UINT32                      ulRet;

    gastAtClientTab[ucIndex].CmdCurrentOpt         = AT_CMD_APSIMST_SET;
    pstApSimStSetCnf->ulResult                     = VOS_ERR;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(pstApSimStSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq(ucIndex), eq((VOS_UINT32)AT_ERROR));

    ulRet = AT_RcvDrvAgentSetApSimstRsp(pRcvMsg);

    EXPECT_EQ(VOS_OK, ulRet);

    GlobalMockObject::verify();
}


class Test_At_Pb_CnumAlaphPrint: public ::testing::Test
{
public:
    TAF_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = 0;
        gstAtSendData.usBufLen      = 0;

        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnTestProc       = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^ANTSTATE";
        stCmdInfo.pszParam          = (VOS_UINT8*)"(0,1)";

        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
    }
    void TearDown()
    {
        ucIndex                 = 1;
        gstAtSendData.usBufLen  = 0;
    }
};


TEST_F(Test_At_Pb_CnumAlaphPrint, Test_At_Pb_CnumAlaphPrint_001)
{
    /* 变量初始化 */
    VOS_UINT32                          ulRslt;
    TAF_UINT16                          usDataLen;
    SI_PB_EVENT_INFO_STRU               stEvent;
    TAF_UINT8                           ucData;

    gucAtCscsType                                    = AT_CSCS_GSM_7Bit_CODE;
    stEvent.PBEvent.PBReadCnf.PBRecord.AlphaTagType = SI_PB_ALPHATAG_TYPE_UCS2_80;

    /* 调用被测函数并结果验证 */
    ulRslt = At_Pb_CnumAlaphPrint(&usDataLen,&stEvent,&ucData);

    EXPECT_EQ(AT_SUCCESS, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_Pb_CnumAlaphPrint, Test_At_Pb_CnumAlaphPrint_002)
{
    /* 变量初始化 */
    VOS_UINT32                          ulRslt;
    TAF_UINT16                          usDataLen;
    SI_PB_EVENT_INFO_STRU               stEvent;
    TAF_UINT8                           ucData;

    gucAtCscsType                                    = AT_CSCS_GSM_7Bit_CODE;
    stEvent.PBEvent.PBReadCnf.PBRecord.AlphaTagType = SI_PB_ALPHATAG_TYPE_GSM;

    /* 函数打桩 */
    MOCKER(At_PbGsmFormatPrint)
        .stubs()
        .will(returnValue(0));

    /* 调用被测函数并结果验证 */
    ulRslt = At_Pb_CnumAlaphPrint(&usDataLen,&stEvent,&ucData);

    EXPECT_EQ(AT_SUCCESS, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_At_Pb_CnumAlaphPrint, Test_At_Pb_CnumAlaphPrint_003)
{
    /* 变量初始化 */
    VOS_UINT32                          ulRslt;
    TAF_UINT16                          usDataLen;
    SI_PB_EVENT_INFO_STRU               stEvent;
    TAF_UINT8                           ucData;

    gucAtCscsType                                    = AT_CSCS_IRA_CODE;
    stEvent.PBEvent.PBReadCnf.PBRecord.AlphaTagType = SI_PB_ALPHATAG_TYPE_GSM;

    /* 函数打桩 */
    MOCKER(At_PbRecordToUnicode)
        .stubs()
        .will(returnValue(1));

    MOCKER(At_PbUnicodeToIraFormatPrint)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数并结果验证 */
    ulRslt = At_Pb_CnumAlaphPrint(&usDataLen,&stEvent,&ucData);

    EXPECT_EQ(AT_SUCCESS, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_At_Pb_CnumAlaphPrint, Test_At_Pb_CnumAlaphPrint_004)
{
    /* 变量初始化 */
    VOS_UINT32                          ulRslt;
    TAF_UINT16                          usDataLen;
    SI_PB_EVENT_INFO_STRU               stEvent;
    TAF_UINT8                           ucData;

    gucAtCscsType                                    = AT_CSCS_UCS2_CODE;
    stEvent.PBEvent.PBReadCnf.PBRecord.AlphaTagType = SI_PB_ALPHATAG_TYPE_GSM;

    /* 函数打桩 */
    MOCKER(At_PbRecordToUnicode)
        .stubs()
        .will(returnValue(1));

    MOCKER(At_Unicode2UnicodePrint)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /* 调用被测函数并结果验证 */
    ulRslt = At_Pb_CnumAlaphPrint(&usDataLen,&stEvent,&ucData);

    EXPECT_EQ(AT_SUCCESS, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_At_PbCNUMCmdPrint: public ::testing::Test
{
public:
    TAF_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = 0;
        gstAtSendData.usBufLen      = 0;

        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnTestProc       = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^ANTSTATE";
        stCmdInfo.pszParam          = (VOS_UINT8*)"(0,1)";

        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
    }
    void TearDown()
    {
        ucIndex                 = 1;
        gstAtSendData.usBufLen  = 0;
    }
};


TEST_F(Test_At_PbCNUMCmdPrint, Test_At_PbCNUMCmdPrint_001)
{
    /* 变量初始化 */
    VOS_UINT32                          ulRslt;
    TAF_UINT16                          usDataLen;
    VOS_UINT8                           ucIndex;
    TAF_UINT8                           ucData;
    SI_PB_EVENT_INFO_STRU               Event;

    ucIndex                                    = 0;
    Event.PBEvent.PBReadCnf.PBRecord.ValidFlag = SI_PB_CONTENT_INVALID;

    /* 调用被测函数并结果验证 */
    ulRslt = At_PbCNUMCmdPrint(ucIndex,&usDataLen,&ucData,&Event);

    EXPECT_EQ(AT_SUCCESS, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_PbCNUMCmdPrint, Test_At_PbCNUMCmdPrint_002)
{
    /* 变量初始化 */
    VOS_UINT32                          ulRslt;
    TAF_UINT16                          usDataLen;
    VOS_UINT8                           ucIndex;
    TAF_UINT8                           ucData;
    SI_PB_EVENT_INFO_STRU               Event;

    ucIndex                                    = 0;
    Event.PBEvent.PBReadCnf.PBRecord.ValidFlag        = SI_PB_CONTENT_VALID;
    Event.PBEvent.PBReadCnf.PBRecord.ucAlphaTagLength = 1;
    /* 函数打桩 */
    MOCKER(At_Pb_CnumAlaphPrint)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    /* 调用被测函数并结果验证 */
    ulRslt = At_PbCNUMCmdPrint(ucIndex,&usDataLen,&ucData,&Event);

    EXPECT_EQ(AT_FAILURE, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_At_PbCNUMCmdPrint, Test_At_PbCNUMCmdPrint_003)
{
    /* 变量初始化 */
    VOS_UINT32                          ulRslt;
    TAF_UINT16                          usDataLen;
    VOS_UINT8                           ucIndex;
    TAF_UINT8                           ucData;
    SI_PB_EVENT_INFO_STRU               Event;

    ucIndex                                           = 0;
    Event.PBEvent.PBReadCnf.PBRecord.ValidFlag        = SI_PB_CONTENT_VALID;
    Event.PBEvent.PBReadCnf.PBRecord.ucAlphaTagLength = 1;
    /* 函数打桩 */
    //MOCKER(At_sprintf)
    //    .expects(exactly(1))
    //    .will(returnValue(1));

    ///* 函数打桩 */
    //MOCKER(At_sprintf)
    //    .expects(exactly(1))
    //    .will(returnValue(1));

    MOCKER(At_Pb_CnumAlaphPrint)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /*MOCKER(At_sprintf)
        .expects(exactly(1))
        .will(returnValue(1));*/

    MOCKER(AT_Pb_NumberPrint)
        .expects(exactly(1));

    /* 调用被测函数并结果验证 */
    ulRslt = At_PbCNUMCmdPrint(ucIndex,&usDataLen,&ucData,&Event);

    EXPECT_EQ(AT_SUCCESS, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_At_PbCNUMCmdPrint, Test_At_PbCNUMCmdPrint_004)
{
    /* 变量初始化 */
    VOS_UINT32                          ulRslt;
    TAF_UINT16                          usDataLen;
    VOS_UINT8                           ucIndex;
    TAF_UINT8                           ucData;
    SI_PB_EVENT_INFO_STRU               Event;

    ucIndex                                           = 0;
    Event.PBEvent.PBReadCnf.PBRecord.ValidFlag        = SI_PB_CONTENT_VALID;
    Event.PBEvent.PBReadCnf.PBRecord.ucAlphaTagLength = 0;
    /* 函数打桩 */
    MOCKER(AT_Pb_NumberPrint)
        .expects(exactly(1));

    /* 调用被测函数并结果验证 */
    ulRslt = At_PbCNUMCmdPrint(ucIndex,&usDataLen,&ucData,&Event);

    EXPECT_EQ(AT_SUCCESS, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvDrvAgentHukSetCnf: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;
    DRV_AGENT_MSG_STRU                 *pMsg;
    DRV_AGENT_HUK_SET_CNF_STRU         *pstEvent;
    VOS_UINT32                          ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_APP_INDEX;

        stCmdInfo.ulCmdIndex        = AT_CMD_HUK;
        stCmdInfo.pfnSetProc        = AT_SetHukPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnTestProc       = AT_TestHsicCmdPara;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^HUK";
        stCmdInfo.pszParam          = (VOS_UINT8 *)"(@huk)";

        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_HUK_SET_CNF_STRU);
        pMsg        = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent = (DRV_AGENT_HUK_SET_CNF_STRU *)pMsg->aucContent;
        pstEvent->enResult                      = DRV_AGENT_PERSONALIZATION_ERR_BUTT;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_HUK_SET;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    }
    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};
TEST_F(Test_AT_RcvDrvAgentHukSetCnf, Test_AT_RcvDrvAgentHukSetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentHukSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentHukSetCnf, Test_AT_RcvDrvAgentHukSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentHukSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentHukSetCnf, Test_AT_RcvDrvAgentHukSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentHukSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentHukSetCnf, Test_AT_RcvDrvAgentHukSetCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_WRITE_HUK_FAIL;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex))).will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_WRITE_HUK_FAIL));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentHukSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentHukSetCnf, Test_AT_RcvDrvAgentHukSetCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentHukSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentFacAuthPubkeySetCnf: public ::testing::Test
{
public:
    VOS_UINT8                               ucIndex;
    DRV_AGENT_MSG_STRU                     *pMsg;
    DRV_AGENT_FACAUTHPUBKEY_SET_CNF_STRU   *pstEvent;
    VOS_UINT32                              ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                             = AT_CLIENT_TAB_MODEM_INDEX;

        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_FACAUTHPUBKEY_SET_CNF_STRU);
        pMsg        = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent = (DRV_AGENT_FACAUTHPUBKEY_SET_CNF_STRU *)pMsg->aucContent;
        pstEvent->enResult                      = DRV_AGENT_PERSONALIZATION_ERR_BUTT;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_FACAUTHPUBKEY_SET;

    }
    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};
TEST_F(Test_AT_RcvDrvAgentFacAuthPubkeySetCnf, Test_AT_RcvDrvAgentFacAuthPubkeySetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentFacAuthPubkeySetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentFacAuthPubkeySetCnf, Test_AT_RcvDrvAgentFacAuthPubkeySetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentFacAuthPubkeySetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentFacAuthPubkeySetCnf, Test_AT_RcvDrvAgentFacAuthPubkeySetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentFacAuthPubkeySetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentFacAuthPubkeySetCnf, Test_AT_RcvDrvAgentFacAuthPubkeySetCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_FLASH_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_FLASH_ERROR));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentFacAuthPubkeySetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentFacAuthPubkeySetCnf, Test_AT_RcvDrvAgentFacAuthPubkeySetCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentFacAuthPubkeySetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentIdentifyStartSetCnf: public ::testing::Test
{
public:
    VOS_UINT8                               ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                 stCmdInfo;
    DRV_AGENT_MSG_STRU                     *pMsg;
    DRV_AGENT_IDENTIFYSTART_SET_CNF_STRU   *pstEvent;
    VOS_UINT32                              ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_MODEM_INDEX;

        stCmdInfo.ulCmdIndex        = AT_CMD_IDENTIFYSTART;
        stCmdInfo.pfnSetProc        = AT_SetIdentifyStartPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnTestProc       = AT_TestHsicCmdPara;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^IDENTIFYSTART";
        stCmdInfo.pszParam          = (VOS_UINT8 *)"(@Rsa)";

        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_IDENTIFYSTART_SET_CNF_STRU);
        pMsg        = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent = (DRV_AGENT_IDENTIFYSTART_SET_CNF_STRU *)pMsg->aucContent;
        pstEvent->enResult                      = DRV_AGENT_PERSONALIZATION_ERR_BUTT;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_IDENTIFYSTART_SET;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    }

    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};
TEST_F(Test_AT_RcvDrvAgentIdentifyStartSetCnf, Test_AT_RcvDrvAgentIdentifyStartSetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentIdentifyStartSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentIdentifyStartSetCnf, Test_AT_RcvDrvAgentIdentifyStartSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentIdentifyStartSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentIdentifyStartSetCnf, Test_AT_RcvDrvAgentIdentifyStartSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentIdentifyStartSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentIdentifyStartSetCnf, Test_AT_RcvDrvAgentIdentifyStartSetCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_GET_RAND_NUMBER_FAIL;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_GET_RAND_NUMBER_FAIL));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentIdentifyStartSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentIdentifyStartSetCnf, Test_AT_RcvDrvAgentIdentifyStartSetCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_CHAR                           *pcExpactedStr;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;
    pcExpactedStr       = "^IDENTIFYSTART: 5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A\
5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A\
5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A\
5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A";
    PS_MEM_SET(pstEvent->aucRsaText, 0x5A, DRV_AGENT_RSA_CIPHERTEXT_LEN);

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentIdentifyStartSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(VOS_StrLen(pcExpactedStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcExpactedStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentIdentifyEndSetCnf: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    DRV_AGENT_MSG_STRU                 *pMsg;
    DRV_AGENT_IDENTIFYEND_SET_CNF_STRU *pstEvent;
    VOS_UINT32                          ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                             = AT_CLIENT_TAB_MODEM_INDEX;

        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_IDENTIFYEND_SET_CNF_STRU);
        pMsg        = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent = (DRV_AGENT_IDENTIFYEND_SET_CNF_STRU *)pMsg->aucContent;
        pstEvent->enResult                      = DRV_AGENT_PERSONALIZATION_ERR_BUTT;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_IDENTIFYEND_SET;

    }
    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};
TEST_F(Test_AT_RcvDrvAgentIdentifyEndSetCnf, Test_AT_RcvDrvAgentIdentifyEndSetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentIdentifyEndSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentIdentifyEndSetCnf, Test_AT_RcvDrvAgentIdentifyEndSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentIdentifyEndSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentIdentifyEndSetCnf, Test_AT_RcvDrvAgentIdentifyEndSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentIdentifyEndSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentIdentifyEndSetCnf, Test_AT_RcvDrvAgentIdentifyEndSetCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_RSA_ENCRYPT_FAIL;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_RSA_ENCRYPT_FAIL));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentIdentifyEndSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentIdentifyEndSetCnf, Test_AT_RcvDrvAgentIdentifyEndSetCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentIdentifyEndSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentSimlockDataWriteSetCnf: public ::testing::Test
{
public:
    VOS_UINT8                                   ucIndex;
    DRV_AGENT_MSG_STRU                         *pMsg;
    DRV_AGENT_SIMLOCKDATAWRITE_SET_CNF_STRU    *pstEvent;
    VOS_UINT32                                  ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                                 = AT_CLIENT_TAB_MODEM_INDEX;

        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_SIMLOCKDATAWRITE_SET_CNF_STRU);
        pMsg        = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent = (DRV_AGENT_SIMLOCKDATAWRITE_SET_CNF_STRU *)pMsg->aucContent;
        pstEvent->enResult                      = DRV_AGENT_PERSONALIZATION_ERR_BUTT;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_SIMLOCKDATAWRITE_SET;

    }
    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};
TEST_F(Test_AT_RcvDrvAgentSimlockDataWriteSetCnf, Test_AT_RcvDrvAgentSimlockDataWriteSetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentSimlockDataWriteSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSimlockDataWriteSetCnf, Test_AT_RcvDrvAgentSimlockDataWriteSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentSimlockDataWriteSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSimlockDataWriteSetCnf, Test_AT_RcvDrvAgentSimlockDataWriteSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentSimlockDataWriteSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSimlockDataWriteSetCnf, Test_AT_RcvDrvAgentSimlockDataWriteSetCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_RSA_DECRYPT_FAIL;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_RSA_DECRYPT_FAIL));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentSimlockDataWriteSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSimlockDataWriteSetCnf, Test_AT_RcvDrvAgentSimlockDataWriteSetCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentSimlockDataWriteSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


class Test_AT_SimlockCodeBcd2Str: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }
    void TearDown()
    {

    }
};


TEST_F(Test_AT_SimlockCodeBcd2Str, Test_AT_SimlockCodeBcd2Str_001)
{
    // 变量声明
    VOS_UINT32                                      ulRet;
    DRV_AGENT_PERSONALIZATION_CATEGORY_ENUM_UINT8   enCategory;
    VOS_CHAR                                       *pcStrNumCheck;
    VOS_CHAR                                        aucStrNum[AT_PERSONALIZATION_CODE_LEN + 1];
    VOS_UINT8                                       aucBcdNum[] = {0x64, 0xF0, 0x10, 0x02};

    // 参数初始化
    enCategory      = DRV_AGENT_PERSONALIZATION_CATEGORY_CORPORATE;
    pcStrNumCheck   = "4600102";

    // 初始化全局变量;

    // MOCKER操作;

    // 调用被测函数
    ulRet = AT_SimlockCodeBcd2Str(enCategory, aucBcdNum, aucStrNum);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf: public ::testing::Test
{
public:
    VOS_UINT8                                   ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                     stCmdInfo;
    DRV_AGENT_MSG_STRU                         *pMsg;
    DRV_AGENT_PHONESIMLOCKINFO_QRY_CNF_STRU    *pstEvent;
    VOS_UINT32                                  ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_MODEM_INDEX;

        stCmdInfo.ulCmdIndex        = AT_CMD_PHONESIMLOCKINFO;
        stCmdInfo.pfnSetProc        = VOS_NULL_PTR;
        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = AT_QryPhoneSimlockInfoPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = AT_TestHsicCmdPara;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^PHONESIMLOCKINFO";
        stCmdInfo.pszParam          = VOS_NULL_PTR;

        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_PHONESIMLOCKINFO_QRY_CNF_STRU);
        pMsg        = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent = (DRV_AGENT_PHONESIMLOCKINFO_QRY_CNF_STRU *)pMsg->aucContent;
        PS_MEM_SET(pstEvent, 0x00, sizeof(DRV_AGENT_PHONESIMLOCKINFO_QRY_CNF_STRU));
        pstEvent->enResult                      = DRV_AGENT_PERSONALIZATION_ERR_BUTT;

        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_PHONESIMLOCKINFO_READ;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    }

    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};
TEST_F(Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf, Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhoneSimlockInfoQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf, Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhoneSimlockInfoQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf, Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhoneSimlockInfoQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf, Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_FLASH_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_FLASH_ERROR));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhoneSimlockInfoQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf, Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_CHAR                           *pcExpactedStr;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;
    pcExpactedStr       = "^PHONESIMLOCKINFO: NET,1,(46001,46002)\r\n\
^PHONESIMLOCKINFO: NETSUB,1,(4600102,4600105),(4600212,4600235)\r\n\
^PHONESIMLOCKINFO: SP,0";

    pstEvent->astCategoryInfo[0].enCategory     = DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK;
    pstEvent->astCategoryInfo[0].enIndicator    = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryInfo[0].ucGroupNum     = 1;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[0]   = 0x64;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[1]   = 0xF0;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[2]   = 0x10;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[3]   = 0xF0;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[0]     = 0x64;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[1]     = 0xF0;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[2]     = 0x20;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[3]     = 0x00;

    pstEvent->astCategoryInfo[1].enCategory     = DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK_SUBSET;
    pstEvent->astCategoryInfo[1].enIndicator    = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryInfo[1].ucGroupNum     = 2;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[0]   = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[1]   = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[2]   = 0x10;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[3]   = 0x20;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[0]     = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[1]     = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[2]     = 0x10;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[3]     = 0x50;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[0]   = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[1]   = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[2]   = 0x20;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[3]   = 0x21;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[0]     = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[1]     = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[2]     = 0x20;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[3]     = 0x53;

    pstEvent->astCategoryInfo[2].enCategory     = DRV_AGENT_PERSONALIZATION_CATEGORY_SERVICE_PROVIDER;
    pstEvent->astCategoryInfo[2].enIndicator    = DRV_AGENT_PERSONALIZATION_INDICATOR_INACTIVE;
    pstEvent->astCategoryInfo[2].ucGroupNum     = 0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhoneSimlockInfoQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(VOS_StrLen(pcExpactedStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcExpactedStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf, Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf_006)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    pstEvent->astCategoryInfo[0].enCategory     = DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK;
    pstEvent->astCategoryInfo[0].enIndicator    = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryInfo[0].ucGroupNum     = 1;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[0]   = 0x64;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[1]   = 0xF0;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[2]   = 0x10;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[3]   = 0xF0;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[0]     = 0x64;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[1]     = 0xF0;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[2]     = 0x20;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[3]     = 0x00;

    pstEvent->astCategoryInfo[1].enCategory     = DRV_AGENT_PERSONALIZATION_CATEGORY_SERVICE_PROVIDER;
    pstEvent->astCategoryInfo[1].enIndicator    = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryInfo[1].ucGroupNum     = 2;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[0]   = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[1]   = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[2]   = 0x10;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[3]   = 0x20;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[0]     = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[1]     = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[2]     = 0x10;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[3]     = 0x50;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[0]   = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[1]   = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[2]   = 0x20;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[3]   = 0x21;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[0]     = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[1]     = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[2]     = 0x20;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[3]     = 0x53;

    pstEvent->astCategoryInfo[2].enCategory     = DRV_AGENT_PERSONALIZATION_CATEGORY_CORPORATE;
    pstEvent->astCategoryInfo[2].enIndicator    = DRV_AGENT_PERSONALIZATION_INDICATOR_INACTIVE;
    pstEvent->astCategoryInfo[2].ucGroupNum     = 0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_OTHER_ERROR));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhoneSimlockInfoQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf, Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf_007)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    pstEvent->astCategoryInfo[0].enCategory     = DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK;
    pstEvent->astCategoryInfo[0].enIndicator    = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryInfo[0].ucGroupNum     = 1;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[0]   = 0x64;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[1]   = 0xF0;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[2]   = 0x10;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[3]   = 0xF0;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[0]     = 0x64;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[1]     = 0xF0;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[2]     = 0x20;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[3]     = 0x00;

    pstEvent->astCategoryInfo[1].enCategory     = DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK_SUBSET;
    pstEvent->astCategoryInfo[1].enIndicator    = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryInfo[1].ucGroupNum     = 2;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[0]   = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[1]   = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[2]   = 0x10;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[3]   = 0x20;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[0]     = 0x6F;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[1]     = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[2]     = 0x10;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[3]     = 0x50;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[0]   = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[1]   = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[2]   = 0x20;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[3]   = 0x21;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[0]     = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[1]     = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[2]     = 0x20;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[3]     = 0x53;

    pstEvent->astCategoryInfo[2].enCategory     = DRV_AGENT_PERSONALIZATION_CATEGORY_SERVICE_PROVIDER;
    pstEvent->astCategoryInfo[2].enIndicator    = DRV_AGENT_PERSONALIZATION_INDICATOR_INACTIVE;
    pstEvent->astCategoryInfo[2].ucGroupNum     = 0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_OTHER_ERROR));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhoneSimlockInfoQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf, Test_AT_RcvDrvAgentPhoneSimlockInfoQryCnf_008)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    pstEvent->astCategoryInfo[0].enCategory     = DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK;
    pstEvent->astCategoryInfo[0].enIndicator    = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryInfo[0].ucGroupNum     = 1;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[0]   = 0x64;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[1]   = 0xF0;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[2]   = 0x10;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeBegin[3]   = 0xF0;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[0]     = 0x64;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[1]     = 0xF0;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[2]     = 0x20;
    pstEvent->astCategoryInfo[0].astLockCode[0].aucPhLockCodeEnd[3]     = 0x00;

    pstEvent->astCategoryInfo[1].enCategory     = DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK_SUBSET;
    pstEvent->astCategoryInfo[1].enIndicator    = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryInfo[1].ucGroupNum     = 2;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[0]   = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[1]   = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[2]   = 0x10;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeBegin[3]   = 0x20;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[0]     = 0xF4;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[1]     = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[2]     = 0x10;
    pstEvent->astCategoryInfo[1].astLockCode[0].aucPhLockCodeEnd[3]     = 0x50;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[0]   = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[1]   = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[2]   = 0x20;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeBegin[3]   = 0x21;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[0]     = 0x64;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[1]     = 0xF0;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[2]     = 0x20;
    pstEvent->astCategoryInfo[1].astLockCode[1].aucPhLockCodeEnd[3]     = 0x53;

    pstEvent->astCategoryInfo[2].enCategory     = DRV_AGENT_PERSONALIZATION_CATEGORY_SERVICE_PROVIDER;
    pstEvent->astCategoryInfo[2].enIndicator    = DRV_AGENT_PERSONALIZATION_INDICATOR_INACTIVE;
    pstEvent->astCategoryInfo[2].ucGroupNum     = 0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_OTHER_ERROR));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhoneSimlockInfoQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentSimlockDataReadQryCnf: public ::testing::Test
{
public:
    VOS_UINT8                               ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                 stCmdInfo;
    DRV_AGENT_MSG_STRU                     *pMsg;
    DRV_AGENT_SIMLOCKDATAREAD_QRY_CNF_STRU *pstEvent;
    VOS_UINT32                              ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_MODEM_INDEX;

        stCmdInfo.ulCmdIndex        = AT_CMD_SIMLOCKDATAREAD;
        stCmdInfo.pfnSetProc        = VOS_NULL_PTR;
        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = AT_QrySimlockDataReadPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = AT_TestHsicCmdPara;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^SIMLOCKDATAREAD";
        stCmdInfo.pszParam          = VOS_NULL_PTR;

        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_SIMLOCKDATAREAD_QRY_CNF_STRU);
        pMsg        = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent = (DRV_AGENT_SIMLOCKDATAREAD_QRY_CNF_STRU *)pMsg->aucContent;
        PS_MEM_SET(pstEvent, 0x00, sizeof(DRV_AGENT_SIMLOCKDATAREAD_QRY_CNF_STRU));
        pstEvent->enResult                      = DRV_AGENT_PERSONALIZATION_ERR_BUTT;

        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_SIMLOCKDATAREAD_READ;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    }

    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};
TEST_F(Test_AT_RcvDrvAgentSimlockDataReadQryCnf, Test_AT_RcvDrvAgentSimlockDataReadQryCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentSimlockDataReadQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSimlockDataReadQryCnf, Test_AT_RcvDrvAgentSimlockDataReadQryCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentSimlockDataReadQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSimlockDataReadQryCnf, Test_AT_RcvDrvAgentSimlockDataReadQryCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentSimlockDataReadQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSimlockDataReadQryCnf, Test_AT_RcvDrvAgentSimlockDataReadQryCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_FLASH_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_FLASH_ERROR));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentSimlockDataReadQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSimlockDataReadQryCnf, Test_AT_RcvDrvAgentSimlockDataReadQryCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_CHAR                           *pcExpactedStr;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;
    pcExpactedStr       = "^SIMLOCKDATAREAD: NET,1,PUK,,\r\n\
^SIMLOCKDATAREAD: NETSUB,1,PIN,10,8\r\n\
^SIMLOCKDATAREAD: SP,0,,,";

    pstEvent->astCategoryData[0].enCategory             = DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK;
    pstEvent->astCategoryData[0].enIndicator            = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryData[0].enStatus               = DRV_AGENT_PERSONALIZATION_STATUS_PUK;
    pstEvent->astCategoryData[0].ucMaxUnlockTimes       = 10;
    pstEvent->astCategoryData[0].ucRemainUnlockTimes    = 0;

    pstEvent->astCategoryData[1].enCategory             = DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK_SUBSET;
    pstEvent->astCategoryData[1].enIndicator            = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryData[1].enStatus               = DRV_AGENT_PERSONALIZATION_STATUS_PIN;
    pstEvent->astCategoryData[1].ucMaxUnlockTimes       = 10;
    pstEvent->astCategoryData[1].ucRemainUnlockTimes    = 8;

    pstEvent->astCategoryData[2].enCategory             = DRV_AGENT_PERSONALIZATION_CATEGORY_SERVICE_PROVIDER;
    pstEvent->astCategoryData[2].enIndicator            = DRV_AGENT_PERSONALIZATION_INDICATOR_INACTIVE;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentSimlockDataReadQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(VOS_StrLen(pcExpactedStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcExpactedStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSimlockDataReadQryCnf, Test_AT_RcvDrvAgentSimlockDataReadQryCnf_006)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    pstEvent->astCategoryData[0].enCategory             = DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK;
    pstEvent->astCategoryData[0].enIndicator            = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryData[0].enStatus               = DRV_AGENT_PERSONALIZATION_STATUS_READY;
    pstEvent->astCategoryData[0].ucMaxUnlockTimes       = 10;
    pstEvent->astCategoryData[0].ucRemainUnlockTimes    = 10;

    pstEvent->astCategoryData[1].enCategory             = DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK_SUBSET;
    pstEvent->astCategoryData[1].enIndicator            = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryData[1].enStatus               = DRV_AGENT_PERSONALIZATION_STATUS_PIN;
    pstEvent->astCategoryData[1].ucMaxUnlockTimes       = 10;
    pstEvent->astCategoryData[1].ucRemainUnlockTimes    = 8;

    pstEvent->astCategoryData[2].enCategory             = DRV_AGENT_PERSONALIZATION_CATEGORY_CORPORATE;
    pstEvent->astCategoryData[2].enIndicator            = DRV_AGENT_PERSONALIZATION_INDICATOR_INACTIVE;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_OTHER_ERROR));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentSimlockDataReadQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentSimlockDataReadQryCnf, Test_AT_RcvDrvAgentSimlockDataReadQryCnf_007)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    pstEvent->astCategoryData[0].enCategory             = DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK;
    pstEvent->astCategoryData[0].enIndicator            = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryData[0].enStatus               = DRV_AGENT_PERSONALIZATION_STATUS_READY;
    pstEvent->astCategoryData[0].ucMaxUnlockTimes       = 10;
    pstEvent->astCategoryData[0].ucRemainUnlockTimes    = 10;

    pstEvent->astCategoryData[1].enCategory             = DRV_AGENT_PERSONALIZATION_CATEGORY_NETWORK_SUBSET;
    pstEvent->astCategoryData[1].enIndicator            = DRV_AGENT_PERSONALIZATION_INDICATOR_ACTIVE;
    pstEvent->astCategoryData[1].enStatus               = DRV_AGENT_PERSONALIZATION_STATUS_BUTT;
    pstEvent->astCategoryData[1].ucMaxUnlockTimes       = 10;
    pstEvent->astCategoryData[1].ucRemainUnlockTimes    = 8;

    pstEvent->astCategoryData[2].enCategory             = DRV_AGENT_PERSONALIZATION_CATEGORY_CORPORATE;
    pstEvent->astCategoryData[2].enIndicator            = DRV_AGENT_PERSONALIZATION_INDICATOR_INACTIVE;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_OTHER_ERROR));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentSimlockDataReadQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentPhonePhynumSetCnf: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    DRV_AGENT_MSG_STRU                 *pMsg;
    DRV_AGENT_PHONEPHYNUM_SET_CNF_STRU *pstEvent;
    VOS_UINT32                          ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                                 = AT_CLIENT_TAB_MODEM_INDEX;

        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_PHONEPHYNUM_SET_CNF_STRU);
        pMsg        = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent = (DRV_AGENT_PHONEPHYNUM_SET_CNF_STRU *)pMsg->aucContent;
        pstEvent->enResult                      = DRV_AGENT_PERSONALIZATION_ERR_BUTT;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_PHONEPHYNUM_SET;

    }
    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};
TEST_F(Test_AT_RcvDrvAgentPhonePhynumSetCnf, Test_AT_RcvDrvAgentPhonePhynumSetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhonePhynumSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhonePhynumSetCnf, Test_AT_RcvDrvAgentPhonePhynumSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhonePhynumSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhonePhynumSetCnf, Test_AT_RcvDrvAgentPhonePhynumSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhonePhynumSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhonePhynumSetCnf, Test_AT_RcvDrvAgentPhonePhynumSetCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_RSA_DECRYPT_FAIL;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_RSA_DECRYPT_FAIL));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhonePhynumSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhonePhynumSetCnf, Test_AT_RcvDrvAgentPhonePhynumSetCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhonePhynumSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentPhonePhynumQryCnf: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;
    DRV_AGENT_MSG_STRU                 *pMsg;
    DRV_AGENT_PHONEPHYNUM_QRY_CNF_STRU *pstEvent;
    VOS_UINT32                          ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_MODEM_INDEX;

        stCmdInfo.ulCmdIndex        = AT_CMD_PHONEPHYNUM;
        stCmdInfo.pfnSetProc        = AT_SetPhonePhynumPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = AT_QryPhonePhynumPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = AT_TestHsicCmdPara;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^PHONEPHYNUM";
        stCmdInfo.pszParam          = (VOS_UINT8 *)"(@type),(@Phynum)";

        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_PHONEPHYNUM_QRY_CNF_STRU);
        pMsg        = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent = (DRV_AGENT_PHONEPHYNUM_QRY_CNF_STRU *)pMsg->aucContent;
        PS_MEM_SET(pstEvent, 0x00, sizeof(DRV_AGENT_PHONEPHYNUM_QRY_CNF_STRU));
        pstEvent->enResult                      = DRV_AGENT_PERSONALIZATION_ERR_BUTT;

        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_PHONEPHYNUM_READ;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    }

    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};
TEST_F(Test_AT_RcvDrvAgentPhonePhynumQryCnf, Test_AT_RcvDrvAgentPhonePhynumQryCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhonePhynumQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhonePhynumQryCnf, Test_AT_RcvDrvAgentPhonePhynumQryCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhonePhynumQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhonePhynumQryCnf, Test_AT_RcvDrvAgentPhonePhynumQryCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhonePhynumQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhonePhynumQryCnf, Test_AT_RcvDrvAgentPhonePhynumQryCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_FLASH_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq(AT_PERSONALIZATION_FLASH_ERROR));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhonePhynumQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPhonePhynumQryCnf, Test_AT_RcvDrvAgentPhonePhynumQryCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_CHAR                           *pcExpactedStr;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;
    pcExpactedStr       = "^PHONEPHYNUM: IMEI,3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C\
3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C\
3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C\
3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C\r\n\
^PHONEPHYNUM: SN,2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D\
2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D\
2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D\
2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D";
    PS_MEM_SET(pstEvent->aucImeiRsa,    0x3C, DRV_AGENT_RSA_CIPHERTEXT_LEN);
    PS_MEM_SET(pstEvent->aucSnRsa,      0x2D, DRV_AGENT_RSA_CIPHERTEXT_LEN);

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq(ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPhonePhynumQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(VOS_StrLen(pcExpactedStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcExpactedStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}



class Test_AT_RcvDrvAgentPortctrlTmpSetCnf: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;
    DRV_AGENT_MSG_STRU                 *pMsg;
    DRV_AGENT_PORTCTRLTMP_SET_CNF_STRU *pstEvent;
    VOS_UINT32                          ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_HSIC1_USER;

        stCmdInfo.ulCmdIndex        = AT_CMD_PORTCTRLTMP;
        stCmdInfo.pfnSetProc        = AT_SetPortCtrlTmpPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = AT_QryPortCtrlTmpPara;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnTestProc       = AT_TestHsicCmdPara;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^PORTCTRLTMP";
        stCmdInfo.pszParam          = (VOS_UINT8 *)"(@password)";

        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_PORTCTRLTMP_SET_CNF_STRU);
        pMsg        = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent    = (DRV_AGENT_PORTCTRLTMP_SET_CNF_STRU *)pMsg->aucContent;
        pstEvent->enResult                      = DRV_AGENT_PERSONALIZATION_ERR_BUTT;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_PORTCTRLTMP_SET;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    }

    void TearDown()
    {
        if (VOS_NULL_PTR != pMsg)
        {
             PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentPortctrlTmpSetCnf, Test_AT_RcvDrvAgentPortctrlTmpSetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
		.expects(exactly(1))
		.with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortctrlTmpSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortctrlTmpSetCnf, Test_AT_RcvDrvAgentPortctrlTmpSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortctrlTmpSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortctrlTmpSetCnf, Test_AT_RcvDrvAgentPortctrlTmpSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
     gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortctrlTmpSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortctrlTmpSetCnf, Test_AT_RcvDrvAgentPortctrlTmpSetCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_DK_INCORRECT;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_PERSONALIZATION_DK_INCORRECT));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortctrlTmpSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortctrlTmpSetCnf, Test_AT_RcvDrvAgentPortctrlTmpSetCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortctrlTmpSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortctrlTmpSetCnf, Test_AT_RcvDrvAgentPortctrlTmpSetCnf_006)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_ERROR));

    MOCKER(V_AllocMsg)
        .stubs()
        .will(returnValue((MsgBlock *)VOS_NULL_PTR));


    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortctrlTmpSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortctrlTmpSetCnf, Test_AT_RcvDrvAgentPortctrlTmpSetCnf_007)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_ERROR));

    MOCKER(Ps_SendMsg)
        .stubs()
        .will(returnValue((VOS_UINT32)VOS_ERR));


    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortctrlTmpSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}



class Test_AT_RcvDrvAgentPortAttribSetCnf: public ::testing::Test
{
public:
    VOS_UINT8                               ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                 stCmdInfo;
    DRV_AGENT_MSG_STRU                     *pMsg;
    DRV_AGENT_PORTATTRIBSET_SET_CNF_STRU   *pstEvent;
    VOS_UINT32                              ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_HSIC1_USER;

        stCmdInfo.ulCmdIndex        = AT_CMD_PORTATTRIBSET;
        stCmdInfo.pfnSetProc        = AT_SetPortAttribSetPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = AT_QryPortAttribSetPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = AT_TestHsicCmdPara;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^PORTATTRIBSET";
        stCmdInfo.pszParam          = (VOS_UINT8 *)"(0,1),(@password)";

        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_PORTATTRIBSET_SET_CNF_STRU);
        pMsg        = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent    = (DRV_AGENT_PORTATTRIBSET_SET_CNF_STRU *)pMsg->aucContent;
        pstEvent->enResult                      = DRV_AGENT_PERSONALIZATION_ERR_BUTT;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_PORTATTRIBSET_SET;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    }

    void TearDown()
    {
        if (VOS_NULL_PTR != pMsg)
        {
             PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentPortAttribSetCnf, Test_AT_RcvDrvAgentPortAttribSetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortAttribSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortAttribSetCnf, Test_AT_RcvDrvAgentPortAttribSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
     ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortAttribSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortAttribSetCnf, Test_AT_RcvDrvAgentPortAttribSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
     gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortAttribSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortAttribSetCnf, Test_AT_RcvDrvAgentPortAttribSetCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
   pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_DK_INCORRECT;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));


    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_PERSONALIZATION_DK_INCORRECT));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortAttribSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortAttribSetCnf, Test_AT_RcvDrvAgentPortAttribSetCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortAttribSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentPortAttribSetQryCnf: public ::testing::Test
{
public:
    VOS_UINT8                               ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                 stCmdInfo;
    DRV_AGENT_MSG_STRU                     *pMsg;
    DRV_AGENT_PORTATTRIBSET_QRY_CNF_STRU   *pstEvent;
    VOS_UINT32                              ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_HSIC1_USER;

        stCmdInfo.ulCmdIndex        = AT_CMD_PORTATTRIBSET;
        stCmdInfo.pfnSetProc        = AT_SetPortAttribSetPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = AT_QryPortAttribSetPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = AT_TestHsicCmdPara;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^PORTATTRIBSET";
        stCmdInfo.pszParam          = (VOS_UINT8 *)"(0,1),(@password)";

        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_PORTATTRIBSET_QRY_CNF_STRU);
        pMsg        = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent    = (DRV_AGENT_PORTATTRIBSET_QRY_CNF_STRU *)pMsg->aucContent;
        pstEvent->enResult                      = DRV_AGENT_PERSONALIZATION_ERR_BUTT;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_PORTATTRIBSET_READ;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    }

    void TearDown()
    {
        if (VOS_NULL_PTR != pMsg)
        {
             PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }
};


TEST_F(Test_AT_RcvDrvAgentPortAttribSetQryCnf, Test_AT_RcvDrvAgentPortAttribSetQryCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortAttribSetQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortAttribSetQryCnf, Test_AT_RcvDrvAgentPortAttribSetQryCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
     ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortAttribSetQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortAttribSetQryCnf, Test_AT_RcvDrvAgentPortAttribSetQryCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
     gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortAttribSetQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortAttribSetQryCnf, Test_AT_RcvDrvAgentPortAttribSetQryCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
   pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_DK_INCORRECT;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_PERSONALIZATION_DK_INCORRECT));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortAttribSetQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortAttribSetQryCnf, Test_AT_RcvDrvAgentPortAttribSetQryCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult      = DRV_AGENT_PERSONALIZATION_NO_ERROR;
    pstEvent->enPortStatus  = DRV_AGENT_PORT_STATUS_BUTT;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_PERSONALIZATION_OTHER_ERROR));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortAttribSetQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentPortAttribSetQryCnf, Test_AT_RcvDrvAgentPortAttribSetQryCnf_006)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_CHAR                           *pcExpactedStr;

    // 参数初始化
    pstEvent->enResult      = DRV_AGENT_PERSONALIZATION_NO_ERROR;
    pstEvent->enPortStatus  = DRV_AGENT_PORT_STATUS_ON;
    pcExpactedStr           = "^PORTATTRIBSET: 1";

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentPortAttribSetQryCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(VOS_StrLen(pcExpactedStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcExpactedStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentOpwordSetCnf: public ::testing::Test
{
public:
    VOS_UINT8                               ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                 stCmdInfo;
    DRV_AGENT_MSG_STRU                     *pMsg;
    DRV_AGENT_OPWORD_SET_CNF_STRU          *pstEvent;
    VOS_UINT32                              ulEventLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_HSIC1_USER;

        stCmdInfo.ulCmdIndex        = AT_CMD_OPWORD;
        stCmdInfo.pfnSetProc        = AT_SetOpwordPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnTestProc       = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^OPWORD";
        stCmdInfo.pszParam          = VOS_NULL_PTR;

        ulEventLen  = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_OPWORD_SET_CNF_STRU);
        pMsg        = (DRV_AGENT_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent    = (DRV_AGENT_OPWORD_SET_CNF_STRU *)pMsg->aucContent;
        pstEvent->enResult                      = DRV_AGENT_PERSONALIZATION_ERR_BUTT;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_OPWORD_SET;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    }

    void TearDown()
    {
        if (VOS_NULL_PTR != pMsg)
        {
             PS_MEM_FREE(WUEPS_PID_AT, pMsg);
        }
    }

};


TEST_F(Test_AT_RcvDrvAgentOpwordSetCnf, Test_AT_RcvDrvAgentOpwordSetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentOpwordSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentOpwordSetCnf, Test_AT_RcvDrvAgentOpwordSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

      ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentOpwordSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentOpwordSetCnf, Test_AT_RcvDrvAgentOpwordSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量;
     gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvDrvAgentOpwordSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentOpwordSetCnf, Test_AT_RcvDrvAgentOpwordSetCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_DK_INCORRECT;
    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_ERROR));


    // 调用被测函数
    ulRet = AT_RcvDrvAgentOpwordSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentOpwordSetCnf, Test_AT_RcvDrvAgentOpwordSetCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEvent->enResult  = DRV_AGENT_PERSONALIZATION_NO_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    // 调用被测函数
    ulRet = AT_RcvDrvAgentOpwordSetCnf((VOS_VOID *)pMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}




class Test_AT_Num2AsciiNum: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }
    void TearDown()
    {

    }
};


TEST_F(Test_AT_Num2AsciiNum, Test_AT_Num2AsciiNum_001)
{
    // 变量声明
    VOS_CHAR                                      cRet;
    VOS_UINT8                                     ucNum;

    // 参数初始化
    ucNum   = 16;
    // 初始化全局变量;

    // MOCKER操作;

    // 调用被测函数
    cRet = AT_Num2AsciiNum(ucNum);

    // 执行检查
    EXPECT_EQ('*', cRet);

    GlobalMockObject::verify();
}


class Test_AT_CheckSimlockCodeStr: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }
    void TearDown()
    {

    }
};


TEST_F(Test_AT_CheckSimlockCodeStr, Test_AT_CheckSimlockCodeStr_001)
{
    // 变量声明
    VOS_UINT32                                      ulRet;
    DRV_AGENT_PERSONALIZATION_CATEGORY_ENUM_UINT8   enCategory;
    VOS_CHAR                                        acStrCode[AT_PERSONALIZATION_CODE_LEN + 1] = "46001FFG";

    // 参数初始化
    enCategory  = DRV_AGENT_PERSONALIZATION_CATEGORY_SERVICE_PROVIDER;
    // 初始化全局变量;

    // MOCKER操作;

    // 调用被测函数
    ulRet = AT_CheckSimlockCodeStr(enCategory,acStrCode);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


/*****************************************************************************
类名     : Test_AT_SupportHsupa
功能描述 : AT_SupportHsupa UT工程类
修改历史 :
1.日   期  : 2012-05-18
  作   者  : z60575
  修改内容 : 新生成类
*****************************************************************************/
class Test_AT_SupportHsupa: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }
    void TearDown()
    {

    }
};

/*******************************************************************
测试用例编号      : Test_AT_SupportHsupa_001
测试用例标题      : R99版本，不支持UPA能力
预期结果          : 返回VOS_FALSE
修改历史          :
1.日   期  : 2012-05-18
  作   者  : z60575
  修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_AT_SupportHsupa, Test_AT_SupportHsupa_001)
{
    // 变量声明
    AT_NVIM_UE_CAPA_STRU               stUECapa;
    VOS_BOOL                           bSupportHsupa;

    // 参数初始化
    memset(&stUECapa.enAsRelIndicator, 0, sizeof(stUECapa.enAsRelIndicator));

    stUECapa.enAsRelIndicator = AT_PTL_VER_ENUM_R3;

    // 初始化全局变量;

    // MOCKER操作;

    // 调用被测函数
    AT_SupportHsupa(&stUECapa,&bSupportHsupa);

    // 执行检查
    EXPECT_EQ(VOS_FALSE, bSupportHsupa);

    GlobalMockObject::verify();
}

/*******************************************************************
测试用例编号      : Test_AT_SupportHsupa_002
测试用例标题      : R8版本，支持UPA能力
预期结果          : 返回VOS_TRUE
修改历史          :
1.日   期  : 2012-05-18
  作   者  : z60575
  修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_AT_SupportHsupa, Test_AT_SupportHsupa_002)
{
    // 变量声明
    AT_NVIM_UE_CAPA_STRU               stUECapa;
    VOS_BOOL                           bSupportHsupa;

    // 参数初始化
    memset(&stUECapa.enAsRelIndicator, 0, sizeof(stUECapa.enAsRelIndicator));

    stUECapa.enAsRelIndicator   = AT_PTL_VER_ENUM_R8;
    stUECapa.enEDCHSupport      = PS_TRUE;
    stUECapa.ulHspaStatus       = 1;

    // 初始化全局变量;

    // MOCKER操作;

    // 调用被测函数
    AT_SupportHsupa(&stUECapa,&bSupportHsupa);

    // 执行检查
    EXPECT_EQ(VOS_TRUE, bSupportHsupa);

    GlobalMockObject::verify();
}

/*******************************************************************
测试用例编号      : Test_AT_SupportHsupa_003
测试用例标题      : R8版本，不支持UPA能力
预期结果          : 返回VOS_FALSE
修改历史          :
1.日   期  : 2012-05-18
  作   者  : z60575
  修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_AT_SupportHsupa, Test_AT_SupportHsupa_003)
{
    // 变量声明
    AT_NVIM_UE_CAPA_STRU               stUECapa;
    VOS_BOOL                           bSupportHsupa;

    // 参数初始化
    memset(&stUECapa.enAsRelIndicator, 0, sizeof(stUECapa.enAsRelIndicator));

    stUECapa.enAsRelIndicator   = AT_PTL_VER_ENUM_R8;
    stUECapa.enEDCHSupport      = PS_FALSE;
    stUECapa.ulHspaStatus       = 1;

    // 初始化全局变量;

    // MOCKER操作;

    // 调用被测函数
    AT_SupportHsupa(&stUECapa,&bSupportHsupa);

    // 执行检查
    EXPECT_EQ(VOS_FALSE, bSupportHsupa);

    GlobalMockObject::verify();
}

/*******************************************************************
测试用例编号      : Test_AT_SupportHsupa_004
测试用例标题      : R8版本，支持UPA能力
预期结果          : 返回VOS_TRUE
修改历史          :
1.日   期  : 2012-05-18
  作   者  : z60575
  修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_AT_SupportHsupa, Test_AT_SupportHsupa_004)
{
    // 变量声明
    AT_NVIM_UE_CAPA_STRU               stUECapa;
    VOS_BOOL                           bSupportHsupa;

    // 参数初始化
    memset(&stUECapa.enAsRelIndicator, 0, sizeof(stUECapa.enAsRelIndicator));

    stUECapa.enAsRelIndicator   = AT_PTL_VER_ENUM_R8;
    stUECapa.enEDCHSupport      = PS_FALSE;
    stUECapa.ulHspaStatus       = 0;

    // 初始化全局变量;

    // MOCKER操作;

    // 调用被测函数
    AT_SupportHsupa(&stUECapa,&bSupportHsupa);

    // 执行检查
    EXPECT_EQ(VOS_TRUE, bSupportHsupa);

    GlobalMockObject::verify();
}

/*****************************************************************************
类名     : Test_AT_SupportHsdpa
功能描述 : AT_SupportHsdpa UT工程类
修改历史 :
1.日   期  : 2012-05-18
  作   者  : z60575
  修改内容 : 新生成类
*****************************************************************************/
class Test_AT_SupportHsdpa: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }
    void TearDown()
    {

    }
};

/*******************************************************************
测试用例编号      : Test_AT_SupportHsdpa_001
测试用例标题      : R99版本，不支持DPA能力
预期结果          : 返回VOS_FALSE
修改历史          :
1.日   期  : 2012-05-18
  作   者  : z60575
  修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_AT_SupportHsdpa, Test_AT_SupportHsdpa_001)
{
    // 变量声明
    AT_NVIM_UE_CAPA_STRU               stUECapa;
    VOS_BOOL                           bSupportHsdpa;

    // 参数初始化
    memset(&stUECapa.enAsRelIndicator, 0, sizeof(stUECapa.enAsRelIndicator));

    stUECapa.enAsRelIndicator = AT_PTL_VER_ENUM_R3;

    // 初始化全局变量;

    // MOCKER操作;

    // 调用被测函数
    AT_SupportHsdpa(&stUECapa,&bSupportHsdpa);

    // 执行检查
    EXPECT_EQ(VOS_FALSE, bSupportHsdpa);

    GlobalMockObject::verify();
}

/*******************************************************************
测试用例编号      : Test_AT_SupportHsdpa_002
测试用例标题      : R8版本，支持dpa能力
预期结果          : 返回VOS_TRUE
修改历史          :
1.日   期  : 2012-05-18
  作   者  : z60575
  修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_AT_SupportHsdpa, Test_AT_SupportHsdpa_002)
{
    // 变量声明
    AT_NVIM_UE_CAPA_STRU               stUECapa;
    VOS_BOOL                           bSupportHsdpa;

    // 参数初始化
    memset(&stUECapa.enAsRelIndicator, 0, sizeof(stUECapa.enAsRelIndicator));

    stUECapa.enAsRelIndicator   = AT_PTL_VER_ENUM_R8;
    stUECapa.enHSDSCHSupport    = PS_TRUE;
    stUECapa.ulHspaStatus       = 1;

    // 初始化全局变量;

    // MOCKER操作;

    // 调用被测函数
    AT_SupportHsdpa(&stUECapa,&bSupportHsdpa);

    // 执行检查
    EXPECT_EQ(VOS_TRUE, bSupportHsdpa);

    GlobalMockObject::verify();
}

/*******************************************************************
测试用例编号      : Test_AT_SupportHsdpa_003
测试用例标题      : R8版本，不支持dpa能力
预期结果          : 返回VOS_FALSE
修改历史          :
1.日   期  : 2012-05-18
  作   者  : z60575
  修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_AT_SupportHsdpa, Test_AT_SupportHsdpa_003)
{
    // 变量声明
    AT_NVIM_UE_CAPA_STRU               stUECapa;
    VOS_BOOL                           bSupportHsdpa;

    // 参数初始化
    memset(&stUECapa.enAsRelIndicator, 0, sizeof(stUECapa.enAsRelIndicator));

    stUECapa.enAsRelIndicator   = AT_PTL_VER_ENUM_R8;
    stUECapa.enHSDSCHSupport    = PS_FALSE;
    stUECapa.ulHspaStatus       = 1;

    // 初始化全局变量;

    // MOCKER操作;

    // 调用被测函数
    AT_SupportHsdpa(&stUECapa,&bSupportHsdpa);

    // 执行检查
    EXPECT_EQ(VOS_FALSE, bSupportHsdpa);

    GlobalMockObject::verify();
}

/*******************************************************************
测试用例编号      : Test_AT_SupportHsdpa_004
测试用例标题      : R8版本，支持dpa能力
预期结果          : 返回VOS_TRUE
修改历史          :
1.日   期  : 2012-05-18
  作   者  : z60575
  修改内容 : 新建CASE
*******************************************************************/
TEST_F(Test_AT_SupportHsdpa, Test_AT_SupportHsdpa_004)
{
    // 变量声明
    AT_NVIM_UE_CAPA_STRU               stUECapa;
    VOS_BOOL                           bSupportHsdpa;

    // 参数初始化
    memset(&stUECapa.enAsRelIndicator, 0, sizeof(stUECapa.enAsRelIndicator));

    stUECapa.enAsRelIndicator   = AT_PTL_VER_ENUM_R8;
    stUECapa.enEDCHSupport      = PS_FALSE;
    stUECapa.ulHspaStatus       = 0;

    // 初始化全局变量;

    // MOCKER操作;

    // 调用被测函数
    AT_SupportHsdpa(&stUECapa,&bSupportHsdpa);

    // 执行检查
    EXPECT_EQ(VOS_TRUE, bSupportHsdpa);

    GlobalMockObject::verify();
}


class Test_AT_RcvMtaCposSetCnf: public ::testing::Test
{
public:

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_APP_INDEX;

        ulEventLen          = sizeof(AT_MTA_MSG_STRU);
        pstMsg              = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent            = (MTA_AT_CPOS_CNF_STRU *)pstMsg->aucContent;
        pstEvent->enResult  = MTA_AT_RESULT_BUTT;

        for ( ulLoop = 0; ulLoop < AT_MAX_CLIENT_NUM; ulLoop++ )
        {
            gastAtClientTab[ulLoop].CmdCurrentOpt  = AT_CMD_INVALID;
            gastAtClientTab[ulLoop].usClientId     = AT_CLIENT_BUTT;
            gastAtClientTab[ulLoop].ucUsed         = AT_CLIENT_USED;
         }
         gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_CPOS_SET;
         gastAtClientTab[ucIndex].usClientId       = ucIndex;
    }

    void TearDown()
    {
        if (VOS_NULL_PTR != pstMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMsg);
            pstMsg   = VOS_NULL_PTR;
            pstEvent = VOS_NULL_PTR;
        }
    }

    VOS_UINT32                          ulLoop;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulEventLen;
    AT_MTA_MSG_STRU                    *pstMsg;
    MTA_AT_CPOS_CNF_STRU               *pstEvent;
};


TEST_F(Test_AT_RcvMtaCposSetCnf, Test_AT_RcvMtaCposSetCnf_001)
{
    // 变量声明

    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_CPOS_SET_CNF;
    pstMsg->stAppCtrl.usClientId        = AT_CLIENT_TAB_APP_INDEX;
    pstEvent->enResult                  = MTA_AT_RESULT_NO_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaCposSetCnf, Test_AT_RcvMtaCposSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_UINT8                           i;
    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_CPOS_SET_CNF;
    pstMsg->stAppCtrl.usClientId        = AT_CLIENT_TAB_APP_INDEX;
    pstEvent->enResult                  = MTA_AT_RESULT_NO_ERROR;

    // 初始化全局变量;
    for ( i = 0; i < AT_MAX_CLIENT_NUM; i++ )
    {
        gastAtClientTab[i].usClientId   = AT_CLIENT_BUTT;
        gastAtClientTab[i].ucUsed       = AT_CLIENT_NULL;
    }

    // MOCKER操作;

    // 调用被测函数
    ulRet = AT_RcvMtaCposSetCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaCposSetCnf, Test_AT_RcvMtaCposSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_CPOS_SET_CNF;
    pstMsg->stAppCtrl.usClientId        = MN_CLIENT_ALL;
    pstEvent->enResult                  = MTA_AT_RESULT_NO_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(AT_StopRelTimer)
        .expects(never());

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaCposSetCnf, Test_AT_RcvMtaCposSetCnf_004)
{
        // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_CPOS_SET_CNF;
    pstMsg->stAppCtrl.usClientId        = AT_CLIENT_TAB_APP_INDEX;

    pstEvent->enResult                  = MTA_AT_RESULT_NO_ERROR;

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(AT_StopRelTimer)
        .expects(never());

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaCposSetCnf, Test_AT_RcvMtaCposSetCnf_005)
{
        // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_CPOS_SET_CNF;
    pstMsg->stAppCtrl.usClientId        = AT_CLIENT_TAB_APP_INDEX;

    pstEvent->enResult                  = MTA_AT_RESULT_ERROR;

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CPOS_SET;

    // MOCKER操作;

    // 调用被测函数
    ulRet = AT_RcvMtaCposSetCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);

    GlobalMockObject::verify();
}


class Test_AT_RcvMtaCgpsClockSetCnf: public ::testing::Test
{
public:

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex     = AT_CLIENT_TAB_MODEM_INDEX;
        ulEventLen  = sizeof(AT_MTA_MSG_STRU);
        pstMsg      = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent    = (MTA_AT_CGPSCLOCK_CNF_STRU *)pstMsg->aucContent;
        pstEvent->enResult                      = MTA_AT_RESULT_BUTT;

        for ( ulLoop = 0; ulLoop < AT_MAX_CLIENT_NUM; ulLoop++ )
        {
            gastAtClientTab[ulLoop].CmdCurrentOpt  = AT_CMD_INVALID;
            gastAtClientTab[ulLoop].usClientId     = AT_CLIENT_BUTT;
            gastAtClientTab[ulLoop].ucUsed         = AT_CLIENT_USED;
         }
         gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_CGPSCLOCK_SET;
         gastAtClientTab[ucIndex].usClientId       = ucIndex;
     }

    void TearDown()
    {
        if (VOS_NULL_PTR != pstMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMsg);
            pstMsg   = VOS_NULL_PTR;
            pstEvent = VOS_NULL_PTR;
        }
    }

    VOS_UINT32                          ulLoop;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulEventLen;
    AT_MTA_MSG_STRU                    *pstMsg;
    MTA_AT_CGPSCLOCK_CNF_STRU          *pstEvent;
};


TEST_F(Test_AT_RcvMtaCgpsClockSetCnf, Test_AT_RcvMtaCgpsClockSetCnf_001)
{
    // 变量声明

    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_CGPSCLOCK_SET_CNF;
    pstMsg->stAppCtrl.usClientId        = AT_CLIENT_TAB_MODEM_INDEX;

    pstEvent->enResult                  = MTA_AT_RESULT_NO_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(any(), eq((VOS_UINT32)AT_OK));


    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaCgpsClockSetCnf, Test_AT_RcvMtaCgpsClockSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_UINT32                          i;

    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_CGPSCLOCK_SET_CNF;
    pstMsg->stAppCtrl.usClientId        = AT_CLIENT_TAB_MODEM_INDEX;

    pstEvent->enResult                  = MTA_AT_RESULT_NO_ERROR;

    // 初始化全局变量;
    for ( i = 0; i < AT_MAX_CLIENT_NUM; i++ )
    {
        gastAtClientTab[i].usClientId   = AT_CLIENT_BUTT;
        gastAtClientTab[i].ucUsed       = AT_CLIENT_NULL;
    }

    // MOCKER操作;

    // 调用被测函数
    ulRet = AT_RcvMtaCgpsClockSetCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaCgpsClockSetCnf, Test_AT_RcvMtaCgpsClockSetCnf_003)
{
        // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_CGPSCLOCK_SET_CNF;
    pstMsg->stAppCtrl.usClientId        = MN_CLIENT_ALL;

    pstEvent->enResult                  = MTA_AT_RESULT_NO_ERROR;

    ucIndex                             = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(AT_StopRelTimer)
        .expects(never());

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaCgpsClockSetCnf, Test_AT_RcvMtaCgpsClockSetCnf_004)
{
        // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_CGPSCLOCK_SET_CNF;
    pstMsg->stAppCtrl.usClientId        = AT_CLIENT_TAB_MODEM_INDEX;

    pstEvent->enResult                  = MTA_AT_RESULT_NO_ERROR;

    // 初始化全局变量;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(AT_StopRelTimer)
        .expects(never());

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaCgpsClockSetCnf, Test_AT_RcvMtaCgpsClockSetCnf_005)
{
        // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_CGPSCLOCK_SET_CNF;
    pstMsg->stAppCtrl.usClientId        = AT_CLIENT_TAB_MODEM_INDEX;

    pstEvent->enResult                  = MTA_AT_RESULT_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
	MOCKER(At_FormatResultData)
		.expects(once());

    // 调用被测函数
    ulRet = AT_RcvMtaCgpsClockSetCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);

    GlobalMockObject::verify();
}


class Test_AT_RcvMtaCposrInd: public ::testing::Test
{
public:

    void SetUp()
	{
		UT_STUB_INIT();
        ulEventLen                      = sizeof(MTA_AT_CPOSR_IND_STRU) + sizeof(AT_MTA_MSG_STRU) - 4;
        pstMsg                          = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent                        = (MTA_AT_CPOSR_IND_STRU *)pstMsg->aucContent;
     }

    void TearDown()
    {
        if (VOS_NULL_PTR != pstMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMsg);
            pstMsg   = VOS_NULL_PTR;
            pstEvent = VOS_NULL_PTR;
        }
    }

    VOS_UINT32                          ulEventLen;
    AT_MTA_MSG_STRU                    *pstMsg;
    MTA_AT_CPOSR_IND_STRU              *pstEvent;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;
};


TEST_F(Test_AT_RcvMtaCposrInd, Test_AT_RcvMtaCposrInd_001)
{
    VOS_CHAR                            acExpectedStr[] = "\r\n+CPOSR: 111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\r\n";

    // 变量声明

    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_CPOSR_IND;
    pstMsg->stAppCtrl.usClientId        = AT_BROADCAST_CLIENT_ID_MODEM_0;
    pstMsg->stAppCtrl.ucOpId            = MTA_INVALID_TAB_INDEX;

    PS_MEM_SET(pstEvent->acXmlText, 0x31, (MTA_CPOSR_XML_MAX_LEN - 1));
    pstEvent->acXmlText[(MTA_CPOSR_XML_MAX_LEN - 1)] = '\0';

    // 初始化全局变量;
    AT_GetModemAgpsCtxAddrFromModemId(MODEM_ID_0)->enCposrReport               = AT_CPOSR_ENABLE;

    // MOCKER操作;
    MOCKER(At_SendResultData)
        .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查
    ASSERT_STREQ(acExpectedStr, (VOS_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}

#if (FEATURE_ON == FEATURE_MULTI_MODEM)

TEST_F(Test_AT_RcvMtaCposrInd, Test_AT_RcvMtaCposrInd_002)
{
    VOS_CHAR                            acExpectedStr[] = "\r\n+CPOSR: 111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\r\n";
    // 变量声明

    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_CPOSR_IND;
    pstMsg->stAppCtrl.usClientId        = AT_BROADCAST_CLIENT_ID_MODEM_1;
    pstMsg->stAppCtrl.ucOpId            = MTA_INVALID_TAB_INDEX;

    PS_MEM_SET(pstEvent->acXmlText, 0x31, (MTA_CPOSR_XML_MAX_LEN - 1));
    pstEvent->acXmlText[(MTA_CPOSR_XML_MAX_LEN - 1)] = '\0';

    // 初始化全局变量;
    AT_GetModemAgpsCtxAddrFromModemId(MODEM_ID_1)->enCposrReport               = AT_CPOSR_ENABLE;

    // MOCKER操作;
    MOCKER(At_SendResultData)
        .expects(exactly(1));

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查
    ASSERT_STREQ(acExpectedStr, (VOS_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}
#endif


TEST_F(Test_AT_RcvMtaCposrInd, Test_AT_RcvMtaCposrInd_003)
{
    // 变量声明
    VOS_UINT32                          i;
    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_CPOSR_IND;
    pstMsg->stAppCtrl.usClientId        = AT_CLIENT_TAB_MODEM_INDEX;
    pstMsg->stAppCtrl.ucOpId            = MTA_INVALID_TAB_INDEX;

    PS_MEM_SET(pstEvent->acXmlText, 0x31, (MTA_CPOSR_XML_MAX_LEN - 1));
    pstEvent->acXmlText[(MTA_CPOSR_XML_MAX_LEN - 1)] = '\0';

    for ( i = 0; i < AT_MAX_CLIENT_NUM; i++ )
    {
        gastAtClientTab[i].usClientId   = AT_CLIENT_BUTT;
        gastAtClientTab[i].ucUsed       = AT_CLIENT_NULL;
    }

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_SendResultData)
        .expects(never());

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查gstAtSendData.usBufLen
    GlobalMockObject::verify();
}


class Test_AT_RcvMtaXcposrRptInd: public ::testing::Test
{
public:

    void SetUp()
	{
		UT_STUB_INIT();
        ulEventLen           = sizeof(MTA_AT_XCPOSRRPT_IND_STRU) + sizeof(AT_MTA_MSG_STRU) - 4;
        pstMsg               = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent             = (MTA_AT_XCPOSRRPT_IND_STRU *)pstMsg->aucContent;
        pstEvent->ulClearFlg = VOS_TRUE;

     }

    void TearDown()
    {
        if (VOS_NULL_PTR != pstMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMsg);
            pstMsg   = VOS_NULL_PTR;
            pstEvent = VOS_NULL_PTR;
        }
    }

    VOS_UINT32                          ulEventLen;
    AT_MTA_MSG_STRU                    *pstMsg;
    MTA_AT_XCPOSRRPT_IND_STRU          *pstEvent;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;
};


TEST_F(Test_AT_RcvMtaXcposrRptInd, Test_AT_RcvMtaXcposrRptInd_001)
{
    // 变量声明
    VOS_UINT32                          ulRlt;
    VOS_CHAR                            acExpectedStr[] = "^XCPOSRRPT: 1";

    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_XCPOSRRPT_IND;
    pstMsg->stAppCtrl.usClientId        = AT_BROADCAST_CLIENT_ID_MODEM_0;
    pstMsg->stAppCtrl.ucOpId            = MTA_INVALID_TAB_INDEX;

    // 初始化全局变量;
    AT_GetModemAgpsCtxAddrFromModemId(MODEM_ID_0)->enXcposrReport   = AT_XCPOSR_ENABLE;

    // MOCKER操作;

    // 调用被测函数
    ulRlt = AT_RcvMtaXcposrRptInd((VOS_VOID *)pstMsg);

    // 执行检查
    ASSERT_STREQ(acExpectedStr, (VOS_CHAR *)pgucAtSndCodeAddr);
    EXPECT_EQ(VOS_OK, ulRlt);

    GlobalMockObject::verify();
}

#if (FEATURE_ON == FEATURE_MULTI_MODEM)

TEST_F(Test_AT_RcvMtaXcposrRptInd, Test_AT_RcvMtaXcposrRptInd_002)
{
    VOS_CHAR                            acExpectedStr[] = "^XCPOSRRPT: 1";

    // 变量声明
    VOS_UINT32                          ulRlt;
    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_XCPOSRRPT_IND;
    pstMsg->stAppCtrl.usClientId        = AT_BROADCAST_CLIENT_ID_MODEM_1;
    pstMsg->stAppCtrl.ucOpId            = MTA_INVALID_TAB_INDEX;

    // 初始化全局变量;
    AT_GetModemAgpsCtxAddrFromModemId(MODEM_ID_1)->enXcposrReport   = AT_XCPOSR_ENABLE;

    // MOCKER操作;

    // 调用被测函数
    ulRlt = AT_RcvMtaXcposrRptInd((VOS_VOID *)pstMsg);

    // 执行检查
    ASSERT_STREQ(acExpectedStr, (VOS_CHAR *)pgucAtSndCodeAddr);
    EXPECT_EQ(VOS_OK, ulRlt);

    GlobalMockObject::verify();
}
#endif


TEST_F(Test_AT_RcvMtaXcposrRptInd, Test_AT_RcvMtaXcposrRptInd_003)
{
    // 变量声明
    VOS_UINT32                          i;
    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_AT_XCPOSRRPT_IND;
    pstMsg->stAppCtrl.usClientId        = AT_CLIENT_TAB_MODEM_INDEX;
    pstMsg->stAppCtrl.ucOpId            = MTA_INVALID_TAB_INDEX;

    for ( i = 0; i < AT_MAX_CLIENT_NUM; i++ )
    {
        gastAtClientTab[i].usClientId   = AT_CLIENT_BUTT;
        gastAtClientTab[i].ucUsed       = AT_CLIENT_NULL;
    }

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_SendResultData)
        .expects(never());

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查gstAtSendData.usBufLen
    GlobalMockObject::verify();
}


class Test_At_ProcMtaMsg: public ::testing::Test
{
public:

    void SetUp()
	{
		UT_STUB_INIT();
        AT_GetModemAgpsCtxAddrFromModemId(MODEM_ID_0)->enXcposrReport   = AT_XCPOSR_ENABLE;

        ulEventLen           = sizeof(MTA_AT_XCPOSRRPT_IND_STRU) + sizeof(AT_MTA_MSG_STRU) - 4;
        pstMsg               = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulEventLen);
        pstEvent             = (MTA_AT_XCPOSRRPT_IND_STRU *)pstMsg->aucContent;
        pstEvent->ulClearFlg = VOS_TRUE;

     }

    void TearDown()
    {
        if (VOS_NULL_PTR != pstMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMsg);
            pstMsg   = VOS_NULL_PTR;
            pstEvent = VOS_NULL_PTR;
        }
    }

    VOS_UINT32                          ulEventLen;
    AT_MTA_MSG_STRU                    *pstMsg;
    MTA_AT_XCPOSRRPT_IND_STRU          *pstEvent;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;
};


TEST_F(Test_At_ProcMtaMsg, Test_At_ProcMtaMsg_001)
{
    // 变量声明
    VOS_UINT32                          ulRlt;
    // 参数初始化
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ulSenderPid                 = UEPS_PID_MTA;
    pstMsg->ulMsgId                     = ID_MTA_MSG_TYPE_BUTT;
    pstMsg->stAppCtrl.usClientId        = MTA_CLIENTID_BROADCAST;
    pstMsg->stAppCtrl.ucOpId            = MTA_INVALID_TAB_INDEX;

    // 初始化全局变量;

    // MOCKER操作;

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查
    GlobalMockObject::verify();
}



class Test_AT_RcvMtaApSecSetCnf: public ::testing::Test
{
public:
    VOS_UINT8                               ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                 stCmdInfo;
    AT_MTA_MSG_STRU                        *pstMsg;
    MTA_AT_APSEC_CNF_STRU                  *pstEvent;
    VOS_UINT32                              ulMsgLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_APP_INDEX;

        stCmdInfo.ulCmdIndex        = AT_CMD_APSEC;
        stCmdInfo.pfnSetProc        = VOS_NULL_PTR;
        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnTestProc       = AT_TestApSecPara;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^APSEC";
        stCmdInfo.pszParam          = (VOS_UINT8 *)"(@SecPacket)";

        ulMsgLen    = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_APSEC_CNF_STRU);
        pstMsg      = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulMsgLen);
        PS_MEM_SET(pstMsg, 0, ulMsgLen);

        pstMsg->ulReceiverPid           = WUEPS_PID_AT;
        pstMsg->ulSenderPid             = UEPS_PID_MTA;
        pstMsg->ulMsgId                 = ID_MTA_AT_APSEC_SET_CNF;
        pstMsg->stAppCtrl.usClientId    = AT_CLIENT_TAB_APP_INDEX;

        pstEvent    = (MTA_AT_APSEC_CNF_STRU *)pstMsg->aucContent;
        pstEvent->enResult                      = MTA_AT_RESULT_NO_ERROR;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_APSEC_SET;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    }

    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMsg);
        }
    }
};
TEST_F(Test_AT_RcvMtaApSecSetCnf, Test_AT_RcvMtaApSecSetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvMtaApSecSetCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaApSecSetCnf, Test_AT_RcvMtaApSecSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvMtaApSecSetCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaApSecSetCnf, Test_AT_RcvMtaApSecSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
     gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvMtaApSecSetCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaApSecSetCnf, Test_AT_RcvMtaApSecSetCnf_004)
{
    // 变量声明

    // 参数初始化
    pstEvent->enResult  = MTA_AT_RESULT_ERROR;

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaApSecSetCnf, Test_AT_RcvMtaApSecSetCnf_005)
{
    // 变量声明
    VOS_UINT8                           aucSecPacket[]  = {0x12, 0x34, 0x56, 0x78,
                                                           0x9A, 0xBC, 0xDE, 0xF0,
                                                           0x12, 0x34, 0x56, 0x78};
    VOS_CHAR                           *pcExpactedStr   = "^APSEC: 123456789ABCDEF012345678";

    // 参数初始化
    pstEvent->enResult  = MTA_AT_RESULT_NO_ERROR;
    pstEvent->ulSPLen   = sizeof(aucSecPacket);
    PS_MEM_CPY(pstEvent->aucSecPacket, aucSecPacket, pstEvent->ulSPLen);

    // 初始化全局变量;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_StrLen(pcExpactedStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcExpactedStr, (VOS_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}


class Test_AT_RcvMtaSimlockUnlockSetCnf: public ::testing::Test
{
public:
    VOS_UINT8                               ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                 stCmdInfo;
    AT_MTA_MSG_STRU                        *pstMsg;
    MTA_AT_SIMLOCKUNLOCK_CNF_STRU          *pstEvent;
    VOS_UINT32                              ulMsgLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_MODEM_INDEX;

        stCmdInfo.ulCmdIndex        = AT_CMD_SIMLOCKUNLOCK;
        stCmdInfo.pfnSetProc        = AT_SetSimlockUnlockPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnTestProc       = AT_TestSimlockUnlockPara;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^SIMLOCKUNLOCK";
        stCmdInfo.pszParam          = (VOS_UINT8 *)"(\"NET\",\"NETSUB\",\"SP\"),(pwd)";

        ulMsgLen    = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_SIMLOCKUNLOCK_CNF_STRU);
        pstMsg      = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulMsgLen);
        PS_MEM_SET(pstMsg, 0, ulMsgLen);

        pstMsg->ulReceiverPid           = WUEPS_PID_AT;
        pstMsg->ulSenderPid             = UEPS_PID_MTA;
        pstMsg->ulMsgId                 = ID_MTA_AT_SIMLOCKUNLOCK_SET_CNF;
        pstMsg->stAppCtrl.usClientId    = AT_CLIENT_TAB_APP_INDEX;

        pstEvent    = (MTA_AT_SIMLOCKUNLOCK_CNF_STRU *)pstMsg->aucContent;
        pstEvent->enResult                      = MTA_AT_RESULT_NO_ERROR;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_SIMLOCKUNLOCK_SET;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

    }

    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMsg);
        }
    }
};
TEST_F(Test_AT_RcvMtaSimlockUnlockSetCnf, Test_AT_RcvMtaSimlockUnlockSetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvMtaSimlockUnlockSetCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaSimlockUnlockSetCnf, Test_AT_RcvMtaSimlockUnlockSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvMtaSimlockUnlockSetCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaSimlockUnlockSetCnf, Test_AT_RcvMtaSimlockUnlockSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
     gastAtClientTab[ucIndex].CmdCurrentOpt    = AT_CMD_INVALID;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvMtaSimlockUnlockSetCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaSimlockUnlockSetCnf, Test_AT_RcvMtaSimlockUnlockSetCnf_004)
{
    // 变量声明

    // 参数初始化
    pstEvent->enResult  = MTA_AT_RESULT_ERROR;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaSimlockUnlockSetCnf, Test_AT_RcvMtaSimlockUnlockSetCnf_005)
{
    // 变量声明

    // 参数初始化
    pstEvent->enResult  = MTA_AT_RESULT_NO_ERROR;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    At_MsgProc((MsgBlock *)pstMsg);

    // 执行检查
    EXPECT_EQ(0, gstAtSendData.usBufLen);

    GlobalMockObject::verify();
}





class Test_At_PbCallBackFunc: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }
    void TearDown()
    {

    }
};


TEST_F(Test_At_PbCallBackFunc, Test_At_PbCallBackFunc_001)
{
    SI_PB_EVENT_INFO_STRU              stEvent;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    At_PbCallBackFunc(&stEvent);

    GlobalMockObject::verify();
}


TEST_F(Test_At_PbCallBackFunc, Test_At_PbCallBackFunc_002)
{
    SI_PB_EVENT_INFO_STRU               stEvent;
    TAF_UINT8                           ucIndex;

    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    MOCKER(At_ClientIdToUserId)
        .stubs()
		.with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    At_PbCallBackFunc(&stEvent);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentSetAdcRsp: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }
    void TearDown()
    {

    }
};


TEST_F(Test_AT_RcvDrvAgentSetAdcRsp, Test_AT_RcvDrvAgentSetAdcRsp_001)
{
    VOS_VOID                           *pMsg;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulRslt;
	DRV_AGENT_MSG_STRU					stMsg;

    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    MOCKER(At_ClientIdToUserId)
        .stubs()
		.with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    ulRslt = AT_RcvDrvAgentSetAdcRsp((VOS_VOID*)&stMsg);

    EXPECT_EQ(VOS_ERR, ulRslt);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentQryTbatRsp: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }
    void TearDown()
    {

    }
};


TEST_F(Test_AT_RcvDrvAgentQryTbatRsp, Test_AT_RcvDrvAgentQryTbatRsp_001)
{
    VOS_VOID                           *pMsg;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulRslt;
	DRV_AGENT_MSG_STRU					stMsg;

    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    MOCKER(At_ClientIdToUserId)
        .stubs()
		.with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    ulRslt = AT_RcvDrvAgentQryTbatRsp((VOS_VOID*)&stMsg);

    EXPECT_EQ(VOS_ERR, ulRslt);

    GlobalMockObject::verify();
}


class Test_AT_RcvMtaQryNmrCnf: public ::testing::Test
{
public:
    VOS_UINT8                               ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                 stCmdInfo;
    AT_MTA_MSG_STRU                        *pstMsg;
    MTA_AT_QRY_NMR_CNF_STRU                *pstEvent;
    VOS_UINT32                              ulMsgLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_MODEM_INDEX;

        stCmdInfo.ulCmdIndex        = AT_CMD_CNMR;
        stCmdInfo.pfnSetProc        = At_SetCnmrPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnTestProc       = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^CNMR";
        stCmdInfo.pszParam          = (VOS_UINT8*)"(1,2,3)";

        ulMsgLen    = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_QRY_NMR_CNF_STRU);
        pstMsg      = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulMsgLen);
        PS_MEM_SET(pstMsg, 0, ulMsgLen);

        pstMsg->ulReceiverPid           = WUEPS_PID_AT;
        pstMsg->ulSenderPid             = UEPS_PID_MTA;
        pstMsg->ulMsgId                 = ID_MTA_AT_QRY_NMR_CNF;
        pstMsg->stAppCtrl.usClientId    = AT_CLIENT_TAB_MODEM_INDEX;

        pstEvent    = (MTA_AT_QRY_NMR_CNF_STRU *)pstMsg->aucContent;
        pstEvent->enResult                      = MTA_AT_RESULT_NO_ERROR;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CNMR_QUERY;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
        gastAtClientTab[ucIndex].usClientId     = AT_CLIENT_TAB_MODEM_INDEX;

    }

    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMsg);
        }

        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;
        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
    }
};
TEST_F(Test_AT_RcvMtaQryNmrCnf, Test_AT_RcvMtaQryNmrCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvMtaQryNmrCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaQryNmrCnf, Test_AT_RcvMtaQryNmrCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;
	AT_MTA_MSG_STRU						Msg;
	VOS_UINT8							Index;

    // 参数初始化
    Index = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&Index, (VOS_UINT32)sizeof(Index)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvMtaQryNmrCnf((VOS_VOID *)&Msg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}



TEST_F(Test_AT_RcvMtaQryNmrCnf, Test_AT_RcvMtaQryNmrCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;


    // 参数初始化
    pstEvent->enResult                      = MTA_AT_RESULT_ERROR;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CNMR_QUERY;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    // 调用被测函数
    ulRet = AT_RcvMtaQryNmrCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaQryNmrCnf, Test_AT_RcvMtaQryNmrCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;


    // 参数初始化


    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CECELLID_QUERY;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvMtaQryNmrCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaQryNmrCnf, Test_AT_RcvMtaQryNmrCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_CHAR                            acExpectedStr[] = "^CNMR: 3,1,01020304\r\n";

    // 参数初始化
    pstEvent->enResult = MTA_AT_RESULT_NO_ERROR;
    pstEvent->ucTotalIndex = 3;
    pstEvent->ucCurrIndex  = 1;
    pstEvent->usNMRLen     = 4;
    pstEvent->aucNMRData[0] = 1;
    pstEvent->aucNMRData[1] = 2;
    pstEvent->aucNMRData[2] = 3;
    pstEvent->aucNMRData[3] = 4;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CNMR_QUERY;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRet = AT_RcvMtaQryNmrCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    ASSERT_STREQ(acExpectedStr, (VOS_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}



TEST_F(Test_AT_RcvMtaQryNmrCnf, Test_AT_RcvMtaQryNmrCnf_006)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    //VOS_CHAR                            acExpectedStr[] = "\r\n^CNMR: 3,3,1234\r\n\r\nOK\r\n";

    // 参数初始化
    pstEvent->enResult = MTA_AT_RESULT_NO_ERROR;
    pstEvent->ucTotalIndex = 3;
    pstEvent->ucCurrIndex  = 3;
    pstEvent->usNMRLen     = 4;
    pstEvent->aucNMRData[0] = 1;
    pstEvent->aucNMRData[1] = 2;
    pstEvent->aucNMRData[2] = 3;
    pstEvent->aucNMRData[3] = 4;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CNMR_QUERY;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRet = AT_RcvMtaQryNmrCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    //ASSERT_STREQ(acExpectedStr, (VOS_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaQryNmrCnf, Test_AT_RcvMtaQryNmrCnf_007)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    //VOS_CHAR                            acExpectedStr[] = "\r\n^CNMR: 0,0,\r\n\r\nOK\r\n";

    // 参数初始化
    pstEvent->enResult = MTA_AT_RESULT_NO_ERROR;
    pstEvent->ucTotalIndex = 0;
    pstEvent->ucCurrIndex  = 3;
    pstEvent->usNMRLen     = 4;
    pstEvent->aucNMRData[0] = 1;
    pstEvent->aucNMRData[1] = 2;
    pstEvent->aucNMRData[2] = 3;
    pstEvent->aucNMRData[3] = 4;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CNMR_QUERY;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRet = AT_RcvMtaQryNmrCnf((VOS_VOID *)pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    //ASSERT_STREQ(acExpectedStr, (VOS_CHAR *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}






class Test_AT_RcvMtaWrrAutotestQryCnf: public ::testing::Test
{
    public:

    AT_MTA_MSG_STRU                    *pRcvMsg;
    MTA_AT_WRR_AUTOTEST_QRY_CNF_STRU   *pstEvent;
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stElement;
    VOS_UINT8                           szCmdName[32];

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        VOS_StrCpy((VOS_CHAR*)szCmdName,"^CWAS");
        g_stParseContext[ucIndex].pstCmdElement = &stElement;
        stElement.pszCmdName                    = szCmdName;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_WRR_AUTOTEST_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstEvent = (MTA_AT_WRR_AUTOTEST_QRY_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstEvent, 0, sizeof(MTA_AT_WRR_AUTOTEST_QRY_CNF_STRU));
    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaWrrAutotestQryCnf, Test_AT_RcvMtaWrrAutotestQryCnf_001)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    // 参数初始化

    // 初始化全局变量
    usClientId = 0;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrAutotestQryCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaWrrAutotestQryCnf, Test_AT_RcvMtaWrrAutotestQryCnf_002)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    VOS_UINT8                                 ucIndexTemp;
    // 参数初始化

    // 初始化全局变量
    usClientId = 0;
    ucIndexTemp = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId), outBoundP(&ucIndexTemp, (VOS_UINT32)sizeof(ucIndexTemp)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrAutotestQryCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaWrrAutotestQryCnf, Test_AT_RcvMtaWrrAutotestQryCnf_003)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    // 参数初始化
    usClientId = 0;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;
    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrAutotestQryCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaWrrAutotestQryCnf, Test_AT_RcvMtaWrrAutotestQryCnf_004)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CWAS_QUERY;
    pstEvent->ulResult                      = VOS_ERR;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_CME_UNKNOWN));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrAutotestQryCnf(pRcvMsg);

    /* 结果验证 */
    EXPECT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(0,gstAtSendData.usBufLen);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaWrrAutotestQryCnf, Test_AT_RcvMtaWrrAutotestQryCnf_005)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    char                                      acExpectedResult[] = "4,5";

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CWAS_QUERY;
    pstEvent->ulResult                      = VOS_OK;
    pstEvent->stWrrAutoTestRslt.ulRsltNum = 2;
    pstEvent->stWrrAutoTestRslt.aulRslt[0] = 4;
    pstEvent->stWrrAutoTestRslt.aulRslt[1] = 5;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrAutotestQryCnf(pRcvMsg);

    /* 结果验证 */
    EXPECT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);

    GlobalMockObject::verify();
}
class Test_AT_RcvMtaWrrCellinfoQryCnf: public ::testing::Test
{
public:
    AT_MTA_MSG_STRU                    *pRcvMsg;
    MTA_AT_WRR_CELLINFO_QRY_CNF_STRU   *pstMtaWasCellInfoQryRsp;
    VOS_UINT8                           ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_WRR_CELLINFO_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstMtaWasCellInfoQryRsp = (MTA_AT_WRR_CELLINFO_QRY_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstMtaWasCellInfoQryRsp, 0, sizeof(MTA_AT_WRR_CELLINFO_QRY_CNF_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaWrrCellinfoQryCnf, Test_AT_RcvMtaWrrCellinfoQryCnf_001)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    // 变量初始化
    usClientId                              = 0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrCellinfoQryCnf(pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR,ulRst);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaWrrCellinfoQryCnf, Test_AT_RcvMtaWrrCellinfoQryCnf_002)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    VOS_UINT8                                 ucIndexTemp;

    /* 变量初始化 */
    usClientId                              = 0;
    ucIndexTemp = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndexTemp, (VOS_UINT32)sizeof(ucIndexTemp)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrCellinfoQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaWrrCellinfoQryCnf, Test_AT_RcvMtaWrrCellinfoQryCnf_003)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrCellinfoQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaWrrCellinfoQryCnf, Test_AT_RcvMtaWrrCellinfoQryCnf_004)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CELLINFO_QUERY;
    pstMtaWasCellInfoQryRsp->ulResult = VOS_ERR;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrCellinfoQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(0,gstAtSendData.usBufLen);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaWrrCellinfoQryCnf, Test_AT_RcvMtaWrrCellinfoQryCnf_005)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    char                                      acExpectedResult[] = "no cellinfo rslt";
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CELLINFO_QUERY;
    pstMtaWasCellInfoQryRsp->ulResult = VOS_OK;

    pstMtaWasCellInfoQryRsp->stWrrCellInfo.ulCellNum = 0;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrCellinfoQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaWrrCellinfoQryCnf, Test_AT_RcvMtaWrrCellinfoQryCnf_006)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    char                                      acExpectedResult[] = "2,2,2,2\r\n";
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CELLINFO_QUERY;
    pstMtaWasCellInfoQryRsp->ulResult = VOS_OK;

    pstMtaWasCellInfoQryRsp->stWrrCellInfo.ulCellNum = 1;
    pstMtaWasCellInfoQryRsp->stWrrCellInfo.astWCellInfo[0].usCellFreq = 2;
    pstMtaWasCellInfoQryRsp->stWrrCellInfo.astWCellInfo[0].usPrimaryScramCode = 2;
    pstMtaWasCellInfoQryRsp->stWrrCellInfo.astWCellInfo[0].sCpichRscp = 2;
    pstMtaWasCellInfoQryRsp->stWrrCellInfo.astWCellInfo[0].sCpichEcN0= 2;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrCellinfoQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvMtaMeanRptQryRsp: public ::testing::Test
{
    public:
    AT_MTA_MSG_STRU                 *pRcvMsg;
    MTA_AT_WRR_MEANRPT_QRY_CNF_STRU *pstMtaMeanRptQryRsp;
    VOS_UINT8                        ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_WRR_MEANRPT_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstMtaMeanRptQryRsp = (MTA_AT_WRR_MEANRPT_QRY_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(DRV_AGENT_MSG_STRU));
        PS_MEM_SET(pstMtaMeanRptQryRsp, 0, sizeof(MTA_AT_WRR_MEANRPT_QRY_CNF_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaMeanRptQryRsp, Test_AT_RcvMtaMeanRptQryRsp_001)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrMeanrptQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaMeanRptQryRsp, Test_AT_RcvMtaMeanRptQryRsp_002)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    VOS_UINT8                                 ucIndexTemp;

    /* 变量初始化 */
    usClientId                              = 0;
    ucIndexTemp = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndexTemp, (VOS_UINT32)sizeof(ucIndexTemp)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrMeanrptQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaMeanRptQryRsp, Test_AT_RcvMtaMeanRptQryRsp_003)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrMeanrptQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaMeanRptQryRsp, Test_AT_RcvMtaMeanRptQryRsp_004)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_MEANRPT_QUERY;
    pstMtaMeanRptQryRsp->ulResult = VOS_ERR;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_CME_UNKNOWN));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrMeanrptQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(0,gstAtSendData.usBufLen);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaMeanRptQryRsp, Test_AT_RcvMtaMeanRptQryRsp_005)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    char                                      acExpectedResult[] = "1\r\n0x4,2,5,6";
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_MEANRPT_QUERY;
    pstMtaMeanRptQryRsp->ulResult = VOS_OK;

    pstMtaMeanRptQryRsp->stMeanRptRslt.ulRptNum = 1;
    pstMtaMeanRptQryRsp->stMeanRptRslt.astMeanRptInfo[0].usCellNum           = 2;
    pstMtaMeanRptQryRsp->stMeanRptRslt.astMeanRptInfo[0].usEventId           = 4;
    pstMtaMeanRptQryRsp->stMeanRptRslt.astMeanRptInfo[0].ausPrimaryScramCode[0] = 5;
    pstMtaMeanRptQryRsp->stMeanRptRslt.astMeanRptInfo[0].ausPrimaryScramCode[1] = 6;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrMeanrptQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvMtaSetCellSrhRsp: public ::testing::Test
{
    public:

    AT_MTA_MSG_STRU                    *pRcvMsg;
    MTA_AT_WRR_CELLSRH_SET_CNF_STRU    *pstEvent;
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stElement;
    VOS_UINT8                           szCmdName[32];

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        VOS_StrCpy((VOS_CHAR*)szCmdName,"^SETCELL");
        g_stParseContext[ucIndex].pstCmdElement = &stElement;
        stElement.pszCmdName                    = szCmdName;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_WRR_CELLSRH_SET_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstEvent = (MTA_AT_WRR_CELLSRH_SET_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstEvent, 0, sizeof(MTA_AT_WRR_CELLSRH_SET_CNF_STRU));
    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaSetCellSrhRsp, Test_AT_RcvMtaSetCellSrhRsp_001)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrCellSrhSetCnf(pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR,ulRst);
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaSetCellSrhRsp, Test_AT_RcvMtaSetCellSrhRsp_002)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    VOS_UINT8                                 ucIndexTemp;

    /* 变量初始化 */
    usClientId                              = 0;
    ucIndexTemp = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndexTemp, (VOS_UINT32)sizeof(ucIndexTemp)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrCellSrhSetCnf(pRcvMsg);

    /* 结果验证 */
    EXPECT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaSetCellSrhRsp, Test_AT_RcvMtaSetCellSrhRsp_003)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrCellSrhSetCnf(pRcvMsg);

    /* 结果验证 */
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaSetCellSrhRsp, Test_AT_RcvMtaSetCellSrhRsp_004)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_WAS_MNTN_SET_CELLSRH;
    pstEvent->ulResult                      = VOS_ERR;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrCellSrhSetCnf(pRcvMsg);

    /* 结果验证 */
    EXPECT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(0,gstAtSendData.usBufLen);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaSetCellSrhRsp, Test_AT_RcvMtaSetCellSrhRsp_005)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_WAS_MNTN_SET_CELLSRH;
    pstEvent->ulResult                      = VOS_OK;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrCellSrhSetCnf(pRcvMsg);

    /* 结果验证 */
    EXPECT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    GlobalMockObject::verify();
}
class Test_AT_RcvMtaQryCellSrhRsp: public ::testing::Test
{
    public:
    AT_MTA_MSG_STRU                    *pRcvMsg;
    MTA_AT_WRR_CELLSRH_QRY_CNF_STRU    *pstEvent;
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stElement;
    VOS_UINT8                           szCmdName[32];

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        VOS_StrCpy((VOS_CHAR*)szCmdName,"^SETCELL");
        g_stParseContext[ucIndex].pstCmdElement = &stElement;
        stElement.pszCmdName                    = szCmdName;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_WRR_CELLSRH_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstEvent = (MTA_AT_WRR_CELLSRH_QRY_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstEvent, 0, sizeof(MTA_AT_WRR_CELLSRH_QRY_CNF_STRU));
    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaQryCellSrhRsp, Test_AT_RcvMtaQryCellSrhRsp_001)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrCellSrhQryCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryCellSrhRsp, Test_AT_RcvMtaQryCellSrhRsp_002)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    VOS_UINT8                                 ucIndexTemp;

    /* 变量初始化 */
    usClientId                              = 0;
    ucIndexTemp = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndexTemp, (VOS_UINT32)sizeof(ucIndexTemp)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrCellSrhQryCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryCellSrhRsp, Test_AT_RcvMtaQryCellSrhRsp_003)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;
    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrCellSrhQryCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryCellSrhRsp, Test_AT_RcvMtaQryCellSrhRsp_004)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    pstEvent->ulResult = VOS_ERR;
    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_WAS_MNTN_QRY_CELLSRH;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrCellSrhQryCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(0,gstAtSendData.usBufLen);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryCellSrhRsp, Test_AT_RcvMtaQryCellSrhRsp_005)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    pstEvent->ulResult = VOS_OK;
    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_WAS_MNTN_QRY_CELLSRH;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrCellSrhQryCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    GlobalMockObject::verify();
}
class Test_AT_RcvMtaFreqLockSetCnf: public ::testing::Test
{
    public:

    AT_MTA_MSG_STRU                    *pRcvMsg;
    MTA_AT_WRR_FREQLOCK_SET_CNF_STRU   *pstMtaFreqLockCtrl;
    VOS_UINT8                           ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_WRR_FREQLOCK_SET_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstMtaFreqLockCtrl = (MTA_AT_WRR_FREQLOCK_SET_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstMtaFreqLockCtrl, 0, sizeof(MTA_AT_WRR_FREQLOCK_SET_CNF_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaFreqLockSetCnf, Test_AT_RcvMtaFreqLockSetCnf_001)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrFreqLockSetCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaFreqLockSetCnf, Test_AT_RcvMtaFreqLockSetCnf_002)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    VOS_UINT8                                 ucIndexTemp;

    /* 变量初始化 */
    usClientId                              = 0;
    ucIndexTemp = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndexTemp, (VOS_UINT32)sizeof(ucIndexTemp)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrFreqLockSetCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaFreqLockSetCnf, Test_AT_RcvMtaFreqLockSetCnf_003)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;
    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrFreqLockSetCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaFreqLockSetCnf, Test_AT_RcvMtaFreqLockSetCnf_004)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_FREQLOCK_SET;
    pstMtaFreqLockCtrl->ulResult       = VOS_ERR;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_CME_UNKNOWN));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrFreqLockSetCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(0,gstAtSendData.usBufLen);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaFreqLockSetCnf, Test_AT_RcvMtaFreqLockSetCnf_005)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_FREQLOCK_SET;
    pstMtaFreqLockCtrl->ulResult       = VOS_OK;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrFreqLockSetCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvMtaFreqLockQryCnf: public ::testing::Test
{
public:
    AT_MTA_MSG_STRU                    *pRcvMsg;
    MTA_AT_WRR_FREQLOCK_QRY_CNF_STRU   *pstMtaFreqLockCtrl;
    VOS_UINT8                           ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_WRR_FREQLOCK_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstMtaFreqLockCtrl = (MTA_AT_WRR_FREQLOCK_QRY_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstMtaFreqLockCtrl, 0, sizeof(MTA_AT_WRR_FREQLOCK_QRY_CNF_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaFreqLockQryCnf, Test_AT_RcvMtaFreqLockQryCnf_001)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrFreqLockQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaFreqLockQryCnf, Test_AT_RcvMtaFreqLockQryCnf_002)
{
   /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    VOS_UINT8                                 ucIndexTemp;

    /* 变量初始化 */
    usClientId                              = 0;

    ucIndexTemp = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndexTemp, (VOS_UINT32)sizeof(ucIndexTemp)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrFreqLockQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaFreqLockQryCnf, Test_AT_RcvMtaFreqLockQryCnf_003)
{
   /* 变量定义 */
    VOS_UINT32                          ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrFreqLockQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaFreqLockQryCnf, Test_AT_RcvMtaFreqLockQryCnf_004)
{
    /* 变量定义 */
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_FREQLOCK_QUERY;
    pstMtaFreqLockCtrl->ulResult       = VOS_ERR;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_CME_UNKNOWN));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrFreqLockQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(0,gstAtSendData.usBufLen);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaFreqLockQryCnf, Test_AT_RcvMtaFreqLockQryCnf_005)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    char                                acExpectedResult[] = "^FREQLOCK: 0";
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       = {AT_CMD_FREQLOCK,
                                                              At_SetFreqLock,           AT_SET_PARA_TIME,
                                                              At_QryFreqLock,           AT_QRY_PARA_TIME,
                                                              VOS_NULL_PTR,             AT_NOT_SET_TIME,
                                                              VOS_NULL_PTR,             AT_NOT_SET_TIME,
                                                              AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
                                                              (VOS_UINT8*)"^FREQLOCK", (VOS_UINT8*)"(0,1),(412-10838)"};
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_FREQLOCK_QUERY;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstMtaFreqLockCtrl->stFreqLockInfo.ucFreqLockEnable = 0;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrFreqLockQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaFreqLockQryCnf, Test_AT_RcvMtaFreqLockQryCnf_006)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    char                                acExpectedResult[] = "^FREQLOCK: 1,0";
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       = {AT_CMD_FREQLOCK,
                                                              At_SetFreqLock,           AT_SET_PARA_TIME,
                                                              At_QryFreqLock,           AT_QRY_PARA_TIME,
                                                              VOS_NULL_PTR,             AT_NOT_SET_TIME,
                                                              VOS_NULL_PTR,             AT_NOT_SET_TIME,
                                                              AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
                                                              (VOS_UINT8*)"^FREQLOCK", (VOS_UINT8*)"(0,1),(412-10838)"};
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_FREQLOCK_QUERY;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstMtaFreqLockCtrl->stFreqLockInfo.ucFreqLockEnable = 1;
    pstMtaFreqLockCtrl->stFreqLockInfo.usLockedFreq = 0;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrFreqLockQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvMtaRrcVersionSetCnf: public ::testing::Test
{
public:
    AT_MTA_MSG_STRU                     *pRcvMsg;
    MTA_AT_WRR_RRC_VERSION_SET_CNF_STRU *pstRrcVersion;
    VOS_UINT8                            ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_WRR_RRC_VERSION_SET_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstRrcVersion = (MTA_AT_WRR_RRC_VERSION_SET_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstRrcVersion, 0, sizeof(MTA_AT_WRR_RRC_VERSION_SET_CNF_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaRrcVersionSetCnf, Test_AT_RcvMtaRrcVersionSetCnf_001)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrRrcVersionSetCnf((VOS_VOID*)pRcvMsg);
    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaRrcVersionSetCnf, Test_AT_RcvMtaRrcVersionSetCnf_002)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    VOS_UINT8                                 ucIndexTemp;

    /* 变量初始化 */
    usClientId                              = 0;
    ucIndexTemp = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndexTemp, (VOS_UINT32)sizeof(ucIndexTemp)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrRrcVersionSetCnf((VOS_VOID*)pRcvMsg);
    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaRrcVersionSetCnf, Test_AT_RcvMtaRrcVersionSetCnf_003)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrRrcVersionSetCnf((VOS_VOID*)pRcvMsg);
    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaRrcVersionSetCnf, Test_AT_RcvMtaRrcVersionSetCnf_004)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    pstRrcVersion->ulResult = VOS_ERR;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_RRC_VERSION_SET;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrRrcVersionSetCnf((VOS_VOID*)pRcvMsg);
    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(0,gstAtSendData.usBufLen);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaRrcVersionSetCnf, Test_AT_RcvMtaRrcVersionSetCnf_005)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    pstRrcVersion->ulResult = VOS_OK;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_RRC_VERSION_SET;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrRrcVersionSetCnf((VOS_VOID*)pRcvMsg);
    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    GlobalMockObject::verify();
}
class Test_AT_RcvMtaRrcVersionQryCnf: public ::testing::Test
{
    public:
    AT_MTA_MSG_STRU                        *pRcvMsg;
    MTA_AT_WRR_RRC_VERSION_QRY_CNF_STRU    *pstRrcVersionQryCnf;
    VOS_UINT8                               ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_WRR_RRC_VERSION_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstRrcVersionQryCnf = (MTA_AT_WRR_RRC_VERSION_QRY_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstRrcVersionQryCnf, 0, sizeof(MTA_AT_WRR_RRC_VERSION_QRY_CNF_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaRrcVersionQryCnf, Test_AT_RcvMtaRrcVersionQryCnf_001)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrRrcVersionQryCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaRrcVersionQryCnf, Test_AT_RcvMtaRrcVersionQryCnf_002)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    VOS_UINT8                                 ucIndexTemp;

    /* 变量初始化 */
    usClientId                              = 0;
    ucIndexTemp = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndexTemp, (VOS_UINT32)sizeof(ucIndexTemp)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrRrcVersionQryCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaRrcVersionQryCnf, Test_AT_RcvMtaRrcVersionQryCnf_003)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrRrcVersionQryCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaRrcVersionQryCnf, Test_AT_RcvMtaRrcVersionQryCnf_004)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    pstRrcVersionQryCnf->ulResult = VOS_ERR;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_RRC_VERSION_QUERY;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaWrrRrcVersionQryCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(0,gstAtSendData.usBufLen);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaRrcVersionQryCnf, Test_AT_RcvMtaRrcVersionQryCnf_005)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    char                                acExpectedResult[] = "^HSPA: 3";
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       = {AT_CMD_HSPA,
                                                              At_SetRRCVersion,         AT_SET_PARA_TIME,
                                                              At_QryRRCVersion,         AT_QRY_PARA_TIME,
                                                              VOS_NULL_PTR,             AT_NOT_SET_TIME,
                                                              VOS_NULL_PTR,             AT_NOT_SET_TIME,
                                                              AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
                                                              (VOS_UINT8*)"^HSPA",      (VOS_UINT8*)"(0-3)"};

    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_RRC_VERSION_QUERY;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstRrcVersionQryCnf->ulResult     = VOS_OK;
    pstRrcVersionQryCnf->ucRrcVersion = 3;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaWrrRrcVersionQryCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvMtaQryCurcCnf: public ::testing::Test
{
    public:
    AT_MTA_MSG_STRU                        *pRcvMsg;
    MTA_AT_CURC_QRY_CNF_STRU               *pstCurcQryCnf;
    VOS_UINT8                               ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_CURC_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstCurcQryCnf = (MTA_AT_CURC_QRY_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstCurcQryCnf, 0, sizeof(MTA_AT_CURC_QRY_CNF_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaQryCurcCnf, Test_AT_RcvMtaQryCurcCnf_001)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRst = AT_RcvMtaQryCurcCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryCurcCnf, Test_AT_RcvMtaQryCurcCnf_002)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    VOS_UINT8                                 ucIndexTemp;

    /* 变量初始化 */
    usClientId                              = 0;
    ucIndexTemp = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndexTemp, (VOS_UINT32)sizeof(ucIndexTemp)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaQryCurcCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryCurcCnf, Test_AT_RcvMtaQryCurcCnf_003)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaQryCurcCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryCurcCnf, Test_AT_RcvMtaQryCurcCnf_004)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    pstCurcQryCnf->enResult = MTA_AT_RESULT_ERROR;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CURC_READ;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaQryCurcCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(0,gstAtSendData.usBufLen);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryCurcCnf, Test_AT_RcvMtaQryCurcCnf_005)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    char                                acExpectedResult[] = "^CURC: 2,0xFFFFFFFFFFFFFFFF";
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       = {AT_CMD_CURC,
                                                                At_SetCurcPara,      AT_SET_PARA_TIME,
                                                                At_QryCurcPara,     AT_QRY_PARA_TIME,
                                                                VOS_NULL_PTR,    AT_NOT_SET_TIME,
                                                                VOS_NULL_PTR,        AT_NOT_SET_TIME,
                                                                AT_ERROR, CMD_TBL_PIN_IS_LOCKED,
                                                                (VOS_UINT8*)"^CURC", (VOS_UINT8*)"(0-2)"};

    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CURC_READ;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstCurcQryCnf->enResult      = MTA_AT_RESULT_NO_ERROR;
    pstCurcQryCnf->enCurcRptType = AT_MTA_RPT_GENERAL_CONTROL_CUSTOM;
    PS_MEM_SET(pstCurcQryCnf->aucRptCfg, 0xFF, sizeof(pstCurcQryCnf->aucRptCfg));

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaQryCurcCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvMtaSetUnsolicitedRptCnf: public ::testing::Test
{
    public:
    AT_MTA_MSG_STRU                        *pRcvMsg;
    MTA_AT_UNSOLICITED_RPT_SET_CNF_STRU    *pstUnsolicitedSetCnf;
    VOS_UINT8                               ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_UNSOLICITED_RPT_SET_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstUnsolicitedSetCnf = (MTA_AT_UNSOLICITED_RPT_SET_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstUnsolicitedSetCnf, 0, sizeof(MTA_AT_UNSOLICITED_RPT_SET_CNF_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaSetUnsolicitedRptCnf, Test_AT_RcvMtaSetUnsolicitedRptCnf_001)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRst = AT_RcvMtaSetUnsolicitedRptCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaSetUnsolicitedRptCnf, Test_AT_RcvMtaSetUnsolicitedRptCnf_002)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    VOS_UINT8                                 ucIndexTemp;

    /* 变量初始化 */
    usClientId                              = 0;
    ucIndexTemp = AT_BROADCAST_CLIENT_INDEX_MODEM_1;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndexTemp, (VOS_UINT32)sizeof(ucIndexTemp)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaSetUnsolicitedRptCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaSetUnsolicitedRptCnf, Test_AT_RcvMtaSetUnsolicitedRptCnf_003)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaSetUnsolicitedRptCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaSetUnsolicitedRptCnf, Test_AT_RcvMtaSetUnsolicitedRptCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRst;
    VOS_UINT16                          usClientId;
    VOS_UINT32                          ulResult = AT_ERROR;

    /* 变量初始化 */
    usClientId                              = 0;
    pstUnsolicitedSetCnf->enResult = MTA_AT_RESULT_ERROR;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_UNSOLICITED_RPT_SET;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq(ulResult));

    // 调用被测函数
    ulRst = AT_RcvMtaSetUnsolicitedRptCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(0,gstAtSendData.usBufLen);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaSetUnsolicitedRptCnf, Test_AT_RcvMtaSetUnsolicitedRptCnf_005)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       = {AT_CMD_SYSMODE,
                                                            At_SetModePara,         AT_NOT_SET_TIME,
                                                            VOS_NULL_PTR,           AT_NOT_SET_TIME,
                                                            VOS_NULL_PTR, AT_NOT_SET_TIME,
                                                            VOS_NULL_PTR,   AT_NOT_SET_TIME,
                                                            AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
                                                            (VOS_UINT8*)"^MODE",       (VOS_UINT8*)"(0,1)"};

    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_UNSOLICITED_RPT_SET;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstUnsolicitedSetCnf->enResult      = MTA_AT_RESULT_NO_ERROR;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaSetUnsolicitedRptCnf(pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvMtaQryUnsolicitedRptCnf : public ::testing::Test
{
    public:
    AT_MTA_MSG_STRU                        *pRcvMsg;
    MTA_AT_UNSOLICITED_RPT_QRY_CNF_STRU               *pstUnsolicitedSetCnf;
    VOS_UINT8                               ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_UNSOLICITED_RPT_QRY_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstUnsolicitedSetCnf = (MTA_AT_UNSOLICITED_RPT_QRY_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstUnsolicitedSetCnf, 0, sizeof(MTA_AT_UNSOLICITED_RPT_QRY_CNF_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaQryUnsolicitedRptCnf, Test_AT_RcvMtaQryUnsolicitedRptCnf_001)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRst = AT_RcvMtaQryUnsolicitedRptCnf ((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryUnsolicitedRptCnf, Test_AT_RcvMtaQryUnsolicitedRptCnf_002)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    VOS_UINT8                                 ucIndexTemp;

    /* 变量初始化 */
    usClientId                              = 0;
    ucIndexTemp = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndexTemp, (VOS_UINT32)sizeof(ucIndexTemp)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaQryUnsolicitedRptCnf ((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryUnsolicitedRptCnf, Test_AT_RcvMtaQryUnsolicitedRptCnf_003)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaQryUnsolicitedRptCnf ((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryUnsolicitedRptCnf, Test_AT_RcvMtaQryUnsolicitedRptCnf_004)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    pstUnsolicitedSetCnf->enResult = MTA_AT_RESULT_ERROR;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_UNSOLICITED_RPT_READ;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRst = AT_RcvMtaQryUnsolicitedRptCnf ((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(0,gstAtSendData.usBufLen);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryUnsolicitedRptCnf, Test_AT_RcvMtaQryUnsolicitedRptCnf_005)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    char                                acExpectedResult[] = "^TIME: 1";
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       = {AT_CMD_TIME,
                                                            At_SetTimePara,         AT_SET_PARA_TIME,
                                                            At_QryTimePara,         AT_QRY_PARA_TIME,
                                                            VOS_NULL_PTR, AT_NOT_SET_TIME,
                                                            VOS_NULL_PTR,   AT_NOT_SET_TIME,
                                                            AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
                                                            (VOS_UINT8*)"^TIME",    (VOS_UINT8*)"(0,1)"};

    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_UNSOLICITED_RPT_READ;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstUnsolicitedSetCnf->enResult       = MTA_AT_RESULT_NO_ERROR;
    pstUnsolicitedSetCnf->enReqType      = AT_MTA_QRY_TIME_RPT_TYPE;
    pstUnsolicitedSetCnf->u.ucTimeRptFlg = VOS_TRUE;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaQryUnsolicitedRptCnf (pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryUnsolicitedRptCnf, Test_AT_RcvMtaQryUnsolicitedRptCnf_006)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    char                                acExpectedResult[] = "+CTZR: 0";
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       = {AT_CMD_CTZR,
                                                            At_SetCtzrPara,     AT_SET_PARA_TIME,
                                                            At_QryCtzrPara,    AT_QRY_PARA_TIME,
                                                            VOS_NULL_PTR,       AT_NOT_SET_TIME,
                                                            VOS_NULL_PTR,        AT_NOT_SET_TIME,
                                                            AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
                                                            (VOS_UINT8*)"+CTZR",    (VOS_UINT8*)"(0,1)"};

    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_UNSOLICITED_RPT_READ;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstUnsolicitedSetCnf->enResult       = MTA_AT_RESULT_NO_ERROR;
    pstUnsolicitedSetCnf->enReqType      = AT_MTA_QRY_CTZR_RPT_TYPE;
    pstUnsolicitedSetCnf->u.ucCtzvRptFlg = VOS_FALSE;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaQryUnsolicitedRptCnf (pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryUnsolicitedRptCnf, Test_AT_RcvMtaQryUnsolicitedRptCnf_007)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    char                                acExpectedResult[] = "+CSSN: 0,1";
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       = {AT_CMD_CSSN,
                                                            At_SetCssnPara,     AT_NOT_SET_TIME,
                                                            At_QryCssnPara,   AT_NOT_SET_TIME,
                                                            VOS_NULL_PTR, AT_NOT_SET_TIME,
                                                            VOS_NULL_PTR,        AT_NOT_SET_TIME,
                                                            AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
                                                            (VOS_UINT8*)"+CSSN", (VOS_UINT8*)"(0,1),(0,1)"};

    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_UNSOLICITED_RPT_READ;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstUnsolicitedSetCnf->enResult       = MTA_AT_RESULT_NO_ERROR;
    pstUnsolicitedSetCnf->enReqType      = AT_MTA_QRY_CSSN_RPT_TYPE;
    pstUnsolicitedSetCnf->u.stCssnRptFlg.ucCssiRptFlg = VOS_FALSE;
    pstUnsolicitedSetCnf->u.stCssnRptFlg.ucCssuRptFlg = VOS_TRUE;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaQryUnsolicitedRptCnf (pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryUnsolicitedRptCnf, Test_AT_RcvMtaQryUnsolicitedRptCnf_008)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    char                                acExpectedResult[] = "+CUSD: 1";
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       = {AT_CMD_CUSD,
                                                            At_SetCusdPara,     AT_SET_PARA_TIME,
                                                            At_QryCusdPara,   AT_NOT_SET_TIME,
                                                            At_TestCusdPara, AT_NOT_SET_TIME,
                                                            VOS_NULL_PTR,        AT_NOT_SET_TIME,
                                                            AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
                                                            (VOS_UINT8*)"+CUSD", (VOS_UINT8*)"(0-2),(str),(0-255)"};

    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_UNSOLICITED_RPT_READ;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstUnsolicitedSetCnf->enResult       = MTA_AT_RESULT_NO_ERROR;
    pstUnsolicitedSetCnf->enReqType      = AT_MTA_QRY_CUSD_RPT_TYPE;
    pstUnsolicitedSetCnf->u.ucCusdRptFlg = VOS_TRUE;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaQryUnsolicitedRptCnf (pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaQryUnsolicitedRptCnf, Test_AT_RcvMtaQryUnsolicitedRptCnf_009)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_UNSOLICITED_RPT_READ;

    pstUnsolicitedSetCnf->enResult       = MTA_AT_RESULT_NO_ERROR;
    pstUnsolicitedSetCnf->enReqType      = AT_MTA_QRY_RPT_TYPE_BUTT;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_CME_UNKNOWN));

    /* 调用被测函数 */
    ulRst = AT_RcvMtaQryUnsolicitedRptCnf (pRcvMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    EXPECT_EQ(g_stParseContext[ucIndex].ucClientStatus,AT_FW_CLIENT_STATUS_READY);
    EXPECT_EQ(gastAtClientTab[ucIndex].CmdCurrentOpt, AT_CMD_CURRENT_OPT_BUTT);
    EXPECT_EQ(gastAtClientTab[ucIndex].opId, 0);
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvMtaBodySarSetCnf: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }

    void TearDown()
    {

    }
};


TEST_F(Test_AT_RcvMtaBodySarSetCnf, Test_AT_RcvMtaBodySarSetCnf_001)
{
	VOS_UINT32  						ulRet;
    AT_MTA_MSG_STRU                    	stRcvMsg;

	// 参数初始化
	PS_MEM_SET(&stRcvMsg,0x00,sizeof(stRcvMsg));

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .stubs()
    	.will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRet = AT_RcvMtaBodySarSetCnf((VOS_VOID *)&stRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaBodySarSetCnf, Test_AT_RcvMtaBodySarSetCnf_002)
{
	VOS_UINT32  						ulRet;
    AT_MTA_MSG_STRU                    	stRcvMsg;

	// 参数初始化
	PS_MEM_SET(&stRcvMsg,0x00,sizeof(stRcvMsg));
	stRcvMsg.stAppCtrl.usClientId	= MN_CLIENT_ALL;

    // 调用被测函数
    ulRet = AT_RcvMtaBodySarSetCnf((VOS_VOID *)&stRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaBodySarSetCnf, Test_AT_RcvMtaBodySarSetCnf_003)
{
	VOS_UINT32  						ulRet;
	VOS_UINT8                           ucIndex;
    AT_MTA_MSG_STRU                    	stRcvMsg;

	// 参数初始化
	ucIndex	= 0;
	PS_MEM_SET(&stRcvMsg,0x00,sizeof(stRcvMsg));

	gastAtClientTab[0].CmdCurrentOpt =	AT_CMD_INVALID;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .stubs()
		.with(any(),outBoundP(&ucIndex,sizeof(ucIndex)))
		.will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRet = AT_RcvMtaBodySarSetCnf((VOS_VOID *)&stRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaBodySarSetCnf, Test_AT_RcvMtaBodySarSetCnf_004)
{
	VOS_UINT32  						ulRet;
	VOS_UINT8                           ucIndex;
	MTA_AT_RESULT_CNF_STRU             	stBodySarSetCnf;
    AT_MTA_MSG_STRU                    	stRcvMsg;

	// 参数初始化
	ucIndex	= 0;
	stBodySarSetCnf.enResult = MTA_AT_RESULT_ERROR;
	PS_MEM_SET(&stRcvMsg,0x00,sizeof(stRcvMsg));
	PS_MEM_CPY(stRcvMsg.aucContent,&stBodySarSetCnf,sizeof(stBodySarSetCnf));

	// 初始化全局变量
	gastAtClientTab[0].CmdCurrentOpt =	AT_CMD_BODYSARON_SET;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .stubs()
		.with(any(),outBoundP(&ucIndex,sizeof(ucIndex)))
		.will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRet = AT_RcvMtaBodySarSetCnf((VOS_VOID *)&stRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaBodySarSetCnf, Test_AT_RcvMtaBodySarSetCnf_005)
{
	VOS_UINT32  						ulRet;
	VOS_UINT8                           ucIndex;
	MTA_AT_RESULT_CNF_STRU             	stBodySarSetCnf;
    AT_MTA_MSG_STRU                    	stRcvMsg;

	// 参数初始化
	ucIndex	= 0;
	stBodySarSetCnf.enResult = MTA_AT_RESULT_NO_ERROR;
	PS_MEM_SET(&stRcvMsg,0x00,sizeof(stRcvMsg));
	PS_MEM_CPY(stRcvMsg.aucContent,&stBodySarSetCnf,sizeof(stBodySarSetCnf));

	// 初始化全局变量
	gastAtClientTab[0].CmdCurrentOpt =	AT_CMD_BODYSARON_SET;
	g_enAtBodySarState = MTA_BODY_SAR_OFF;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .stubs()
		.with(any(),outBoundP(&ucIndex,sizeof(ucIndex)))
		.will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRet = AT_RcvMtaBodySarSetCnf((VOS_VOID *)&stRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(MTA_BODY_SAR_ON, g_enAtBodySarState);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaBodySarSetCnf, Test_AT_RcvMtaBodySarSetCnf_006)
{
	VOS_UINT32  						ulRet;
	VOS_UINT8                           ucIndex;
	MTA_AT_RESULT_CNF_STRU             	stBodySarSetCnf;
    AT_MTA_MSG_STRU                    	stRcvMsg;

	// 参数初始化
	ucIndex	= 0;
	stBodySarSetCnf.enResult = MTA_AT_RESULT_NO_ERROR;
	PS_MEM_SET(&stRcvMsg,0x00,sizeof(stRcvMsg));
	PS_MEM_CPY(stRcvMsg.aucContent,&stBodySarSetCnf,sizeof(stBodySarSetCnf));

	// 初始化全局变量
	gastAtClientTab[0].CmdCurrentOpt =	AT_CMD_BODYSARON_SET;
	g_enAtBodySarState = MTA_BODY_SAR_ON;

    // MOCKER操作;
    MOCKER(At_ClientIdToUserId)
        .stubs()
		.with(any(),outBoundP(&ucIndex,sizeof(ucIndex)))
		.will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRet = AT_RcvMtaBodySarSetCnf((VOS_VOID *)&stRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(MTA_BODY_SAR_OFF, g_enAtBodySarState);

    GlobalMockObject::verify();
}

class Test_AT_RcvMmaCerssiInfoQueryCnf: public ::testing::Test
{
public:
    VOS_UINT8                               ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                 stCmdInfo;
    AT_MTA_MSG_STRU                        *pstMsg;
    VOS_UINT32                              ulMsgLen;
    TAF_MMA_CERSSI_INFO_QUERY_CNF_STRU     *pstCerssiInfoQueryCnf;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_ID_PCUI;

        stCmdInfo.ulCmdIndex        = AT_CMD_CERSSI;
        stCmdInfo.pfnSetProc        = VOS_NULL_PTR;
        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = At_QryCerssiPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^CERSSI";
        stCmdInfo.pszParam          = (VOS_UINT8*)"(0,1,2,3,4,5),(0-20)";

        ulMsgLen    = sizeof(TAF_MMA_CERSSI_INFO_QUERY_CNF_STRU);
        pstMsg      = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulMsgLen);
        PS_MEM_SET(pstMsg, 0, ulMsgLen);

        pstMsg->ulReceiverPid           = WUEPS_PID_AT;
        pstMsg->ulSenderPid             = WUEPS_PID_MMA;
        pstMsg->ulMsgId                 = AT_MMA_CERSSI_INFO_QUERY_CNF;

        pstCerssiInfoQueryCnf    = (TAF_MMA_CERSSI_INFO_QUERY_CNF_STRU *)pstMsg;
        pstCerssiInfoQueryCnf->stAtAppCtrl.usClientId      = AT_CLIENT_TAB_PCUI_INDEX;
        pstCerssiInfoQueryCnf->stAtAppCtrl.ucOpId          = 1;
        pstCerssiInfoQueryCnf->stCerssi.ucCurrentUtranMode = 0; //NAS_UTRANCTRL_UTRAN_MODE_FDD
        pstCerssiInfoQueryCnf->stCerssi.ucRssiNum          = 1;
        pstCerssiInfoQueryCnf->ucRslt                      = VOS_OK;

        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CERSSI_READ;
        gastAtClientTab[ucIndex].usClientId     = AT_CLIENT_TAB_PCUI_INDEX;
        gastAtClientTab[ucIndex].ucUsed         = AT_CLIENT_USED;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
    }

    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMsg);
        }

        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;
        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
    }
};
TEST_F(Test_AT_RcvMmaCerssiInfoQueryCnf, Test_AT_RcvMmaCerssiInfoQueryCnf_001)
{
   // 变量声明

    // 参数初始化
    pstCerssiInfoQueryCnf->ucRslt = VOS_ERR;

    // 调用被测函数
    AT_RcvMmaCerssiInfoQueryCnf(pstCerssiInfoQueryCnf);

    // 执行检查
    EXPECT_EQ(AT_FW_CLIENT_STATUS_READY,g_stParseContext[ucIndex].ucClientStatus);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT,gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaCerssiInfoQueryCnf, Test_AT_RcvMmaCerssiInfoQueryCnf_002)
{
    // 变量声明
  //  char                               aucRslt[] = "\r\n^CERSSI:1,0,0,-60,255,0,0,0,0\r\n\r\n\r\nOK\r\n";                /*回车换行*/

    AT_MODEM_NET_CTX_STRU              *pstNetCtx = VOS_NULL_PTR;

    // 参数初始化
    pstCerssiInfoQueryCnf->ucRslt = VOS_OK;
    pstCerssiInfoQueryCnf->stCerssi.aRssi[0].u.stWCellSignInfo.sRscpValue =  -60;
	pstCerssiInfoQueryCnf->stCerssi.enRatType = TAF_PH_RAT_WCDMA;
	pstCerssiInfoQueryCnf->stCerssi.ucCurrentUtranMode = TAF_UTRANCTRL_UTRAN_MODE_TDD;

    pstNetCtx = AT_GetModemNetCtxAddrFromClientId(ucIndex);

    pstNetCtx->ucCerssiReportType       = 1;
    pstNetCtx->ucCerssiMinTimerInterval = 0;

    // 调用被测函数
    AT_RcvMmaCerssiInfoQueryCnf(pstCerssiInfoQueryCnf);

    // 执行检查
  //  ASSERT_STREQ(aucRslt , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(AT_FW_CLIENT_STATUS_READY,g_stParseContext[ucIndex].ucClientStatus);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT,gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMmaCerssiInfoQueryCnf, Test_AT_RcvMmaCerssiInfoQueryCnf_003)
{
    // 变量声明
   // char                               aucRslt[] = "\r\n^CERSSI:1,0,0,-60,10,0,0,0,0\r\n\r\n\r\nOK\r\n";                /*回车换行*/
    AT_MODEM_NET_CTX_STRU              *pstNetCtx = VOS_NULL_PTR;

    // 参数初始化
    pstCerssiInfoQueryCnf->ucRslt = VOS_OK;
    pstCerssiInfoQueryCnf->stCerssi.aRssi[0].u.stWCellSignInfo.sRscpValue =  -60;
    pstCerssiInfoQueryCnf->stCerssi.aRssi[0].u.stWCellSignInfo.sEcioValue =  10;
    pstNetCtx = AT_GetModemNetCtxAddrFromClientId(ucIndex);
	pstCerssiInfoQueryCnf->stCerssi.enRatType = TAF_PH_RAT_WCDMA;
	pstCerssiInfoQueryCnf->stCerssi.ucCurrentUtranMode = TAF_UTRANCTRL_UTRAN_MODE_FDD;


    pstNetCtx->ucCerssiReportType       = 1;
    pstNetCtx->ucCerssiMinTimerInterval = 0;

    // 调用被测函数
    At_MmaMsgProc((MSG_HEADER_STRU *)pstMsg);

    // 执行检查
  //  ASSERT_STREQ(aucRslt , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(AT_FW_CLIENT_STATUS_READY,g_stParseContext[ucIndex].ucClientStatus);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT,gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMmaCerssiInfoQueryCnf, Test_AT_RcvMmaCerssiInfoQueryCnf_004)
{
    // 变量声明
  //  char                               aucRslt[] = "\r\n^CERSSI:1,0,-60,0,255,0,0,0,0\r\n\r\n\r\nOK\r\n";                /*回车换行*/
    AT_MODEM_NET_CTX_STRU              *pstNetCtx = VOS_NULL_PTR;

    // 参数初始化
    pstCerssiInfoQueryCnf->ucRslt = VOS_OK;
    pstCerssiInfoQueryCnf->stCerssi.aRssi[0].u.stGCellSignInfo.sRssiValue =  -60;
    pstNetCtx = AT_GetModemNetCtxAddrFromClientId(ucIndex);

    pstNetCtx->ucCerssiReportType       = 1;
    pstNetCtx->ucCerssiMinTimerInterval = 0;

	pstCerssiInfoQueryCnf->stCerssi.enRatType = TAF_PH_RAT_GSM;
	pstCerssiInfoQueryCnf->stCerssi.ucCurrentUtranMode = TAF_UTRANCTRL_UTRAN_MODE_FDD;


    // 调用被测函数
    AT_RcvMmaCerssiInfoQueryCnf(pstCerssiInfoQueryCnf);

    // 执行检查
  //  ASSERT_STREQ(aucRslt , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(AT_FW_CLIENT_STATUS_READY,g_stParseContext[ucIndex].ucClientStatus);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT,gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMmaCerssiInfoQueryCnf, Test_AT_RcvMmaCerssiInfoQueryCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    AT_MODEM_NET_CTX_STRU              *pstNetCtx = VOS_NULL_PTR;

    // 参数初始化
    pstCerssiInfoQueryCnf->ucRslt = VOS_OK;
    pstCerssiInfoQueryCnf->stCerssi.aRssi[0].u.stGCellSignInfo.sRssiValue =  -60;
    pstNetCtx = AT_GetModemNetCtxAddrFromClientId(ucIndex);

    pstNetCtx->ucCerssiReportType       = 1;
    pstNetCtx->ucCerssiMinTimerInterval = 0;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRet = AT_RcvMmaCerssiInfoQueryCnf(pstCerssiInfoQueryCnf);

    // 执行检查
    EXPECT_EQ(VOS_ERR , ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaCerssiInfoQueryCnf, Test_AT_RcvMmaCerssiInfoQueryCnf_006)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    AT_MODEM_NET_CTX_STRU              *pstNetCtx = VOS_NULL_PTR;
	VOS_UINT8							Index;

    // 参数初始化
    pstCerssiInfoQueryCnf->ucRslt = VOS_OK;
    pstCerssiInfoQueryCnf->stCerssi.aRssi[0].u.stGCellSignInfo.sRssiValue =  -60;
    pstNetCtx = AT_GetModemNetCtxAddrFromClientId(ucIndex);

    pstNetCtx->ucCerssiReportType       = 1;
    pstNetCtx->ucCerssiMinTimerInterval = 0;
    Index = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&Index, (VOS_UINT32)sizeof(Index)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRet = AT_RcvMmaCerssiInfoQueryCnf(pstCerssiInfoQueryCnf);

    // 执行检查
    EXPECT_EQ(VOS_ERR , ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaCerssiInfoQueryCnf, Test_AT_RcvMmaCerssiInfoQueryCnf_007)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    AT_MODEM_NET_CTX_STRU              *pstNetCtx = VOS_NULL_PTR;

    // 参数初始化
    pstCerssiInfoQueryCnf->ucRslt = VOS_OK;
    pstCerssiInfoQueryCnf->stCerssi.aRssi[0].u.stGCellSignInfo.sRssiValue =  -60;
    pstNetCtx = AT_GetModemNetCtxAddrFromClientId(ucIndex);

    pstNetCtx->ucCerssiReportType       = 1;
    pstNetCtx->ucCerssiMinTimerInterval = 0;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_ACINFO_READ;

    // MOCKER操作

    // 调用被测函数
    ulRet = AT_RcvMmaCerssiInfoQueryCnf(pstCerssiInfoQueryCnf);

    // 执行检查
    EXPECT_EQ(VOS_ERR , ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaCerssiInfoQueryCnf, Test_AT_RcvMmaCerssiInfoQueryCnf_008)
{
    // 变量声明
  //  char                               aucRslt[] = "\r\n^CERSSI:1,0,0,0,255,0,0,0,0\r\n\r\n\r\nOK\r\n";                /*回车换行*/
    AT_MODEM_NET_CTX_STRU              *pstNetCtx = VOS_NULL_PTR;

    // 参数初始化
    pstCerssiInfoQueryCnf->ucRslt = VOS_OK;
    pstCerssiInfoQueryCnf->stCerssi.aRssi[0].u.stWCellSignInfo.sRscpValue =  0;
    pstCerssiInfoQueryCnf->stCerssi.aRssi[0].u.stWCellSignInfo.sEcioValue =  0;
    pstNetCtx = AT_GetModemNetCtxAddrFromClientId(ucIndex);
	pstCerssiInfoQueryCnf->stCerssi.enRatType = TAF_PH_RAT_BUTT;
	pstCerssiInfoQueryCnf->stCerssi.ucCurrentUtranMode = TAF_UTRANCTRL_UTRAN_MODE_FDD;

    pstNetCtx->ucCerssiReportType       = 1;
    pstNetCtx->ucCerssiMinTimerInterval = 0;

    // 调用被测函数
    At_MmaMsgProc((MSG_HEADER_STRU *)pstMsg);

    // 执行检查
 //   ASSERT_STREQ(aucRslt , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(AT_FW_CLIENT_STATUS_READY,g_stParseContext[ucIndex].ucClientStatus);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT,gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMmaCerssiInfoQueryCnf, Test_AT_RcvMmaCerssiInfoQueryCnf_009)
{
    // 变量声明
 //   char                                aucRslt[] = "\r\n^CERSSI:5,10,0,-97,255,0,0,0,0\r\n\r\n\r\nOK\r\n";                /*回车换行*/
	VOS_UINT8							aucMsg[] = {0x00,0x00,0x00,0x00,0x9d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0d,0x01,0x00,0x00,0x58,0x00,
		0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x63,0x00,0x00,0x9f,
		0xff,0x9f,0xff,0x00,0x00,0x00,0x00,0x74,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x74,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x74,0xff,0x00,0x00,
		0x00,0x00,0x00,0x00,0x74,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x74,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x74,0xff,0x00,0x00,0x00,0x00,0x00,
		0x00,0x74,0xff,0x00,0x00,0x00,0x00,0x00,0x00};
	TAF_MMA_CERSSI_INFO_QUERY_CNF_STRU *pstCerssiInfoQueryCnf = VOS_NULL_PTR;
	AT_MODEM_NET_CTX_STRU              *pstNetCtx = VOS_NULL_PTR;

    // 参数初始化
    pstNetCtx = AT_GetModemNetCtxAddrFromClientId(ucIndex);
    pstNetCtx->ucCerssiReportType       = 5;
    pstNetCtx->ucCerssiMinTimerInterval = 10;

    // 调用被测函数
	pstCerssiInfoQueryCnf = (TAF_MMA_CERSSI_INFO_QUERY_CNF_STRU *)aucMsg;
    At_MmaMsgProc((MSG_HEADER_STRU *)pstCerssiInfoQueryCnf);

    // 执行检查
 //   ASSERT_STREQ(aucRslt , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(AT_FW_CLIENT_STATUS_READY,g_stParseContext[ucIndex].ucClientStatus);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT,gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}
class Test_AT_RcvCcpuResetStartInd: public ::testing::Test
{
public:
    VOS_UINT8                           ucModemIndex;
    AT_MODEM_CC_CTX_STRU               *pstCcCtx;
    AT_PARSE_CONTEXT_STRU              *pstParseContext;

    void SetUp()
	{
		UT_STUB_INIT();
        for (ucModemIndex = 0; ucModemIndex < MODEM_ID_BUTT; ucModemIndex++)
        {
            pstCcCtx = AT_GetModemCcCtxAddrFromClientId(ucModemIndex);
            pstCcCtx->stS0TimeInfo.bTimerStart = VOS_TRUE;
        }

        pstParseContext = &(g_stParseContext[0]);
        pstParseContext->pucCmdLine = (VOS_UINT8 *)malloc(4);
    }
    void TearDown()
    {
        for (ucModemIndex = 0; ucModemIndex < MODEM_ID_BUTT; ucModemIndex++)
        {
            pstCcCtx = AT_GetModemCcCtxAddrFromClientId(ucModemIndex);
            pstCcCtx->stS0TimeInfo.bTimerStart = VOS_FALSE;
        }

        if (NULL != pstParseContext->pucCmdLine)
        {
            free(pstParseContext->pucCmdLine);
            pstParseContext->pucCmdLine = NULL;
        }
    }
};


TEST_F(Test_AT_RcvCcpuResetStartInd, Test_AT_RcvCcpuResetStartInd_001)
{
    VOS_UINT32                          ulResetingFlag;
    AT_MSG_STRU                         stResetInd;

    /* 参数初始化 */

    /* 构造ID_CCPU_RNIC_RESET_START_IND消息 */
    VOS_MemSet(&stResetInd, 0, sizeof(AT_MSG_STRU));
    stResetInd.ulSenderPid     = WUEPS_PID_AT;
    stResetInd.ulReceiverPid   = WUEPS_PID_AT;
    stResetInd.ucType          = ID_CCPU_AT_RESET_START_IND;

    /* 桩函数 */
	MOCKER(VOS_SmV)
		.expects(once())
		.will(returnValue((VOS_UINT32)VOS_OK));

    /* 函数调用 */

    /* 调用入口函数 */
    At_MsgProc((MsgBlock *)&stResetInd);

    //检查调用时, 需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
class Test_AT_RcvCcpuResetEndInd: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();
    }
    void TearDown()
    {
    }
};


TEST_F(Test_AT_RcvCcpuResetEndInd, Test_AT_RcvCcpuResetEndInd_001)
{
    VOS_UINT32                          ulResetingFlag;
    AT_MSG_STRU                         stResetInd;

    /* 参数初始化 */

    /* 构造ID_CCPU_RNIC_RESET_START_IND消息 */
    VOS_MemSet(&stResetInd, 0, sizeof(AT_MSG_STRU));
    stResetInd.ulSenderPid     = WUEPS_PID_AT;
    stResetInd.ulReceiverPid   = WUEPS_PID_AT;
    stResetInd.ucType          = ID_CCPU_AT_RESET_END_IND;

    /* 桩函数 */

    /* 函数调用 */

    /* 调用入口函数 */
    At_MsgProc((MsgBlock *)&stResetInd);

    /* 获取结果 */
    ulResetingFlag = AT_GetResetFlag();

    /* 参数匹配 */
    EXPECT_EQ(VOS_FALSE, ulResetingFlag);

    //检查调用时, 需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
class Test_AT_RcvHifiResetStartInd: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();
        g_stAtStatsInfo.stCCpuResetStatsInfo.ulHifiResetNum = 0;
    }
    void TearDown()
    {
        g_stAtStatsInfo.stCCpuResetStatsInfo.ulHifiResetNum = 0;
    }
};


TEST_F(Test_AT_RcvHifiResetStartInd, Test_AT_RcvHifiResetStartInd_001)
{
    VOS_UINT32                          ulResetingFlag;
    AT_MSG_STRU                         stResetInd;

    /* 参数初始化 */

    /* 构造ID_CCPU_RNIC_RESET_START_IND消息 */
    VOS_MemSet(&stResetInd, 0, sizeof(AT_MSG_STRU));
    stResetInd.ulSenderPid     = WUEPS_PID_AT;
    stResetInd.ulReceiverPid   = WUEPS_PID_AT;
    stResetInd.ucType          = ID_HIFI_AT_RESET_START_IND;

    /* 桩函数 */

    /* 函数调用 */

    /* 调用入口函数 */
    At_MsgProc((MsgBlock *)&stResetInd);

    /* 获取结果 */

    /* 参数匹配 */
    EXPECT_EQ(1, g_stAtStatsInfo.stCCpuResetStatsInfo.ulHifiResetNum);

    //检查调用时, 需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
class Test_AT_RcvTempprtStatusInd: public ::testing::Test
{
	public:
	TEMP_PROTECT_EVENT_AT_IND_STRU    	   *pstTempPrt;

    void SetUp()
	{
		UT_STUB_INIT();
        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pstTempPrt = (TEMP_PROTECT_EVENT_AT_IND_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(TEMP_PROTECT_EVENT_AT_IND_STRU));

        if (VOS_NULL_PTR == pstTempPrt)
        {
            return;
        }

        PS_MEM_SET(pstTempPrt, 0, sizeof(TEMP_PROTECT_EVENT_AT_IND_STRU));
		pstTempPrt->ulMsgId = ID_TEMPPRT_STATUS_AT_EVENT_IND;
    }

    void TearDown()
    {
        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstTempPrt)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstTempPrt);
        }
    }
};


TEST_F(Test_AT_RcvTempprtStatusInd, Test_AT_RcvTempprtStatusInd_001)
{
    // 参数初始化

    // MOCKER操作;
    MOCKER(NV_ReadEx)
        .stubs()
    	.will(returnValue(NV_ID_NOT_EXIST));

	MOCKER(At_SendResultData)
		.expects(never());

    // 调用被测函数
    AT_RcvTempprtStatusInd((VOS_VOID*)pstTempPrt);

    // 执行检查
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvTempprtStatusInd, Test_AT_RcvTempprtStatusInd_002)
{
    // 变量声明
    TAF_TEMP_PROTECT_CONFIG_STRU        stTempProtectPara;

    // 参数初始化
    PS_MEM_SET(&stTempProtectPara, 0, sizeof(stTempProtectPara));
    stTempProtectPara.ucSpyStatusIndSupport = AT_TEMPPRT_STATUS_IND_DISABLE;

    // 初始化全局变量;

    // MOCKER操作;
	MOCKER(At_SendResultData)
		.expects(never());

    // 调用被测函数
    AT_RcvTempprtStatusInd((VOS_VOID*)pstTempPrt);

    // 执行检查
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvTempprtStatusInd, Test_AT_RcvTempprtStatusInd_003)
{
    // 变量声明
    TAF_TEMP_PROTECT_CONFIG_STRU        stTempProtectPara;
    VOS_CHAR                           *pOutStr;

    // 参数初始化
    pOutStr = "\r\n^THERM: 1\r\n";

    PS_MEM_SET(&stTempProtectPara, 0, sizeof(stTempProtectPara));
    stTempProtectPara.ucSpyStatusIndSupport = AT_TEMPPRT_STATUS_IND_ENABLE;

	pstTempPrt->ulTempProtectEvent = SPY_TEMPROTECT_SWITCH_ON;

    // MOCKER操作;
    MOCKER(NV_ReadEx)
        .stubs()
        .with(eq((VOS_UINT16)MODEM_ID_0),eq((VOS_UINT16)en_NV_Item_TEMP_PROTECT_CONFIG),outBoundP((void *)&stTempProtectPara, (VOS_UINT32)sizeof(TAF_TEMP_PROTECT_CONFIG_STRU)),eq((VOS_UINT32)sizeof(TAF_TEMP_PROTECT_CONFIG_STRU)))
        .will(returnValue((VOS_UINT32)NV_OK));

    MOCKER(At_SendResultData)
        .expects(once());

    // 调用被测函数
    AT_RcvTempprtStatusInd((VOS_VOID*)pstTempPrt);

    // 执行检查
    EXPECT_EQ(gstAtSendData.usBufLen,13);

    GlobalMockObject::verify();
}


class Test_AT_SpyMsgProc: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }

    void TearDown()
    {

    }
};

#if ( FEATURE_ON == FEATURE_LTE )

TEST_F(Test_AT_SpyMsgProc, Test_AT_SpyMsgProc_001)
{
    // 变量声明
	TEMP_PROTECT_EVENT_AT_IND_STRU		stMsg;

    // 参数初始化
	PS_MEM_SET(&stMsg, 0, sizeof(stMsg));
	stMsg.ulMsgId =	ID_TEMPPRT_AT_EVENT_IND;

    // 调用被测函数
    AT_SpyMsgProc((VOS_VOID*)&stMsg);

    // 执行检查
    GlobalMockObject::verify();
}
#endif


TEST_F(Test_AT_SpyMsgProc, Test_AT_SpyMsgProc_002)
{
    // 变量声明
	TEMP_PROTECT_EVENT_AT_IND_STRU		stMsg;

    // 参数初始化
	PS_MEM_SET(&stMsg, 0, sizeof(stMsg));
	stMsg.ulMsgId =	ID_TEMPPRT_AT_MSG_ID_BUTT;

    // MOCKER操作;

    // 调用被测函数
    AT_SpyMsgProc((VOS_VOID*)&stMsg);

    // 执行检查
    GlobalMockObject::verify();
}


TEST_F(Test_AT_SpyMsgProc, Test_AT_SpyMsgProc_003)
{
    // 变量声明
	TEMP_PROTECT_EVENT_AT_IND_STRU		stMsg;
    TAF_TEMP_PROTECT_CONFIG_STRU        stTempProtectPara;

    // 参数初始化
    PS_MEM_SET(&stTempProtectPara, 0, sizeof(stTempProtectPara));
    stTempProtectPara.ucSpyStatusIndSupport = AT_TEMPPRT_STATUS_IND_ENABLE;

	PS_MEM_SET(&stMsg, 0, sizeof(stMsg));
	stMsg.ulMsgId =	ID_TEMPPRT_STATUS_AT_EVENT_IND;
	stMsg.ulTempProtectEvent = SPY_TEMPROTECT_SWITCH_ON;

    // MOCKER操作;
    MOCKER(NV_ReadEx)
        .stubs()
        .with(eq((VOS_UINT16)MODEM_ID_0),eq((VOS_UINT16)en_NV_Item_TEMP_PROTECT_CONFIG),outBoundP((void *)&stTempProtectPara, (VOS_UINT32)sizeof(TAF_TEMP_PROTECT_CONFIG_STRU)),eq((VOS_UINT32)sizeof(TAF_TEMP_PROTECT_CONFIG_STRU)))
        .will(returnValue((VOS_UINT32)NV_OK));

	MOCKER(At_SendResultData)
		.expects(exactly(1));

    // 调用被测函数
    AT_SpyMsgProc((VOS_VOID*)&stMsg);

    // 执行检查
    GlobalMockObject::verify();
}


class Test_AT_RcvMtaImeiVerifyQryCnf: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;
    AT_MTA_MSG_STRU                    *pstMsg;
    VOS_UINT32                          ulImeiVerify;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_ID_APP;

        stCmdInfo.ulCmdIndex        = AT_CMD_IMEIVERIFY;
        stCmdInfo.pfnSetProc        = VOS_NULL_PTR;
        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = At_QryIMEIVerifyPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^IMEIVERIFY";
        stCmdInfo.pszParam          = VOS_NULL_PTR;

        pstMsg      = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstMsg, 0, sizeof(AT_MTA_MSG_STRU));

        pstMsg->ulReceiverPid           = WUEPS_PID_AT;
        pstMsg->ulSenderPid             = UEPS_PID_MTA;
        pstMsg->ulMsgId                 = ID_MTA_AT_IMEI_VERIFY_QRY_CNF;

        pstMsg->stAppCtrl.usClientId    = ucIndex;
        pstMsg->stAppCtrl.ucOpId        = 1;
        ulImeiVerify                    = VOS_TRUE;
        PS_MEM_CPY(pstMsg->aucContent, (VOS_UINT8 *)&ulImeiVerify, sizeof(VOS_UINT32));

        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_IMEI_VERIFY_READ;
        gastAtClientTab[ucIndex].usClientId     = ucIndex;
        gastAtClientTab[ucIndex].ucUsed         = AT_CLIENT_USED;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

        gstAtSendData.usBufLen = 0;
    }

    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMsg);
        }

        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;
        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
    }
};
TEST_F(Test_AT_RcvMtaImeiVerifyQryCnf, Test_AT_RcvMtaImeiVerifyQryCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRelt;

    //参数初始化

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    // 调用被测函数
    ulRelt = AT_RcvMtaImeiVerifyQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(gstAtSendData.usBufLen, VOS_StrLen("^IMEIVERIFY: 1"));
    EXPECT_STREQ("^IMEIVERIFY: 1", (char *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaImeiVerifyQryCnf, Test_AT_RcvMtaImeiVerifyQryCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRelt;

    //参数初始化


    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRelt = AT_RcvMtaImeiVerifyQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(ulRelt, VOS_ERR);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaImeiVerifyQryCnf, Test_AT_RcvMtaImeiVerifyQryCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRelt;
    VOS_UINT8                           ucIndexStub;

    //参数初始化
    ucIndexStub = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndexStub,(VOS_UINT32)sizeof(ucIndexStub)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRelt = AT_RcvMtaImeiVerifyQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(ulRelt, VOS_ERR);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaImeiVerifyQryCnf, Test_AT_RcvMtaImeiVerifyQryCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRelt;

    //参数初始化

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    // MOCKER操作


    // 调用被测函数
    ulRelt = AT_RcvMtaImeiVerifyQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(ulRelt, VOS_ERR);
    GlobalMockObject::verify();
}


class Test_AT_RcvMtaCgsnQryCnf: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;
    AT_MTA_MSG_STRU                    *pstMsg;
    VOS_UINT32                          ulImeiVerify;
    VOS_UINT8                           aucImei[TAF_PH_IMEI_LEN];

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_ID_APP;

        stCmdInfo.ulCmdIndex        = AT_CMD_CGSN;
        stCmdInfo.pfnSetProc        = At_SetCgsnPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnTestProc       = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"+CGSN";
        stCmdInfo.pszParam          = VOS_NULL_PTR;

        pstMsg      = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, (sizeof(AT_MTA_MSG_STRU) + TAF_PH_IMEI_LEN - 4));
        PS_MEM_SET(pstMsg, 0, sizeof(AT_MTA_MSG_STRU));

        pstMsg->ulReceiverPid           = WUEPS_PID_AT;
        pstMsg->ulSenderPid             = UEPS_PID_MTA;
        pstMsg->ulMsgId                 = ID_MTA_AT_CGSN_QRY_CNF;

        pstMsg->stAppCtrl.usClientId    = ucIndex;
        pstMsg->stAppCtrl.ucOpId        = 1;
        PS_MEM_CPY(aucImei, "012345000000005", TAF_PH_IMEI_LEN);

        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CGSN_READ;
        gastAtClientTab[ucIndex].usClientId     = ucIndex;
        gastAtClientTab[ucIndex].ucUsed         = AT_CLIENT_USED;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

        gstAtSendData.usBufLen = 0;
    }

    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMsg);
        }

        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;
        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
    }
};
TEST_F(Test_AT_RcvMtaCgsnQryCnf, Test_AT_RcvMtaCgsnQryCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRelt;
    VOS_UINT8                           aucImeiBcd[TAF_PH_IMEI_LEN] = {0x00,0x01,0x02,0x03,0x04,0x05};

    //参数初始化
    PS_MEM_CPY(pstMsg->aucContent, aucImeiBcd, TAF_PH_IMEI_LEN);

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    // 调用被测函数
    ulRelt = AT_RcvMtaCgsnQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(gstAtSendData.usBufLen, (TAF_PH_IMEI_LEN - 1));
    EXPECT_STREQ((char *)aucImei, (char *)pgucAtSndCodeAddr);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaCgsnQryCnf, Test_AT_RcvMtaCgsnQryCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRelt;

    //参数初始化


    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRelt = AT_RcvMtaCgsnQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(ulRelt, VOS_ERR);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaCgsnQryCnf, Test_AT_RcvMtaCgsnQryCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRelt;
    VOS_UINT8                           ucIndexStub;

    //参数初始化
    ucIndexStub = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(any(), outBoundP(&ucIndexStub,(VOS_UINT32)sizeof(ucIndexStub)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRelt = AT_RcvMtaCgsnQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(ulRelt, VOS_ERR);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaCgsnQryCnf, Test_AT_RcvMtaCgsnQryCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRelt;

    //参数初始化

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    // MOCKER操作


    // 调用被测函数
    ulRelt = AT_RcvMtaCgsnQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(ulRelt, VOS_ERR);
    GlobalMockObject::verify();
}



class Test_AT_RcvMtaSetNCellMonitorCnf: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stElement;
    VOS_UINT8                           szCmdName[32];
    MTA_AT_RESULT_CNF_STRU             *pstNCellMonitorSetCnf;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 1;

        VOS_StrCpy((VOS_CHAR*)szCmdName,"NCELLMONITOR");
        g_stParseContext[ucIndex].pstCmdElement = &stElement;
        stElement.pszCmdName                    = szCmdName;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
        PS_MEM_SET(gstAtCombineSendData, 0, (sizeof(AT_SEND_DATA_BUFFER_STRU) * AT_MAX_CLIENT_NUM));
    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
        free(pstNCellMonitorSetCnf);
    }
};


TEST_F(Test_AT_RcvMtaSetNCellMonitorCnf, Test_AT_RcvMtaSetNCellMonitorCnf_001)
{
    VOS_UINT32                          ulResult;
    AT_MTA_MSG_STRU                     stMsg;

    /* 构造入口消息 */
    pstNCellMonitorSetCnf = (MTA_AT_RESULT_CNF_STRU *)malloc(sizeof(MTA_AT_RESULT_CNF_STRU));
    PS_MEM_SET(pstNCellMonitorSetCnf, 0, sizeof(MTA_AT_RESULT_CNF_STRU));

    PS_MEM_SET(&stMsg, 0, sizeof(stMsg));
    stMsg.ulMsgId = ID_MTA_AT_NCELL_MONITOR_SET_CNF;
    stMsg.stAppCtrl.usClientId = 0;
    PS_MEM_CPY(&stMsg.aucContent, pstNCellMonitorSetCnf, sizeof(MTA_AT_RESULT_CNF_STRU));

    /* 打桩函数 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /* 调用测试函数 */
    ulResult = AT_RcvMtaSetNCellMonitorCnf((VOS_VOID *)&stMsg);

    /* 检查 */
    EXPECT_EQ(VOS_ERR, ulResult);

    // 执行检查
    GlobalMockObject::verify();

}


TEST_F(Test_AT_RcvMtaSetNCellMonitorCnf, Test_AT_RcvMtaSetNCellMonitorCnf_002)
{
    VOS_UINT32                          ulResult;
    AT_MTA_MSG_STRU                     stMsg;

    /* 构造入口消息 */
    pstNCellMonitorSetCnf = (MTA_AT_RESULT_CNF_STRU *)malloc(sizeof(MTA_AT_RESULT_CNF_STRU));
    PS_MEM_SET(pstNCellMonitorSetCnf, 0, sizeof(MTA_AT_RESULT_CNF_STRU));

    PS_MEM_SET(&stMsg, 0, sizeof(stMsg));
    stMsg.ulMsgId = ID_MTA_AT_NCELL_MONITOR_SET_CNF;
    stMsg.stAppCtrl.usClientId = AT_BROADCAST_CLIENT_ID_MODEM_0;
    PS_MEM_CPY(&stMsg.aucContent, pstNCellMonitorSetCnf, sizeof(MTA_AT_RESULT_CNF_STRU));

    /* 打桩函数 */

    /* 调用测试函数 */
    ulResult = AT_RcvMtaSetNCellMonitorCnf((VOS_VOID *)&stMsg);

    /* 检查 */
    EXPECT_EQ(VOS_ERR, ulResult);

    // 执行检查
    GlobalMockObject::verify();

}


TEST_F(Test_AT_RcvMtaSetNCellMonitorCnf, Test_AT_RcvMtaSetNCellMonitorCnf_003)
{
    VOS_UINT32                          ulResult;
    AT_MTA_MSG_STRU                     stMsg;

    /* 构造入口消息 */
    pstNCellMonitorSetCnf = (MTA_AT_RESULT_CNF_STRU *)malloc(sizeof(MTA_AT_RESULT_CNF_STRU));
    PS_MEM_SET(pstNCellMonitorSetCnf, 0, sizeof(MTA_AT_RESULT_CNF_STRU));

    PS_MEM_SET(&stMsg, 0, sizeof(stMsg));
    stMsg.ulMsgId = ID_MTA_AT_NCELL_MONITOR_SET_CNF;
    gastAtClientTab[0].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[0].usClientId = 1;
    stMsg.stAppCtrl.usClientId = 1;
    PS_MEM_CPY(&stMsg.aucContent, pstNCellMonitorSetCnf, sizeof(MTA_AT_RESULT_CNF_STRU));

    /* 全局变量 */
    gastAtClientTab[1].CmdCurrentOpt = AT_CMD_NCELL_MONITOR_READ;

    /* 打桩函数 */

    /* 调用测试函数 */
    ulResult = AT_RcvMtaSetNCellMonitorCnf((VOS_VOID *)&stMsg);

    /* 检查 */
    EXPECT_EQ(VOS_ERR, ulResult);

    // 执行检查
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvMtaSetNCellMonitorCnf, Test_AT_RcvMtaSetNCellMonitorCnf_004)
{
    VOS_UINT32                          ulResult;
    AT_MTA_MSG_STRU                     stMsg;

    /* 构造入口消息 */
    pstNCellMonitorSetCnf = (MTA_AT_RESULT_CNF_STRU *)malloc(sizeof(MTA_AT_RESULT_CNF_STRU));
    PS_MEM_SET(pstNCellMonitorSetCnf, 0, sizeof(MTA_AT_RESULT_CNF_STRU));
    pstNCellMonitorSetCnf->enResult = MTA_AT_RESULT_NO_ERROR;

    PS_MEM_SET(&stMsg, 0, sizeof(stMsg));
    stMsg.ulMsgId = ID_MTA_AT_NCELL_MONITOR_SET_CNF;
    stMsg.stAppCtrl.usClientId = 0;
    PS_MEM_CPY(&stMsg.aucContent, pstNCellMonitorSetCnf, sizeof(MTA_AT_RESULT_CNF_STRU));

    /* 全局变量 */
    gastAtClientTab[0].CmdCurrentOpt = AT_CMD_NCELL_MONITOR_SET;
    gastAtClientTab[0].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[0].usClientId = 0;

    /* 打桩函数 */
    MOCKER(At_FormatResultData)
        .expects(once());

    /* 调用测试函数 */
    ulResult = AT_RcvMtaSetNCellMonitorCnf((VOS_VOID *)&stMsg);

    /* 检查 */
    EXPECT_EQ(VOS_OK, ulResult);

    // 执行检查
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvMtaSetNCellMonitorCnf, Test_AT_RcvMtaSetNCellMonitorCnf_005)
{
    VOS_UINT32                          ulResult;
    AT_MTA_MSG_STRU                     stMsg;
    VOS_UINT8                           ucIndex;

    ucIndex = 0;

    /* 构造入口消息 */
    pstNCellMonitorSetCnf = (MTA_AT_RESULT_CNF_STRU *)malloc(sizeof(MTA_AT_RESULT_CNF_STRU));
    PS_MEM_SET(pstNCellMonitorSetCnf, 0, sizeof(MTA_AT_RESULT_CNF_STRU));
    pstNCellMonitorSetCnf->enResult = MTA_AT_RESULT_ERROR;

    PS_MEM_SET(&stMsg, 0, sizeof(stMsg));
    stMsg.ulMsgId = ID_MTA_AT_NCELL_MONITOR_SET_CNF;
    stMsg.stAppCtrl.usClientId = 0;
    PS_MEM_CPY(&stMsg.aucContent, pstNCellMonitorSetCnf, sizeof(MTA_AT_RESULT_CNF_STRU));

    /* 全局变量 */
    gastAtClientTab[0].CmdCurrentOpt = AT_CMD_NCELL_MONITOR_SET;
    gastAtClientTab[0].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[0].usClientId = 0;

    /* 打桩函数 */
    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_ERROR));

    /* 调用测试函数 */
    ulResult = AT_RcvMtaSetNCellMonitorCnf((VOS_VOID *)&stMsg);

    /* 检查 */
    EXPECT_EQ(VOS_OK, ulResult);

    // 执行检查
    GlobalMockObject::verify();

}
class Test_AT_RcvMtaQryNCellMonitorCnf: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stElement;
    VOS_UINT8                           szCmdName[32];
    MTA_AT_NCELL_MONITOR_QRY_CNF_STRU  *pstQryCnf;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        VOS_StrCpy((VOS_CHAR*)szCmdName,"NCELLMONITOR");
        g_stParseContext[ucIndex].pstCmdElement = &stElement;
        stElement.pszCmdName                    = szCmdName;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
        PS_MEM_SET(gstAtCombineSendData, 0, (sizeof(AT_SEND_DATA_BUFFER_STRU) * AT_MAX_CLIENT_NUM));
    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
        free(pstQryCnf);
    }
};


TEST_F(Test_AT_RcvMtaQryNCellMonitorCnf, Test_AT_RcvMtaQryNCellMonitorCnf_001)
{
    VOS_UINT32                          ulResult;
    AT_MTA_MSG_STRU                    *pstMsg;
    VOS_UINT32                          ulMsgSize;

    /* 构造入口消息 */
    pstQryCnf = (MTA_AT_NCELL_MONITOR_QRY_CNF_STRU *)malloc(sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));
    PS_MEM_SET(pstQryCnf, 0, sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));

    ulMsgSize = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU);
    pstMsg = (AT_MTA_MSG_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstMsg, 0, ulMsgSize);
    pstMsg->ulMsgId = ID_MTA_AT_NCELL_MONITOR_QRY_CNF;
    pstMsg->stAppCtrl.usClientId = 0;
    PS_MEM_CPY(pstMsg->aucContent, pstQryCnf, sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));

    /* 打桩函数 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /* 调用测试函数 */
    ulResult = AT_RcvMtaQryNCellMonitorCnf((VOS_VOID *)pstMsg);

    /* 检查 */
    EXPECT_EQ(VOS_ERR, ulResult);

    // 执行检查
    GlobalMockObject::verify();

}


TEST_F(Test_AT_RcvMtaQryNCellMonitorCnf, Test_AT_RcvMtaQryNCellMonitorCnf_002)
{
    VOS_UINT32                          ulResult;
    AT_MTA_MSG_STRU                    *pstMsg;
    VOS_UINT32                          ulMsgSize;

    /* 构造入口消息 */
    pstQryCnf = (MTA_AT_NCELL_MONITOR_QRY_CNF_STRU *)malloc(sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));
    PS_MEM_SET(pstQryCnf, 0, sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));

    ulMsgSize = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU);
    pstMsg = (AT_MTA_MSG_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstMsg, 0, ulMsgSize);
    pstMsg->ulMsgId = ID_MTA_AT_NCELL_MONITOR_QRY_CNF;
    pstMsg->stAppCtrl.usClientId = AT_BROADCAST_CLIENT_ID_MODEM_1;
    PS_MEM_CPY(pstMsg->aucContent, pstQryCnf, sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));

    /* 打桩函数 */

    /* 调用测试函数 */
    ulResult = AT_RcvMtaQryNCellMonitorCnf((VOS_VOID *)pstMsg);

    /* 检查 */
    EXPECT_EQ(VOS_ERR, ulResult);

    // 执行检查
    GlobalMockObject::verify();

}


TEST_F(Test_AT_RcvMtaQryNCellMonitorCnf, Test_AT_RcvMtaQryNCellMonitorCnf_003)
{
    VOS_UINT32                          ulResult;
    AT_MTA_MSG_STRU                    *pstMsg;
    VOS_UINT32                          ulMsgSize;

    /* 构造入口消息 */
    pstQryCnf = (MTA_AT_NCELL_MONITOR_QRY_CNF_STRU *)malloc(sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));
    PS_MEM_SET(pstQryCnf, 0, sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));

    ulMsgSize = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU);
    pstMsg = (AT_MTA_MSG_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstMsg, 0, ulMsgSize);
    pstMsg->ulMsgId = ID_MTA_AT_NCELL_MONITOR_QRY_CNF;
    pstMsg->stAppCtrl.usClientId = 0;
    PS_MEM_CPY(pstMsg->aucContent, pstQryCnf, sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));

    /* 全局变量 */
    gastAtClientTab[1].CmdCurrentOpt = AT_CMD_NCELL_MONITOR_SET;
    gastAtClientTab[1].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[1].usClientId = 0;

    /* 打桩函数 */

    /* 调用测试函数 */
    ulResult = AT_RcvMtaQryNCellMonitorCnf((VOS_VOID *)pstMsg);

    /* 检查 */
    EXPECT_EQ(VOS_ERR, ulResult);

    // 执行检查
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvMtaQryNCellMonitorCnf, Test_AT_RcvMtaQryNCellMonitorCnf_004)
{
    VOS_UINT32                          ulResult;
    AT_MTA_MSG_STRU                    *pstMsg;
    VOS_UINT32                          ulMsgSize;
    VOS_CHAR                           *pcPrintStr = "NCELLMONITOR: 1,0\r\n\r\nOK\r\n";

    /* 构造入口消息 */
    pstQryCnf = (MTA_AT_NCELL_MONITOR_QRY_CNF_STRU *)malloc(sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));
    PS_MEM_SET(pstQryCnf, 0, sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));
    pstQryCnf->enResult = MTA_AT_RESULT_NO_ERROR;
    pstQryCnf->ucSwitch = VOS_TRUE;
    pstQryCnf->ucNcellState = 0;

    ulMsgSize = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU);
    pstMsg = (AT_MTA_MSG_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstMsg, 0, ulMsgSize);
    pstMsg->ulMsgId = ID_MTA_AT_NCELL_MONITOR_QRY_CNF;
    pstMsg->stAppCtrl.usClientId = 0;
    PS_MEM_CPY(pstMsg->aucContent, pstQryCnf, sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));

    /* 全局变量 */
    gastAtClientTab[0].CmdCurrentOpt = AT_CMD_NCELL_MONITOR_READ;
    gastAtClientTab[0].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[0].usClientId = 0;

    /* 打桩函数 */

    /* 调用测试函数 */
    ulResult = AT_RcvMtaQryNCellMonitorCnf((VOS_VOID *)pstMsg);

    /* 检查 */
    EXPECT_EQ(VOS_OK, ulResult);

    // 执行检查
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvMtaQryNCellMonitorCnf, Test_AT_RcvMtaQryNCellMonitorCnf_005)
{
    VOS_UINT32                          ulResult;
    AT_MTA_MSG_STRU                    *pstMsg;
    VOS_UINT32                          ulMsgSize;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulReturnCode;

    ucIndex         = 0;
    ulReturnCode    = AT_CME_UNKNOWN;

    /* 构造入口消息 */
    pstQryCnf = (MTA_AT_NCELL_MONITOR_QRY_CNF_STRU *)malloc(sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));
    PS_MEM_SET(pstQryCnf, 0, sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));
    pstQryCnf->enResult = MTA_AT_RESULT_ERROR;

    ulMsgSize = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU);
    pstMsg = (AT_MTA_MSG_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstMsg, 0, ulMsgSize);
    pstMsg->ulMsgId = ID_MTA_AT_NCELL_MONITOR_QRY_CNF;
    pstMsg->stAppCtrl.usClientId = 0;
    PS_MEM_CPY(pstMsg->aucContent, pstQryCnf, sizeof(MTA_AT_NCELL_MONITOR_QRY_CNF_STRU));

    /* 全局变量 */
    gastAtClientTab[0].CmdCurrentOpt = AT_CMD_NCELL_MONITOR_READ;
    gastAtClientTab[0].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[0].usClientId = 0;

    /* 打桩函数 */
    MOCKER(At_FormatResultData)
        .stubs()
        .with(eq((VOS_UINT8)ucIndex), eq(ulReturnCode));

    /* 调用测试函数 */
    ulResult = AT_RcvMtaQryNCellMonitorCnf((VOS_VOID *)pstMsg);

    /* 检查 */
    EXPECT_EQ(VOS_OK, ulResult);

    // 执行检查
    GlobalMockObject::verify();

}
class Test_AT_RcvMtaNCellMonitorInd: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stElement;
    VOS_UINT8                           szCmdName[32];
    MTA_AT_NCELL_MONITOR_IND_STRU      *pstRcvMsg;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        VOS_StrCpy((VOS_CHAR*)szCmdName,"NCELLMONITOR");
        g_stParseContext[ucIndex].pstCmdElement = &stElement;
        stElement.pszCmdName                    = szCmdName;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
        PS_MEM_SET(gstAtCombineSendData, 0, (sizeof(AT_SEND_DATA_BUFFER_STRU) * AT_MAX_CLIENT_NUM));
    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
        free(pstRcvMsg);
    }
};


TEST_F(Test_AT_RcvMtaNCellMonitorInd, Test_AT_RcvMtaNCellMonitorInd_001)
{
    VOS_UINT32                          ulResult;
    AT_MTA_MSG_STRU                    *pstMsg;
    VOS_UINT32                          ulMsgSize;

    /* 构造入口消息 */
    pstRcvMsg = (MTA_AT_NCELL_MONITOR_IND_STRU *)malloc(sizeof(MTA_AT_NCELL_MONITOR_IND_STRU));
    PS_MEM_SET(pstRcvMsg, 0, sizeof(MTA_AT_NCELL_MONITOR_IND_STRU));

    ulMsgSize = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_NCELL_MONITOR_IND_STRU);
    pstMsg = (AT_MTA_MSG_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstMsg, 0, ulMsgSize);
    pstMsg->ulMsgId = ID_MTA_AT_NCELL_MONITOR_IND;
    pstMsg->stAppCtrl.usClientId = (AT_BROADCAST_CLIENT_ID_MODEM_1 & MTA_CLIENTID_BROADCAST);
    pstMsg->stAppCtrl.ucOpId = MTA_INVALID_TAB_INDEX;
    PS_MEM_CPY(pstMsg->aucContent, pstRcvMsg, sizeof(MTA_AT_NCELL_MONITOR_IND_STRU));

    /* 打桩函数 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    /* 调用测试函数 */
    ulResult = AT_RcvMtaNCellMonitorInd((VOS_VOID *)pstMsg);

    /* 检查 */
    EXPECT_EQ(VOS_ERR, ulResult);

    // 执行检查
    GlobalMockObject::verify();

}


TEST_F(Test_AT_RcvMtaNCellMonitorInd, Test_AT_RcvMtaNCellMonitorInd_004)
{
    VOS_UINT32                          ulResult;
    AT_MTA_MSG_STRU                    *pstMsg;
    VOS_UINT32                          ulMsgSize;
    VOS_CHAR                           *pcPrintStr = "\r\n^NCELLMONITOR: 2\r\n";

    /* 构造入口消息 */
    pstRcvMsg = (MTA_AT_NCELL_MONITOR_IND_STRU *)malloc(sizeof(MTA_AT_NCELL_MONITOR_IND_STRU));
    PS_MEM_SET(pstRcvMsg, 0, sizeof(MTA_AT_NCELL_MONITOR_IND_STRU));
    pstRcvMsg->ucNcellState = 2;

    ulMsgSize = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_NCELL_MONITOR_IND_STRU);
    pstMsg = (AT_MTA_MSG_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstMsg, 0, ulMsgSize);
    pstMsg->ulMsgId = ID_MTA_AT_NCELL_MONITOR_IND;
    pstMsg->stAppCtrl.usClientId = (AT_BROADCAST_CLIENT_ID_MODEM_0 & MTA_CLIENTID_BROADCAST);
    pstMsg->stAppCtrl.ucOpId = MTA_INVALID_TAB_INDEX;
    PS_MEM_CPY(pstMsg->aucContent, pstRcvMsg, sizeof(MTA_AT_NCELL_MONITOR_IND_STRU));

    /* 全局变量 */

    /* 打桩函数 */

    /* 调用测试函数 */
    ulResult = AT_RcvMtaNCellMonitorInd((VOS_VOID *)pstMsg);

    /* 检查 */
    ASSERT_STREQ(pcPrintStr, (VOS_CHAR *)pgucAtSndCodeAddr);
    EXPECT_EQ(VOS_OK, ulResult);

    // 执行检查
    GlobalMockObject::verify();

}
class Test_AT_RcvHifiResetEndInd: public ::testing::Test
{
    public:

    void SetUp()
	{
		UT_STUB_INIT();
    }
    void TearDown()
    {
    }
};


TEST_F(Test_AT_RcvHifiResetEndInd, Test_AT_RcvHifiResetEndInd_001)
{
    // 变量声明
    VOS_UINT32                         ulRelt;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作
    MOCKER(AT_ReportResetCmd)
        .expects(exactly(1));

    // 调用被测函数
    ulRelt = AT_RcvHifiResetEndInd(NULL);

    // 执行检查
    EXPECT_EQ(ulRelt, VOS_OK);
    GlobalMockObject::verify();
}



class Test_AT_ProcRabmReleaseRrcCnf: public ::testing::Test
{
public:
    void SetUp()
	{
		UT_STUB_INIT();

    }
    void TearDown()
    {

    }
};


TEST_F(Test_AT_ProcRabmReleaseRrcCnf, Test_AT_ProcRabmReleaseRrcCnf_001)
{
    RABM_AT_RELEASE_RRC_CNF_STRU stMsg;

    stMsg.stMsgHeader.ulMsgName = ID_RABM_AT_SET_RELEASE_RRC_CNF;
    stMsg.stMsgHeader.ulSenderPid = I0_WUEPS_PID_RABM;

    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));
    MOCKER(AT_StopRelTimer)
        .expects(never());

    At_MsgProc((MsgBlock*)&stMsg);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_ProcRabmReleaseRrcCnf, Test_AT_ProcRabmReleaseRrcCnf_002)
{
    RABM_AT_RELEASE_RRC_CNF_STRU stMsg;

	gastAtClientTab[0].usClientId = 1;
	stMsg.usClientId =1;
    stMsg.stMsgHeader.ulMsgName = ID_RABM_AT_SET_RELEASE_RRC_CNF;
    stMsg.stMsgHeader.ulSenderPid = I0_WUEPS_PID_RABM;

    stMsg.ulRslt = VOS_TRUE;

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    At_MsgProc((MsgBlock*)&stMsg);

    GlobalMockObject::verify();
}




class Test_AT_RcvMtaRefclkfreqSetCnf: public ::testing::Test
{
public:
    VOS_UINT8                                   ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                     stCmdInfo;
    AT_MTA_MSG_STRU                            *pstMsg;
    MTA_AT_RESULT_CNF_STRU                     *pstRltCnf;
    VOS_UINT32                                  ulMsgLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_APP_INDEX;

        stCmdInfo.ulCmdIndex        = AT_CMD_REFCLKFREQ;
        stCmdInfo.pfnSetProc        = AT_SetRefclkfreqPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = AT_QryRefclkfreqPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = AT_TestRefclkfreqPara;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnAbortProc      = VOS_NULL_PTR;
        stCmdInfo.ulAbortTimeOut    = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_ERROR;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^REFCLKFREQ";
        stCmdInfo.pszParam          = (VOS_UINT8 *)"(0,1)";

        ulMsgLen    = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_RESULT_CNF_STRU);
        pstMsg      = (AT_MTA_MSG_STRU *)malloc(ulMsgLen);
        pstRltCnf   = (MTA_AT_RESULT_CNF_STRU *)pstMsg->aucContent;

        PS_MEM_SET(pstRltCnf, 0x00, sizeof(MTA_AT_RESULT_CNF_STRU));
        pstMsg->stAppCtrl.usClientId  = AT_CLIENT_ID_APP;
        pstRltCnf->enResult         = MTA_AT_RESULT_NO_ERROR;

        AT_ResetClientTab();
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_REFCLKFREQ_SET;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
    }
    void TearDown()
    {
        free(pstMsg);
    }
};


TEST_F(Test_AT_RcvMtaRefclkfreqSetCnf, Test_AT_RcvMtaRefclkfreqSetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRet = AT_RcvMtaRefclkfreqSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaRefclkfreqSetCnf, Test_AT_RcvMtaRefclkfreqSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(eq(pstMsg->stAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRet = AT_RcvMtaRefclkfreqSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaRefclkfreqSetCnf, Test_AT_RcvMtaRefclkfreqSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_REFCLKFREQ_READ;

    // MOCKER操作

    // 调用被测函数
    ulRet = AT_RcvMtaRefclkfreqSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaRefclkfreqSetCnf, Test_AT_RcvMtaRefclkfreqSetCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvMtaRefclkfreqSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaRefclkfreqSetCnf, Test_AT_RcvMtaRefclkfreqSetCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstRltCnf->enResult         = MTA_AT_RESULT_ERROR;

    // 初始化全局变量

    // MOCKER操作

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数
    ulRet = AT_RcvMtaRefclkfreqSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}


class Test_AT_RcvMtaRefclkfreqQryCnf: public ::testing::Test
{
public:
    VOS_UINT8                                   ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                     stCmdInfo;
    AT_MTA_MSG_STRU                            *pstMsg;
    MTA_AT_REFCLKFREQ_QRY_CNF_STRU             *pstRefclkfreqCnf;
    VOS_UINT32                                  ulMsgLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_APP_INDEX;

        stCmdInfo.ulCmdIndex        = AT_CMD_REFCLKFREQ;
        stCmdInfo.pfnSetProc        = AT_SetRefclkfreqPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = AT_QryRefclkfreqPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = AT_TestRefclkfreqPara;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnAbortProc      = VOS_NULL_PTR;
        stCmdInfo.ulAbortTimeOut    = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_ERROR;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^REFCLKFREQ";
        stCmdInfo.pszParam          = (VOS_UINT8 *)"(0,1)";

        ulMsgLen            = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_REFCLKFREQ_QRY_CNF_STRU);
        pstMsg              = (AT_MTA_MSG_STRU *)malloc(ulMsgLen);
        pstRefclkfreqCnf    = (MTA_AT_REFCLKFREQ_QRY_CNF_STRU *)pstMsg->aucContent;

        PS_MEM_SET(pstRefclkfreqCnf, 0x00, sizeof(MTA_AT_REFCLKFREQ_QRY_CNF_STRU));
        pstMsg->stAppCtrl.usClientId    = AT_CLIENT_ID_APP;
        pstRefclkfreqCnf->enResult      = MTA_AT_RESULT_NO_ERROR;
        pstRefclkfreqCnf->ulFreq        = 19200000;
        pstRefclkfreqCnf->ulPrecision   = 100;
        pstRefclkfreqCnf->enStatus      = PHY_MTA_REFCLOCK_LOCKED;

        AT_ResetClientTab();
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_REFCLKFREQ_READ;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
    }
    void TearDown()
    {
        free(pstMsg);
    }
};


TEST_F(Test_AT_RcvMtaRefclkfreqQryCnf, Test_AT_RcvMtaRefclkfreqQryCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRet = AT_RcvMtaRefclkfreqQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaRefclkfreqQryCnf, Test_AT_RcvMtaRefclkfreqQryCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(eq(pstMsg->stAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

    // 调用被测函数
    ulRet = AT_RcvMtaRefclkfreqQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaRefclkfreqQryCnf, Test_AT_RcvMtaRefclkfreqQryCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_REFCLKFREQ_SET;

    // MOCKER操作

    // 调用被测函数
    ulRet = AT_RcvMtaRefclkfreqQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaRefclkfreqQryCnf, Test_AT_RcvMtaRefclkfreqQryCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_CHAR                           *pcExpectStr = VOS_NULL_PTR;

    // 参数初始化
    pcExpectStr = "^REFCLKFREQ: 0,19200000,100,1";

    // 初始化全局变量

    // MOCKER操作

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    ulRet = AT_RcvMtaRefclkfreqQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_STREQ((VOS_CHAR *)pgucAtSndCodeAddr, pcExpectStr);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaRefclkfreqQryCnf, Test_AT_RcvMtaRefclkfreqQryCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_CHAR                           *pcExpectStr = VOS_NULL_PTR;

    // 参数初始化
    pstRefclkfreqCnf->enResult      = MTA_AT_RESULT_ERROR;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数
    ulRet = AT_RcvMtaRefclkfreqQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}


class Test_AT_RcvMtaRefclkfreqInd: public ::testing::Test
{
public:
    VOS_UINT8                                   ucIndex;
    AT_MTA_MSG_STRU                            *pstMsg;
    MTA_AT_REFCLKFREQ_IND_STRU                 *pstRefclkfreqInd;
    VOS_UINT32                                  ulMsgLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex             = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

        ulMsgLen            = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_REFCLKFREQ_IND_STRU);
        pstMsg              = (AT_MTA_MSG_STRU *)malloc(ulMsgLen);
        pstRefclkfreqInd    = (MTA_AT_REFCLKFREQ_IND_STRU *)pstMsg->aucContent;

        PS_MEM_SET(pstRefclkfreqInd, 0x00, sizeof(MTA_AT_REFCLKFREQ_IND_STRU));
        pstMsg->stAppCtrl.usClientId    = MTA_CLIENTID_BROADCAST & AT_BROADCAST_CLIENT_ID_MODEM_0;
        pstRefclkfreqInd->ulFreq        = 19200000;
        pstRefclkfreqInd->ulPrecision   = 100;
        pstRefclkfreqInd->enStatus      = PHY_MTA_REFCLOCK_LOCKED;

    }
    void TearDown()
    {
        free(pstMsg);
    }
};


TEST_F(Test_AT_RcvMtaRefclkfreqInd, Test_AT_RcvMtaRefclkfreqInd_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(At_SendResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvMtaRefclkfreqInd(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaRefclkfreqInd, Test_AT_RcvMtaRefclkfreqInd_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_CHAR                           *pcExpectStr = VOS_NULL_PTR;

    // 参数初始化
    pcExpectStr = "\r\n^REFCLKFREQ: 0,19200000,100,1\r\n";

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_SendResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), any(), any());

    // 调用被测函数
    ulRet = AT_RcvMtaRefclkfreqInd(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_STREQ((VOS_CHAR *)pgucAtSndCodeAddr, pcExpectStr);

    GlobalMockObject::verify();
}



class Test_AT_RcvMtaPsTransferInd: public ::testing::Test
{
public:
    VOS_UINT8                                   ucIndex;
    AT_MTA_MSG_STRU                            *pstMsg;
    MTA_AT_PS_TRANSFER_IND_STRU                *pstPsTranferInd;
    VOS_UINT32                                  ulMsgLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex             = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

        ulMsgLen            = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_PS_TRANSFER_IND_STRU);
        pstMsg              = (AT_MTA_MSG_STRU *)malloc(ulMsgLen);
        pstPsTranferInd     = (MTA_AT_PS_TRANSFER_IND_STRU *)pstMsg->aucContent;

        PS_MEM_SET(pstPsTranferInd, 0x00, sizeof(MTA_AT_PS_TRANSFER_IND_STRU));
        pstMsg->stAppCtrl.usClientId    = MTA_CLIENTID_BROADCAST & AT_BROADCAST_CLIENT_ID_MODEM_0;
        pstPsTranferInd->ucCause        = 0;
    }
    void TearDown()
    {
        free(pstMsg);
    }
};


TEST_F(Test_AT_RcvMtaPsTransferInd, Test_AT_RcvMtaPsTransferInd_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    MOCKER(At_SendResultData)
        .expects(never());

    // 调用被测函数
    ulRet = AT_RcvMtaPsTransferInd(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaPsTransferInd, Test_AT_RcvMtaPsTransferInd_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_CHAR                           *pcExpectStr = VOS_NULL_PTR;

    // 参数初始化
    pcExpectStr = "\r\n^PSTRANSFER: 0\r\n";

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_SendResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), any(), any());

    // 调用被测函数
    ulRet = AT_RcvMtaPsTransferInd(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_STREQ((VOS_CHAR *)pgucAtSndCodeAddr, pcExpectStr);

    GlobalMockObject::verify();
}


class Test_AT_RcvMtaEcidSetCnf: public ::testing::Test
{
public:
    VOS_UINT8                                   ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                     stCmdInfo;
    AT_MTA_MSG_STRU                            *pstMsg;
    MTA_AT_ECID_SET_CNF_STRU                   *pstEcidSetCnf;
    VOS_UINT32                                  ulMsgLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_APP_INDEX;

        stCmdInfo.ulCmdIndex        = AT_CMD_ECID;
        stCmdInfo.pfnSetProc        = AT_SetEcidPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnTestProc       = At_CmdTestProcERROR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnAbortProc      = VOS_NULL_PTR;
        stCmdInfo.ulAbortTimeOut    = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"+ECID";
        stCmdInfo.pszParam          = (VOS_UINT8 *)"(0)";

        ulMsgLen            = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_ECID_SET_CNF_STRU) + 2000;
        pstMsg              = (AT_MTA_MSG_STRU *)malloc(ulMsgLen);
        pstEcidSetCnf       = (MTA_AT_ECID_SET_CNF_STRU *)pstMsg->aucContent;

        PS_MEM_SET(pstEcidSetCnf, 0, sizeof(MTA_AT_ECID_SET_CNF_STRU) + 2000);
        pstMsg->stAppCtrl.usClientId    = AT_CLIENT_ID_APP;
        pstEcidSetCnf->enResult      = MTA_AT_RESULT_NO_ERROR;

        AT_ResetClientTab();
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_ECID_SET;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
    }
    void TearDown()
    {
        free(pstMsg);
    }
};


TEST_F(Test_AT_RcvMtaEcidSetCnf, Test_AT_RcvMtaEcidSetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

   // 调用被测函数
    ulRet = AT_RcvMtaEcidSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaEcidSetCnf, Test_AT_RcvMtaEcidSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(eq(pstMsg->stAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

   // 调用被测函数
    ulRet = AT_RcvMtaEcidSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaEcidSetCnf, Test_AT_RcvMtaEcidSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_REFCLKFREQ_SET;

    // MOCKER操作

   // 调用被测函数
    ulRet = AT_RcvMtaEcidSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaEcidSetCnf, Test_AT_RcvMtaEcidSetCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_CHAR                           *pcCellinfoStr = VOS_NULL_PTR;
    VOS_CHAR                           *pcExpectStr = VOS_NULL_PTR;

    // 参数初始化
    pcCellinfoStr = "0,GSM:,310,410,6042,4626,3,2,1,653,39,19,234,36";
    pcExpectStr = "+ECID=0,GSM:,310,410,6042,4626,3,2,1,653,39,19,234,36";
    memcpy(pstEcidSetCnf->aucCellInfoStr, pcCellinfoStr, strlen(pcCellinfoStr));

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_OK));

   // 调用被测函数
    ulRet = AT_RcvMtaEcidSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_STREQ((VOS_CHAR *)pgucAtSndCodeAddr, pcExpectStr);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaEcidSetCnf, Test_AT_RcvMtaEcidSetCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstEcidSetCnf->enResult      = MTA_AT_RESULT_ERROR;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_ERROR));

    // 执行检查

   // 调用被测函数
    ulRet = AT_RcvMtaEcidSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentAntSwitchSetCnf: public ::testing::Test
{
public:
    VOS_UINT8                                   ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                     stCmdInfo;
    DRV_AGENT_MSG_STRU                         *pstMsg;
    DRV_AGENT_ANTSWITCH_SET_CNF_STRU           *pstAntSwitchSetCnf;
    VOS_UINT32                                  ulMsgLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_PCUI_INDEX;

        stCmdInfo.ulCmdIndex        = AT_CMD_ANTSWITCH;
        stCmdInfo.pfnSetProc        = At_SetAntSwitchPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = AT_QryAntSwitchPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnAbortProc      = VOS_NULL_PTR;
        stCmdInfo.ulAbortTimeOut    = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^ANTSWITCH";
        stCmdInfo.pszParam          = (VOS_UINT8 *)"(0,1)";

        ulMsgLen            = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_ANTSWITCH_SET_CNF_STRU);
        pstMsg              = (DRV_AGENT_MSG_STRU *)malloc(ulMsgLen);
        pstAntSwitchSetCnf  = (DRV_AGENT_ANTSWITCH_SET_CNF_STRU *)pstMsg->aucContent;

        PS_MEM_SET(pstAntSwitchSetCnf, 0, sizeof(DRV_AGENT_ANTSWITCH_SET_CNF_STRU));
        pstAntSwitchSetCnf->stAtAppCtrl.usClientId    = AT_CLIENT_ID_PCUI;
        pstAntSwitchSetCnf->ulResult    = VOS_OK;

        AT_ResetClientTab();
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_ANTSWITCH_SET;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
    }
    void TearDown()
    {
        free(pstMsg);
    }
};


TEST_F(Test_AT_RcvDrvAgentAntSwitchSetCnf, Test_AT_RcvDrvAgentAntSwitchSetCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

   // 调用被测函数
    ulRet = AT_RcvDrvAgentAntSwitchSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentAntSwitchSetCnf, Test_AT_RcvDrvAgentAntSwitchSetCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(eq(pstAntSwitchSetCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

   // 调用被测函数
    ulRet = AT_RcvDrvAgentAntSwitchSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentAntSwitchSetCnf, Test_AT_RcvDrvAgentAntSwitchSetCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_REFCLKFREQ_SET;

    // MOCKER操作

   // 调用被测函数
    ulRet = AT_RcvDrvAgentAntSwitchSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentAntSwitchSetCnf, Test_AT_RcvDrvAgentAntSwitchSetCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_OK));

   // 调用被测函数
    ulRet = AT_RcvDrvAgentAntSwitchSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentAntSwitchSetCnf, Test_AT_RcvDrvAgentAntSwitchSetCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
   pstAntSwitchSetCnf->ulResult    = VOS_ERROR;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_ERROR));

    // 执行检查

   // 调用被测函数
    ulRet = AT_RcvDrvAgentAntSwitchSetCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}


class Test_AT_RcvDrvAgentAntSwitchQryCnf: public ::testing::Test
{
public:
    VOS_UINT8                                   ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                     stCmdInfo;
    DRV_AGENT_MSG_STRU                         *pstMsg;
    DRV_AGENT_ANTSWITCH_QRY_CNF_STRU           *pstAntSwitchQryCnf;
    VOS_UINT32                                  ulMsgLen;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_TAB_PCUI_INDEX;

        stCmdInfo.ulCmdIndex        = AT_CMD_ANTSWITCH;
        stCmdInfo.pfnSetProc        = At_SetAntSwitchPara;
        stCmdInfo.ulSetTimeOut      = AT_SET_PARA_TIME;
        stCmdInfo.pfnQryProc        = AT_QryAntSwitchPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.pfnAbortProc      = VOS_NULL_PTR;
        stCmdInfo.ulAbortTimeOut    = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_PIN_IS_LOCKED;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^ANTSWITCH";
        stCmdInfo.pszParam          = (VOS_UINT8 *)"(0,1)";

        ulMsgLen            = sizeof(DRV_AGENT_MSG_STRU) - 4 + sizeof(DRV_AGENT_ANTSWITCH_QRY_CNF_STRU);
        pstMsg              = (DRV_AGENT_MSG_STRU *)malloc(ulMsgLen);
        pstAntSwitchQryCnf  = (DRV_AGENT_ANTSWITCH_QRY_CNF_STRU *)pstMsg->aucContent;

        PS_MEM_SET(pstAntSwitchQryCnf, 0, sizeof(DRV_AGENT_ANTSWITCH_QRY_CNF_STRU));
        pstAntSwitchQryCnf->stAtAppCtrl.usClientId    = AT_CLIENT_ID_PCUI;
        pstAntSwitchQryCnf->ulResult    = VOS_OK;
        pstAntSwitchQryCnf->ulState     = 0;

        AT_ResetClientTab();
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_ANTSWITCH_QRY;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
    }
    void TearDown()
    {
        free(pstMsg);
    }
};


TEST_F(Test_AT_RcvDrvAgentAntSwitchQryCnf, Test_AT_RcvDrvAgentAntSwitchQryCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

   // 调用被测函数
    ulRet = AT_RcvDrvAgentAntSwitchQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentAntSwitchQryCnf, Test_AT_RcvDrvAgentAntSwitchQryCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .expects(exactly(1))
        .with(eq(pstAntSwitchQryCnf->stAtAppCtrl.usClientId), outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_SUCCESS));

   // 调用被测函数
    ulRet = AT_RcvDrvAgentAntSwitchQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentAntSwitchQryCnf, Test_AT_RcvDrvAgentAntSwitchQryCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_REFCLKFREQ_SET;

    // MOCKER操作

   // 调用被测函数
    ulRet = AT_RcvDrvAgentAntSwitchQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRet);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentAntSwitchQryCnf, Test_AT_RcvDrvAgentAntSwitchQryCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRet;
    VOS_CHAR                           *pcExpectStr = VOS_NULL_PTR;

    // 参数初始化
    pcExpectStr = "^ANTSWITCH: 0";

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_OK));

   // 调用被测函数
    ulRet = AT_RcvDrvAgentAntSwitchQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_STREQ((VOS_CHAR *)pgucAtSndCodeAddr, pcExpectStr);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvDrvAgentAntSwitchQryCnf, Test_AT_RcvDrvAgentAntSwitchQryCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRet;

    // 参数初始化
    pstAntSwitchQryCnf->ulResult    = VOS_ERROR;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex), eq((VOS_UINT32)AT_ERROR));

    // 执行检查

   // 调用被测函数
    ulRet = AT_RcvDrvAgentAntSwitchQryCnf(pstMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRet);
    EXPECT_EQ(0, gstAtSendData.usBufLen);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT, gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}



class Test_AT_RcvMtaMipiInfoInd: public ::testing::Test
{
public:

    void SetUp()
	{
		UT_STUB_INIT();
    }
    void TearDown()
    {
    }
};


TEST_F(Test_AT_RcvMtaMipiInfoInd, Test_AT_RcvMtaMipiInfoInd_001)
{
    // 变量声明
    AT_MTA_MSG_STRU                    *pstMtaMsg;
    VOS_UINT32                          ulLen;
    MTA_AT_RF_LCD_MIPICLK_IND_STRU     *pstMipiClkInd;

    // 参数初始化
    ulLen = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_RF_LCD_MIPICLK_IND_STRU) - VOS_MSG_HEAD_LENGTH;

    pstMtaMsg = (AT_MTA_MSG_STRU*)VOS_AllocMsg(I0_UEPS_PID_MTA, ulLen);

    pstMtaMsg->ulSenderCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulSenderPid = I0_UEPS_PID_MTA;
    pstMtaMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulReceiverPid = WUEPS_PID_AT;
    pstMtaMsg->ulLength = ulLen;
    pstMtaMsg->ulMsgId = ID_MTA_AT_MIPICLK_INFO_IND;
    pstMtaMsg->stAppCtrl.usClientId = AT_CLIENT_ID_BUTT; //AT_BROADCAST_CLIENT_ID_MODEM_0
    pstMipiClkInd = (MTA_AT_RF_LCD_MIPICLK_IND_STRU*)&pstMtaMsg->aucContent[0];
    pstMipiClkInd->usMipiClk = 0x1;

    // 初始化全局变量

    // MOCKER操作


    // 调用被测函数
    At_MsgProc((MsgBlock*)pstMtaMsg);

    // 执行检查

    /* 释放内存 */
    VOS_FreeMsg(I0_UEPS_PID_MTA, pstMtaMsg);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaMipiInfoInd, Test_AT_RcvMtaMipiInfoInd_002)
{
    // 变量声明
    AT_MTA_MSG_STRU                    *pstMtaMsg;
    VOS_UINT32                          ulLen;
    MTA_AT_RF_LCD_MIPICLK_IND_STRU     *pstMipiClkInd;

    // 参数初始化
    ulLen = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_RF_LCD_MIPICLK_IND_STRU) - VOS_MSG_HEAD_LENGTH;

    pstMtaMsg = (AT_MTA_MSG_STRU*)VOS_AllocMsg(I0_UEPS_PID_MTA, ulLen);

    pstMtaMsg->ulSenderCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulSenderPid = I0_UEPS_PID_MTA;
    pstMtaMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulReceiverPid = WUEPS_PID_AT;
    pstMtaMsg->ulLength = ulLen;
    pstMtaMsg->ulMsgId = ID_MTA_AT_MIPICLK_INFO_IND;
    pstMtaMsg->stAppCtrl.usClientId = AT_BROADCAST_CLIENT_ID_MODEM_0;
    pstMipiClkInd = (MTA_AT_RF_LCD_MIPICLK_IND_STRU*)&pstMtaMsg->aucContent[0];
    pstMipiClkInd->usMipiClk = 0x1;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_SendResultData)
        .expects(exactly(1));


    // 执行检查

    // 调用被测函数
    At_MsgProc((MsgBlock*)pstMtaMsg);

    /* 释放内存 */
    VOS_FreeMsg(I0_UEPS_PID_MTA, pstMtaMsg);
    GlobalMockObject::verify();
}
class Test_AT_RcvMtaMipiInfoCnf: public ::testing::Test
{
public:

    void SetUp()
	{
		UT_STUB_INIT();
    }
    void TearDown()
    {
    }
};


TEST_F(Test_AT_RcvMtaMipiInfoCnf, Test_AT_RcvMtaMipiInfoCnf_001)
{
    // 变量声明
    AT_MTA_MSG_STRU                    *pstMtaMsg;
    MTA_AT_RF_LCD_MIPICLK_CNF_STRU     *pstMipiClkCnf;
    VOS_UINT32                          ulLen;


    // 参数初始化
    ulLen = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_RF_LCD_MIPICLK_CNF_STRU);
    pstMtaMsg = (AT_MTA_MSG_STRU*)malloc(ulLen);

    pstMtaMsg->ulSenderCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulSenderPid = WUEPS_PID_AT;
    pstMtaMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulReceiverPid = I0_UEPS_PID_MTA;
    pstMtaMsg->ulLength = ulLen;
    pstMtaMsg->ulMsgId = ID_MTA_AT_MIPICLK_QRY_CNF;
    pstMtaMsg->stAppCtrl.usClientId = AT_CLIENT_ID_BUTT;//AT_BROADCAST_CLIENT_ID_MODEM_0;
    pstMipiClkCnf = (MTA_AT_RF_LCD_MIPICLK_CNF_STRU*)&pstMtaMsg->aucContent[0];
    pstMipiClkCnf->usMipiClk = 0x1;

    // 初始化全局变量

    // MOCKER操作

    // 执行检查

    // 调用被测函数
    //At_MsgProc((MsgBlock*)pstMtaMsg);

    EXPECT_EQ(VOS_ERR, AT_RcvMtaMipiInfoCnf((VOS_VOID*)pstMtaMsg));

    /* 释放内存 */
    VOS_FreeMsg(I0_UEPS_PID_MTA, pstMtaMsg);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaMipiInfoCnf, Test_AT_RcvMtaMipiInfoCnf_002)
{
    // 变量声明
    AT_MTA_MSG_STRU                    *pstMtaMsg;
    MTA_AT_RF_LCD_MIPICLK_CNF_STRU     *pstMipiClkCnf;
    VOS_UINT32                          ulLen;


    // 参数初始化
    ulLen = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_RF_LCD_MIPICLK_CNF_STRU);
    pstMtaMsg = (AT_MTA_MSG_STRU*)malloc(ulLen);

    pstMtaMsg->ulSenderCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulSenderPid = WUEPS_PID_AT;
    pstMtaMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulReceiverPid = I0_UEPS_PID_MTA;
    pstMtaMsg->ulLength = ulLen;
    pstMtaMsg->ulMsgId = ID_MTA_AT_MIPICLK_QRY_CNF;
    pstMtaMsg->stAppCtrl.usClientId = AT_BROADCAST_CLIENT_ID_MODEM_0;
    pstMipiClkCnf = (MTA_AT_RF_LCD_MIPICLK_CNF_STRU*)&pstMtaMsg->aucContent[0];
    pstMipiClkCnf->usMipiClk = 0x1;

    // 初始化全局变量

    // MOCKER操作

    // 执行检查

    // 调用被测函数
    //At_MsgProc((MsgBlock*)pstMtaMsg);

    EXPECT_EQ(VOS_ERR, AT_RcvMtaMipiInfoCnf((VOS_VOID*)pstMtaMsg));

    /* 释放内存 */
    VOS_FreeMsg(I0_UEPS_PID_MTA, pstMtaMsg);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaMipiInfoCnf, Test_AT_RcvMtaMipiInfoCnf_003)
{
    // 变量声明
    AT_MTA_MSG_STRU                    *pstMtaMsg;
    MTA_AT_RF_LCD_MIPICLK_CNF_STRU     *pstMipiClkCnf;
    VOS_UINT32                          ulLen;


    // 参数初始化
    ulLen = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_RF_LCD_MIPICLK_CNF_STRU);
    pstMtaMsg = (AT_MTA_MSG_STRU*)malloc(ulLen);

    pstMtaMsg->ulSenderCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulSenderPid = WUEPS_PID_AT;
    pstMtaMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulReceiverPid = I0_UEPS_PID_MTA;
    pstMtaMsg->ulLength = ulLen;
    pstMtaMsg->ulMsgId = ID_MTA_AT_MIPICLK_QRY_CNF;
    pstMtaMsg->stAppCtrl.usClientId = AT_CLIENT_ID_PCUI;
    pstMipiClkCnf = (MTA_AT_RF_LCD_MIPICLK_CNF_STRU*)&pstMtaMsg->aucContent[0];
    pstMipiClkCnf->usMipiClk = 0x1;

    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_ID_PCUI].CmdCurrentOpt = AT_CMD_COMM_CURRENT_OPT;

    // MOCKER操作

    // 执行检查

    // 调用被测函数
    //At_MsgProc((MsgBlock*)pstMtaMsg);

    EXPECT_EQ(VOS_ERR, AT_RcvMtaMipiInfoCnf((VOS_VOID*)pstMtaMsg));

    /* 释放内存 */
    free(pstMtaMsg);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaMipiInfoCnf, Test_AT_RcvMtaMipiInfoCnf_004)
{
    // 变量声明
    AT_MTA_MSG_STRU                    *pstMtaMsg;
    MTA_AT_RF_LCD_MIPICLK_CNF_STRU     *pstMipiClkCnf;
    VOS_UINT32                          ulLen;


    // 参数初始化
    ulLen = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_RF_LCD_MIPICLK_CNF_STRU);
    pstMtaMsg = (AT_MTA_MSG_STRU*)malloc(ulLen);

    pstMtaMsg->ulSenderCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulSenderPid = WUEPS_PID_AT;
    pstMtaMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulReceiverPid = I0_UEPS_PID_MTA;
    pstMtaMsg->ulLength = ulLen;
    pstMtaMsg->ulMsgId = ID_MTA_AT_MIPICLK_QRY_CNF;
    pstMtaMsg->stAppCtrl.usClientId = AT_CLIENT_ID_PCUI;
    pstMipiClkCnf = (MTA_AT_RF_LCD_MIPICLK_CNF_STRU*)&pstMtaMsg->aucContent[0];
    pstMipiClkCnf->usMipiClk = 0x1;
    pstMipiClkCnf->usResult = VOS_ERR;

    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_ID_PCUI].CmdCurrentOpt = AT_CMD_MIPI_CLK_QRY;
    gastAtClientTab[AT_CLIENT_ID_PCUI].ucUsed  = AT_CLIENT_USED;

    // MOCKER操作
    MOCKER(At_FormatResultData)
        .expects(exactly(1))
		.with(any(), eq((VOS_UINT32)AT_ERROR));

    // 执行检查

    // 调用被测函数
    EXPECT_EQ(VOS_OK, AT_RcvMtaMipiInfoCnf((VOS_VOID*)pstMtaMsg));

    /* 释放内存 */
    free(pstMtaMsg);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaMipiInfoCnf, Test_AT_RcvMtaMipiInfoCnf_005)
{
    // 变量声明
    AT_MTA_MSG_STRU                    *pstMtaMsg;
    MTA_AT_RF_LCD_MIPICLK_CNF_STRU     *pstMipiClkCnf;
    VOS_UINT32                          ulLen;
    VOS_UINT8                          *pucName;
    AT_PAR_CMD_ELEMENT_STRU            *pstAtParCmd;


    // 参数初始化
    ulLen = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(MTA_AT_RF_LCD_MIPICLK_CNF_STRU);
    pstMtaMsg = (AT_MTA_MSG_STRU*)malloc(ulLen);
    pucName = (VOS_UINT8 *)malloc(10);
    pstAtParCmd = (AT_PAR_CMD_ELEMENT_STRU *)malloc(sizeof(AT_PAR_CMD_ELEMENT_STRU));

    pstMtaMsg->ulSenderCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulSenderPid = WUEPS_PID_AT;
    pstMtaMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMtaMsg->ulReceiverPid = I0_UEPS_PID_MTA;
    pstMtaMsg->ulLength = ulLen;
    pstMtaMsg->ulMsgId = ID_MTA_AT_MIPICLK_QRY_CNF;
    pstMtaMsg->stAppCtrl.usClientId = AT_CLIENT_ID_PCUI;
    pstMipiClkCnf = (MTA_AT_RF_LCD_MIPICLK_CNF_STRU*)&pstMtaMsg->aucContent[0];
    pstMipiClkCnf->usMipiClk = 0x1;
    pstMipiClkCnf->usResult = VOS_OK;

    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_ID_PCUI].CmdCurrentOpt = AT_CMD_MIPI_CLK_QRY;
    gastAtClientTab[AT_CLIENT_ID_PCUI].ucUsed  = AT_CLIENT_USED;
    g_stParseContext[AT_CLIENT_ID_PCUI].pstCmdElement = pstAtParCmd;
    pstAtParCmd->pszCmdName = pucName;

    // MOCKER操作
    MOCKER(At_FormatResultData)
        .expects(exactly(1))
		.with(any(), eq((VOS_UINT32)AT_OK));

    // 执行检查

    // 调用被测函数
    EXPECT_EQ(VOS_OK, AT_RcvMtaMipiInfoCnf((VOS_VOID*)pstMtaMsg));

    /* 释放内存 */
    free(pstMtaMsg);
    free(pucName);
    free(pstAtParCmd);

    GlobalMockObject::verify();
}



#if (FEATURE_ON == FEATURE_ECALL)
class Test_AT_RcvVcMsgSetMsdCnfProc: public ::testing::Test
{
    void SetUp()
    {
		UT_STUB_INIT();
    }
    void TearDown()
    {

    }
};


TEST_F(Test_AT_RcvVcMsgSetMsdCnfProc, Test_AT_RcvVcMsgSetMsdCnfProc_001)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 stMsg;

    // 参数初始化
    memset(&stMsg, 0x0, sizeof(stMsg));
    stMsg.usMsgName = APP_VC_MSG_SET_MSD_CNF;
    stMsg.clientId = AT_CLIENT_BUTT;


    // 初始化全局变量

    // MOCKER操作

    // 调用被测函数
    EXPECT_EQ(VOS_ERR, AT_RcvVcMsgSetMsdCnfProc((MN_AT_IND_EVT_STRU *)&stMsg));

    // 执行检查

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvVcMsgSetMsdCnfProc, Test_AT_RcvVcMsgSetMsdCnfProc_002)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 stMsg;

    // 参数初始化
    memset(&stMsg, 0x0, sizeof(stMsg));
    stMsg.usMsgName = APP_VC_MSG_SET_MSD_CNF;
    stMsg.clientId = AT_BROADCAST_CLIENT_ID_MODEM_0;


    // 初始化全局变量

    // MOCKER操作

    // 调用被测函数
    EXPECT_EQ(VOS_ERR, AT_RcvVcMsgSetMsdCnfProc((MN_AT_IND_EVT_STRU *)&stMsg));

    // 执行检查

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvVcMsgSetMsdCnfProc, Test_AT_RcvVcMsgSetMsdCnfProc_003)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 stMsg;

    // 参数初始化
    memset(&stMsg, 0x0, sizeof(stMsg));
    stMsg.usMsgName = APP_VC_MSG_SET_MSD_CNF;
    stMsg.clientId = AT_CLIENT_TAB_PCUI_INDEX;


    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].CmdCurrentOpt = AT_CMD_ECLCFG_QRY;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].usClientId = AT_CLIENT_TAB_PCUI_INDEX;

    // MOCKER操作

    // 调用被测函数
    EXPECT_EQ(VOS_ERR, AT_RcvVcMsgSetMsdCnfProc((MN_AT_IND_EVT_STRU *)&stMsg));

    // 执行检查

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvVcMsgSetMsdCnfProc, Test_AT_RcvVcMsgSetMsdCnfProc_004)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 stMsg;

    // 参数初始化
    memset(&stMsg, 0x0, sizeof(stMsg));
    stMsg.usMsgName = APP_VC_MSG_SET_MSD_CNF;
    stMsg.clientId = AT_CLIENT_TAB_PCUI_INDEX;

    stMsg.aucContent[0] = VOS_ERR;


    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].CmdCurrentOpt = AT_CMD_ECLMSD_SET;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].usClientId = AT_CLIENT_TAB_PCUI_INDEX;

    // MOCKER操作
    MOCKER(AT_StopRelTimer)
		.expects(once())
		.will(returnValue((VOS_UINT32)VOS_OK));

	MOCKER(At_FormatResultData)
		.expects(once())
		.with(any(), eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数
    EXPECT_EQ(VOS_OK, AT_RcvVcMsgSetMsdCnfProc((MN_AT_IND_EVT_STRU *)&stMsg));

    // 执行检查

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvVcMsgSetMsdCnfProc, Test_AT_RcvVcMsgSetMsdCnfProc_005)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 stMsg;

    // 参数初始化
    memset(&stMsg, 0x0, sizeof(stMsg));
    stMsg.usMsgName = APP_VC_MSG_SET_MSD_CNF;
    stMsg.clientId = AT_CLIENT_TAB_PCUI_INDEX;

    stMsg.aucContent[0] = VOS_OK;


    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].CmdCurrentOpt = AT_CMD_ECLMSD_SET;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].usClientId = AT_CLIENT_TAB_PCUI_INDEX;

    // MOCKER操作
	MOCKER(AT_StopRelTimer)
		.expects(once())
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
		.expects(once())
		.with(any(), eq((VOS_UINT32)AT_OK));

    // 调用被测函数
    At_ProcMsgFromVc((MN_AT_IND_EVT_STRU *)&stMsg);

    // 执行检查

    GlobalMockObject::verify();
}


class Test_AT_RcvVcMsgQryMsdCnfProc: public ::testing::Test
{
    void SetUp()
    {
		UT_STUB_INIT();
    }
    void TearDown()
    {

    }
};


TEST_F(Test_AT_RcvVcMsgQryMsdCnfProc, Test_AT_RcvVcMsgQryMsdCnfProc_001)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulLen;

    ulLen = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(APP_VC_MSG_QRY_MSD_CNF_STRU) - VOS_MSG_HEAD_LENGTH;

    pstMsg = (MN_AT_IND_EVT_STRU *)VOS_AllocMsg(I0_WUEPS_PID_VC, ulLen);

    // 参数初始化
    PS_MEM_SET((VOS_UINT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0x0, ulLen);
    pstMsg->usMsgName = APP_VC_MSG_QRY_MSD_CNF;
    pstMsg->clientId = AT_CLIENT_BUTT;


    // 初始化全局变量

    // MOCKER操作

    // 调用被测函数

    // 执行检查
    EXPECT_EQ(VOS_ERR, AT_RcvVcMsgQryMsdCnfProc((MN_AT_IND_EVT_STRU *)pstMsg));

    VOS_FreeMsg(I0_WUEPS_PID_VC, pstMsg);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvVcMsgQryMsdCnfProc, Test_AT_RcvVcMsgQryMsdCnfProc_002)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulLen;

    ulLen = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(APP_VC_MSG_QRY_MSD_CNF_STRU) - VOS_MSG_HEAD_LENGTH;

    pstMsg = (MN_AT_IND_EVT_STRU *)VOS_AllocMsg(I0_WUEPS_PID_VC, ulLen);

    // 参数初始化
    PS_MEM_SET((VOS_UINT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0x0, ulLen);
    pstMsg->usMsgName = APP_VC_MSG_QRY_MSD_CNF;
    pstMsg->clientId = AT_BROADCAST_CLIENT_ID_MODEM_0;


    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].usClientId = AT_CLIENT_TAB_PCUI_INDEX;

    // MOCKER操作

    // 调用被测函数

    // 执行检查
    EXPECT_EQ(VOS_ERR, AT_RcvVcMsgQryMsdCnfProc((MN_AT_IND_EVT_STRU *)pstMsg));

    VOS_FreeMsg(I0_WUEPS_PID_VC, pstMsg);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvVcMsgQryMsdCnfProc, Test_AT_RcvVcMsgQryMsdCnfProc_003)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulLen;

    ulLen = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(APP_VC_MSG_QRY_MSD_CNF_STRU) - VOS_MSG_HEAD_LENGTH;

    pstMsg = (MN_AT_IND_EVT_STRU *)VOS_AllocMsg(I0_WUEPS_PID_VC, ulLen);

    // 参数初始化
    PS_MEM_SET((VOS_UINT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0x0, ulLen);
    pstMsg->usMsgName = APP_VC_MSG_QRY_MSD_CNF;
    pstMsg->clientId = AT_CLIENT_TAB_PCUI_INDEX;


    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].CmdCurrentOpt = AT_CMD_ECLMSD_SET;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].usClientId = AT_CLIENT_TAB_PCUI_INDEX;

    // MOCKER操作

    // 调用被测函数

    // 执行检查
    EXPECT_EQ(VOS_ERR, AT_RcvVcMsgQryMsdCnfProc((MN_AT_IND_EVT_STRU *)pstMsg));

    VOS_FreeMsg(I0_WUEPS_PID_VC, pstMsg);

    GlobalMockObject::verify();
}



TEST_F(Test_AT_RcvVcMsgQryMsdCnfProc, Test_AT_RcvVcMsgQryMsdCnfProc_004)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 *pstMsg = VOS_NULL_PTR;
    APP_VC_MSG_QRY_MSD_CNF_STRU        *pstQryMsd = VOS_NULL_PTR;
    VOS_UINT32                          ulLen;

    ulLen = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(APP_VC_MSG_QRY_MSD_CNF_STRU) - VOS_MSG_HEAD_LENGTH;

    pstMsg = (MN_AT_IND_EVT_STRU *)VOS_AllocMsg(I0_WUEPS_PID_VC, ulLen);

    // 参数初始化
    PS_MEM_SET((VOS_UINT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0x0, ulLen);
    pstMsg->usMsgName = APP_VC_MSG_QRY_MSD_CNF;
    pstMsg->clientId = AT_CLIENT_TAB_PCUI_INDEX;

    pstQryMsd = (APP_VC_MSG_QRY_MSD_CNF_STRU *)pstMsg->aucContent;
    pstQryMsd->ucQryRslt = VOS_ERR;


    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].CmdCurrentOpt = AT_CMD_ECLMSD_QRY;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].usClientId = AT_CLIENT_TAB_PCUI_INDEX;

    // MOCKER操作
	MOCKER(AT_StopRelTimer)
		.expects(once())
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
		.expects(once())
		.with(any(), eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数

    // 执行检查
    EXPECT_EQ(VOS_OK, AT_RcvVcMsgQryMsdCnfProc((MN_AT_IND_EVT_STRU *)pstMsg));

    VOS_FreeMsg(I0_WUEPS_PID_VC, pstMsg);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvVcMsgQryMsdCnfProc, Test_AT_RcvVcMsgQryMsdCnfProc_005)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 *pstMsg = VOS_NULL_PTR;
    APP_VC_MSG_QRY_MSD_CNF_STRU        *pstQryMsd = VOS_NULL_PTR;
    VOS_UINT32                          ulLen;

    ulLen = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(APP_VC_MSG_QRY_MSD_CNF_STRU) - VOS_MSG_HEAD_LENGTH;

    pstMsg = (MN_AT_IND_EVT_STRU *)VOS_AllocMsg(I0_WUEPS_PID_VC, ulLen);

    // 参数初始化
    PS_MEM_SET((VOS_UINT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0x0, ulLen);
    pstMsg->usMsgName = APP_VC_MSG_QRY_MSD_CNF;
    pstMsg->clientId = AT_CLIENT_TAB_PCUI_INDEX;

    pstQryMsd = (APP_VC_MSG_QRY_MSD_CNF_STRU *)pstMsg->aucContent;
    pstQryMsd->ucQryRslt = VOS_OK;


    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].CmdCurrentOpt = AT_CMD_ECLMSD_QRY;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].usClientId = AT_CLIENT_TAB_PCUI_INDEX;
    g_stParseContext[AT_CLIENT_TAB_PCUI_INDEX].pstCmdElement = (AT_PAR_CMD_ELEMENT_STRU*)malloc(sizeof(AT_PAR_CMD_ELEMENT_STRU));


    // MOCKER操作

    // 调用被测函数
	MOCKER(AT_StopRelTimer)
		.expects(once())
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
		.expects(once())
		.with(any(), eq((VOS_UINT32)AT_OK));

    // 执行检查
    At_ProcMsgFromVc((MN_AT_IND_EVT_STRU *)pstMsg);

    VOS_FreeMsg(I0_WUEPS_PID_VC, pstMsg);
    free(g_stParseContext[AT_CLIENT_TAB_PCUI_INDEX].pstCmdElement);

    GlobalMockObject::verify();
}


class Test_AT_RcvVcMsgQryEcallCfgCnfProc: public ::testing::Test
{
    void SetUp()
    {
		UT_STUB_INIT();
    }
    void TearDown()
    {

    }
};


TEST_F(Test_AT_RcvVcMsgQryEcallCfgCnfProc, Test_AT_RcvVcMsgQryEcallCfgCnfProc_001)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulLen;

    ulLen = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(APP_VC_MSG_QRY_ECALL_CFG_CNF_STRU) - VOS_MSG_HEAD_LENGTH;

    pstMsg = (MN_AT_IND_EVT_STRU *)VOS_AllocMsg(I0_WUEPS_PID_VC, ulLen);

    // 参数初始化
    PS_MEM_SET((VOS_UINT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0x0, ulLen);
    pstMsg->usMsgName = APP_VC_MSG_QRY_ECALL_CFG_CNF;
    pstMsg->clientId = AT_CLIENT_BUTT;


    // 初始化全局变量

    // MOCKER操作

    // 调用被测函数

    // 执行检查
    EXPECT_EQ(VOS_ERR, AT_RcvVcMsgQryEcallCfgCnfProc((MN_AT_IND_EVT_STRU *)pstMsg));

    VOS_FreeMsg(I0_WUEPS_PID_VC, pstMsg);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvVcMsgQryEcallCfgCnfProc, Test_AT_RcvVcMsgQryEcallCfgCnfProc_002)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulLen;

    ulLen = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(APP_VC_MSG_QRY_ECALL_CFG_CNF_STRU) - VOS_MSG_HEAD_LENGTH;

    pstMsg = (MN_AT_IND_EVT_STRU *)VOS_AllocMsg(I0_WUEPS_PID_VC, ulLen);

    // 参数初始化
    PS_MEM_SET((VOS_UINT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0x0, ulLen);
    pstMsg->usMsgName = APP_VC_MSG_QRY_ECALL_CFG_CNF;
    pstMsg->clientId = AT_BROADCAST_CLIENT_ID_MODEM_0;


    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].usClientId = AT_CLIENT_TAB_PCUI_INDEX;

    // MOCKER操作

    // 调用被测函数

    // 执行检查
    EXPECT_EQ(VOS_ERR, AT_RcvVcMsgQryEcallCfgCnfProc((MN_AT_IND_EVT_STRU *)pstMsg));

    VOS_FreeMsg(I0_WUEPS_PID_VC, pstMsg);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvVcMsgQryEcallCfgCnfProc, Test_AT_RcvVcMsgQryEcallCfgCnfProc_003)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulLen;

    ulLen = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(APP_VC_MSG_QRY_ECALL_CFG_CNF_STRU) - VOS_MSG_HEAD_LENGTH;

    pstMsg = (MN_AT_IND_EVT_STRU *)VOS_AllocMsg(I0_WUEPS_PID_VC, ulLen);

    // 参数初始化
    PS_MEM_SET((VOS_UINT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0x0, ulLen);
    pstMsg->usMsgName = APP_VC_MSG_QRY_ECALL_CFG_CNF;
    pstMsg->clientId = AT_CLIENT_TAB_PCUI_INDEX;


    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].CmdCurrentOpt = AT_CMD_ECLMSD_SET;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].usClientId = AT_CLIENT_TAB_PCUI_INDEX;

    // MOCKER操作

    // 调用被测函数

    // 执行检查
    EXPECT_EQ(VOS_ERR, AT_RcvVcMsgQryEcallCfgCnfProc((MN_AT_IND_EVT_STRU *)pstMsg));

    VOS_FreeMsg(I0_WUEPS_PID_VC, pstMsg);

    GlobalMockObject::verify();
}



TEST_F(Test_AT_RcvVcMsgQryEcallCfgCnfProc, Test_AT_RcvVcMsgQryEcallCfgCnfProc_004)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 *pstMsg = VOS_NULL_PTR;
    APP_VC_MSG_QRY_ECALL_CFG_CNF_STRU  *pstQryCfg = VOS_NULL_PTR;
    VOS_UINT32                          ulLen;

    ulLen = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(APP_VC_MSG_QRY_ECALL_CFG_CNF_STRU) - VOS_MSG_HEAD_LENGTH;

    pstMsg = (MN_AT_IND_EVT_STRU *)VOS_AllocMsg(I0_WUEPS_PID_VC, ulLen);

    // 参数初始化
    PS_MEM_SET((VOS_UINT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0x0, ulLen);
    pstMsg->usMsgName = APP_VC_MSG_QRY_ECALL_CFG_CNF;
    pstMsg->clientId = AT_CLIENT_TAB_PCUI_INDEX;

    pstQryCfg = (APP_VC_MSG_QRY_ECALL_CFG_CNF_STRU *)pstMsg->aucContent;
    pstQryCfg->ucQryRslt = VOS_ERR;


    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].CmdCurrentOpt = AT_CMD_ECLCFG_QRY;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].usClientId = AT_CLIENT_TAB_PCUI_INDEX;

    // MOCKER操作
	MOCKER(AT_StopRelTimer)
		.expects(once())
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
		.expects(once())
		.with(any(), eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数

    // 执行检查
    EXPECT_EQ(VOS_OK, AT_RcvVcMsgQryEcallCfgCnfProc((MN_AT_IND_EVT_STRU *)pstMsg));

    VOS_FreeMsg(I0_WUEPS_PID_VC, pstMsg);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvVcMsgQryEcallCfgCnfProc, Test_AT_RcvVcMsgQryEcallCfgCnfProc_005)
{
    // 变量声明
    MN_AT_IND_EVT_STRU                 *pstMsg = VOS_NULL_PTR;
    APP_VC_MSG_QRY_ECALL_CFG_CNF_STRU  *pstQryCfg = VOS_NULL_PTR;
    VOS_UINT32                          ulLen;

    ulLen = sizeof(MN_AT_IND_EVT_STRU) - 4 + sizeof(APP_VC_MSG_QRY_ECALL_CFG_CNF_STRU) - VOS_MSG_HEAD_LENGTH;

    pstMsg = (MN_AT_IND_EVT_STRU *)VOS_AllocMsg(I0_WUEPS_PID_VC, ulLen);

    // 参数初始化
    PS_MEM_SET((VOS_UINT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0x0, ulLen);
    pstMsg->usMsgName = APP_VC_MSG_QRY_ECALL_CFG_CNF;
    pstMsg->clientId = AT_CLIENT_TAB_PCUI_INDEX;

    pstQryCfg = (APP_VC_MSG_QRY_ECALL_CFG_CNF_STRU *)pstMsg->aucContent;
    pstQryCfg->ucQryRslt = VOS_OK;


    // 初始化全局变量
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].CmdCurrentOpt = AT_CMD_ECLCFG_QRY;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].ucUsed = AT_CLIENT_USED;
    gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].usClientId = AT_CLIENT_TAB_PCUI_INDEX;
    g_stParseContext[AT_CLIENT_TAB_PCUI_INDEX].pstCmdElement = (AT_PAR_CMD_ELEMENT_STRU*)malloc(sizeof(AT_PAR_CMD_ELEMENT_STRU));


    // MOCKER操作

    // 调用被测函数
	MOCKER(AT_StopRelTimer)
		.expects(once())
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
		.expects(once())
		.with(any(), eq((VOS_UINT32)AT_OK));

    // 执行检查
    At_ProcMsgFromVc((MN_AT_IND_EVT_STRU *)pstMsg);

    VOS_FreeMsg(I0_WUEPS_PID_VC, pstMsg);
    free(g_stParseContext[AT_CLIENT_TAB_PCUI_INDEX].pstCmdElement);

    GlobalMockObject::verify();
}


#endif

#if (FEATURE_ON == FEATURE_DSDS)

class Test_AT_RcvMtaPsProtectSetCnf: public ::testing::Test
{
    public:
    AT_MTA_MSG_STRU                        *pRcvMsg;
    MTA_AT_RRC_PROTECT_PS_CNF_STRU         *pstProtectPsCnf;
    VOS_UINT8                               ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_RRC_PROTECT_PS_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstProtectPsCnf = (MTA_AT_RRC_PROTECT_PS_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstProtectPsCnf, 0, sizeof(MTA_AT_RRC_PROTECT_PS_CNF_STRU));
        PS_MEM_SET(gastAtClientTab, 0, AT_MAX_CLIENT_NUM*sizeof(AT_CLIENT_MANAGE_STRU));
    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaPsProtectSetCnf, Test_AT_RcvMtaPsProtectSetCnf_001)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRst = AT_RcvMtaPsProtectSetCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaPsProtectSetCnf, Test_AT_RcvMtaPsProtectSetCnf_002)
{
    // 变量声明
    VOS_UINT32                                ulRst;

    // 初始化变量
    ucIndex                                 = AT_BROADCAST_CLIENT_ID_MODEM_0;
    pRcvMsg->stAppCtrl.usClientId           = AT_BROADCAST_CLIENT_ID_MODEM_0;
    // 初始化全局变量
    gastAtClientTab[ucIndex].ucUsed         = 1;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_REFCLKFREQ_SET;
    gastAtClientTab[ucIndex].usClientId     = 0;


    // MOCKER操作

    // 调用被测函数
    ulRst = AT_RcvMtaPsProtectSetCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}



TEST_F(Test_AT_RcvMtaPsProtectSetCnf, Test_AT_RcvMtaPsProtectSetCnf_003)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    AT_CLIENT_TAB_INDEX_UINT8                 ucIndex;
    /* 变量初始化 */
    ucIndex                                 = AT_CLIENT_TAB_APP_INDEX;
    pRcvMsg->stAppCtrl.usClientId           = 0;

    /* 全局变量初始化 */
    gastAtClientTab[ucIndex].ucUsed         = 1;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_REFCLKFREQ_SET;
    gastAtClientTab[ucIndex].usClientId     = 0;

    // MOCKER操作

    // 调用被测函数
    ulRst = AT_RcvMtaPsProtectSetCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaPsProtectSetCnf, Test_AT_RcvMtaPsProtectSetCnf_004)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    AT_CLIENT_TAB_INDEX_UINT8                 ucIndex;
    /* 变量初始化 */
    ucIndex                                 = AT_CLIENT_TAB_APP_INDEX;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_PSPROTECTMODE_SET;
    gastAtClientTab[ucIndex].usClientId     = 0;
    gastAtClientTab[ucIndex].ucUsed         = 1;
    // MOCKER操作
	MOCKER(AT_StopRelTimer)
		.expects(once())
		.will(returnValue((VOS_UINT32)VOS_OK));

    // 调用被测函数
    ulRst = AT_RcvMtaPsProtectSetCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaPsProtectSetCnf, Test_AT_RcvMtaPsProtectSetCnf_005)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    AT_CLIENT_TAB_INDEX_UINT8                 ucIndex;
    /* 变量初始化 */
    ucIndex                                 = AT_CLIENT_TAB_APP_INDEX;
    pstProtectPsCnf->enResult               = MTA_AT_RESULT_NO_ERROR;
    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_PSPROTECTMODE_SET;
    gastAtClientTab[ucIndex].usClientId     = 0;
    gastAtClientTab[ucIndex].ucUsed         = 1;

    // MOCKER操作
	MOCKER(AT_StopRelTimer)
		.expects(once())
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex),eq((VOS_UINT32)AT_OK));
    // 调用被测函数
    ulRst = AT_RcvMtaPsProtectSetCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaPsProtectSetCnf, Test_AT_RcvMtaPsProtectSetCnf_006)
{
    // 变量声明
    VOS_UINT32                                ulRst;
    VOS_UINT16                                usClientId;
    AT_CLIENT_TAB_INDEX_UINT8                 ucIndex;
    /* 变量初始化 */
    ucIndex                                 = AT_CLIENT_TAB_APP_INDEX;
    pstProtectPsCnf->enResult               = MTA_AT_RESULT_ERROR;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_PSPROTECTMODE_SET;
    gastAtClientTab[ucIndex].usClientId     = 0;
    gastAtClientTab[ucIndex].ucUsed         = 1;

    // MOCKER操作
	MOCKER(AT_StopRelTimer)
		.expects(once())
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex),eq((VOS_UINT32)AT_ERROR));
    // 调用被测函数
    ulRst = AT_RcvMtaPsProtectSetCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);

    GlobalMockObject::verify();
}
#endif
class Test_AT_RcvMtaPhyInitCnf: public ::testing::Test
{
    public:
    AT_MTA_MSG_STRU                        *pRcvMsg;
    MTA_AT_PHY_INIT_CNF_STRU         *pstProtectPsCnf;
    VOS_UINT8                               ucIndex;

    void SetUp()
	{
		UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pRcvMsg = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_MSG_STRU) + sizeof(MTA_AT_PHY_INIT_CNF_STRU) - 4);

        if (VOS_NULL_PTR == pRcvMsg)
        {
            return;
        }
        pRcvMsg->stAppCtrl.usClientId = 0;
        pstProtectPsCnf = (MTA_AT_PHY_INIT_CNF_STRU *)(pRcvMsg->aucContent);

        PS_MEM_SET(pRcvMsg, 0, sizeof(AT_MTA_MSG_STRU));
        PS_MEM_SET(pstProtectPsCnf, 0, sizeof(MTA_AT_PHY_INIT_CNF_STRU));
        PS_MEM_SET(gastAtClientTab, 0, sizeof(gastAtClientTab));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pRcvMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pRcvMsg);
        }
    }
};


TEST_F(Test_AT_RcvMtaPhyInitCnf, Test_AT_RcvMtaPhyInitCnf_001)
{
    // 变量声明
    VOS_UINT32                          ulRst;
    VOS_UINT16                          usClientId;

    /* 变量初始化 */
    usClientId                              = 0;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(eq((VOS_UINT16)usClientId),outBoundP(&ucIndex, (VOS_UINT8)sizeof(ucIndex)))
        .will(returnValue((VOS_UINT32)AT_FAILURE));

    // 调用被测函数
    ulRst = AT_RcvMtaPhyInitCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaPhyInitCnf, Test_AT_RcvMtaPhyInitCnf_002)
{
    // 变量声明
    VOS_UINT32                          ulRst;

    // 初始化变量
    ucIndex                                 = AT_BROADCAST_CLIENT_ID_MODEM_0;
    pRcvMsg->stAppCtrl.usClientId           = AT_BROADCAST_CLIENT_ID_MODEM_0;
    // 初始化全局变量
    gastAtClientTab[ucIndex].ucUsed         = 1;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_REFCLKFREQ_SET;
    gastAtClientTab[ucIndex].usClientId     = 0;


    // MOCKER操作

    // 调用被测函数
    ulRst = AT_RcvMtaPhyInitCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}



TEST_F(Test_AT_RcvMtaPhyInitCnf, Test_AT_RcvMtaPhyInitCnf_003)
{
    // 变量声明
    VOS_UINT32                          ulRst;
    VOS_UINT16                          usClientId;
    AT_CLIENT_TAB_INDEX_UINT8           ucIndex;
    /* 变量初始化 */
    ucIndex                                 = AT_CLIENT_TAB_APP_INDEX;
    pRcvMsg->stAppCtrl.usClientId           = 0;

    /* 全局变量初始化 */
    gastAtClientTab[ucIndex].ucUsed         = 1;
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_REFCLKFREQ_SET;
    gastAtClientTab[ucIndex].usClientId     = 0;

    // MOCKER操作

    // 调用被测函数
    ulRst = AT_RcvMtaPhyInitCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_ERR, ulRst);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMtaPhyInitCnf, Test_AT_RcvMtaPhyInitCnf_004)
{
    // 变量声明
    VOS_UINT32                          ulRst;
    VOS_UINT16                          usClientId;
    AT_CLIENT_TAB_INDEX_UINT8           ucIndex;
    /* 变量初始化 */
    ucIndex                                 = AT_CLIENT_TAB_APP_INDEX;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_PHYINIT_SET;
    gastAtClientTab[ucIndex].usClientId     = 0;
    gastAtClientTab[ucIndex].ucUsed         = 1;
    // MOCKER操作
	MOCKER(AT_StopRelTimer)
		.expects(once())
		.will(returnValue((VOS_UINT32)VOS_OK));

    // 调用被测函数
    ulRst = AT_RcvMtaPhyInitCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaPhyInitCnf, Test_AT_RcvMtaPhyInitCnf_005)
{
    // 变量声明
    VOS_UINT32                          ulRst;
    VOS_UINT16                          usClientId;
    AT_CLIENT_TAB_INDEX_UINT8           ucIndex;
    /* 变量初始化 */
    ucIndex                                 = AT_CLIENT_TAB_APP_INDEX;
    pstProtectPsCnf->enResult               = MTA_AT_RESULT_NO_ERROR;
    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_PHYINIT_SET;
    gastAtClientTab[ucIndex].usClientId     = 0;
    gastAtClientTab[ucIndex].ucUsed         = 1;

    // MOCKER操作
	MOCKER(AT_StopRelTimer)
		.expects(once())
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex),eq((VOS_UINT32)AT_OK));
    // 调用被测函数
    ulRst = AT_RcvMtaPhyInitCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);

    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMtaPhyInitCnf, Test_AT_RcvMtaPhyInitCnf_006)
{
    // 变量声明
    VOS_UINT32                          ulRst;
    VOS_UINT16                          usClientId;
    AT_CLIENT_TAB_INDEX_UINT8           ucIndex;
    /* 变量初始化 */
    ucIndex                                 = AT_CLIENT_TAB_APP_INDEX;
    pstProtectPsCnf->enResult               = MTA_AT_RESULT_ERROR;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_PHYINIT_SET;
    gastAtClientTab[ucIndex].usClientId     = 0;
    gastAtClientTab[ucIndex].ucUsed         = 1;

    // MOCKER操作
	MOCKER(AT_StopRelTimer)
		.expects(once())
		.will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1))
        .with(eq((VOS_UINT8)ucIndex),eq((VOS_UINT32)AT_ERROR));
    // 调用被测函数
    ulRst = AT_RcvMtaPhyInitCnf((VOS_VOID*)pRcvMsg);

    // 执行检查
    EXPECT_EQ(VOS_OK, ulRst);

    GlobalMockObject::verify();
}

/*****************************以下NAS2负责******************************/


class Test_At_MmaMsgProc: public ::testing::Test
{
public:
    void SetUp()
    {
        UT_STUB_INIT();
    }
    void TearDown()
    {
    }

};


TEST_F(Test_At_MmaMsgProc, Test_At_MmaMsgProc_001)
{
    ATMMA_OM_MAINTAIN_INFO_IND_STRU     stMsg;

    stMsg.ulMsgName = AT_MMA_OM_MAINTAIN_INFO_IND;
    stMsg.ucOmConnectFlg       = VOS_TRUE;
    stMsg.ucOmPcRecurEnableFlg = VOS_TRUE;

    At_MmaMsgProc((MSG_HEADER_STRU*)&stMsg);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}

TEST_F(Test_At_MmaMsgProc, Test_At_MmaMsgProc_002)
{
    ATMMA_OM_MAINTAIN_INFO_IND_STRU     stMsg;

    stMsg.ulMsgName = AT_MMA_OM_MAINTAIN_INFO_IND;
    stMsg.ucOmConnectFlg       = VOS_TRUE;
    stMsg.ucOmPcRecurEnableFlg = VOS_FALSE;

    At_MmaMsgProc((MSG_HEADER_STRU*)&stMsg);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}



TEST_F(Test_At_MmaMsgProc, Test_At_MmaMsgProc_003)
{

    ATMMA_OM_MAINTAIN_INFO_IND_STRU     stMsg;

    stMsg.ulMsgName = AT_MMA_USIM_STATUS_IND;
    stMsg.ucOmConnectFlg       = VOS_TRUE;
    stMsg.ucOmPcRecurEnableFlg = VOS_FALSE;

    MOCKER(AT_RcvMmaCrpnQueryRsp)
        .expects(never());

    At_MmaMsgProc((MSG_HEADER_STRU*)&stMsg);
    
    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MmaMsgProc, Test_At_MmaMsgProc_004)
{
    ATMMA_OM_MAINTAIN_INFO_IND_STRU     stMsg;

    stMsg.ulMsgName = AT_MMA_USIM_STATUS_IND;
    stMsg.ucOmConnectFlg       = VOS_TRUE;
    stMsg.ucOmPcRecurEnableFlg = VOS_FALSE;

    MOCKER(AT_RcvMmaCrpnQueryRsp)
        .expects(never());
    
    MOCKER(VOS_GetModemIDFromPid)
        .stubs()
        .will(returnValue((MODEM_ID_ENUM_UINT16)MODEM_ID_BUTT));

    AT_InitCtx();
    
    At_MmaMsgProc((MSG_HEADER_STRU*)&stMsg);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MmaMsgProc, Test_At_MmaMsgProc_005)
{
    TAF_MMA_SIM_INSERT_RSP_STRU        stMsg;


    stMsg.ulMsgName              = AT_MMA_SIM_INSERT_RSP;

    MOCKER(AT_RcvMmaSimInsertRsp)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_OK));

    At_MmaMsgProc((MSG_HEADER_STRU*)&stMsg);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_At_MmaMsgProc, Test_At_MmaMsgProc_006)
{
    TAF_MMA_SIM_INSERT_RSP_STRU        stMsg;

    MOCKER(AT_RcvMmaSimInsertRsp)
        .expects(never());
    
    stMsg.ulMsgName              = AT_MMA_SIM_INSERT_RSP + 10999;

    At_MmaMsgProc((MSG_HEADER_STRU*)&stMsg);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}



class Test_AT_RcvMmaSysCfgSetCnf: public ::testing::Test
{
public:
    void SetUp()
    {
        UT_STUB_INIT();
    }
    void TearDown()
    {
    }

};


TEST_F(Test_AT_RcvMmaSysCfgSetCnf, Test_AT_RcvMmaSysCfgSetCnf_001)
{
    /* 局部变量定义 */
    TAF_MMA_SYS_CFG_CNF_STRU            stRcvMsg;
    VOS_UINT32                          ulRslt;

    /* 局部变量初始化 */
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid              = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId          = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid            = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                = ID_TAF_MMA_SYS_CFG_SET_CNF;
    stRcvMsg.usClientId               = 11;
    stRcvMsg.ucOpid                   = 2;
    stRcvMsg.aucReserve[0]            = 0;
    stRcvMsg.enRslt                   = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    stRcvMsg.usErrorCause             = 0;

    /* 全局变量初始化  */

    /* MOCKER检测 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((TAF_UINT32)AT_FAILURE));

    ulRslt = AT_RcvMmaSysCfgSetCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_ERR, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMmaSysCfgSetCnf, Test_AT_RcvMmaSysCfgSetCnf_002)
{
    /* 局部变量定义 */
    TAF_MMA_SYS_CFG_CNF_STRU            stRcvMsg;
    VOS_UINT32                          ulRslt;

    /* 局部变量初始化  */
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId              = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid                = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid              = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                  = ID_TAF_MMA_SYS_CFG_SET_CNF;
    stRcvMsg.usClientId                 = AT_BROADCAST_CLIENT_ID_MODEM_0;
    stRcvMsg.ucOpid                     = 2;
    stRcvMsg.aucReserve[0]              = 0;
    stRcvMsg.enRslt                     = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    stRcvMsg.usErrorCause               = 0;

    /* 全局变量初始化  */

    /* 上报的CLIENTID索引为0 */
    gastAtClientTab[0].usClientId       = 11;
    gastAtClientTab[0].ucUsed           = AT_CLIENT_USED;

    /* MOCKER检测 */

    ulRslt = AT_RcvMmaSysCfgSetCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_ERR, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaSysCfgSetCnf, Test_AT_RcvMmaSysCfgSetCnf_003)
{
    /* 局部变量定义 */
    TAF_MMA_SYS_CFG_CNF_STRU            stRcvMsg;
    VOS_UINT32                          ulRslt;

    /* 局部变量初始化  */
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId              = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid                = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid              = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                  = ID_TAF_MMA_SYS_CFG_SET_CNF;
    stRcvMsg.usClientId                 = 11;
    stRcvMsg.ucOpid                     = 2;
    stRcvMsg.aucReserve[0]              = 0;
    stRcvMsg.enRslt                     = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    stRcvMsg.usErrorCause               = 0;

    /* 全局变量初始化  */

    /* 上报的CLIENTID索引为0 */
    gastAtClientTab[0].usClientId       = 11;
    gastAtClientTab[0].ucUsed           = AT_CLIENT_USED;

    /* MOCKER检测 */
    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    ulRslt = AT_RcvMmaSysCfgSetCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_OK,   ulRslt);
    EXPECT_EQ(0,        gstAtSendData.usBufLen);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}



TEST_F(Test_AT_RcvMmaSysCfgSetCnf, Test_AT_RcvMmaSysCfgSetCnf_004)
{
    /* 局部变量定义 */
    TAF_MMA_SYS_CFG_CNF_STRU            stRcvMsg;
    VOS_UINT32                          ulRslt;

    /* 局部变量初始化  */
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId              = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid                = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid              = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                  = ID_TAF_MMA_SYS_CFG_SET_CNF;
    stRcvMsg.usClientId                 = 11;
    stRcvMsg.ucOpid                     = 2;
    stRcvMsg.aucReserve[0]              = 0;
    stRcvMsg.enRslt                     = TAF_MMA_APP_OPER_RESULT_FAILURE;
    stRcvMsg.usErrorCause               = TAF_ERR_UNSPECIFIED_ERROR;

    /* 全局变量初始化  */

    /* 上报的CLIENTID索引为0 */
    gastAtClientTab[0].usClientId       = 11;
    gastAtClientTab[0].ucUsed           = AT_CLIENT_USED;

    /* MOCKER检测 */
    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    ulRslt = AT_RcvMmaSysCfgSetCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_OK,   ulRslt);
    EXPECT_EQ(0,        gstAtSendData.usBufLen);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}



class Test_AT_RcvMmaPhoneModeSetCnf: public ::testing::Test
{
public:
    void SetUp()
    {
        UT_STUB_INIT();
    }
    void TearDown()
    {
    }

};


TEST_F(Test_AT_RcvMmaPhoneModeSetCnf, Test_AT_RcvMmaPhoneModeSetCnf_001)
{
    /* 局部变量定义 */
    TAF_MMA_PHONE_MODE_SET_CNF_STRU     stRcvMsg;
    VOS_UINT32                          ulRslt;

    /* 局部变量初始化  */
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid              = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId          = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid            = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                = ID_TAF_MMA_PHONE_MODE_SET_CNF;
    stRcvMsg.usClientId               = 11;
    stRcvMsg.ucOpid                   = 2;
    stRcvMsg.ucPhMode                 = TAF_PH_MODE_FULL;
    stRcvMsg.enRslt                   = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    stRcvMsg.usErrorCause             = 0;

    /* 全局变量初始化 */

    /* MOCKER检测 */
    MOCKER(AT_GetModemIdFromClient)
        .stubs()
        .will(returnValue((VOS_UINT32)VOS_ERR));

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((TAF_UINT32)AT_FAILURE));

    /* 执行测试函数 */
    ulRslt = AT_RcvMmaPhoneModeSetCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_ERR, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaPhoneModeSetCnf, Test_AT_RcvMmaPhoneModeSetCnf_002)
{
    /* 局部变量定义 */
    TAF_MMA_PHONE_MODE_SET_CNF_STRU     stRcvMsg;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    /* 局部变量初始化  */
    enModemId                           = MODEM_ID_0;
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId              = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid                = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid              = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                  = ID_TAF_MMA_PHONE_MODE_SET_CNF;
    stRcvMsg.usClientId                 = 11;
    stRcvMsg.ucOpid                     = 2;
    stRcvMsg.ucPhMode                   = TAF_PH_MODE_FULL;
    stRcvMsg.enRslt                     = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    stRcvMsg.usErrorCause               = 0;

    /* 全局变量初始化  */

    /* MOCKER检测 */
    MOCKER(AT_GetModemIdFromClient)
        .stubs()
        .with( any(), outBoundP((MODEM_ID_ENUM_UINT16 *)&enModemId, (VOS_UINT32)sizeof(enModemId)))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((TAF_UINT32)AT_FAILURE));

    /* 执行测试函数 */
    ulRslt = AT_RcvMmaPhoneModeSetCnf(&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_ERR, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaPhoneModeSetCnf, Test_AT_RcvMmaPhoneModeSetCnf_003)
{
    /* 局部变量定义 */
    TAF_MMA_PHONE_MODE_SET_CNF_STRU     stRcvMsg;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    /* 局部变量初始化  */
    enModemId                           = MODEM_ID_0;
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId              = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid                = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid              = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                  = ID_TAF_MMA_PHONE_MODE_SET_CNF;
    stRcvMsg.usClientId                 = AT_BROADCAST_CLIENT_ID_MODEM_0;
    stRcvMsg.ucOpid                     = 2;
    stRcvMsg.ucPhMode                   = TAF_PH_MODE_FULL;
    stRcvMsg.enRslt                     = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    stRcvMsg.usErrorCause               = 0;

    /* 全局变量初始化  */

    /* 上报的CLIENTID索引为0 */
    gastAtClientTab[0].usClientId       = 11;
    gastAtClientTab[0].ucUsed           = AT_CLIENT_USED;

    /* MOCKER检测 */
    MOCKER(AT_GetModemIdFromClient)
        .stubs()
        .with( any(), outBoundP((MODEM_ID_ENUM_UINT16 *)&enModemId, (VOS_UINT32)sizeof(enModemId)))
        .will(returnValue((VOS_UINT32)VOS_OK));

    /* 执行测试函数 */
    ulRslt = AT_RcvMmaPhoneModeSetCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_ERR, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}

#ifdef FEATURE_UPGRADE_TL
TEST_F(Test_AT_RcvMmaPhoneModeSetCnf, Test_AT_RcvMmaPhoneModeSetCnf_004)
{
    /* 局部变量定义 */
    TAF_MMA_PHONE_MODE_SET_CNF_STRU     stRcvMsg;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    /* 局部变量初始化  */
    enModemId                           = MODEM_ID_0;
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId              = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid                = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid              = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                  = ID_TAF_MMA_PHONE_MODE_SET_CNF;
    stRcvMsg.usClientId                 = 11;
    stRcvMsg.ucOpid                     = 2;
    stRcvMsg.ucPhMode                   = TAF_PH_MODE_FULL;
    stRcvMsg.enRslt                     = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    stRcvMsg.usErrorCause               = 0;

    /* 全局变量初始化  */

    /* 上报的CLIENTID索引为0 */
    gastAtClientTab[0].usClientId       = 11;
    gastAtClientTab[0].ucUsed           = AT_CLIENT_USED;
    gastAtClientTab[0].CmdCurrentOpt    = AT_CMD_PSTANDBY_SET;

    /* MOCKER检测 */
    MOCKER(AT_GetModemIdFromClient)
        .stubs()
        .with( any(), outBoundP((MODEM_ID_ENUM_UINT16 *)&enModemId, (VOS_UINT32)sizeof(enModemId)))
        .will(returnValue((VOS_UINT32)VOS_OK));

    /* 执行测试函数 */
    ulRslt = AT_RcvMmaPhoneModeSetCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_OK, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
#endif

#if((FEATURE_ON == FEATURE_LTE) || (FEATURE_ON == FEATURE_UE_MODE_TDS))
TEST_F(Test_AT_RcvMmaPhoneModeSetCnf, Test_AT_RcvMmaPhoneModeSetCnf_005)
{
    /* 局部变量定义 */
    TAF_MMA_PHONE_MODE_SET_CNF_STRU     stRcvMsg;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    /* 局部变量初始化  */
    enModemId                           = MODEM_ID_0;
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId              = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid                = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid              = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                  = ID_TAF_MMA_PHONE_MODE_SET_CNF;
    stRcvMsg.usClientId                 = 11;
    stRcvMsg.ucOpid                     = 2;
    stRcvMsg.ucPhMode                   = TAF_PH_MODE_FULL;
    stRcvMsg.enRslt                     = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    stRcvMsg.usErrorCause               = 0;

    /* 全局变量初始化  */

    /* 上报的CLIENTID索引为0 */
    gastAtClientTab[0].usClientId       = 11;
    gastAtClientTab[0].ucUsed           = AT_CLIENT_USED;
    gastAtClientTab[0].CmdCurrentOpt    = AT_CMD_TMODE_SET;

    /* MOCKER检测 */
    MOCKER(AT_GetModemIdFromClient)
        .stubs()
        .with( any(), outBoundP((MODEM_ID_ENUM_UINT16 *)&enModemId, (VOS_UINT32)sizeof(enModemId)))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(AT_IsModemSupportRat)
        .stubs()
        .will(returnValue((VOS_UINT8)VOS_TRUE));

    MOCKER(AT_ProcOperModeWhenLteOn)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_OK));

    /* 执行测试函数 */
    ulRslt = AT_RcvMmaPhoneModeSetCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_OK, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMmaPhoneModeSetCnf, Test_AT_RcvMmaPhoneModeSetCnf_006)
{
    /* 局部变量定义 */
    TAF_MMA_PHONE_MODE_SET_CNF_STRU     stRcvMsg;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    /* 局部变量初始化  */
    enModemId                           = MODEM_ID_0;
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId              = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid                = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid              = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                  = ID_TAF_MMA_PHONE_MODE_SET_CNF;
    stRcvMsg.usClientId                 = 11;
    stRcvMsg.ucOpid                     = 2;
    stRcvMsg.ucPhMode                   = TAF_PH_MODE_FULL;
    stRcvMsg.enRslt                     = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    stRcvMsg.usErrorCause               = 0;

    /* 全局变量初始化  */

    /* 上报的CLIENTID索引为0 */
    gastAtClientTab[0].usClientId       = 11;
    gastAtClientTab[0].ucUsed           = AT_CLIENT_USED;
    gastAtClientTab[0].CmdCurrentOpt    = AT_CMD_SET_TMODE;

    /* MOCKER检测 */
    MOCKER(AT_GetModemIdFromClient)
        .stubs()
        .with( any(), outBoundP((MODEM_ID_ENUM_UINT16 *)&enModemId, (VOS_UINT32)sizeof(enModemId)))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(AT_IsModemSupportRat)
        .stubs()
        .will(returnValue((VOS_UINT8)VOS_FALSE));

    MOCKER(AT_IsModemSupportUtralTDDRat)
        .stubs()
        .will(returnValue((VOS_UINT8)VOS_TRUE));

    MOCKER(AT_ProcOperModeWhenLteOn)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)AT_OK));

    MOCKER(At_FormatResultData)
        .expects(never());

    /* 执行测试函数 */
    ulRslt = AT_RcvMmaPhoneModeSetCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_OK, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMmaPhoneModeSetCnf, Test_AT_RcvMmaPhoneModeSetCnf_007)
{
    /* 局部变量定义 */
    TAF_MMA_PHONE_MODE_SET_CNF_STRU     stRcvMsg;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    /* 局部变量初始化  */
    enModemId                           = MODEM_ID_0;
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId              = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid                = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid              = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                  = ID_TAF_MMA_PHONE_MODE_SET_CNF;
    stRcvMsg.usClientId                 = 11;
    stRcvMsg.ucOpid                     = 2;
    stRcvMsg.ucPhMode                   = TAF_PH_MODE_FULL;
    stRcvMsg.enRslt                     = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    stRcvMsg.usErrorCause               = 0;

    /* 全局变量初始化  */

    /* 上报的CLIENTID索引为0 */
    gastAtClientTab[0].usClientId       = 11;
    gastAtClientTab[0].ucUsed           = AT_CLIENT_USED;
    gastAtClientTab[0].CmdCurrentOpt    = AT_CMD_SET_TMODE;

    /* MOCKER检测 */
    MOCKER(AT_GetModemIdFromClient)
        .stubs()
        .with( any(), outBoundP((MODEM_ID_ENUM_UINT16 *)&enModemId, (VOS_UINT32)sizeof(enModemId)))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(AT_IsModemSupportRat)
        .stubs()
        .will(returnValue((VOS_UINT8)VOS_FALSE));

    MOCKER(AT_IsModemSupportUtralTDDRat)
        .stubs()
        .will(returnValue((VOS_UINT8)VOS_FALSE));

    MOCKER(AT_ProcOperModeWhenLteOn)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    /* 执行测试函数 */
    ulRslt = AT_RcvMmaPhoneModeSetCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_OK, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMmaPhoneModeSetCnf, Test_AT_RcvMmaPhoneModeSetCnf_008)
{
    /* 局部变量定义 */
    TAF_MMA_PHONE_MODE_SET_CNF_STRU     stRcvMsg;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;

    /* 局部变量初始化  */
    enModemId                           = MODEM_ID_0;
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId              = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid                = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid              = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                  = ID_TAF_MMA_PHONE_MODE_SET_CNF;
    stRcvMsg.usClientId                 = 11;
    stRcvMsg.ucOpid                     = 2;
    stRcvMsg.ucPhMode                   = TAF_PH_MODE_FULL;
    stRcvMsg.enRslt                     = TAF_MMA_APP_OPER_RESULT_FAILURE;
    stRcvMsg.usErrorCause               = TAF_ERR_ERROR;

    /* 全局变量初始化  */

    /* 上报的CLIENTID索引为0 */
    gastAtClientTab[0].usClientId       = 11;
    gastAtClientTab[0].ucUsed           = AT_CLIENT_USED;

    /* MOCKER检测 */
    MOCKER(AT_GetModemIdFromClient)
        .stubs()
        .with( any(), outBoundP((MODEM_ID_ENUM_UINT16 *)&enModemId, (VOS_UINT32)sizeof(enModemId)))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(AT_IsModemSupportRat)
        .stubs()
        .will(returnValue((VOS_UINT8)VOS_FALSE));

    MOCKER(AT_IsModemSupportUtralTDDRat)
        .stubs()
        .will(returnValue((VOS_UINT8)VOS_FALSE));

    MOCKER(AT_ProcOperModeWhenLteOn)
        .expects(never());

    MOCKER(At_ChgTafErrorCode)
        .stubs()
        .will(returnValue((TAF_UINT32)AT_CME_UNKNOWN));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    /* 执行测试函数 */
    ulRslt = AT_RcvMmaPhoneModeSetCnf(&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_OK, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
#endif
class Test_AT_RcvMmaDetachCnf: public ::testing::Test
{
public:
    void SetUp()
    {
        UT_STUB_INIT();
    }
    void TearDown()
    {
    }

};


TEST_F(Test_AT_RcvMmaDetachCnf, Test_AT_RcvMmaDetachCnf_001)
{
    /* 局部变量定义 */
    TAF_MMA_DETACH_CNF_STRU             stRcvMsg;
    VOS_UINT32                          ulRslt;

    /* 局部变量初始化 */
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid              = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId          = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid            = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                = ID_TAF_MMA_DETACH_CNF;
    stRcvMsg.usClientId               = 11;
    stRcvMsg.ucOpid                   = 2;
    stRcvMsg.aucReserve[0]            = 0;
    stRcvMsg.enRslt                   = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    stRcvMsg.usErrorCause             = 0;

    /* 全局变量初始化  */

    /* MOCKER检测 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((TAF_UINT32)AT_FAILURE));

    ulRslt = AT_RcvMmaDetachCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_ERR, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMmaDetachCnf, Test_AT_RcvMmaDetachCnf_002)
{
    /* 局部变量定义 */
    TAF_MMA_DETACH_CNF_STRU             stRcvMsg;
    VOS_UINT32                          ulRslt;

    /* 局部变量初始化  */
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId              = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid                = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid              = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                  = ID_TAF_MMA_DETACH_CNF;
    stRcvMsg.usClientId                 = AT_BROADCAST_CLIENT_ID_MODEM_0;
    stRcvMsg.ucOpid                     = 2;
    stRcvMsg.aucReserve[0]              = 0;
    stRcvMsg.enRslt                     = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    stRcvMsg.usErrorCause               = 0;

    /* 全局变量初始化  */

    /* 上报的CLIENTID索引为0 */
    gastAtClientTab[0].usClientId       = 11;
    gastAtClientTab[0].ucUsed           = AT_CLIENT_USED;

    /* MOCKER检测 */
    ulRslt = AT_RcvMmaDetachCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_ERR, ulRslt);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaDetachCnf, Test_AT_RcvMmaDetachCnf_003)
{
    /* 局部变量定义 */
    TAF_MMA_DETACH_CNF_STRU             stRcvMsg;
    VOS_UINT32                          ulRslt;

    /* 局部变量初始化  */
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId              = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid                = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid              = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                  = ID_TAF_MMA_DETACH_CNF;
    stRcvMsg.usClientId                 = 11;
    stRcvMsg.ucOpid                     = 2;
    stRcvMsg.aucReserve[0]              = 0;
    stRcvMsg.enRslt                     = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    stRcvMsg.usErrorCause               = 0;

    /* 全局变量初始化  */

    /* 上报的CLIENTID索引为0 */
    gastAtClientTab[0].usClientId       = 11;
    gastAtClientTab[0].ucUsed           = AT_CLIENT_USED;

    /* MOCKER检测 */
    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    ulRslt = AT_RcvMmaDetachCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_OK,   ulRslt);
    EXPECT_EQ(0,        gstAtSendData.usBufLen);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}



TEST_F(Test_AT_RcvMmaDetachCnf, Test_AT_RcvMmaDetachCnf_004)
{
    /* 局部变量定义 */
    TAF_MMA_DETACH_CNF_STRU             stRcvMsg;
    VOS_UINT32                          ulRslt;

    /* 局部变量初始化  */
    PS_MEM_SET(&stRcvMsg, 0X00, sizeof(stRcvMsg));
    stRcvMsg.ulSenderCpuId              = VOS_LOCAL_CPUID;
    stRcvMsg.ulSenderPid                = WUEPS_PID_MMA;
    stRcvMsg.ulReceiverCpuId            = VOS_LOCAL_CPUID;
    stRcvMsg.ulReceiverPid              = WUEPS_PID_AT;
    stRcvMsg.ulMsgName                  = ID_TAF_MMA_DETACH_CNF;
    stRcvMsg.usClientId                 = 11;
    stRcvMsg.ucOpid                     = 2;
    stRcvMsg.aucReserve[0]              = 0;
    stRcvMsg.enRslt                     = TAF_MMA_APP_OPER_RESULT_FAILURE;
    stRcvMsg.usErrorCause               = TAF_ERR_UNSPECIFIED_ERROR;

    /* 全局变量初始化  */

    /* 上报的CLIENTID索引为0 */
    gastAtClientTab[0].usClientId       = 11;
    gastAtClientTab[0].ucUsed           = AT_CLIENT_USED;

    /* MOCKER检测 */
    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    ulRslt = AT_RcvMmaDetachCnf((VOS_VOID *)&stRcvMsg);

    /* 结果检测 */
    EXPECT_EQ(VOS_OK,   ulRslt);
    EXPECT_EQ(0,        gstAtSendData.usBufLen);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}


class Test_AT_RcvMmaCmmSetCmdRsp: public ::testing::Test
{
public:
    MN_MM_TEST_CMD_RSP_MSG_STRU              stMsg;
    VOS_UINT8                                ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                  stElement;
    VOS_UINT8                                szCmdName[32];

    void SetUp()
    {
        UT_STUB_INIT();
        ucIndex                                 = 0;

        VOS_StrCpy((VOS_CHAR*)szCmdName,"^CMM");
        g_stParseContext[ucIndex].pstCmdElement = &stElement;
        stElement.pszCmdName                    = szCmdName;

        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_INVALID;
    }

    void TearDown()
    {
        ucIndex                                 = 0;
        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_INVALID;
    }
};
/*******************************************************************
测试用例编号      :Test_AT_RcvMmaCmmSetCmdRsp_001
测试用例标题      :无效的CLIENT_ID值
预期结果          :返回AT_ERROR
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvMmaCmmSetCmdRsp,Test_AT_RcvMmaCmmSetCmdRsp_001)
{
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((TAF_UINT32)AT_FAILURE));

    EXPECT_EQ(VOS_ERR,AT_RcvMmaCmmSetCmdRsp((VOS_VOID *)&stMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvMmaCmmSetCmdRsp_002
测试用例标题      :回复消息只是命令执行失败
预期结果          :返回VOS_OK
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvMmaCmmSetCmdRsp,Test_AT_RcvMmaCmmSetCmdRsp_002)
{
    stMsg.ulResult          = VOS_ERR;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP((VOS_UINT8 *)&ucIndex, (VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_OK,AT_RcvMmaCmmSetCmdRsp((VOS_VOID *)&stMsg));

    GlobalMockObject::verify();
}
/*******************************************************************
测试用例编号      :Test_AT_RcvMmaCmmSetCmdRsp_003
测试用例标题      :回复消息只是命令执行成功
预期结果          :返回VOS_OK
************************* 修改记录 *************************
#  1.日    期:2011年10月13日
#    作    者:c00173809
#    修改内容:新建CASE
*******************************************************************/
TEST_F(Test_AT_RcvMmaCmmSetCmdRsp,Test_AT_RcvMmaCmmSetCmdRsp_003)
{
    stMsg.ulResult              = VOS_OK;
    stMsg.stAtCmdRslt.ulRsltNum = 2;

    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP((VOS_UINT8 *)&ucIndex, (VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

 /*   MOCKER(At_sprintf)
        .expects(exactly(2));*/

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    EXPECT_EQ(VOS_OK,AT_RcvMmaCmmSetCmdRsp((VOS_VOID *)&stMsg));

    GlobalMockObject::verify();
}




class Test_AT_RcvMmaCrpnQueryRsp: public ::testing::Test
{
public:
    MN_MMA_CRPN_QRY_CNF_MSG_STRU       *pstMnMmaCrpnQryCnfMsg;
    MN_MMA_CRPN_QRY_CNF_STRU           *pstMnMmaCrpnQryCnf;
    VOS_UINT8                           ucIndex;

    void SetUp()
    {
        UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pstMnMmaCrpnQryCnfMsg = (MN_MMA_CRPN_QRY_CNF_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(MN_MMA_CRPN_QRY_CNF_MSG_STRU) );

        if (VOS_NULL_PTR == pstMnMmaCrpnQryCnfMsg)
        {
            return;
        }

        pstMnMmaCrpnQryCnf      = &pstMnMmaCrpnQryCnfMsg->stMnMmaCrpnQryCnf;

        PS_MEM_SET(pstMnMmaCrpnQryCnfMsg, 0, sizeof(MN_MMA_CRPN_QRY_CNF_MSG_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstMnMmaCrpnQryCnfMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMnMmaCrpnQryCnfMsg);
        }
    }
};


TEST_F(Test_AT_RcvMmaCrpnQueryRsp, Test_AT_RcvMmaCrpnQueryRsp_001)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;

    /* 变量初始化 */


    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((TAF_UINT32)AT_FAILURE));

    /* 调用被测函数 */
    ulRst = AT_RcvMmaCrpnQueryRsp((VOS_VOID *)pstMnMmaCrpnQryCnfMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMmaCrpnQueryRsp, Test_AT_RcvMmaCrpnQueryRsp_002)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = AT_RcvMmaCrpnQueryRsp((VOS_VOID *)pstMnMmaCrpnQryCnfMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_AT_RcvMmaCrpnQueryRsp, Test_AT_RcvMmaCrpnQueryRsp_003)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;

    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CRPN_QUERY;
    pstMnMmaCrpnQryCnfMsg->ulResult         = TAF_ERR_PB_WRONG_INDEX;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_CME_INVALID_INDEX));

    /* 调用被测函数 */
    ulRst = AT_RcvMmaCrpnQueryRsp((VOS_VOID *)pstMnMmaCrpnQryCnfMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMmaCrpnQueryRsp, Test_AT_RcvMmaCrpnQueryRsp_004)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    char                                acExpectedResult[] = "^CRPN: \"310090\",\"Edge\",\"Edge Wireless\"";
    TAF_UCHAR                           aucgpioInfo[TAF_MAX_GPIO_INFO_LEN + 1] = "abcd";
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       = {AT_CMD_CRPN,
                                                              At_SetCrpnPara,           AT_SET_PARA_TIME,
                                                              At_QryCrpnPara,           AT_NOT_SET_TIME,
                                                              At_CmdTestProcOK,         AT_NOT_SET_TIME,
                                                              VOS_NULL_PTR,   AT_NOT_SET_TIME,
                                                              AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
                                                              (VOS_UINT8*)"^CRPN",      (VOS_UINT8*)"(0-2),(PLMN)"};

    MN_MMA_CRPN_QRY_REQ_STRU            stMnMmaCrpnQry;
    VOS_UINT8                           autPlmnId[]        = "310090";
    VOS_UINT8                           aucPlmnName[]      = "Edge Wireless";
    VOS_UINT8                           aucPlmnNameShort[] = "Edge";

    /* 变量初始化 */
    PS_MEM_SET(&stMnMmaCrpnQry, 0, sizeof(MN_MMA_CRPN_QRY_REQ_STRU));

    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CRPN_QUERY;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstMnMmaCrpnQryCnfMsg->ulResult = VOS_OK;

    stMnMmaCrpnQry.ucPlmnType     = 2;
    stMnMmaCrpnQry.usPlmnNameLen  = sizeof(autPlmnId) - 1;
    PS_MEM_CPY(stMnMmaCrpnQry.aucPlmnName, autPlmnId, sizeof(autPlmnId));


    PS_MEM_CPY(&pstMnMmaCrpnQryCnf->stMnMmaCrpnQry, &stMnMmaCrpnQry, sizeof(MN_MMA_CRPN_QRY_REQ_STRU));
    pstMnMmaCrpnQryCnf->ucTotalNum  = 1;
    pstMnMmaCrpnQryCnf->ucMaxMncLen = 6;

    PS_MEM_CPY(pstMnMmaCrpnQryCnf->stOperNameList[0].aucOperatorNameLong, aucPlmnName, sizeof(aucPlmnName));
    PS_MEM_CPY(pstMnMmaCrpnQryCnf->stOperNameList[0].aucOperatorNameShort, aucPlmnNameShort, sizeof(aucPlmnNameShort));

    pstMnMmaCrpnQryCnf->stOperNameList[0].PlmnId.Mcc = 0xfffff268;
    pstMnMmaCrpnQryCnf->stOperNameList[0].PlmnId.Mnc = 0xfffff090;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMmaCrpnQueryRsp((VOS_VOID *)pstMnMmaCrpnQryCnfMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_AT_RcvMmaCrpnQueryRsp, Test_AT_RcvMmaCrpnQueryRsp_005)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    char                                acExpectedResult[] = "^CRPN: \"268090\",\"Edge\",\"Edge Wireless\"\r\n^CRPN: \"269091\",\"Edge\",\"Edge Wireless\"";
    TAF_UCHAR                           aucgpioInfo[TAF_MAX_GPIO_INFO_LEN + 1] = "abcd";
    AT_PAR_CMD_ELEMENT_STRU             stCmdElement       = {AT_CMD_CRPN,
                                                              At_SetCrpnPara,           AT_SET_PARA_TIME,
                                                              At_QryCrpnPara,           AT_NOT_SET_TIME,
                                                              At_CmdTestProcOK,         AT_NOT_SET_TIME,
                                                              VOS_NULL_PTR,             AT_NOT_SET_TIME,
                                                              AT_CME_INCORRECT_PARAMETERS, CMD_TBL_LIMITED_NULL,
                                                              (VOS_UINT8*)"^CRPN",      (VOS_UINT8*)"(0-2),(PLMN)"};

    MN_MMA_CRPN_QRY_REQ_STRU            stMnMmaCrpnQry;
    VOS_UINT8                           aucPlmnName[]      = "Edge Wireless";
    VOS_UINT8                           aucPlmnNameShort[] = "Edge";

    /* 变量初始化 */
    PS_MEM_SET(&stMnMmaCrpnQry, 0, sizeof(MN_MMA_CRPN_QRY_REQ_STRU));

    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CRPN_QUERY;
    g_stParseContext[ucIndex].pstCmdElement = &stCmdElement;

    pstMnMmaCrpnQryCnfMsg->ulResult = VOS_OK;

    stMnMmaCrpnQry.ucPlmnType     = 0;
    stMnMmaCrpnQry.usPlmnNameLen  = sizeof(aucPlmnName) - 1;
    PS_MEM_CPY(stMnMmaCrpnQry.aucPlmnName, aucPlmnName, sizeof(aucPlmnName));


    PS_MEM_CPY(&pstMnMmaCrpnQryCnf->stMnMmaCrpnQry, &stMnMmaCrpnQry, sizeof(MN_MMA_CRPN_QRY_REQ_STRU));
    pstMnMmaCrpnQryCnf->ucTotalNum  = 2;
    pstMnMmaCrpnQryCnf->ucMaxMncLen = 6;

    PS_MEM_CPY(pstMnMmaCrpnQryCnf->stOperNameList[0].aucOperatorNameLong, aucPlmnName, sizeof(aucPlmnName));
    PS_MEM_CPY(pstMnMmaCrpnQryCnf->stOperNameList[0].aucOperatorNameShort, aucPlmnNameShort, sizeof(aucPlmnNameShort));

    pstMnMmaCrpnQryCnf->stOperNameList[0].PlmnId.Mcc = 0xfffff268;
    pstMnMmaCrpnQryCnf->stOperNameList[0].PlmnId.Mnc = 0xfffff090;

    PS_MEM_CPY(pstMnMmaCrpnQryCnf->stOperNameList[1].aucOperatorNameLong, aucPlmnName, sizeof(aucPlmnName));
    PS_MEM_CPY(pstMnMmaCrpnQryCnf->stOperNameList[1].aucOperatorNameShort, aucPlmnNameShort, sizeof(aucPlmnNameShort));

    pstMnMmaCrpnQryCnf->stOperNameList[1].PlmnId.Mcc = 0xfffff269;
    pstMnMmaCrpnQryCnf->stOperNameList[1].PlmnId.Mnc = 0xfffff091;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = AT_RcvMmaCrpnQueryRsp((VOS_VOID *)pstMnMmaCrpnQryCnfMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);
    ASSERT_STREQ(acExpectedResult , (char *)pgucAtSndCodeAddr);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_At_ProcMsgFromCc: public ::testing::Test
{
public:
    MSG_HEADER_STRU                    *pstMsgHeader;

    void SetUp()
    {
        UT_STUB_INIT();

        /* 生成输入消息 */
        pstMsgHeader = (MSG_HEADER_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(MSG_HEADER_STRU));

        if (VOS_NULL_PTR == pstMsgHeader)
        {
            return;
        }

        PS_MEM_SET(pstMsgHeader, 0, sizeof(MSG_HEADER_STRU));

    }

    void TearDown()
    {

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstMsgHeader)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMsgHeader);
        }
    }
};


TEST_F(Test_At_ProcMsgFromCc,Test_At_ProcMsgFromCc_001)
{
    /* 变量定义 */

    /* 变量初始化 */
    pstMsgHeader->ulMsgName = AT_CC_MSG_STATE_QRY_CNF;

    /* 函数打桩 */
    MOCKER(At_RcvAtCcMsgStateQryCnfProc)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    /* 调用被测函数 */
    At_ProcMsgFromCc((VOS_VOID *)pstMsgHeader);

    /* 结果验证 */

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_At_ProcMsgFromCc,Test_At_ProcMsgFromCc_002)
{
    /* 变量定义 */

    /* 变量初始化 */
    pstMsgHeader->ulMsgName = AT_CC_MSG_BUTT;

    /* 函数打桩 */

    /* 调用被测函数 */
    At_ProcMsgFromCc((VOS_VOID *)pstMsgHeader);

    /* 结果验证 */

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_At_RcvAtCcMsgStateQryCnfProc: public ::testing::Test
{
public:
    AT_CC_STATE_QRY_CNF_MSG_STRU           *pstAtCcStateQryCnfMsg;
    VOS_UINT8                               ucIndex;

    void SetUp()
    {
        UT_STUB_INIT();
        ucIndex = 0;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 生成输入消息 */
        pstAtCcStateQryCnfMsg = (AT_CC_STATE_QRY_CNF_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_CC_STATE_QRY_CNF_MSG_STRU));

        if (VOS_NULL_PTR == pstAtCcStateQryCnfMsg)
        {
            return;
        }

        PS_MEM_SET(pstAtCcStateQryCnfMsg, 0, sizeof(AT_CC_STATE_QRY_CNF_MSG_STRU));

    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));

        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstAtCcStateQryCnfMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstAtCcStateQryCnfMsg);
        }
    }
};


TEST_F(Test_At_RcvAtCcMsgStateQryCnfProc, Test_At_RcvAtCcMsgStateQryCnfProc_001)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;

    /* 变量初始化 */


    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((TAF_UINT32)AT_FAILURE));

    /* 调用被测函数 */
    ulRst = At_RcvAtCcMsgStateQryCnfProc((VOS_VOID *)pstAtCcStateQryCnfMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_At_RcvAtCcMsgStateQryCnfProc, Test_At_RcvAtCcMsgStateQryCnfProc_002)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;


    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    /* 调用被测函数 */
    ulRst = At_RcvAtCcMsgStateQryCnfProc((VOS_VOID *)pstAtCcStateQryCnfMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_ERR, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();

}
TEST_F(Test_At_RcvAtCcMsgStateQryCnfProc, Test_At_RcvAtCcMsgStateQryCnfProc_003)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;

    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CC_STATE_QUERY;
    pstAtCcStateQryCnfMsg->ucCallNum        = 0;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_ERROR));

    /* 调用被测函数 */
    ulRst = At_RcvAtCcMsgStateQryCnfProc((VOS_VOID *)pstAtCcStateQryCnfMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_At_RcvAtCcMsgStateQryCnfProc, Test_At_RcvAtCcMsgStateQryCnfProc_004)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    VOS_UINT16                          usLength;

    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CC_STATE_QUERY;
    pstAtCcStateQryCnfMsg->ucCallNum        = 1;
    usLength                                = 0;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(AT_ShowCccRst)
        .stubs()
        .with(any(), outBoundP((VOS_UINT16 *)&usLength,(VOS_UINT32)sizeof(usLength)));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_ERROR));

    /* 调用被测函数 */
    ulRst = At_RcvAtCcMsgStateQryCnfProc((VOS_VOID *)pstAtCcStateQryCnfMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
TEST_F(Test_At_RcvAtCcMsgStateQryCnfProc, Test_At_RcvAtCcMsgStateQryCnfProc_005)
{
    /* 变量定义 */
    VOS_UINT32                          ulRst;
    VOS_UINT16                          usLength;

    /* 变量初始化 */
    gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_CC_STATE_QUERY;
    pstAtCcStateQryCnfMsg->ucCallNum        = 1;
    usLength                                = 1;

    /* 函数打桩 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(),outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(AT_ShowCccRst)
        .stubs()
        .with(any(), outBoundP((VOS_UINT16 *)&usLength,(VOS_UINT32)sizeof(usLength)));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用被测函数 */
    ulRst = At_RcvAtCcMsgStateQryCnfProc((VOS_VOID *)pstAtCcStateQryCnfMsg);

    /* 结果验证 */
    ASSERT_EQ(VOS_OK, ulRst);

    //检查调用时，需要在用例结束位置加上这句
    GlobalMockObject::verify();
}
class Test_AT_RcvMmaCipherInfoQueryCnf: public ::testing::Test
{
public:
    void SetUp()
    {
        UT_STUB_INIT();
        PS_MEM_SET(&stCipherQryCnf, 0, sizeof(MN_MMA_CIPHER_QRY_CNF_STRU));
        stCipherQryCnf.stAtAppCtrl.ucOpId       = 0;
        stCipherQryCnf.stAtAppCtrl.usClientId   = 0;
        stCipherQryCnf.ulSenderPid              = WUEPS_PID_MMA;
        stCipherQryCnf.ulReceiverPid            = WUEPS_PID_AT;
        stCipherQryCnf.ulMsgName                = AT_MMA_CIPHER_INFO_QUERY_CNF;

        PS_MEM_SET(&stCmdInfo, 0, sizeof(AT_PAR_CMD_ELEMENT_STRU));
        stCmdInfo.ulCmdIndex        = AT_CMD_CIPHERQRY;
        stCmdInfo.pfnSetProc        = VOS_NULL_PTR;
        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = AT_QryCipherPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_LIMITED_NULL;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^CIPHERQRY";
        stCmdInfo.pszParam          = VOS_NULL_PTR;

        ucIndex = 0;

        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
    }
    void TearDown()
    {
    }
protected:
    VOS_UINT8                           ucIndex;
    MN_MMA_CIPHER_QRY_CNF_STRU          stCipherQryCnf;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;
};


TEST_F(Test_AT_RcvMmaCipherInfoQueryCnf, Test_AT_RcvMmaCipherInfoQueryCnf_001)
{
    VOS_CHAR                           *pcExpactedStr;

    /* 初始化变量 */
    stCipherQryCnf.ulResult             = VOS_OK;
    stCipherQryCnf.ucCipherInfo         = 2;
    pcExpactedStr                       = "^CIPHERQRY:2";

    /* 初始化全局变量 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CIPERQRY_READ;

    /* 桩函数 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用消息入口函数 */
    At_MsgProc((MsgBlock *)&stCipherQryCnf);

    /* 结果验证*/
    EXPECT_EQ(VOS_StrLen(pcExpactedStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcExpactedStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    /* 检查调用时, 需要在用例结束位置加上这句 */
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaCipherInfoQueryCnf, Test_AT_RcvMmaCipherInfoQueryCnf_002)
{
    VOS_UINT32                          ulResult;

    /* 初始化变量 */
    ulResult                            = VOS_NULL;
    stCipherQryCnf.ulResult             = VOS_OK;
    stCipherQryCnf.ucCipherInfo         = 2;

    /* 初始化全局变量 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CIPERQRY_READ;

    /* 桩函数 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    /* 调用消息入口函数 */
    ulResult = AT_RcvMmaCipherInfoQueryCnf((VOS_VOID *)&stCipherQryCnf);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulResult);

    /* 检查调用时, 需要在用例结束位置加上这句 */
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaCipherInfoQueryCnf, Test_AT_RcvMmaCipherInfoQueryCnf_003)
{
    VOS_UINT32                          ulResult;

    /* 初始化变量 */
    ulResult                            = VOS_NULL;
    stCipherQryCnf.ulResult             = VOS_OK;
    stCipherQryCnf.ucCipherInfo         = 2;

    /* 初始化全局变量 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CIPERQRY_READ;

    /* 桩函数 */
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    /* 调用消息入口函数 */
    ulResult = AT_RcvMmaCipherInfoQueryCnf((VOS_VOID *)&stCipherQryCnf);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulResult);

    /* 检查调用时, 需要在用例结束位置加上这句 */
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaCipherInfoQueryCnf, Test_AT_RcvMmaCipherInfoQueryCnf_004)
{
    VOS_UINT32                          ulResult;

    /* 初始化变量 */
    ulResult                            = VOS_NULL;
    stCipherQryCnf.ulResult             = VOS_OK;
    stCipherQryCnf.ucCipherInfo         = 2;

    /* 初始化全局变量 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    /* 桩函数 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    /* 调用消息入口函数 */
    ulResult = AT_RcvMmaCipherInfoQueryCnf((VOS_VOID *)&stCipherQryCnf);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulResult);

    /* 检查调用时, 需要在用例结束位置加上这句 */
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaCipherInfoQueryCnf, Test_AT_RcvMmaCipherInfoQueryCnf_005)
{
    VOS_UINT32                          ulResult;

    /* 初始化变量 */
    ulResult                            = VOS_NULL;
    stCipherQryCnf.ulResult             = VOS_ERROR;
    stCipherQryCnf.ucCipherInfo         = 2;

    /* 初始化全局变量 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_CIPERQRY_READ;

    /* 桩函数 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue((VOS_UINT32)VOS_OK));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_ERROR));

    /* 调用消息入口函数 */
    ulResult = AT_RcvMmaCipherInfoQueryCnf((VOS_VOID *)&stCipherQryCnf);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulResult);

    /* 检查调用时, 需要在用例结束位置加上这句 */
    GlobalMockObject::verify();
}


class Test_AT_RcvMmaLocInfoQueryCnf: public ::testing::Test
{
public:
    void SetUp()
    {
        UT_STUB_INIT();
        PS_MEM_SET(&stLocInfoCnf, 0, sizeof(MN_MMA_LOCATION_INFO_CNF_STRU));
        stLocInfoCnf.stAtAppCtrl.ucOpId       = 0;
        stLocInfoCnf.stAtAppCtrl.usClientId   = 0;
        stLocInfoCnf.ulSenderPid              = WUEPS_PID_MMA;
        stLocInfoCnf.ulReceiverPid            = WUEPS_PID_AT;
        stLocInfoCnf.ulMsgName                = AT_MMA_LOCATION_INFO_QUERY_CNF;

        PS_MEM_SET(&stCmdInfo, 0, sizeof(AT_PAR_CMD_ELEMENT_STRU));
        stCmdInfo.ulCmdIndex        = AT_CMD_LOCINFO;
        stCmdInfo.pfnSetProc        = VOS_NULL_PTR;
        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = AT_QryLocinfoPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_LIMITED_NULL;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^LOCINFO";
        stCmdInfo.pszParam          = VOS_NULL_PTR;

        ucIndex = 0;

        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;
    }
    void TearDown()
    {
    }
protected:
    VOS_UINT8                           ucIndex;
    MN_MMA_LOCATION_INFO_CNF_STRU       stLocInfoCnf;
    AT_PAR_CMD_ELEMENT_STRU             stCmdInfo;
};


TEST_F(Test_AT_RcvMmaLocInfoQueryCnf, Test_AT_RcvMmaLocInfoQueryCnf_001)
{
    VOS_CHAR                           *pcExpactedStr;

    /* 初始化变量 */
    stLocInfoCnf.ulResult               = VOS_OK;
    stLocInfoCnf.ulCellid               = 2;
    stLocInfoCnf.ulMcc                  = 24;
    stLocInfoCnf.ulMnc                  = 6;
    stLocInfoCnf.ucRac                  = 5;
    stLocInfoCnf.usLac                  = 4;

    pcExpactedStr                       = "^LOCINFO:800600,0x4,0x5,0x2";

    /* 初始化全局变量 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_LOCINFO_READ;

    /* 桩函数 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .with(eq((VOS_UINT32)ucIndex), any())
        .will(returnValue(VOS_UINT32(VOS_OK)));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_OK));

    /* 调用消息入口函数 */
    At_MsgProc((MsgBlock *)&stLocInfoCnf);

    /* 结果验证*/
    EXPECT_EQ(VOS_StrLen(pcExpactedStr), gstAtSendData.usBufLen);
    ASSERT_STREQ(pcExpactedStr, (TAF_CHAR *)pgucAtSndCodeAddr);

    /* 检查调用时, 需要在用例结束位置加上这句 */
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaLocInfoQueryCnf, Test_AT_RcvMmaLocInfoQueryCnf_002)
{
    VOS_UINT32                          ulResult;

    /* 初始化变量 */
    stLocInfoCnf.ulResult               = VOS_OK;
    stLocInfoCnf.ulCellid               = 2;
    stLocInfoCnf.ulMcc                  = 24;
    stLocInfoCnf.ulMnc                  = 6;
    stLocInfoCnf.ucRac                  = 5;
    stLocInfoCnf.usLac                  = 4;

    /* 初始化全局变量 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_LOCINFO_READ;

    /* 桩函数 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_FAILURE));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    MOCKER(At_FormatResultData)
        .expects(never());


    /* 调用消息入口函数 */
    ulResult = AT_RcvMmaLocInfoQueryCnf((VOS_VOID *)&stLocInfoCnf);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulResult);

    /* 检查调用时, 需要在用例结束位置加上这句 */
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaLocInfoQueryCnf, Test_AT_RcvMmaLocInfoQueryCnf_003)
{
    VOS_UINT32                          ulResult;

    /* 初始化变量 */
    stLocInfoCnf.ulResult               = VOS_OK;
    stLocInfoCnf.ulCellid               = 2;
    stLocInfoCnf.ulMcc                  = 24;
    stLocInfoCnf.ulMnc                  = 6;
    stLocInfoCnf.ucRac                  = 5;
    stLocInfoCnf.usLac                  = 4;

    /* 初始化全局变量 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_LOCINFO_READ;

    /* 桩函数 */
    ucIndex = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    /* 调用消息入口函数 */
    ulResult = AT_RcvMmaLocInfoQueryCnf((VOS_VOID *)&stLocInfoCnf);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulResult);

    /* 检查调用时, 需要在用例结束位置加上这句 */
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaLocInfoQueryCnf, Test_AT_RcvMmaLocInfoQueryCnf_004)
{
    VOS_UINT32                          ulResult;

    /* 初始化变量 */
    stLocInfoCnf.ulResult               = VOS_OK;
    stLocInfoCnf.ulCellid               = 2;
    stLocInfoCnf.ulMcc                  = 24;
    stLocInfoCnf.ulMnc                  = 6;
    stLocInfoCnf.ucRac                  = 5;
    stLocInfoCnf.usLac                  = 4;

    /* 初始化全局变量 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    /* 桩函数 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(never());

    /* 调用消息入口函数 */
    ulResult = AT_RcvMmaLocInfoQueryCnf((VOS_VOID *)&stLocInfoCnf);

    /* 结果验证*/
    EXPECT_EQ(VOS_ERR, ulResult);

    /* 检查调用时, 需要在用例结束位置加上这句 */
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaLocInfoQueryCnf, Test_AT_RcvMmaLocInfoQueryCnf_005)
{
    VOS_UINT32                          ulResult;

    /* 初始化变量 */
    stLocInfoCnf.ulResult               = VOS_ERROR;
    stLocInfoCnf.ulCellid               = 2;
    stLocInfoCnf.ulMcc                  = 24;
    stLocInfoCnf.ulMnc                  = 6;
    stLocInfoCnf.ucRac                  = 5;
    stLocInfoCnf.usLac                  = 4;

    /* 初始化全局变量 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_LOCINFO_READ;

    /* 桩函数 */
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(AT_StopRelTimer)
        .expects(exactly(1))
        .will(returnValue(VOS_UINT32(VOS_OK)));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_ERROR));

    /* 调用消息入口函数 */
    ulResult = AT_RcvMmaLocInfoQueryCnf((VOS_VOID *)&stLocInfoCnf);

    /* 结果验证*/
    EXPECT_EQ(VOS_OK, ulResult);

    /* 检查调用时, 需要在用例结束位置加上这句 */
    GlobalMockObject::verify();
}




class Test_AT_RcvMmaAcInfoQueryCnf: public ::testing::Test
{
public:
    VOS_UINT8                               ucIndex;
    AT_PAR_CMD_ELEMENT_STRU                 stCmdInfo;
    AT_MTA_MSG_STRU                        *pstMsg;
    VOS_UINT32                              ulMsgLen;
    TAF_MMA_AC_INFO_QUERY_CNF_STRU         *pstAcInfoQueryCnf;

    void SetUp()
    {
        UT_STUB_INIT();
        ucIndex                     = AT_CLIENT_ID_PCUI;

        stCmdInfo.ulCmdIndex        = AT_CMD_ACINFO;
        stCmdInfo.pfnSetProc        = VOS_NULL_PTR;
        stCmdInfo.ulSetTimeOut      = AT_NOT_SET_TIME;
        stCmdInfo.pfnQryProc        = AT_QryAcInfoPara;
        stCmdInfo.ulTestTimeOut     = AT_QRY_PARA_TIME;
        stCmdInfo.pfnTestProc       = VOS_NULL_PTR;
        stCmdInfo.ulTestTimeOut     = AT_NOT_SET_TIME;
        stCmdInfo.ulParamErrCode    = AT_CME_INCORRECT_PARAMETERS;
        stCmdInfo.ulChkFlag         = CMD_TBL_LIMITED_NULL;
        stCmdInfo.pszCmdName        = (VOS_UINT8*)"^ACINFO";
        stCmdInfo.pszParam          = VOS_NULL_PTR;

        ulMsgLen    = sizeof(AT_MTA_MSG_STRU) - 4 + sizeof(TAF_MMA_AC_INFO_QUERY_CNF_STRU);
        pstMsg      = (AT_MTA_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT, ulMsgLen);
        PS_MEM_SET(pstMsg, 0, ulMsgLen);

        pstMsg->ulReceiverPid           = WUEPS_PID_AT;
        pstMsg->ulSenderPid             = WUEPS_PID_MMA;
        pstMsg->ulMsgId                 = AT_MMA_AC_INFO_QUERY_CNF;
        pstMsg->stAppCtrl.usClientId    = AT_CLIENT_TAB_PCUI_INDEX;

        pstAcInfoQueryCnf    = (TAF_MMA_AC_INFO_QUERY_CNF_STRU *)pstMsg->aucContent;
        pstAcInfoQueryCnf->stAtAppCtrl.usClientId         = AT_CLIENT_TAB_PCUI_INDEX;
        pstAcInfoQueryCnf->stAtAppCtrl.ucOpId             = 1;
        pstAcInfoQueryCnf->stCellCsAcInfo.enCellAcType    = 0;
        pstAcInfoQueryCnf->stCellCsAcInfo.ucSrvDomain     = 0;
        pstAcInfoQueryCnf->stCellCsAcInfo.ucRestrictPagingRsp = 0;
        pstAcInfoQueryCnf->stCellCsAcInfo.ucRestrictRegister  = 1;
        pstAcInfoQueryCnf->stCellPsAcInfo.enCellAcType    = 0;
        pstAcInfoQueryCnf->stCellPsAcInfo.ucSrvDomain     = 0;
        pstAcInfoQueryCnf->stCellPsAcInfo.ucRestrictPagingRsp = 0;
        pstAcInfoQueryCnf->stCellPsAcInfo.ucRestrictRegister  = 0;
        pstAcInfoQueryCnf->ulRslt                             = TAF_ERR_NO_ERROR;

        gastAtClientTab[ucIndex].CmdCurrentOpt  = AT_CMD_ACINFO_READ;
        gastAtClientTab[ucIndex].usClientId     = AT_CLIENT_TAB_PCUI_INDEX;
        gastAtClientTab[ucIndex].ucUsed         = AT_CLIENT_USED;
        g_stParseContext[ucIndex].pstCmdElement = &stCmdInfo;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
    }

    void TearDown()
    {
        /* 释放消息内存 */
        if (VOS_NULL_PTR != pstMsg)
        {
            PS_MEM_FREE(WUEPS_PID_AT, pstMsg);
        }

        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;
        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
    }
};
TEST_F(Test_AT_RcvMmaAcInfoQueryCnf, Test_AT_RcvMmaAcInfoQueryCnf_001)
{
   // 变量声明

    // 参数初始化
    pstAcInfoQueryCnf->ulRslt                             = TAF_ERR_ERROR;

    // 调用被测函数
    AT_RcvMmaAcInfoQueryCnf((VOS_VOID *)pstAcInfoQueryCnf);

    // 执行检查
    EXPECT_EQ(AT_FW_CLIENT_STATUS_READY,g_stParseContext[ucIndex].ucClientStatus);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT,gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaAcInfoQueryCnf, Test_AT_RcvMmaAcInfoQueryCnf_002)
{
    // 变量声明
    char                               aucRslt[] = "^ACINFO:0,0,1,0\r\n^ACINFO:0,0,0,0\r\n\r\nOK\r\n";                /*回车换行*/

    // 调用被测函数
    AT_RcvMmaAcInfoQueryCnf((VOS_VOID *)pstAcInfoQueryCnf);

    // 执行检查
    ASSERT_STREQ(aucRslt , (char *)pgucAtSndCodeAddr);
    EXPECT_EQ(AT_FW_CLIENT_STATUS_READY,g_stParseContext[ucIndex].ucClientStatus);
    EXPECT_EQ(AT_CMD_CURRENT_OPT_BUTT,gastAtClientTab[ucIndex].CmdCurrentOpt);

    GlobalMockObject::verify();
}
class Test_AT_RcvMmaCopnInfoQueryCnf: public ::testing::Test
{
public:
    VOS_UINT8                           ucIndex;
    AT_PAR_CMD_ELEMENT_STRU             stElement;
    VOS_UINT8                           szCmdName[32];
    TAF_MMA_COPN_INFO_QUERY_CNF_STRU   *pstCopnInfoQryCnf;

    void SetUp()
    {
        UT_STUB_INIT();
        ucIndex = 1;

        VOS_StrCpy((VOS_CHAR*)szCmdName,"COPN");
        g_stParseContext[ucIndex].pstCmdElement = &stElement;
        stElement.pszCmdName                    = szCmdName;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
        PS_MEM_SET(gstAtCombineSendData, 0, (sizeof(AT_SEND_DATA_BUFFER_STRU) * AT_MAX_CLIENT_NUM));
    }

    void TearDown()
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

        gstAtSendData.usBufLen = 0;
        PS_MEM_SET(gstAtSendData.aucBuffer, 0, sizeof(gstAtSendData.aucBuffer));
        free(pstCopnInfoQryCnf);
    }
};


TEST_F(Test_AT_RcvMmaCopnInfoQueryCnf, Test_AT_RcvMmaCopnInfoQueryCnf_001)
{

    VOS_UINT32                          ulMsgSize;
    TAF_PH_OPERATOR_NAME_STRU          *pstOperNameList   = VOS_NULL_PTR;
    VOS_CHAR                           *pcPrintStr = "COPN: \"208002\",\"F - Contact\"\r\nCOPN: \"206001\",\"Proximus\"\r\n\r\nOK\r\n";

    ulMsgSize  = sizeof(TAF_MMA_COPN_INFO_QUERY_CNF_STRU) - sizeof(pstCopnInfoQryCnf->aucContent);
    ulMsgSize += 2 * sizeof(TAF_PH_OPERATOR_NAME_STRU);

    pstCopnInfoQryCnf = (TAF_MMA_COPN_INFO_QUERY_CNF_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstCopnInfoQryCnf, 0x0, ulMsgSize);

    pstCopnInfoQryCnf->ulMsgName      = AT_MMA_COPN_INFO_QUERY_CNF;
    pstCopnInfoQryCnf->usFromIndex    = 51;
    pstCopnInfoQryCnf->usPlmnNum      = 2;

    pstCopnInfoQryCnf->ulReceiverPid  = WUEPS_PID_AT;
    pstCopnInfoQryCnf->ulSenderPid    = WUEPS_PID_MMA;

    pstCopnInfoQryCnf->stAtAppCtrl.ucOpId     = 0;
    pstCopnInfoQryCnf->stAtAppCtrl.usClientId = 110;

    pstCopnInfoQryCnf->ulLength       = ulMsgSize;
    pstOperNameList                 = (TAF_PH_OPERATOR_NAME_STRU *)pstCopnInfoQryCnf->aucContent;

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameShort,
                   "Contact ",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameLong,
                   "F - Contact",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[0].PlmnId.Mcc = 0x208;
    pstOperNameList[0].PlmnId.Mnc = 0x002;

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameShort,
                   "Proximus",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameLong,
                   "Proximus",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[1].PlmnId.Mcc = 0x206;
    pstOperNameList[1].PlmnId.Mnc = 0x001;

    // 参数初始化
    gastAtClientTab[1].usClientId       = 110;
    gastAtClientTab[1].ucUsed           = AT_CLIENT_USED;
    gastAtClientTab[1].CmdCurrentOpt    = AT_CMD_COPN_QRY;

    // 调用被测函数
    At_MmaMsgProc((MSG_HEADER_STRU*)pstCopnInfoQryCnf);
    EXPECT_EQ(0, gstAtSendData.usBufLen);
    ASSERT_STREQ(pcPrintStr, (TAF_CHAR *)gstAtCombineSendData[1].aucBuffer);

    // 执行检查
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaCopnInfoQueryCnf, Test_AT_RcvMmaCopnInfoQueryCnf_002)
{

    VOS_UINT32                          ulMsgSize;
    TAF_PH_OPERATOR_NAME_STRU          *pstOperNameList   = VOS_NULL_PTR;
    VOS_CHAR                           *pcPrintStr = "COPN: \"208002\",\"F - Contact\"\r\nCOPN: \"206001\",\"Proximus\"\r\n\r\nOK\r\n";
    TAF_MMA_COPN_INFO_QUERY_REQ_STRU    stCopnInfo;

    ulMsgSize  = sizeof(TAF_MMA_COPN_INFO_QUERY_CNF_STRU) - sizeof(pstCopnInfoQryCnf->aucContent);
    ulMsgSize += 2 * sizeof(TAF_PH_OPERATOR_NAME_STRU);

    pstCopnInfoQryCnf = (TAF_MMA_COPN_INFO_QUERY_CNF_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstCopnInfoQryCnf, 0x0, ulMsgSize);

    pstCopnInfoQryCnf->ulMsgName      = AT_MMA_COPN_INFO_QUERY_CNF;
    pstCopnInfoQryCnf->usFromIndex    = 51;
    pstCopnInfoQryCnf->usPlmnNum      = 2;

    pstCopnInfoQryCnf->ulReceiverPid  = WUEPS_PID_AT;
    pstCopnInfoQryCnf->ulSenderPid    = WUEPS_PID_MMA;

    pstCopnInfoQryCnf->stAtAppCtrl.ucOpId     = 0;
    pstCopnInfoQryCnf->stAtAppCtrl.usClientId = 110;

    pstCopnInfoQryCnf->ulLength       = ulMsgSize;
    pstOperNameList                 = (TAF_PH_OPERATOR_NAME_STRU *)pstCopnInfoQryCnf->aucContent;

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameShort,
                   "Contact ",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameLong,
                   "F - Contact",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[0].PlmnId.Mcc = 0x208;
    pstOperNameList[0].PlmnId.Mnc = 0x002;

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameShort,
                   "Proximus",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameLong,
                   "Proximus",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[1].PlmnId.Mcc = 0x206;
    pstOperNameList[1].PlmnId.Mnc = 0x001;

    // 参数初始化
    gastAtClientTab[1].usClientId       = 110;
    gastAtClientTab[1].ucUsed           = AT_CLIENT_USED;
    gastAtClientTab[1].CmdCurrentOpt    = AT_CMD_COPN_QRY;

    stCopnInfo.usFromIndex  = 101;
    stCopnInfo.usPlmnNum    = 50;

    MOCKER(AT_FillAndSndAppReqMsg)
        .stubs()
        .with(any(), any(),any(),mirror((TAF_MMA_COPN_INFO_QUERY_REQ_STRU *)&stCopnInfo, (VOS_UINT32)sizeof(TAF_MMA_COPN_INFO_QUERY_REQ_STRU)), any(),any())
        .will(returnValue((VOS_UINT32)TAF_SUCCESS));

    // 调用被测函数
    At_MmaMsgProc((MSG_HEADER_STRU*)pstCopnInfoQryCnf);

    // 执行检查
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaCopnInfoQueryCnf, Test_AT_RcvMmaCopnInfoQueryCnf_003)
{

    VOS_UINT32                          ulMsgSize;
    TAF_PH_OPERATOR_NAME_STRU          *pstOperNameList   = VOS_NULL_PTR;
    VOS_CHAR                           *pcPrintStr = "COPN: \"208002\",\"F - Contact\"\r\nCOPN: \"206001\",\"Proximus\"\r\n\r\nOK\r\n";

    ulMsgSize  = sizeof(TAF_MMA_COPN_INFO_QUERY_CNF_STRU) - sizeof(pstCopnInfoQryCnf->aucContent);
    ulMsgSize += 50 * sizeof(TAF_PH_OPERATOR_NAME_STRU);

    pstCopnInfoQryCnf = (TAF_MMA_COPN_INFO_QUERY_CNF_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstCopnInfoQryCnf, 0x0, ulMsgSize);

    pstCopnInfoQryCnf->ulMsgName      = AT_MMA_COPN_INFO_QUERY_CNF;
    pstCopnInfoQryCnf->usFromIndex    = 51;
    pstCopnInfoQryCnf->usPlmnNum      = 50;

    pstCopnInfoQryCnf->ulReceiverPid  = WUEPS_PID_AT;
    pstCopnInfoQryCnf->ulSenderPid    = WUEPS_PID_MMA;

    pstCopnInfoQryCnf->stAtAppCtrl.ucOpId     = 0;
    pstCopnInfoQryCnf->stAtAppCtrl.usClientId = 110;

    pstCopnInfoQryCnf->ulLength       = ulMsgSize;
    pstOperNameList                 = (TAF_PH_OPERATOR_NAME_STRU *)pstCopnInfoQryCnf->aucContent;

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameShort,
                   "Contact ",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameLong,
                   "F - Contact",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[0].PlmnId.Mcc = 0x208;
    pstOperNameList[0].PlmnId.Mnc = 0x002;

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameShort,
                   "Proximus",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameLong,
                   "Proximus",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[1].PlmnId.Mcc = 0x206;
    pstOperNameList[1].PlmnId.Mnc = 0x001;

    // 参数初始化
    gastAtClientTab[1].usClientId       = 110;
    gastAtClientTab[1].ucUsed           = AT_CLIENT_USED;
    gastAtClientTab[1].CmdCurrentOpt    = AT_CMD_COPN_QRY;


    MOCKER(AT_FillAndSndAppReqMsg)
        .stubs()
        .will(returnValue((VOS_UINT32)TAF_FAILURE));

    MOCKER(At_FormatResultData)
        .stubs()
        .with(any(), eq((VOS_UINT32)AT_ERROR));

    // 调用被测函数
    At_MmaMsgProc((MSG_HEADER_STRU*)pstCopnInfoQryCnf);

    EXPECT_EQ(0, gstAtSendData.usBufLen);

    // 执行检查
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaCopnInfoQueryCnf, Test_AT_RcvMmaCopnInfoQueryCnf_004)
{

    VOS_UINT32                          ulMsgSize;
    TAF_PH_OPERATOR_NAME_STRU          *pstOperNameList   = VOS_NULL_PTR;
    VOS_CHAR                           *pcPrintStr = "COPN: \"208002\",\"F - Contact\"\r\nCOPN: \"206001\",\"Proximus\"\r\n\r\nOK\r\n";
    TAF_MMA_COPN_INFO_QUERY_REQ_STRU    stCopnInfo;
    VOS_UINT32                          ulRet;

    ulMsgSize  = sizeof(TAF_MMA_COPN_INFO_QUERY_CNF_STRU) - sizeof(pstCopnInfoQryCnf->aucContent);
    ulMsgSize += 2 * sizeof(TAF_PH_OPERATOR_NAME_STRU);

    pstCopnInfoQryCnf = (TAF_MMA_COPN_INFO_QUERY_CNF_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstCopnInfoQryCnf, 0x0, ulMsgSize);

    pstCopnInfoQryCnf->ulMsgName      = AT_MMA_COPN_INFO_QUERY_CNF;
    pstCopnInfoQryCnf->usFromIndex    = 51;
    pstCopnInfoQryCnf->usPlmnNum      = 2;

    pstCopnInfoQryCnf->ulReceiverPid  = WUEPS_PID_AT;
    pstCopnInfoQryCnf->ulSenderPid    = WUEPS_PID_MMA;

    pstCopnInfoQryCnf->stAtAppCtrl.ucOpId     = 0;
    pstCopnInfoQryCnf->stAtAppCtrl.usClientId = 110;

    pstCopnInfoQryCnf->ulLength       = ulMsgSize;
    pstOperNameList                 = (TAF_PH_OPERATOR_NAME_STRU *)pstCopnInfoQryCnf->aucContent;

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameShort,
                   "Contact ",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameLong,
                   "F - Contact",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[0].PlmnId.Mcc = 0x208;
    pstOperNameList[0].PlmnId.Mnc = 0x002;

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameShort,
                   "Proximus",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameLong,
                   "Proximus",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[1].PlmnId.Mcc = 0x206;
    pstOperNameList[1].PlmnId.Mnc = 0x001;

    // 参数初始化
    gastAtClientTab[1].usClientId       = 110;
    gastAtClientTab[1].ucUsed           = AT_CLIENT_NULL;
    gastAtClientTab[1].CmdCurrentOpt    = AT_CMD_COPN_QRY;

    stCopnInfo.usFromIndex  = 101;
    stCopnInfo.usPlmnNum    = 50;

    // 调用被测函数
    ulRet = AT_RcvMmaCopnInfoQueryCnf((VOS_VOID*)pstCopnInfoQryCnf);

    EXPECT_EQ(VOS_ERR, ulRet);

    // 执行检查
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaCopnInfoQueryCnf, Test_AT_RcvMmaCopnInfoQueryCnf_005)
{

    VOS_UINT32                          ulMsgSize;
    TAF_PH_OPERATOR_NAME_STRU          *pstOperNameList   = VOS_NULL_PTR;
    VOS_CHAR                           *pcPrintStr = "COPN: \"208002\",\"F - Contact\"\r\nCOPN: \"206001\",\"Proximus\"\r\n\r\nOK\r\n";
    TAF_MMA_COPN_INFO_QUERY_REQ_STRU    stCopnInfo;
    VOS_UINT32                          ulRet;

    ulMsgSize  = sizeof(TAF_MMA_COPN_INFO_QUERY_CNF_STRU) - sizeof(pstCopnInfoQryCnf->aucContent);
    ulMsgSize += 2 * sizeof(TAF_PH_OPERATOR_NAME_STRU);

    pstCopnInfoQryCnf = (TAF_MMA_COPN_INFO_QUERY_CNF_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstCopnInfoQryCnf, 0x0, ulMsgSize);

    pstCopnInfoQryCnf->ulMsgName      = AT_MMA_COPN_INFO_QUERY_CNF;
    pstCopnInfoQryCnf->usFromIndex    = 51;
    pstCopnInfoQryCnf->usPlmnNum      = 2;

    pstCopnInfoQryCnf->ulReceiverPid  = WUEPS_PID_AT;
    pstCopnInfoQryCnf->ulSenderPid    = WUEPS_PID_MMA;

    pstCopnInfoQryCnf->stAtAppCtrl.ucOpId     = 0;
    pstCopnInfoQryCnf->stAtAppCtrl.usClientId = AT_BROADCAST_CLIENT_ID_MODEM_0;

    pstCopnInfoQryCnf->ulLength       = ulMsgSize;
    pstOperNameList                 = (TAF_PH_OPERATOR_NAME_STRU *)pstCopnInfoQryCnf->aucContent;

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameShort,
                   "Contact ",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameLong,
                   "F - Contact",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[0].PlmnId.Mcc = 0x208;
    pstOperNameList[0].PlmnId.Mnc = 0x002;

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameShort,
                   "Proximus",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameLong,
                   "Proximus",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[1].PlmnId.Mcc = 0x206;
    pstOperNameList[1].PlmnId.Mnc = 0x001;

    // 参数初始化
    gastAtClientTab[1].usClientId       = AT_BROADCAST_CLIENT_ID_MODEM_0;
    gastAtClientTab[1].ucUsed           = AT_CLIENT_USED;
    gastAtClientTab[1].CmdCurrentOpt    = AT_CMD_COPN_QRY;

    stCopnInfo.usFromIndex  = 101;
    stCopnInfo.usPlmnNum    = 50;

    // 调用被测函数
    ulRet = AT_RcvMmaCopnInfoQueryCnf((VOS_VOID*)pstCopnInfoQryCnf);

    EXPECT_EQ(VOS_ERR, ulRet);

    // 执行检查
    GlobalMockObject::verify();
}



TEST_F(Test_AT_RcvMmaCopnInfoQueryCnf, Test_AT_RcvMmaCopnInfoQueryCnf_006)
{

    VOS_UINT32                          ulMsgSize;
    TAF_PH_OPERATOR_NAME_STRU          *pstOperNameList   = VOS_NULL_PTR;
    VOS_CHAR                           *pcPrintStr = "COPN: \"208002\",\"F - Contact\"\r\nCOPN: \"206001\",\"Proximus\"\r\n\r\nOK\r\n";
    TAF_MMA_COPN_INFO_QUERY_REQ_STRU    stCopnInfo;
    VOS_UINT32                          ulRet;

    ulMsgSize  = sizeof(TAF_MMA_COPN_INFO_QUERY_CNF_STRU) - sizeof(pstCopnInfoQryCnf->aucContent);
    ulMsgSize += 2 * sizeof(TAF_PH_OPERATOR_NAME_STRU);

    pstCopnInfoQryCnf = (TAF_MMA_COPN_INFO_QUERY_CNF_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstCopnInfoQryCnf, 0x0, ulMsgSize);

    pstCopnInfoQryCnf->ulMsgName      = AT_MMA_COPN_INFO_QUERY_CNF;
    pstCopnInfoQryCnf->usFromIndex    = 51;
    pstCopnInfoQryCnf->usPlmnNum      = 2;

    pstCopnInfoQryCnf->ulReceiverPid  = WUEPS_PID_AT;
    pstCopnInfoQryCnf->ulSenderPid    = WUEPS_PID_MMA;

    pstCopnInfoQryCnf->stAtAppCtrl.ucOpId     = 0;
    pstCopnInfoQryCnf->stAtAppCtrl.usClientId = 110;

    pstCopnInfoQryCnf->ulLength       = ulMsgSize;
    pstOperNameList                 = (TAF_PH_OPERATOR_NAME_STRU *)pstCopnInfoQryCnf->aucContent;

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameShort,
                   "Contact ",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameLong,
                   "F - Contact",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[0].PlmnId.Mcc = 0x208;
    pstOperNameList[0].PlmnId.Mnc = 0x002;

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameShort,
                   "Proximus",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameLong,
                   "Proximus",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[1].PlmnId.Mcc = 0x206;
    pstOperNameList[1].PlmnId.Mnc = 0x001;

    // 参数初始化
    gastAtClientTab[1].usClientId       = 110;
    gastAtClientTab[1].ucUsed           = AT_CLIENT_USED;
    gastAtClientTab[1].CmdCurrentOpt    = AT_CMD_PLMN_QRY;

    stCopnInfo.usFromIndex  = 101;
    stCopnInfo.usPlmnNum    = 50;

    // 调用被测函数
    ulRet = AT_RcvMmaCopnInfoQueryCnf((VOS_VOID*)pstCopnInfoQryCnf);

    EXPECT_EQ(VOS_ERR, ulRet);

    // 执行检查
    GlobalMockObject::verify();
}


/* Added by f62575 for SS FDN&Call Control, 2013-05-06, begin */
/*******************************************************************
测试用例编号      : Test_AT_RcvMmaCopnInfoQueryCnf_007
测试用例标题      : MMA回复消息的CLIENT ID为MODEM 0或MODEM 1的广播消息
预期结果          : 返回VOS_ERR
修改历史          :
1.日    期   : 2013年05月29日
  作    者   : f62575
  修改内容   : 新增函数
*******************************************************************/
TEST_F(Test_AT_RcvMmaCopnInfoQueryCnf, Test_AT_RcvMmaCopnInfoQueryCnf_007)
{

    VOS_UINT32                          ulMsgSize;
    TAF_PH_OPERATOR_NAME_STRU          *pstOperNameList   = VOS_NULL_PTR;
    VOS_CHAR                           *pcPrintStr = "COPN: \"208002\",\"F - Contact\"\r\nCOPN: \"206001\",\"Proximus\"\r\n\r\nOK\r\n";
    TAF_MMA_COPN_INFO_QUERY_REQ_STRU    stCopnInfo;
    VOS_UINT32                          ulRet;

    ulMsgSize  = sizeof(TAF_MMA_COPN_INFO_QUERY_CNF_STRU) - sizeof(pstCopnInfoQryCnf->aucContent);
    ulMsgSize += 2 * sizeof(TAF_PH_OPERATOR_NAME_STRU);

    pstCopnInfoQryCnf = (TAF_MMA_COPN_INFO_QUERY_CNF_STRU *)malloc(ulMsgSize);
    PS_MEM_SET(pstCopnInfoQryCnf, 0x0, ulMsgSize);

    pstCopnInfoQryCnf->ulMsgName      = AT_MMA_COPN_INFO_QUERY_CNF;
    pstCopnInfoQryCnf->usFromIndex    = 51;
    pstCopnInfoQryCnf->usPlmnNum      = 2;

    pstCopnInfoQryCnf->ulReceiverPid  = WUEPS_PID_AT;
    pstCopnInfoQryCnf->ulSenderPid    = WUEPS_PID_MMA;

    pstCopnInfoQryCnf->stAtAppCtrl.ucOpId     = 0;
    pstCopnInfoQryCnf->stAtAppCtrl.usClientId = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    pstCopnInfoQryCnf->ulLength       = ulMsgSize;
    pstOperNameList                 = (TAF_PH_OPERATOR_NAME_STRU *)pstCopnInfoQryCnf->aucContent;

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameShort,
                   "Contact ",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[0].aucOperatorNameLong,
                   "F - Contact",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[0].PlmnId.Mcc = 0x208;
    pstOperNameList[0].PlmnId.Mnc = 0x002;

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameShort,
                   "Proximus",
                   TAF_PH_OPER_NAME_SHORT);

    VOS_StrNCpy(pstOperNameList[1].aucOperatorNameLong,
                   "Proximus",
                   TAF_PH_OPER_NAME_LONG);

    pstOperNameList[1].PlmnId.Mcc = 0x206;
    pstOperNameList[1].PlmnId.Mnc = 0x001;

    // 参数初始化
    gastAtClientTab[1].usClientId       = 120;
    gastAtClientTab[1].ucUsed           = AT_CLIENT_USED;
    gastAtClientTab[1].CmdCurrentOpt    = AT_CMD_PLMN_QRY;

    stCopnInfo.usFromIndex  = 101;
    stCopnInfo.usPlmnNum    = 50;

    // 调用被测函数
    ulRet = AT_RcvMmaCopnInfoQueryCnf((VOS_VOID*)pstCopnInfoQryCnf);

    EXPECT_EQ(VOS_ERR, ulRet);

    pstCopnInfoQryCnf->stAtAppCtrl.usClientId = AT_BROADCAST_CLIENT_INDEX_MODEM_1;

    EXPECT_EQ(VOS_ERR, ulRet);


    // 执行检查
    GlobalMockObject::verify();
}
/* Added by f62575 for SS FDN&Call Control, 2013-05-06, end */

class Test_AT_RcvMmaSimInsertRsp: public ::testing::Test
{
    public:

    void SetUp()
    {
        UT_STUB_INIT();
    }
    void TearDown()
    {
    }
};


TEST_F(Test_AT_RcvMmaSimInsertRsp, Test_AT_RcvMmaSimInsertRsp_001)
{
    // 变量声明
    TAF_MMA_SIM_INSERT_RSP_STRU        stMsg;
    VOS_UINT32                         ulRelt;
    VOS_UINT8                          ucIndex;

    // 参数初始化
    ucIndex = 0;
    stMsg.ulResult = VOS_OK;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_SIMINSERT_SET;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    // 调用被测函数
    ulRelt = AT_RcvMmaSimInsertRsp((VOS_VOID *)&stMsg);

    // 执行检查
    EXPECT_EQ(ulRelt, VOS_OK);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaSimInsertRsp, Test_AT_RcvMmaSimInsertRsp_002)
{
    // 变量声明
    TAF_MMA_SIM_INSERT_RSP_STRU        stMsg;
    VOS_UINT32                         ulRelt;

    // 参数初始化

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .will(returnValue((TAF_UINT32)AT_FAILURE));

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRelt = AT_RcvMmaSimInsertRsp((VOS_VOID *)&stMsg);

    // 执行检查
    EXPECT_EQ(ulRelt, VOS_ERR);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaSimInsertRsp, Test_AT_RcvMmaSimInsertRsp_003)
{
    // 变量声明
    TAF_MMA_SIM_INSERT_RSP_STRU        stMsg;
    VOS_UINT32                         ulRelt;
    VOS_UINT8                          ucIndex;

    // 参数初始化
    ucIndex = 0x3F;

    // 初始化全局变量

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));


    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRelt = AT_RcvMmaSimInsertRsp((VOS_VOID *)&stMsg);

    // 执行检查
    EXPECT_EQ(ulRelt, VOS_ERR);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaSimInsertRsp, Test_AT_RcvMmaSimInsertRsp_004)
{
    // 变量声明
    TAF_MMA_SIM_INSERT_RSP_STRU        stMsg;
    VOS_UINT32                         ulRelt;
    VOS_UINT8                          ucIndex;

    // 参数初始化
    ucIndex = 0;
    stMsg.ulResult = VOS_OK;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_INVALID;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(never());

    // 调用被测函数
    ulRelt = AT_RcvMmaSimInsertRsp((VOS_VOID *)&stMsg);

    // 执行检查
    EXPECT_EQ(ulRelt, VOS_ERR);
    GlobalMockObject::verify();
}


TEST_F(Test_AT_RcvMmaSimInsertRsp, Test_AT_RcvMmaSimInsertRsp_005)
{
    // 变量声明
    TAF_MMA_SIM_INSERT_RSP_STRU        stMsg;
    VOS_UINT32                         ulRelt;
    VOS_UINT8                          ucIndex;

    // 参数初始化
    ucIndex = 0;
    stMsg.ulResult = VOS_ERR;

    // 初始化全局变量
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_SIMINSERT_SET;

    // MOCKER操作
    MOCKER(At_ClientIdToUserId)
        .stubs()
        .with(any(), outBoundP((VOS_UINT8 *)&ucIndex,(VOS_UINT32)sizeof(ucIndex)))
        .will(returnValue((TAF_UINT32)AT_SUCCESS));

    MOCKER(At_FormatResultData)
        .expects(exactly(1));

    // 调用被测函数
    ulRelt = AT_RcvMmaSimInsertRsp((VOS_VOID *)&stMsg);

    // 执行检查
    EXPECT_EQ(ulRelt, VOS_OK);
    GlobalMockObject::verify();
}


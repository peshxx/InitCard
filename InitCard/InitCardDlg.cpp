
// InitCardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "InitCard.h"
#include "InitCardDlg.h"
#include "afxdialogex.h"
#include "dcrf32.h"
#include "hsmcmd.h"
#include "hsm_tcpsub.h"
#include "MyTools.h"
#include "sjl22_ic_api.h"

#include <windows.h>
#include <WinBase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE icdev;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CInitCardDlg 对话框



CInitCardDlg::CInitCardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INITCARD_DIALOG, pParent)
	, m_cardno(_T(""))
{
	//m_cardno = _T("");
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInitCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT1, m_cardno);
	DDX_Text(pDX, IDC_EDIT1, m_cardno);
	DDV_MaxChars(pDX, m_cardno, 16);
}

BEGIN_MESSAGE_MAP(CInitCardDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CInitCardDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CInitCardDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CInitCardDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CInitCardDlg 消息处理程序

BOOL CInitCardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//m_cardno.SetLimitText(16);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInitCardDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInitCardDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}




//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CInitCardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CInitCardDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


int CardCommand(HANDLE icdev, unsigned char* sendbuffer, int *rlen, unsigned char *recvbuffer)
{
	int ret = 0;
	unsigned char tempbuffer[512];
	unsigned char buff[512];
	unsigned char recvlen[2];
	int clen = 0;
	char *p;

	memset(tempbuffer, 0x00, sizeof(tempbuffer));
	memset(buff, 0x00, sizeof(buff));
	memset(recvlen, 0x00, sizeof(recvlen));

	clen = strlen((char *)sendbuffer);
	a_hex(sendbuffer, &tempbuffer[0], clen);
	clen = clen / 2;

	ret = dc_pro_commandlink(icdev, clen, tempbuffer, recvlen, recvbuffer, 7, 40);
	if (ret != 0 && ret != 27)
	{
		//sprintf((char *)tempbuff, "dc_pro_command Card Error code=%d", st);
		//AfxMessageBox((char *)tempbuff);
		return -1;
	}
	else if (ret == 27)
	{
		ret = dc_pro_commandlink(icdev, clen, tempbuffer, recvlen, recvbuffer, 7, 40);
		if (ret != 0)
		{
			//sprintf((char *)tempbuff, "dc_pro_command Card Error code=%d", st);
			//AfxMessageBox((char *)tempbuff);
			return -1;
		}
	}
	memset(tempbuffer, 0x00, sizeof(tempbuffer));

	hex_a(recvbuffer, tempbuffer, recvlen[0]);
	tempbuffer[recvlen[0] * 2] = 0;
	clen = recvlen[0] * 2;
	
	p = (char *)tempbuffer;

	p += clen - 4;

	memcpy(buff, p, 4);

	*p = 0x00;
	//AfxMessageBox((char *)recbuff);
	if (strcmp((char *)buff, "9000") != 0)
	{
		//MessageBox((char *)recvbuffer);
		AfxMessageBox((char *)tempbuffer);
		ret = atoi((char *)buff);
		return -ret;
	}
	ret = 0;
	memset(recvbuffer, 0x00, sizeof(recvbuffer));
	if (clen > 4)
	{
	//	AfxMessageBox((char *)tempbuffer);
		memcpy(recvbuffer, tempbuffer, clen - 4);
		*rlen = clen - 4;
	}

	return ret;
}


/*External Authentication*/
int ExAuthen(HANDLE icdev)
{
	int ret, socketfd = 0;
	int recvlen, sendlen = 0;
	unsigned char sendbuff[128];
	unsigned char random[8];
	unsigned char recvbuff[128], crlen[2];
	unsigned char buffer[128];
	char *p;

	memset(sendbuff, 0x00, sizeof(sendbuff));
	memset(recvbuff, 0x00, sizeof(recvbuff));
	memset(buffer, 0x00, sizeof(buffer));
	memset(recvbuff, 0x00, sizeof(crlen));

	//int CardCommand(HANDLE icdev, unsigned char clen, unsigned char* sendbuffer, unsigned char *recvlen,
	//unsigned char *recvbuffer)
	memcpy(sendbuff, "0084000004", 10);
	sendbuff[14] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	//AfxMessageBox((char *)recvbuff);
	strncat((char *)recvbuff, "00000000", 8);

	memcpy(sendbuff, recvbuff, strlen((char *)recvbuff));

	memset(recvbuff, 0x00, sizeof(recvbuff));
	socketfd = InitHsmDevice("172.20.1.69", 6666, 30);
	if (socketfd < 0)
	{
		AfxMessageBox("连接加密机失败！");
		return -100;
	}
	ret = secure_msg(socketfd, "1234567", 0, "010209", 0, NULL, 3, NULL, 8, 8, NULL, NULL, 8, (char *)sendbuff, &recvlen, (char *)recvbuff, (char *)buffer);
	if (ret < 0)
	{
		sprintf((char *)buffer, "%02d", ret);
		AfxMessageBox((char *)buffer);
		AfxMessageBox("secure_msg error!");
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	memcpy(sendbuff, "0082000008", 10);

	//AfxMessageBox((char *)recvbuff);

	strncat((char *)sendbuff, (char *)recvbuff, 16);
	memset(recvbuff, 0x00, sizeof(recvbuff));

	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	memset(buffer, 0x00, sizeof(buffer));
	sprintf((char *)buffer, "%02d", ret);
	//AfxMessageBox((char *)buffer);
	//AfxMessageBox("外部认证成功!");
	CloseHsmDevice(socketfd);
	return ret;
}

/*Internal Certification*/
int InAuthen(HANDLE icdev)
{
	int ret, socketfd = 0;
	int recvlen, sendlen = 0;
	unsigned char sendbuff[128];
	unsigned char random[8];
	unsigned char recvbuff[128], crlen[2];
	unsigned char buffer[128];
	char *p;

	memset(sendbuff, 0x00, sizeof(sendbuff));
	memset(recvbuff, 0x00, sizeof(recvbuff));
	memset(recvbuff, 0x00, sizeof(crlen));

	//int CardCommand(HANDLE icdev, unsigned char clen, unsigned char* sendbuffer, unsigned char *recvlen,
	//unsigned char *recvbuffer)
	memcpy(sendbuff, "0084000004", 10);
	sendbuff[14] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	strncat((char *)recvbuff, "00000000", 8);

	memcpy(sendbuff, recvbuff, strlen((char *)recvbuff));

	memset(recvbuff, 0x00, sizeof(recvbuff));
	socketfd = InitHsmDevice("172.20.1.69", 6666, 30);
	if (socketfd < 0)
	{
		AfxMessageBox("连接加密机失败！");
		return -100;
	}
	ret = secure_msg(socketfd, "1234567", 0, "010210", 0, NULL, 3, NULL, 8, 8, NULL, NULL, 8, (char *)sendbuff, &recvlen, (char *)recvbuff, (char *)buffer);
	if (ret < 0)
	{
		sprintf((char *)buffer, "%02d", ret);
		AfxMessageBox((char *)buffer);
		AfxMessageBox("secure_msg error!");
		CloseHsmDevice(socketfd);
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	memcpy(sendbuff, "0082000008", 10);

	strncat((char *)sendbuff, (char *)recvbuff, 16);
	memset(recvbuff, 0x00, sizeof(recvbuff));

	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	sprintf((char *)buffer, "%02d", ret);
	AfxMessageBox((char *)buffer);
	CloseHsmDevice(socketfd);
	return ret;
}


int CalClearkey(char *index, char *cardno, char *clearkey)
{
	int socketfd = 0;
	int ret = 0;
	char kcv[16];
	int cipher_data_len;
	int output_message_len;
	char demesg[33];
	char cipher[33];
	char mac[9];
	char buffer[128];

	memset(kcv, 0x00, sizeof(kcv));
	memset(cipher, 0x00, sizeof(cipher));
	memset(mac, 0x00, sizeof(mac));
	memset(buffer, 0x00, sizeof(buffer));
	memset(demesg, 0x00, sizeof(demesg));

	socketfd = InitHsmDevice("172.20.1.69", 6666, 30);
	if (socketfd < 0)
	{
		AfxMessageBox("连接加密机失败！");
		//return socketfd;
		return -100;
	}

	ret = export_key1(socketfd, "1234567", "FF", 0, 1, index, 0, "0000000000000000", 1, cardno, 0, "00000000", 0, "00000000", 0x0f, "0000000000000000", "0A0B0C0D0E0F1A", kcv, &cipher_data_len, cipher, mac);

	sprintf(buffer, "%02d", ret);
	//AfxMessageBox(buffer);
	if (ret != 0)
	{
		return ret;
	}
	else
	{
		//AfxMessageBox(cipher);
	}

	ret = encrypted_and_decrypted(socketfd, "1234567", 0, 1, 1, "FE", 0, 1, "030102", 0, \
		"0000000000000000", 3, NULL, 0, "0000", 0, \
		NULL, 16, cipher, &output_message_len, demesg, "00");

	sprintf(buffer, "%02d", ret);
	//AfxMessageBox(buffer);
	if (ret != 0)
	{
		CloseHsmDevice(socketfd);
		return ret;
	}
	else
	{
		//AfxMessageBox(demesg);
	}

	memcpy(clearkey, demesg, 32);
	CloseHsmDevice(socketfd);
	return 0;

}


int init1(char *cardno)
{
	int ret, socketfd = 0;
	int recvlen, sendlen = 0;
	int st = 0;
	unsigned char sendbuff[512];
	unsigned char random[9];
	unsigned char recvbuff[512], crlen[2];
	unsigned long cardsnr;
	unsigned char sss[5];
	unsigned char iv[17];
	unsigned char mac[9];
	unsigned char buffer[33];
	unsigned __int16 ttt;
	unsigned char recbuff[512];
	char clearkey[33];
	char *p;

	memset(sendbuff, 0x00, sizeof(sendbuff));
	memset(recvbuff, 0x00, sizeof(recvbuff));
	memset(recvbuff, 0x00, sizeof(crlen));
	memset(clearkey, 0x00, sizeof(clearkey));
	memset(buffer, 0x00, sizeof(buffer));
	memset(iv, 0x00, sizeof(iv));
	memset(mac, 0x00, sizeof(mac));


	//int CardCommand(HANDLE icdev, unsigned char clen, unsigned char* sendbuffer, unsigned char *recvlen,
	//unsigned char *recvbuffer)
	icdev=dc_init(100,115200);  //100表示USB口
	if((long)icdev<=0)
	   AfxMessageBox("Init Com Error!");
	else
	     AfxMessageBox("Init Com OK!");

	st = dc_reset(icdev, 10000);
	if (st < 0)
	{
		AfxMessageBox("dc_reset Error!");
		return st;
	}
	AfxMessageBox("dc_reset ok!");

	st = dc_request(icdev, 1, &ttt);
	if (st != 0)
	{
		AfxMessageBox("request Card Error!");
		return st;
	}
	st = dc_anticoll(icdev, 0, &cardsnr);
	if (st != 0)
	{
		AfxMessageBox("anticoll Error!");
		return st;
	}
	st = dc_select(icdev, cardsnr, sss);
	if (st != 0)
	{
		AfxMessageBox("dc_select Error!");
		return st;
	}
	//复位

	st = dc_pro_resethex(icdev, crlen, (char *)recbuff);
	if (st != 0)
	{
		AfxMessageBox("dc_pro_reset Card Error!");
		return st;
	}

	memcpy(sendbuff, "00A40000023F00", 14);
	sendbuff[14] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	//AfxMessageBox(ret);
	if (ret < 0)
	{
		AfxMessageBox("CardCommadn ERROR!");
		return ret;
	}
	//AfxMessageBox((char *)recvbuff);
	memset(sendbuff, 0x00, sizeof(sendbuff));

	ret = ExAuthen(icdev);
	if (ret)
	{
		AfxMessageBox("外部认证出错！");
		return ret;
	}


	//AfxMessageBox("外部认证成功！");

	memcpy(sendbuff, "800E000000", 10);
	sendbuff[10] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		AfxMessageBox((char *)sendbuff);
		sprintf((char *)buffer, "%02d", ret);
		AfxMessageBox((char *)buffer);
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	memset(recvbuff, 0x00, sizeof(recvbuff));

	memcpy(sendbuff, "80E00000073F006001AAFFFF", 24);
	sendbuff[24] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		AfxMessageBox((char *)sendbuff);
		sprintf((char *)buffer, "%02d", ret);
		AfxMessageBox((char *)buffer);
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));

	/*memcpy(sendbuff, "80E00000073F006001AAFFFF", 24);
	sendbuff[24] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		AfxMessageBox((char *)sendbuff);
		sprintf((char *)buffer, "%02d", ret);
		AfxMessageBox((char *)buffer);
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));*/

	memcpy(sendbuff, "80D4010015F9F0AA0A33FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", 52);
	sendbuff[52] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		AfxMessageBox((char *)sendbuff);
		sprintf((char *)buffer, "%02d", ret);
		AfxMessageBox((char *)buffer);
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));

	memcpy(sendbuff, "80D4010015F6F0F0FF3311223344556677881122334455667788", 52);
	sendbuff[52] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		AfxMessageBox((char *)sendbuff);
		sprintf((char *)buffer, "%02d", ret);
		AfxMessageBox((char *)buffer);
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));

	memcpy(sendbuff, "80E00001072C0007F0F0FFFF", 24);
	sendbuff[24] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		AfxMessageBox((char *)sendbuff);
		sprintf((char *)buffer, "%02d", ret);
		AfxMessageBox((char *)buffer);
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));

	memcpy(sendbuff, "80E03F0111381000FAAA95FFFF3F013F013F013F013F", 44);
	sendbuff[44] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		AfxMessageBox((char *)sendbuff);
		sprintf((char *)buffer, "%02d", ret);
		AfxMessageBox((char *)buffer);
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));

	memcpy(sendbuff, "00A40400093F013F013F013F013F", 28);
	sendbuff[28] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		AfxMessageBox((char *)sendbuff);
		sprintf((char *)buffer, "%02d", ret);
		AfxMessageBox((char *)buffer);
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));

	memcpy(sendbuff, "80E00000073F040081FAFFFF", 24);
	sendbuff[24] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		AfxMessageBox((char *)sendbuff);
		sprintf((char *)buffer, "%02d", ret);
		AfxMessageBox((char *)buffer);
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));

	//初始化外部认证密钥
	memcpy(sendbuff, "80D4010015F9F0FA0A3311223344556677881122334455667788", 52);
	sendbuff[52] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		AfxMessageBox((char *)sendbuff);
		sprintf((char *)buffer, "%02d", ret);
		AfxMessageBox((char *)buffer);
		return ret;
	}
	//AfxMessageBox("初始化外部认证密钥成功！");
	memset(sendbuff, 0x00, sizeof(sendbuff));

	//初始化文件线路保护密钥
	memcpy(sendbuff, "80D4010015F6F0F0FF3311223344556677881122334455667788", 52);
	sendbuff[52] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	//AfxMessageBox("初始化文件线路保护密钥成功！");

	//AfxMessageBox(cardno);
	//圈存密钥
	ret = CalClearkey("010203", cardno, clearkey);
	if (ret < 0)
	{
		AfxMessageBox("计算圈存密钥出错！");
		return ret;
	}
	memcpy(sendbuff, "80D4010115FFF1F00101", 20);
	strncat((char *)sendbuff, clearkey, 32);
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	//AfxMessageBox("初始化圈存密钥成功！");

	//消费密钥
	ret = CalClearkey("010202", cardno, clearkey);
	if (ret < 0)
	{
		AfxMessageBox("计算消费密钥出错！");
		return ret;
	}
	memcpy(sendbuff, "80D4010115FEF1F00101", 20);
	strncat((char *)sendbuff, clearkey, 32);
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	//AfxMessageBox("初始化消费密钥成功！");

	//圈提密钥
	ret = CalClearkey("010201", cardno, clearkey);
	if (ret < 0)
	{
		AfxMessageBox("计算圈提密钥出错！");
		return ret;
	}
	memcpy(sendbuff, "80D4010115FDF1F00101", 20);
	strncat((char *)sendbuff, clearkey, 32);
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	//AfxMessageBox("初始化圈提密钥成功！");

	//TAC密钥
	ret = CalClearkey("010204", cardno, clearkey);
	if (ret < 0)
	{
		AfxMessageBox("计算TAC密钥出错！");
		return ret;
	}
	memcpy(sendbuff, "80D4010015F4F0F00101", 20);
	strncat((char *)sendbuff, clearkey, 32);
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	//AfxMessageBox("初始化TAC密钥成功！");

	//口令密钥
	memcpy(sendbuff, "80D40100083AF0EF0133123456FFFFFFFFFFFFFFFFFFFFFFFFFF", 52);
	sendbuff[52] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	//AfxMessageBox("初始化口令密钥成功！");

	//重装口令密钥
	memcpy(sendbuff, "80D4010015F8F0F0FF3311223344556677881122334455667788", 52);
	sendbuff[52] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	//AfxMessageBox("初始化重装口令密钥成功！");

	memcpy(sendbuff, "80E0EF1507A8003CF0F0FFFF", 24);
	sendbuff[24] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));

	memcpy(sendbuff, "80E0EF1607A80046F0F0FFFF", 24);
	sendbuff[24] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));

	memcpy(sendbuff, "80E0EF1907A80026F0F0FFFF", 24);
	sendbuff[24] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));

	memcpy(sendbuff, "80E00002072F0208F0FFFF18", 24);
	sendbuff[24] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));

	memcpy(sendbuff, "80E00018072E0A17F1EFFFFF", 24);
	sendbuff[24] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));

	memcpy(sendbuff, "80E0001A07A8003CF0F0FFFF", 24);
	sendbuff[24] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}

	memset(sendbuff, 0x00, sizeof(sendbuff));
	memset(recvbuff, 0x00, sizeof(recvbuff));

	memcpy(sendbuff, "00A40000023F00", 14);
	sendbuff[14] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	memset(recvbuff, 0x00, sizeof(recvbuff));

	memcpy(sendbuff, "00A40000023F01", 14);
	sendbuff[14] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	memset(recvbuff, 0x00, sizeof(recvbuff));

	memcpy(sendbuff, "0084000004", 10);
	sendbuff[10] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		AfxMessageBox((char *)sendbuff);
		sprintf((char *)buffer, "%02d", ret);
		AfxMessageBox((char *)buffer);
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	strncat((char *)recvbuff, "00000000", 8);
	memcpy(iv, (char *)recvbuff, 16);
	//AfxMessageBox("iv");
	//AfxMessageBox((char *)iv);

	memcpy(sendbuff, "04D6950040000000008888999901010000", 34);
	strncat((char *)sendbuff, cardno, 16);
	strncat((char *)sendbuff,    "00000000000000000000000000000000000000000000000000000000000000000000000000000000", 80);
	sendbuff[130] = 0;

	socketfd = InitHsmDevice("172.20.1.69", 6666, 30);
	if (socketfd < 0)
	{
		AfxMessageBox("连接加密机失败！");
		return -100;
	}
	ret = generate_mac_tac(socketfd, "1234567", 0, "010210", 0, "0000000000000000", 3, \
		NULL, 4, 73, (char *)iv, (char *)sendbuff, (char *)mac);

	sprintf((char *)buffer, "%02d", ret);
	//AfxMessageBox((char *)buffer);
	if (ret != 0)
	{
		return ret;
	}
	else
	{
		//AfxMessageBox((char *)mac);
	}
	strncat((char *)sendbuff, (char *)mac, 8);
	sendbuff[138] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	//AfxMessageBox("15文件创建成功！");
	memset(sendbuff, 0x00, sizeof(sendbuff));
	memset(recvbuff, 0x00, sizeof(recvbuff));

	/*16文件*/
	memcpy(sendbuff, "0084000004", 10);
	sendbuff[10] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	strncat((char *)recvbuff, "00000000", 8);
	memcpy(iv, (char *)recvbuff, 16);

	memcpy(sendbuff, "04D696004A00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 150);

	ret = generate_mac_tac(socketfd, "1234567", 0, "010210", 0, "0000000000000000", 3, \
		NULL, 4, 83, (char *)iv, (char *)sendbuff, (char *)mac);

	sprintf((char *)buffer, "%02d", ret);
	//AfxMessageBox((char *)buffer);
	if (ret != 0)
	{
		return ret;
	}
	else
	{
		//AfxMessageBox((char *)mac);
	}
	strncat((char *)sendbuff, (char *)mac, 8);
	sendbuff[158] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	//AfxMessageBox("16文件创建成功！");
	memset(sendbuff, 0x00, sizeof(sendbuff));
	memset(recvbuff, 0x00, sizeof(recvbuff));

	/*17文件*/
	memcpy(sendbuff, "0084000004", 10);
	sendbuff[10] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	strncat((char *)recvbuff, "00000000", 8);
	memcpy(iv, (char *)recvbuff, 16);

	memcpy(sendbuff, "04D699002A0000000000000000000000000000000000000000000000000000000000000000000000000000", 86);

	ret = generate_mac_tac(socketfd, "1234567", 0, "010210", 0, "0000000000000000", 3, \
		NULL, 4, 51, (char *)iv, (char *)sendbuff, (char *)mac);

	sprintf((char *)buffer, "%02d", ret);
	//AfxMessageBox((char *)buffer);
	if (ret != 0)
	{
		return ret;
	}
	else
	{
		//AfxMessageBox((char *)mac);
	}
	strncat((char *)sendbuff, (char *)mac, 8);
	sendbuff[94] = 0;
	ret = CardCommand(icdev, sendbuff, &recvlen, recvbuff);
	if (ret < 0)
	{
		return ret;
	}
	memset(sendbuff, 0x00, sizeof(sendbuff));
	//AfxMessageBox("17文件创建成功！");
	/*ret = InAuthen(icdev);
	if (ret)
		AfxMessageBox("内部认证出错！");*/

	CloseHsmDevice(socketfd);
	return ret;


}


void CInitCardDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString card_no;
	int ret = 0;
	char buffer[128];
	//AfxMessageBox(m_cardno);
	UpdateData(TRUE);
	//memcpy(card_no, m_cardno, strlen(m_cardno));
	//m_cardno.GetWindowTextA(card_no);

	memset(buffer, 0x00, sizeof(buffer));
	memcpy(buffer, m_cardno, 16);

	AfxMessageBox(buffer);

	ret  = init1(buffer);
	if (ret !=0)
	{
		AfxMessageBox("发卡失败！");
	}
	else
	{
		AfxMessageBox("发卡成功!");
	}
	long long no = atoll(buffer);
	no = no + 1;
	memset(buffer, 0x00, sizeof(buffer));
	sprintf(buffer, "%016lld", no);
	AfxMessageBox(buffer);

	m_cardno = (unsigned char *)buffer;

	UpdateData(false);
}



void CInitCardDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString card_no;
	int socketfd = 0;
	int ret = 0;
	char kcv[16];
	int cipher_data_len;
	int output_message_len;
	char demesg[33];
	char cipher[33];
	char mac[9];
	char buffer[128];
	char sendbuff[512];

	memset(kcv, 0x00, sizeof(kcv));
	memset(cipher, 0x00, sizeof(cipher));
	memset(mac, 0x00, sizeof(mac));
	memset(buffer, 0x00, sizeof(buffer));
	memset(demesg, 0x00, sizeof(demesg));
	memset(sendbuff, 0x00, sizeof(sendbuff));
	//AfxMessageBox(m_cardno);
	UpdateData(TRUE);
	//memcpy(card_no, m_cardno, strlen(m_cardno));
	//m_cardno.GetWindowTextA(card_no);
	memcpy(buffer, m_cardno, 16);
	AfxMessageBox(buffer);
	//socketfd = InitHsmDevice("172.20.1.69", 6666, 30);
	//if (socketfd < 0)
	//{
	//	AfxMessageBox("连接加密机失败！");
	//	return;
	//	//return -100;
	//}

	//ret = export_key1(socketfd, "1234567", "FF", 0, 1, "010201", 0, "0000000000000000", 1, "1111111111111111", 0, "00000000", 0, "00000000", 0x0f, "0000000000000000", "0A0B0C0D0E0F1A", kcv, &cipher_data_len, cipher, mac);

	//sprintf(buffer, "%02d", ret);
	//AfxMessageBox(buffer);
	//if (ret != 0)
	//{
	//	return;
	//}
	//else
	//{
	//	AfxMessageBox(cipher);
	//}

	//ret = encrypted_and_decrypted(socketfd, "1234567", 0, 1, 1, "FE", 0, 1, "030102", 0, \
	//	"0000000000000000", 3, NULL, 0, "0000", 0, \
	//	NULL, 16, cipher, &output_message_len, demesg, "00");

	//sprintf(buffer, "%02d", ret);
	//AfxMessageBox(buffer);
	//if (ret != 0)
	//{
	//	return;
	//}
	//else
	//{
	//	AfxMessageBox(demesg);
	//}

	/*ret = generate_mac_tac(socketfd, "1234567", 0, "010210", 0, "0000000000000000", 3, \
		NULL, 4, 73, "2633E7AD00000000", "04D6950040000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", mac);
	sprintf(buffer, "%02d", ret);
	AfxMessageBox(buffer);
	if (ret != 0)
	{
		return;
	}
	else
	{
		AfxMessageBox(mac);
	}*/

	//ret = CalClearkey("010201", card_no.GetBuffer(16), demesg);

	////圈存密钥
	//CalClearkey("010203", card_no.GetBuffer(16), demesg);

	////消费密钥
	//CalClearkey("010202", card_no.GetBuffer(16), demesg);

	////圈提密钥
	//CalClearkey("010201", card_no.GetBuffer(16), demesg);

	////TAC密钥
	//CalClearkey("010204", card_no.GetBuffer(16), demesg);

	/*memcpy(sendbuff, "04D6950040000000008888999901010000", 34);
	strncat((char *)sendbuff, card_no.GetBuffer(16), 16);
	strncat((char *)sendbuff, "00000000000000000000000000000000000000000000000000000000000000000000000000000000", 80);
	sendbuff[130] = 0;

	ret = generate_mac_tac(socketfd, "1234567", 0, "010210", 0, "0000000000000000", 3, \
		NULL, 4, 73, "1234567890123456", "04D6950040000000008888999901010000100191822800007800000000000000000000000000000000000000000000000000000000000000000000000000000000", mac);

	sprintf((char *)buffer, "%02d", ret);
	AfxMessageBox((char *)buffer);*/
	memcpy(buffer, m_cardno, 16);
	long long no = atoll(buffer);
	no = no + 1;
	memset(buffer, 0x00, sizeof(buffer));
	sprintf(buffer, "%016lld", no);
	AfxMessageBox(buffer);
	
	m_cardno = (unsigned char *)buffer;
	UpdateData(false);
	return;
}

// UpdateAccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LolCenter.h"
#include "UpdateAccountDlg.h"
#include "afxdialogex.h"
#include "AddAccountDlg.h"

#define _SELF L"UpdateAccountDlg.cpp"

// CUpdateAccountDlg dialog
vector<ServerAccountInfo> UpdateAccountList;
int g_nSelListIndex = -1;

IMPLEMENT_DYNAMIC(CUpdateAccountDlg, CDialogEx)

CUpdateAccountDlg::CUpdateAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdateAccountDlg::IDD, pParent)
{

}

CUpdateAccountDlg::~CUpdateAccountDlg()
{
}

void CUpdateAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpdateAccountDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_UpdateAccount, &CUpdateAccountDlg::OnNMClickListUpdateaccount)
	ON_BN_CLICKED(IDC_RADIO_Custome, &CUpdateAccountDlg::OnBnClickedRadioCustome)
	ON_BN_CLICKED(IDC_RADIO_Default, &CUpdateAccountDlg::OnBnClickedRadioDefault)
	ON_BN_CLICKED(IDC_BTN_Delete, &CUpdateAccountDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_UpdateAccount, &CUpdateAccountDlg::OnBnClickedBtnUpdateaccount)
	ON_BN_CLICKED(IDC_BTN_AddAccount, &CUpdateAccountDlg::OnBnClickedBtnAddaccount)
END_MESSAGE_MAP()

DWORD WINAPI _ShowListThread(LPVOID lpParm)
{
	int nOrder = 0;
	CUpdateAccountDlg* dlg = (CUpdateAccountDlg*)lpParm;
	CListCtrl* pList = (CListCtrl*)dlg->GetDlgItem(IDC_LIST_UpdateAccount);
	// Set FULLRowSelect
	pList->SetExtendedStyle(pList->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	pList->SetTextBkColor(RGB(240, 247, 233));

	// ��ԭList
	pList->DeleteAllItems();
	int nColumnCount = pList->GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
		pList->DeleteColumn(0);

	pList->InsertColumn(0, L"���", LVCFMT_LEFT, 50);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"���", LVCFMT_LEFT, 80);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"QQ", LVCFMT_LEFT, 80);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"�ǳ�", LVCFMT_LEFT, 100);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"����", LVCFMT_LEFT, 80);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"��ͼ", LVCFMT_LEFT, 80);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"�ȼ�����", LVCFMT_LEFT, 80);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"�������", LVCFMT_LEFT, 80);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"ģʽ", LVCFMT_LEFT, 80);

	// ��������
	AccountListLock.Access([]{UpdateAccountList.assign(AccountList.begin(), AccountList.end()); });

	for (auto& __ServerAccountInfo : UpdateAccountList)
		dlg->AddListItem(__ServerAccountInfo);

	return 0;
}

VOID CUpdateAccountDlg::AddListItem(const ServerAccountInfo& __ServerAccountInfo)
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_UpdateAccount);

	int nCount = 0;
	int nRow = pList->InsertItem(pList->GetItemCount(), CAccountConfig::FormatNumber(pList->GetItemCount() + 1));

	pList->SetItemText(nRow, ++nCount, __ServerAccountInfo.wszComputerName);
	pList->SetItemText(nRow, ++nCount, __ServerAccountInfo.wszQQ);
	pList->SetItemText(nRow, ++nCount, __ServerAccountInfo.wszNiceName);
	pList->SetItemText(nRow, ++nCount, __ServerAccountInfo.wszServer);
	pList->SetItemText(nRow, ++nCount, __ServerAccountInfo.wszMapName);
	pList->SetItemText(nRow, ++nCount, CAccountConfig::FormatNumber(__ServerAccountInfo.dwMaxLevel));
	pList->SetItemText(nRow, ++nCount, CAccountConfig::FormatNumber(__ServerAccountInfo.dwMaxGold));
	pList->SetItemText(nRow, ++nCount, __ServerAccountInfo.PlayMode == AccountPlayMode_Default ? L"Ĭ��ģʽ" : L"�Զ���ģʽ");
}

// CUpdateAccountDlg message handlers
BOOL CUpdateAccountDlg::OnInitDialog()
{
	UpdateAccountList.clear();

	// ����Ĭ��ģʽ
	((CButton*)GetDlgItem(IDC_RADIO_Default))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_Custome))->SetCheck(FALSE);

	// Ĭ���ظ���ʾ
	((CButton*)GetDlgItem(IDC_CHECK_Repeatedly))->SetCheck(TRUE);

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_Server);
	

	int nCount = 0;
	pComboBox->InsertString(nCount++, L"��ŷ����");
	pComboBox->InsertString(nCount++, L"�ȶ�������");
	pComboBox->InsertString(nCount++, L"�氲");
	pComboBox->InsertString(nCount++, L"ŵ����˹");
	pComboBox->InsertString(nCount++, L"��������");
	pComboBox->InsertString(nCount++, L"��¶���");
	pComboBox->InsertString(nCount++, L"Ƥ�����ַ�");
	pComboBox->InsertString(nCount++, L"ս��ѧԺ");
	pComboBox->InsertString(nCount++, L"���׶�׿��");
	pComboBox->InsertString(nCount++, L"�����");
	pComboBox->InsertString(nCount++, L"��ɪ�ر�");
	pComboBox->InsertString(nCount++, L"��η�ȷ�");
	pComboBox->InsertString(nCount++, L"�þ�֮��");
	pComboBox->InsertString(nCount++, L"��ɫõ��");
	pComboBox->InsertString(nCount++, L"��Ӱ��");
	pComboBox->InsertString(nCount++, L"��������");
	pComboBox->InsertString(nCount++, L"ˡ����");
	pComboBox->InsertString(nCount++, L"�������");
	pComboBox->InsertString(nCount++, L"ˮ��֮��");
	pComboBox->InsertString(nCount++, L"������ר��");
	pComboBox->InsertString(nCount++, L"Ӱ��");
	pComboBox->InsertString(nCount++, L"����֮��");
	pComboBox->InsertString(nCount++, L"Ť������");
	pComboBox->InsertString(nCount++, L"����֮��");
	pComboBox->InsertString(nCount++, L"��������");
	pComboBox->InsertString(nCount++, L"Ƥ�Ǿ���");
	pComboBox->InsertString(nCount++, L"����֮��");

	CComboBox* pComboBoxMap = (CComboBox*)GetDlgItem(IDC_COMBO_Map);
	pComboBoxMap->InsertString(0, L"�ٻ�ʦϿ��");
	pComboBoxMap->InsertString(1, L"ˮ��֮��");
	pComboBoxMap->InsertString(2, L"Ť������");
	pComboBoxMap->InsertString(3, L"������Ԩ");
	pComboBoxMap->InsertString(4, L"����");
	pComboBoxMap->EnableWindow(FALSE);

	chBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)_ShowListThread, this, NULL, NULL);
	return TRUE;
}

void CUpdateAccountDlg::OnNMClickListUpdateaccount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1 && pNMListView->iSubItem != -1)
	{
		g_nSelListIndex = pNMListView->iItem;

		CListCtrl* pList = (CListCtrl *)GetDlgItem(IDC_LIST_UpdateAccount);
		((CStatic*)GetDlgItem(IDC_STATIC_ComputerName))->SetWindowTextW(pList->GetItemText(pNMListView->iItem, 1));
		((CStatic*)GetDlgItem(IDC_STATIC_QQ))->SetWindowTextW(pList->GetItemText(pNMListView->iItem, 2));
		((CStatic*)GetDlgItem(IDC_STATIC_NiceName))->SetWindowTextW(pList->GetItemText(pNMListView->iItem, 3));

		////////����//////////////////////////////////////////////////////////////////
		CComboBox* pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_Server);
		CString strServer = pList->GetItemText(pNMListView->iItem, 4);
		
		for (INT i = 0; i < pComboBox->GetCount(); ++i)
		{
			CString str;
			pComboBox->GetLBText(i, str);
			if (str == strServer)
			{
				pComboBox->SetCurSel(i);
				break;
			}
		}

		///////��ͼ///////////////////////////////////////////////////////////////////
		CComboBox* pComboBoxMap = (CComboBox *)GetDlgItem(IDC_COMBO_Map);
		CString strMap = pList->GetItemText(pNMListView->iItem, 5);

		for (INT i = 0; i < pComboBoxMap->GetCount(); ++i)
		{
			CString str;
			pComboBoxMap->GetLBText(i, str);
			if (str == strMap)
			{
				pComboBoxMap->SetCurSel(i);
				break;
			}
		}

		CString strLevel = pList->GetItemText(pNMListView->iItem, 6);
		CString strGold = pList->GetItemText(pNMListView->iItem, 7);
		strLevel = strLevel.Mid(strLevel.Find(L"/") + 1);
		strGold = strGold.Mid(strGold.Find(L"/") + 1);

		((CStatic*)GetDlgItem(IDC_EDIT_Level))->SetWindowTextW(strLevel);
		((CStatic*)GetDlgItem(IDC_EDIT_Gold))->SetWindowTextW(strGold);

		if (CCharacter::wstrcmp_my(L"Ĭ��ģʽ", pList->GetItemText(pNMListView->iItem, 8)))
		{
			((CButton*)GetDlgItem(IDC_RADIO_Default))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_RADIO_Custome))->SetCheck(FALSE);
			((CComboBox*)GetDlgItem(IDC_COMBO_Map))->EnableWindow(FALSE);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_RADIO_Default))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_RADIO_Custome))->SetCheck(TRUE);
			((CComboBox*)GetDlgItem(IDC_COMBO_Map))->EnableWindow(TRUE);
		}
	}

	*pResult = 0;
}


void CUpdateAccountDlg::OnBnClickedRadioCustome()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_Map))->EnableWindow(TRUE);
}


void CUpdateAccountDlg::OnBnClickedRadioDefault()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_Map))->EnableWindow(FALSE);
	((CComboBox*)GetDlgItem(IDC_COMBO_Map))->SetCurSel(4);
}


void CUpdateAccountDlg::OnBnClickedBtnDelete()
{
	try
	{
		CListCtrl* pList = (CListCtrl *)GetDlgItem(IDC_LIST_UpdateAccount);

		if (((CButton*)GetDlgItem(IDC_CHECK_Repeatedly))->GetCheck())
		{
			if (AfxMessageBox(L"�Ƿ�ȷ����ȷ��Ҫɾ��?", MB_YESNO) == IDNO)
				return;
		}

		if (g_nSelListIndex == -1)
			return;

		// ���ѡ��
		ClearListItem();

		// �Ȱ��б�����ݸ�ɾ����
		pList->DeleteItem(g_nSelListIndex);

		ServerAccountInfo __ServerAccountInfo = UpdateAccountList.at(g_nSelListIndex);

		// ɾ���ܱ�
		CAccountModel::RemoveAccount(__ServerAccountInfo);

		// ɾ����ʱ��
		UpdateAccountList.erase(UpdateAccountList.begin() + g_nSelListIndex);

		g_nSelListIndex = -1;
	}
	catch(...)
	{
		CPrintLog::MsgBoxLog_W(_SELF,__LINE__,L"OnBnClickedBtnDelete�����쳣");
	}
}

VOID CUpdateAccountDlg::ClearListItem()
{
	((CStatic*)GetDlgItem(IDC_STATIC_ComputerName))->SetWindowTextW(L"������������");
	((CStatic*)GetDlgItem(IDC_STATIC_QQ))->SetWindowTextW(L"������������");
	((CStatic*)GetDlgItem(IDC_STATIC_NiceName))->SetWindowTextW(L"������������");
	((CComboBox*)GetDlgItem(IDC_COMBO_Server))->SetCurSel(-1);
	((CStatic*)GetDlgItem(IDC_EDIT_Level))->SetWindowTextW(L"");
	((CStatic*)GetDlgItem(IDC_EDIT_Gold))->SetWindowTextW(L"");
	((CButton*)GetDlgItem(IDC_RADIO_Default))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_Custome))->SetCheck(FALSE);
	((CComboBox*)GetDlgItem(IDC_COMBO_Map))->SetCurSel(-1);
	((CComboBox*)GetDlgItem(IDC_COMBO_Map))->EnableWindow(FALSE);
}

void CUpdateAccountDlg::OnBnClickedBtnUpdateaccount()
{
	CString strLevel;
	CString strGold;
	CString strServer;

	CListCtrl* pList = (CListCtrl *)GetDlgItem(IDC_LIST_UpdateAccount);

	// �޸Ĵ���
	CComboBox* pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_Server);
	if (pComboBox->GetCurSel() == -1 || g_nSelListIndex == -1)
	{
		AfxMessageBox(L"ľ�������!");
		return;
	}


	ServerAccountInfo __ServerAccountInfo = UpdateAccountList.at(g_nSelListIndex);

	// �޸Ĵ���
	pComboBox->GetLBText(pComboBox->GetCurSel(), strServer);
	CCharacter::wstrcpy_my(__ServerAccountInfo.wszServer, strServer);

	// �޸ĵȼ� �� ���
	((CEdit*)GetDlgItem(IDC_EDIT_Level))->GetWindowTextW(strLevel);
	((CEdit*)GetDlgItem(IDC_EDIT_Gold))->GetWindowTextW(strGold);

	__ServerAccountInfo.dwMaxLevel = _wtoi(strLevel.GetBuffer());
	__ServerAccountInfo.dwMaxGold = _wtoi(strGold.GetBuffer());

	// �ж�ģʽ
	if (((CButton*)GetDlgItem(IDC_RADIO_Custome))->GetCheck())
	{
		CComboBox* pComboBoxMap = (CComboBox *)GetDlgItem(IDC_COMBO_Map);
		if (pComboBoxMap->GetCurSel() == -1)
		{
			AfxMessageBox(L"�Զ���ģʽ����Ҫѡһ����ͼ!");
			return;
		}
		CString strMap;
		pComboBoxMap->GetLBText(pComboBoxMap->GetCurSel(), strMap);
		CCharacter::wstrcpy_my(__ServerAccountInfo.wszMapName, strMap);

		__ServerAccountInfo.PlayMode = AccountPlayMode_Custome;
	}
	if (__ServerAccountInfo.PlayMode == AccountPlayMode_Custome && CCharacter::wstrcmp_my(__ServerAccountInfo.wszMapName, L"����"))
	{
		AfxMessageBox(L"�Զ���ģʽ�²���ѡ����!");
		return;
	}

	// �޸Ļ���ʱ��
	UpdateAccountList.at(g_nSelListIndex) = __ServerAccountInfo;

	// �޸���ʾ�б�
	pList->SetItemText(g_nSelListIndex, 6, strLevel.GetBuffer());
	pList->SetItemText(g_nSelListIndex, 7, strGold.GetBuffer());
	pList->SetItemText(g_nSelListIndex, 8, __ServerAccountInfo.PlayMode == AccountPlayMode_Default ? L"Ĭ��ģʽ" : L"�Զ���ģʽ");
	pList->SetItemText(g_nSelListIndex, 4, __ServerAccountInfo.wszServer);
	pList->SetItemText(g_nSelListIndex, 5, __ServerAccountInfo.wszMapName);

	// �޸��ܱ�
	CAccountModel::UpdateAccount(__ServerAccountInfo);
	ClearListItem();
	g_nSelListIndex = -1;
}


void CUpdateAccountDlg::OnBnClickedBtnAddaccount()
{
	CAddAccountDlg dlg;
	dlg.DoModal();

	auto fnExist = [](const ServerAccountInfo& __ServerAccountInfo)
	{
		for (auto& tmpServerAccountInfo : UpdateAccountList)
		{
			if (tmpServerAccountInfo.CompQqCrc(__ServerAccountInfo.dwQqCrc))
				return true;
		}
		return false;
	};

	for (auto& tmpServerAccountInfo : dlg.UpdateAccountList)
	{
		// �Ƿ������ظ�
		if (!fnExist(tmpServerAccountInfo))
		{
			// �޸���ʱ��
			UpdateAccountList.push_back(tmpServerAccountInfo);

			// �޸Ľ���
			AddListItem(tmpServerAccountInfo);

			// �޸��ܱ�
			CAccountModel::AddAccount(tmpServerAccountInfo);
		}
		else
		{
			CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"�ʺ�:%s�Ѿ����� ���б���,�Ͳ��ظ������!", tmpServerAccountInfo.wszQQ);
		}
	}
}

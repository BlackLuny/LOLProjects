#ifndef __LOL_BMPFILE_H__
#define __LOL_BMPFILE_H__

#include "VarBase.h"
#include <string>
using namespace std;
#ifndef tstring
#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif
#endif

struct Points
{
	POINT pts;
	struct Points *next;
};

class CBmpFind
{
public:
	CBmpFind(void);
	virtual ~CBmpFind(void);
	int BindWindow(HWND hWnd);
	void SetPath(LPCTSTR lpszPath);

	void GetScreenData(int dx, int dy);
	bool GetBmpData(LPCTSTR lpszName);
	//�ͷ�lpBuf
	int ReleaseLP();
	int GetScrPixel(int x, int y);//��ȡ��Ļ��ɫ
	int GetBmpPixel(int x, int y);//��ȡλͼ��ɫ
	//���Ҿ�����������ɫֵΪ color�ĵ�
	//������������������
	//�ҵ�����1,�������긳ֵ��������fx��fy��,�Ҳ����򷵻�0
	int FindColor(int x1, int y1, int x2, int y2, int color, int &fx, int &fy);
	POINT FindFirstPoint(int x1, int y1, int x2, int y2);
	//�Ƚ���Ļpt��λͼ���δ�С��������ɫ��λ���Ƿ���λͼ��ͬ
	int Compare(POINT pt);
	//������char *lpBufscr�в���*lpBufbmp;
	int FindBmpBuf(int x1, int y1, int x2, int y2, int &fx, int &fy);
	//�Ӿ���x1,y1,x2,y2�ڲ�����λͼlpszName��ȫƥ�����ʼ��
	//���ҵ�,(fx,fy)ΪͼƬ��һ�γ��ֵ����ϽǶ���,������1
	//���Ҳ���,(fx,fy)=(-1,-1)������0
	int FindPic(int x1, int y1, int x2, int y2, LPCTSTR lpszName, int &fx, int &fy);
	//�Ƚ���Ļpt��λͼ���δ�С��������ɫ��λ���Ƿ���λͼ����ƥ��
	//����λͼbackcolor��ɫ�㣬��ɫƫ��deltacolor��ƥ�侫��sim
	//�Ƚϳɹ�����1�����򷵻�0
	int CompareEx(POINT pt, int backcolor, int deltacolor, double sim);
	int ColorCompare(int colorB, int colorS, int deltacolor);
	//�Ӿ���x1,y1,x2,y2�ڲ�����λͼlpszName����ƥ�����ʼ��
	//����ƥ���,(fx,fy)ΪͼƬ��һ�γ��ֵ����ϽǶ���,������ƥ�����
	//����λͼbackcolor��ɫ�㣬��ɫƫ��deltacolor��ƥ�侫��sim
	int FindPic(int x1, int y1, int x2, int y2, LPCTSTR lpszName, int backcolor, int deltacolor, double sim, int &fx, int &fy);
	void FromWndPoint(int &x1, int &y1, int &x2, int &y2);
private:
	char   *lpBufscr, *lpBufbmp;
	BITMAP   bb, bm;//bb=��Ļ  bm=ͼƬ
	DWORD   dwX, dwY;
	HWND	hWnd;
	tstring  sPath;
};


#endif// __LOL_BMPFILE_H__
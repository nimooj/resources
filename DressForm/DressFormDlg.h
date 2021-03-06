
// DressFormDlg.h : header file
//

#pragma once

#include "Body.h"
#include "ObjBody.h"
#include "Camera.h"
#include <stdlib.h>

#include "./include/GL/glew.h"
#include "./include/GL/wglew.h"
#include "./include/GL/glut.h"
#include "./include/glm/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "./include/glm/gtx/transform.hpp"
#include "./include/glm/gtc/type_ptr.hpp"

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "./lib/glew32.lib")


// CDressFormDlg dialog
class CDressFormDlg : public CDialogEx
{
// Construction
public:
	CDressFormDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRESSFORM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_picture;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


protected:
	virtual BOOL GetOldStyleRenderingContext(void);

private:
	//OpenGL Setup
	BOOL GetRenderingContext();
	//Rendering Context and Device Context Pointeres
	HGLRC m_hRC;
	CDC* m_pDC;

	int width;
	int height;

	Camera c;
	Body b;
	ObjBody ob;


	vector<Vertex> vertices;
	int vertNum;

	vector<Vertex> joints;
	int jointNum;

	vector<Vertex> mannequinBody; // Both head and arm less body.
	vector<Vertex> dressFormBody; // just Torso. Later expand hip circumference to ankle height.
	int f = 0;

public:
	afx_msg void OnBnClickedFront();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedGenerate();
	CEdit bustSize;
	CEdit hipSize;
	CEdit waistSize;
	afx_msg void OnBnClickedDefaultsize();
	CEdit easeSize;
	afx_msg void OnBnClickedOk();
};

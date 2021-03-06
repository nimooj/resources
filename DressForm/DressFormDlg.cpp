
// DressFormDlg.cpp : implementation file
//


#include "stdafx.h"
#include "Resource.h"
#include "DressForm.h"
#include "DressFormDlg.h"
#include "afxdialogex.h"

#define MAX_PATH 1024

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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

// CDressFormDlg dialog

CDressFormDlg::CDressFormDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRESSFORM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDressFormDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Control(pDX, IDC_BUST, bustSize);
	DDX_Control(pDX, IDC_HIP, hipSize);
	DDX_Control(pDX, IDC_WAIST, waistSize);
	DDX_Control(pDX, IDC_EASE, easeSize);
}

BEGIN_MESSAGE_MAP(CDressFormDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_Front, &CDressFormDlg::OnBnClickedFront)
	ON_BN_CLICKED(IDC_Right, &CDressFormDlg::OnBnClickedRight)
	ON_BN_CLICKED(IDC_Left, &CDressFormDlg::OnBnClickedLeft)
	ON_BN_CLICKED(IDC_Up, &CDressFormDlg::OnBnClickedUp)
	ON_BN_CLICKED(IDC_EXPORT, &CDressFormDlg::OnBnClickedExport)
	ON_BN_CLICKED(IDC_GENERATE, &CDressFormDlg::OnBnClickedGenerate)
	ON_BN_CLICKED(IDC_DEFAULTSIZE, &CDressFormDlg::OnBnClickedDefaultsize)
	ON_BN_CLICKED(IDOK, &CDressFormDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDressFormDlg message handlers

BOOL CDressFormDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	string::size_type posit = string(buffer).find_last_of("\\/");
	string rootPath = string(buffer).substr(0, posit);

	ifstream infile("makehuman/Result/dressform.txt");
	if (infile.good()) {
		infile.close();
		remove("makehuman/Result/dressform.txt");
	}

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//Create OpenGL Context
	if (!GetRenderingContext()) {
		AfxMessageBox(CString("Error in OpenGL initialization."));
		return -1;
	}


	// Get Rect size
	CRect rect;
	GetClientRect(&rect);
	width = rect.Width();
	height = rect.Height();

	c = Camera();
	vec3 pos = c.cameraPosition;
	vec3 dir = c.cameraFront;
	vec3 up = c.cameraUp;

	b = Body();
	joints = b.GetJoints();
	vertices = b.GetVertices();
	vertNum = vertices.size();
	jointNum = joints.size();

	/*
	//dressFormBody = b.severBody(TORSO);
	//b.GetLooseDressForm(dressFormBody);
	//dressFormBody = b.severBody(FOOTLESS);
	//b.GetTightDressForm(dressFormBody);
	dressFormBody = b.severBody(TORSO);
	b.GetTop(dressFormBody);
	vector<Vertex> torso = b.severBody(TORSO);
	b.GetTop(torso);
	vector<Vertex> lowerBody = b.severBody(LOWERBODY);
	vector<Vertex> legs = b.severBody(LEGS);
	b.GetPants(legs);
	*/
	dressFormBody = b.severBody(LEGS);
	b.GetPants(dressFormBody);
	

	//AfxGetMainWnd()->SendMessage(WM_CLOSE);

	/*
	ob = ObjBody();
	ob.importVertices();
	ob.importJoints();
	ob.importFaces();
	joints = ob.joints;
	vertices = ob.vertices;
	dressFormBody = ob.getDressForm();
	*/

	CString bust;
	CString waist;
	CString hip;
	CString ease;
	bust.Format(_T("%f"), b.bust);
	waist.Format(_T("%f"), b.waist);
	hip.Format(_T("%f"), b.hip);
	ease.Format(_T("%f"), 0.0f);

	bustSize.SetWindowTextW(bust);
	waistSize.SetWindowTextW(waist);
	hipSize.SetWindowTextW(hip);
	easeSize.SetWindowTextW(ease);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_BLEND);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	//gluPerspective(radians(45.0f), (float)width/(float)height, -100.0f, 100.0f);
	glLoadIdentity();
	glOrthof(-10, 10, -10, 10, 0, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//SetTimer(1000, 3000, NULL);
	SetTimer(1000, 30, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDressFormDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDressFormDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDressFormDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDressFormDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here

	if (FALSE == ::wglDeleteContext(m_hRC)) {
		AfxMessageBox(CString("wglDeleteContext failed"));
	}
}


void CDressFormDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnTimer(nIDEvent);

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	vec3 pos = c.cameraPosition;
	vec3 dir = c.cameraFront;
	vec3 up = c.cameraUp;

	glPushMatrix();

	gluLookAt(pos[0], pos[1], pos[2], dir[0], dir[1], dir[2], up[0], up[1], up[2]);
	//gluLookAt(0, 10, 0, 0, 0, 0, 0, 0, 1);

	/*
	glColor4f(1, 0, 0, 0.8);
	ob.layers->curr = ob.layers->head;
	for ( int i = 0; i < ob.layers->length; i++) {
		//glBegin(GL_LINE_LOOP);
		glBegin(GL_POINTS);
		for (int j = 0; j < ob.layers->curr->value.size(); j++) {
			Vertex v = ob.layers->curr->value[j];
			glVertex3f(v.x, v.y, v.z);
		}

		glEnd();

		ob.layers->curr = ob.layers->curr->next;
	}
	f = 1;
	*/

	glColor4f(1, 1, 1, 0.8);
	b.layers->curr = b.layers->head;
	for ( int i = 0; i < b.layers->length; i++) {
		//glBegin(GL_LINE_LOOP);
		glBegin(GL_POINTS);
		for (int j = 0; j < b.layers->curr->value.size(); j++) {
			Vertex v = b.layers->curr->value[j];
			glVertex3f(v.x, v.y, v.z);
		}

		glEnd();

		b.layers->curr = b.layers->curr->next;
	}
	f = 1;

	glBegin(GL_POINTS);
		glColor4f(1, 1, 1, 0.4);
		for (int i = 0; i < dressFormBody.size(); i++) {
		//for( int i =0 ; i < mannequinBody.size(); i++) {
		//for( int i =0 ; i < vertices.size(); i++) {
			//glVertex3f(mannequinBody[i].x, mannequinBody[i].y, mannequinBody[i].z);
			glVertex3f(dressFormBody[i].x, dressFormBody[i].y, dressFormBody[i].z);
			//glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		}

		/*
		*/
		glColor4f(1, 0, 0, 1);
		for (int i = 0; i < joints.size(); i++)
		{
			glVertex3f(joints[i].x, joints[i].y, joints[i].z);
		}
	glEnd();

	glPopMatrix();
	glFlush();


	//화면 업데이트
	SwapBuffers(m_pDC->GetSafeHdc());
}

BOOL CDressFormDlg::GetRenderingContext() {
	//픽처 컨트롤에만 그리도록 DC 생성
	//참고 https://goo.gl/CK36zE
	CWnd* pImage = GetDlgItem(IDC_PICTURE);
	CRect rc;
	pImage->GetWindowRect(rc);
	m_pDC = pImage->GetDC();


	if (NULL == m_pDC)
	{
		AfxMessageBox(CString("Unable to get a DC"));
		return FALSE;
	}


	if (!GetOldStyleRenderingContext())
	{
		return TRUE;
	}


	//Get access to modern OpenGL functionality from this old style context.
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		AfxMessageBox(CString("GLEW could not be initialized!"));
		return FALSE;
	}


	//참고 http://gamedev.stackexchange.com/a/30443
	GLint attribs[] =
	{
		//OpenGL 2.0 사용
		WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		// Uncomment this for forward compatibility mode
		//WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		// Uncomment this for Compatibility profile
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		// We are using Core profile here
		//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};


	HGLRC CompHRC = wglCreateContextAttribsARB(m_pDC->GetSafeHdc(), 0, attribs);
	if (CompHRC && wglMakeCurrent(m_pDC->GetSafeHdc(), CompHRC))
		m_hRC = CompHRC;

	return TRUE;
}

BOOL CDressFormDlg::GetOldStyleRenderingContext() {
	//A generic pixel format descriptor. This will be replaced with a more
	//specific and modern one later, so don't worry about it too much.
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,               // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		32,                             // 32-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		24,                        // 24-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};

	// Get the id number for the best match supported by the hardware device context
	// to what is described in pfd
	int pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	//If there's no match, report an error
	if (0 == pixelFormat)
	{
		AfxMessageBox(CString("ChoosePixelFormat failed"));
		return FALSE;
	}

	//If there is an acceptable match, set it as the current 
	if (FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd))
	{
		AfxMessageBox(CString("SetPixelFormat failed"));
		return FALSE;
	}

	//Create a context with this pixel format
	if (0 == (m_hRC = wglCreateContext(m_pDC->GetSafeHdc())))
	{
		AfxMessageBox(CString("wglCreateContext failed"));
		return FALSE;
	}

	//Make it current. 
	if (FALSE == wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
	{
		AfxMessageBox(CString("wglMakeCurrent failed"));
		return FALSE;
	}
	return TRUE;
}

void CDressFormDlg::OnBnClickedFront()
{
	// TODO: Add your control notification handler code here
	c.ViewFront();
}

void CDressFormDlg::OnBnClickedRight()
{
	// TODO: Add your control notification handler code here
	c.ViewRight();
}

void CDressFormDlg::OnBnClickedLeft()
{
	// TODO: Add your control notification handler code here
	c.ViewLeft();
}

void CDressFormDlg::OnBnClickedUp()
{
	// TODO: Add your control notification handler code here
	c.ViewUp();
}

void CDressFormDlg::OnBnClickedExport()
{
	// TODO: Add your control notification handler code here
	//b.WriteToOBJ();
	//b.writeToObj(dressFormBody);
	//b.writeToObj(b.vertices);
	//ob.writeToObj();
}


void CDressFormDlg::OnBnClickedGenerate()
{
	// TODO: Add your control notification handler code here
	CString bust;
	CString waist;
	CString hip;

	bustSize.GetWindowTextW(bust);
	waistSize.GetWindowTextW(waist);
	hipSize.GetWindowTextW(hip);

	b.SetSize(_ttof(bust), _ttof(waist), _ttof(hip));

	bust.Format(_T("%f"), b.bust);
	waist.Format(_T("%f"), b.waist);
	hip.Format(_T("%f"), b.hip);

	bustSize.SetWindowTextW(bust);
	waistSize.SetWindowTextW(waist);
	hipSize.SetWindowTextW(hip);
}


void CDressFormDlg::OnBnClickedDefaultsize()
{
	// TODO: Add your control notification handler code here
	CString bust;
	CString waist;
	CString hip;
	CString ease;

	easeSize.GetWindowTextW(ease);
	float easef = _ttof(ease);

	ifstream sizeFile("makehuman/Result/sizes.txt");
	vector<string> st;

	string line;
	while (getline(sizeFile, line)) {
		istringstream iss(line);
		st.push_back(line);
	}

	b.SetSize(atof(st[2].c_str()) + easef, atof(st[3].c_str()) + easef, atof(st[4].c_str()) + easef);

	bust.Format(_T("%f"), b.bust);
	waist.Format(_T("%f"), b.waist);
	hip.Format(_T("%f"), b.hip);

	bustSize.SetWindowTextW(bust);
	waistSize.SetWindowTextW(waist);
	hipSize.SetWindowTextW(hip);

	sizeFile.close();
}


void CDressFormDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

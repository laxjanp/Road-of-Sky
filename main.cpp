//=============================================================================
// �Q�[����i�wRoad of sky�x
// �����:���{�b��
//=============================================================================

//*****************************************************************************
// �C���N���[�h�K�[�h(#include)
//*****************************************************************************
#include "main.h"
#include "polygon.h"
#include "input.h"
#include "number.h"
#include "Sound.h"
#include "scene.h"

//-------- ���C�u�����̃����N
#pragma comment(lib, "winmm")
#pragma comment(lib, "imm32")
#pragma comment(lib, "d3d11")

//*****************************************************************************
// �}�N����`(#define)
//*****************************************************************************
#define CLASS_NAME _T("AppClass")			// �E�C���h�E�̃N���X��
#define WINDOW_NAME _T("Road of sky")		// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾(�֐��Ăяo��)
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int OnCreate(HWND hWnd, LPCREATESTRUCT lpcs);
HRESULT Init(HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// �O���[�o���ϐ�(g_)
//*****************************************************************************
HWND g_hWnd;									// ���C�� �E�B���h�E �n���h��
HINSTANCE g_hInst;								// �C���X�^���X �n���h��
ID3D11Device* g_pDevice;						// �f�o�C�X
ID3D11DeviceContext* g_pDeviceContext;			// �f�o�C�X �R���e�L�X�g
IDXGISwapChain* g_pSwapChain;					// �X���b�v�`�F�[��
ID3D11RenderTargetView* g_pRenderTargetView;	// �t���[���o�b�t�@
UINT g_uSyncInterval = 0;						// �������� (0:��, 1:�L)
ID3D11BlendState* g_pBlendState[MAX_BLENDSTATE];// �u�����h �X�e�[�g

int g_nCountFPS;								// FPS�\���̃J�E���^�p
unsigned g_uNumber;								// ���l�p�̃J�E���^�p(�e�X�g)

CScene scene;									// �V�[���N���X�Ŏg�p����ϐ�
CNumber number;									// �i���o�[�N���X�̕ϐ�

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine,int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// COM �̏�����
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		return -1;
	}

	WNDCLASSEX wcex = {sizeof(WNDCLASSEX),CS_CLASSDC,WndProc,
	0,0,hInstance,LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINFRM)),LoadCursor
	(nullptr, IDC_ARROW),(HBRUSH)(COLOR_WINDOW + 1),nullptr,CLASS_NAME,nullptr};
	MSG msg;
	
	// �C���X�^���X �n���h���ۑ�
	g_hInst = hInstance;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);
	
	// �N���C�A���g�̈�T�C�Y����E�B���h�E �T�C�Y�Z�o
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION| WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX;
	DWORD dwExStyle = 0;
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);
	
	// �E�B���h�E�̍쐬
	g_hWnd = CreateWindowEx(dwExStyle,CLASS_NAME,WINDOW_NAME,dwStyle,
	CW_USEDEFAULT,		// �E�B���h�E�̍����W
	CW_USEDEFAULT,		// �E�B���h�E�̏���W
	rc.right - rc.left, // �E�B���h�E����
	rc.bottom - rc.top, // �E�B���h�E�c��
	nullptr,nullptr,hInstance,nullptr);
	
	// �t���[���J�E���g�̏�����
	timeBeginPeriod(1);				//����\��ݒ肷��J�E���^�p
	DWORD dwExecLastTime = timeGetTime();
	DWORD dwFPSLastTime = dwExecLastTime;
	DWORD dwCurrentTime = 0;
	DWORD dwFrameCount = 0;

	// �E�C���h�E�̕\��
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	if	(FAILED(Init(g_hWnd, true))) 
	{
		return -1;
	}

	// ���b�Z�[�W���[�v
	for (;;)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) 
			{
				// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if (dwCurrentTime - dwFPSLastTime >= 500)	//0.5�b���Ɍv�Z����
			{
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if (dwCurrentTime - dwExecLastTime >= 1000 / 60)
			{
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

			}

			

			// �`�揈��
			Draw();
			dwFrameCount++;
		}
	}

	// �I������
	Uninit();

	// �^�C�}�����ɖ߂�
	timeEndPeriod(1);
	
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, g_hInst);

	// COM �̉��
	CoUninitialize();

	return (int)msg.wParam;

}

//=============================================================================
// �E�B���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
	case WM_CREATE: //----- �E�B���h�E���������ꂽ
		return OnCreate(hWnd, (LPCREATESTRUCT)lParam);
	
	case WM_DESTROY: //----- �E�B���h�E�j���w��������
		PostQuitMessage(0); // �V�X�e���ɃX���b�h�̏I����v��
		return 0;
	
	case WM_KEYDOWN: //----- �L�[�{�[�h�������ꂽ
		switch (wParam)
		{
		case VK_ESCAPE: // [ESC]�L�[�������ꂽ
			PostMessage(hWnd, WM_CLOSE, 0, 0); // [x]�������ꂽ�悤�ɐU����
			return 0;
		}
		break;
	
	case WM_MENUCHAR:
		return MNC_CLOSE << 16; // [Alt]+[Enter]����BEEP��}�~
	
	default:
		break;
	}
	
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// WM_CREATE���b�Z�[�W�n���h��
//=============================================================================
int OnCreate(HWND hWnd, LPCREATESTRUCT lpcs)
{
	// �N���C�A���g�̈�T�C�Y��SCREEN_WIDTH�~SCREEN_HEIGHT�ɍĐݒ�.
	RECT rcClnt;
	GetClientRect(hWnd, &rcClnt);
	rcClnt.right -= rcClnt.left;
	rcClnt.bottom -= rcClnt.top;
	if (rcClnt.right != SCREEN_WIDTH || rcClnt.bottom != SCREEN_HEIGHT) 
	{
		RECT rcWnd;
		GetWindowRect(hWnd, &rcWnd);
		SIZE sizeWnd;
		sizeWnd.cx = (rcWnd.right - rcWnd.left) - rcClnt.right + SCREEN_WIDTH;
		sizeWnd.cy = (rcWnd.bottom - rcWnd.top) - rcClnt.bottom + SCREEN_HEIGHT;
		SetWindowPos(hWnd, nullptr, 0, 0, sizeWnd.cx, sizeWnd.cy, SWP_NOMOVE |SWP_NOZORDER | SWP_NOOWNERZORDER);
	}
	
	// IME������
	ImmAssociateContext(hWnd, nullptr);
	return 0; // -1��Ԃ���CreateWindow(Ex)�����s����.

}

//=============================================================================
// �o�b�N�o�b�t�@����
//=============================================================================
HRESULT CreateBackBuffer(void)
{
	// �����_�[�^�[�Q�b�g�r���[����
	ID3D11Texture2D* pBackBuffer = nullptr;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);
	
	// �e�^�[�Q�b�g�r���[�������_�[�^�[�Q�b�g�ɐݒ�
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
	
	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = (float)SCREEN_WIDTH;
	vp.Height = (float)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pDeviceContext->RSSetViewports(1, &vp);
	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;
	
	// �f�o�C�X�A�X���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = bWindow;
	
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_3,
	D3D_FEATURE_LEVEL_9_2,
	D3D_FEATURE_LEVEL_9_1,
	};

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &scd,
		&g_pSwapChain, &g_pDevice, nullptr, &g_pDeviceContext);
	
	if (FAILED(hr)) 
	{
		return hr;
	}

	// �o�b�N�o�b�t�@����
	hr = CreateBackBuffer();
	if (FAILED(hr)) 
	{
		return hr;
	}
	
	// ���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.CullMode = D3D11_CULL_NONE;
	rd.FillMode = D3D11_FILL_SOLID;
	ID3D11RasterizerState* pRs = nullptr;
	g_pDevice->CreateRasterizerState(&rd, &pRs);
	g_pDeviceContext->RSSetState(pRs);
	SAFE_RELEASE(pRs);
	
	// �u�����h �X�e�[�g����
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// ��������������
	g_pDevice->CreateBlendState(&BlendDesc,&g_pBlendState[BS_NONE]);
	
	// ����������
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	g_pDevice->CreateBlendState(&BlendDesc,&g_pBlendState[BS_ALPHABLEND]);
	
	// ���Z����
	BlendDesc.RenderTarget[0].DestBlend =D3D11_BLEND_ONE;
	g_pDevice->CreateBlendState(&BlendDesc,&g_pBlendState[BS_ADDITIVE]);
	
	// ���Z����
	BlendDesc.RenderTarget[0].BlendOp =D3D11_BLEND_OP_REV_SUBTRACT;
	g_pDevice->CreateBlendState(&BlendDesc,&g_pBlendState[BS_SUBTRACTION]);
	
	// �K��l�𔼓��������ɐݒ�
	SetBlendState(BS_ALPHABLEND);

	// �|���S��������
	hr = InitPolygon(g_pDevice);
	if (FAILED(hr)) 
	{
		return hr;
	}
	
	// ���͏�����
	hr = InitInput();
	if (FAILED(hr)) 
	{
		return hr;
	}

	// �T�E���h������
	CSound::Init();
	
	// ���l�\��������
	hr = number.Init();
	//hr = InitNumber();
	if (FAILED(hr)) 
	{
		MessageBox(g_hWnd, _T("���l�\�����������s"), NULL, MB_OK);
		return hr;
	}

	// ��ʑJ�ڏ�����
	hr = scene.Init();
	if (FAILED(hr))
	{
		MessageBox(g_hWnd, _T("��ʑJ�ڏ��������s"), NULL, MB_OK);
		return hr;
	}

	return hr;
}

//=============================================================================
// �o�b�N�o�b�t�@���
//=============================================================================
void ReleaseBackBuffer()
{
	if (g_pDeviceContext) 
	{
		g_pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}
	
	SAFE_RELEASE(g_pRenderTargetView);
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	// ��ʑJ�ڏI������
	scene.Uninit();
	
	// ���l�\���I������
	number.Uninit();
	//UninitNumber();
	
	// �T�E���h�I������
	CSound::Fin();
	
	// ���͏I������
	UninitInput();
	
	// �|���S���I������
	UninitPolygon();
	
	// �u�����h �X�e�[�g���
	for (int i = 0; i < MAX_BLENDSTATE; ++i) 
	{
		SAFE_RELEASE(g_pBlendState[i]);
	}
	
	// �o�b�N�o�b�t�@���
	ReleaseBackBuffer();
	
	// �X���b�v�`�F�[�����
	SAFE_RELEASE(g_pSwapChain);
	
	// �f�o�C�X �R���e�L�X�g�̊J��
	SAFE_RELEASE(g_pDeviceContext);
	
	// �f�o�C�X�̊J��
	SAFE_RELEASE(g_pDevice);

}

//=============================================================================
// ���C�� �E�B���h�E �n���h���擾
//=============================================================================
HWND GetMainWnd()
{
	return g_hWnd;
}

//=============================================================================
// �C���X�^���X �n���h���擾
//=============================================================================
HINSTANCE GetInstance()
{
	return g_hInst;
}

//=============================================================================
// �f�o�C�X�擾
//=============================================================================
ID3D11Device* GetDevice()
{
	return g_pDevice;
}

//=============================================================================
// �f�o�C�X �R���e�L�X�g�擾
//=============================================================================
ID3D11DeviceContext* GetDeviceContext()
{
	return g_pDeviceContext;
}

//=============================================================================
//SetBlendState�֐�
//���e:�u�����h �X�e�[�g�ݒ�
//=============================================================================
void SetBlendState(int nBlendState)
{
	if (nBlendState < 0 || nBlendState >= MAX_BLENDSTATE)
	{
		return;
	}
		
	float blendFactor[4] ={ 0.0f, 0.0f, 0.0f, 0.0f };
	g_pDeviceContext->OMSetBlendState(g_pBlendState[nBlendState], blendFactor,0xffffffff);
}


//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	// TODO: �����ɍX�V�������L�q.
	if (GetActiveWindow() != g_hWnd)
	{
		return;
	}
		
	// ���͍X�V(Update�֐��̏��߂ɌĂяo��)
	UpdateInput();
	
	// �V�[���X�V
	scene.Update();
	
	// �T�E���h�X�V
	CSound::Update();

}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@�̃N���A
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	g_pDeviceContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	
	// �V�[���`��
	scene.Draw();
	
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pSwapChain->Present(g_uSyncInterval, 0);
}

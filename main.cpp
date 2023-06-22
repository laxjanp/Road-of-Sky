//=============================================================================
// ゲーム作品『Road of sky』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
// インクルードガード(#include)
//*****************************************************************************
#include "main.h"
#include "polygon.h"
#include "input.h"
#include "number.h"
#include "Sound.h"
#include "scene.h"

//-------- ライブラリのリンク
#pragma comment(lib, "winmm")
#pragma comment(lib, "imm32")
#pragma comment(lib, "d3d11")

//*****************************************************************************
// マクロ定義(#define)
//*****************************************************************************
#define CLASS_NAME _T("AppClass")			// ウインドウのクラス名
#define WINDOW_NAME _T("Road of sky")		// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言(関数呼び出し)
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int OnCreate(HWND hWnd, LPCREATESTRUCT lpcs);
HRESULT Init(HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// グローバル変数(g_)
//*****************************************************************************
HWND g_hWnd;									// メイン ウィンドウ ハンドル
HINSTANCE g_hInst;								// インスタンス ハンドル
ID3D11Device* g_pDevice;						// デバイス
ID3D11DeviceContext* g_pDeviceContext;			// デバイス コンテキスト
IDXGISwapChain* g_pSwapChain;					// スワップチェーン
ID3D11RenderTargetView* g_pRenderTargetView;	// フレームバッファ
UINT g_uSyncInterval = 0;						// 垂直同期 (0:無, 1:有)
ID3D11BlendState* g_pBlendState[MAX_BLENDSTATE];// ブレンド ステート

int g_nCountFPS;								// FPS表示のカウンタ用
unsigned g_uNumber;								// 数値用のカウンタ用(テスト)

CScene scene;									// シーンクラスで使用する変数
CNumber number;									// ナンバークラスの変数

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine,int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// COM の初期化
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		return -1;
	}

	WNDCLASSEX wcex = {sizeof(WNDCLASSEX),CS_CLASSDC,WndProc,
	0,0,hInstance,LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINFRM)),LoadCursor
	(nullptr, IDC_ARROW),(HBRUSH)(COLOR_WINDOW + 1),nullptr,CLASS_NAME,nullptr};
	MSG msg;
	
	// インスタンス ハンドル保存
	g_hInst = hInstance;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);
	
	// クライアント領域サイズからウィンドウ サイズ算出
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION| WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX;
	DWORD dwExStyle = 0;
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);
	
	// ウィンドウの作成
	g_hWnd = CreateWindowEx(dwExStyle,CLASS_NAME,WINDOW_NAME,dwStyle,
	CW_USEDEFAULT,		// ウィンドウの左座標
	CW_USEDEFAULT,		// ウィンドウの上座標
	rc.right - rc.left, // ウィンドウ横幅
	rc.bottom - rc.top, // ウィンドウ縦幅
	nullptr,nullptr,hInstance,nullptr);
	
	// フレームカウントの初期化
	timeBeginPeriod(1);				//分解能を設定するカウンタ用
	DWORD dwExecLastTime = timeGetTime();
	DWORD dwFPSLastTime = dwExecLastTime;
	DWORD dwCurrentTime = 0;
	DWORD dwFrameCount = 0;

	// ウインドウの表示
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// DirectXの初期化(ウィンドウを作成してから行う)
	if	(FAILED(Init(g_hWnd, true))) 
	{
		return -1;
	}

	// メッセージループ
	for (;;)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) 
			{
				// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if (dwCurrentTime - dwFPSLastTime >= 500)	//0.5秒毎に計算する
			{
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if (dwCurrentTime - dwExecLastTime >= 1000 / 60)
			{
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

			}

			

			// 描画処理
			Draw();
			dwFrameCount++;
		}
	}

	// 終了処理
	Uninit();

	// タイマを元に戻す
	timeEndPeriod(1);
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, g_hInst);

	// COM の解放
	CoUninitialize();

	return (int)msg.wParam;

}

//=============================================================================
// ウィンドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
	case WM_CREATE: //----- ウィンドウが生成された
		return OnCreate(hWnd, (LPCREATESTRUCT)lParam);
	
	case WM_DESTROY: //----- ウィンドウ破棄指示がきた
		PostQuitMessage(0); // システムにスレッドの終了を要求
		return 0;
	
	case WM_KEYDOWN: //----- キーボードが押された
		switch (wParam)
		{
		case VK_ESCAPE: // [ESC]キーが押された
			PostMessage(hWnd, WM_CLOSE, 0, 0); // [x]が押されたように振舞う
			return 0;
		}
		break;
	
	case WM_MENUCHAR:
		return MNC_CLOSE << 16; // [Alt]+[Enter]時のBEEPを抑止
	
	default:
		break;
	}
	
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// WM_CREATEメッセージハンドラ
//=============================================================================
int OnCreate(HWND hWnd, LPCREATESTRUCT lpcs)
{
	// クライアント領域サイズをSCREEN_WIDTH×SCREEN_HEIGHTに再設定.
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
	
	// IME無効化
	ImmAssociateContext(hWnd, nullptr);
	return 0; // -1を返すとCreateWindow(Ex)が失敗する.

}

//=============================================================================
// バックバッファ生成
//=============================================================================
HRESULT CreateBackBuffer(void)
{
	// レンダーターゲットビュー生成
	ID3D11Texture2D* pBackBuffer = nullptr;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);
	
	// 各ターゲットビューをレンダーターゲットに設定
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
	
	// ビューポート設定
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
// 初期化処理
//=============================================================================
HRESULT Init(HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;
	
	// デバイス、スワップチェーンの作成
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

	// バックバッファ生成
	hr = CreateBackBuffer();
	if (FAILED(hr)) 
	{
		return hr;
	}
	
	// ラスタライズ設定
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.CullMode = D3D11_CULL_NONE;
	rd.FillMode = D3D11_FILL_SOLID;
	ID3D11RasterizerState* pRs = nullptr;
	g_pDevice->CreateRasterizerState(&rd, &pRs);
	g_pDeviceContext->RSSetState(pRs);
	SAFE_RELEASE(pRs);
	
	// ブレンド ステート生成
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

	// 半透明合成無し
	g_pDevice->CreateBlendState(&BlendDesc,&g_pBlendState[BS_NONE]);
	
	// 半透明合成
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	g_pDevice->CreateBlendState(&BlendDesc,&g_pBlendState[BS_ALPHABLEND]);
	
	// 加算合成
	BlendDesc.RenderTarget[0].DestBlend =D3D11_BLEND_ONE;
	g_pDevice->CreateBlendState(&BlendDesc,&g_pBlendState[BS_ADDITIVE]);
	
	// 減算合成
	BlendDesc.RenderTarget[0].BlendOp =D3D11_BLEND_OP_REV_SUBTRACT;
	g_pDevice->CreateBlendState(&BlendDesc,&g_pBlendState[BS_SUBTRACTION]);
	
	// 規定値を半透明合成に設定
	SetBlendState(BS_ALPHABLEND);

	// ポリゴン初期化
	hr = InitPolygon(g_pDevice);
	if (FAILED(hr)) 
	{
		return hr;
	}
	
	// 入力初期化
	hr = InitInput();
	if (FAILED(hr)) 
	{
		return hr;
	}

	// サウンド初期化
	CSound::Init();
	
	// 数値表示初期化
	hr = number.Init();
	//hr = InitNumber();
	if (FAILED(hr)) 
	{
		MessageBox(g_hWnd, _T("数値表示初期化失敗"), NULL, MB_OK);
		return hr;
	}

	// 画面遷移初期化
	hr = scene.Init();
	if (FAILED(hr))
	{
		MessageBox(g_hWnd, _T("画面遷移初期化失敗"), NULL, MB_OK);
		return hr;
	}

	return hr;
}

//=============================================================================
// バックバッファ解放
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
// 終了処理
//=============================================================================
void Uninit(void)
{
	// 画面遷移終了処理
	scene.Uninit();
	
	// 数値表示終了処理
	number.Uninit();
	//UninitNumber();
	
	// サウンド終了処理
	CSound::Fin();
	
	// 入力終了処理
	UninitInput();
	
	// ポリゴン終了処理
	UninitPolygon();
	
	// ブレンド ステート解放
	for (int i = 0; i < MAX_BLENDSTATE; ++i) 
	{
		SAFE_RELEASE(g_pBlendState[i]);
	}
	
	// バックバッファ解放
	ReleaseBackBuffer();
	
	// スワップチェーン解放
	SAFE_RELEASE(g_pSwapChain);
	
	// デバイス コンテキストの開放
	SAFE_RELEASE(g_pDeviceContext);
	
	// デバイスの開放
	SAFE_RELEASE(g_pDevice);

}

//=============================================================================
// メイン ウィンドウ ハンドル取得
//=============================================================================
HWND GetMainWnd()
{
	return g_hWnd;
}

//=============================================================================
// インスタンス ハンドル取得
//=============================================================================
HINSTANCE GetInstance()
{
	return g_hInst;
}

//=============================================================================
// デバイス取得
//=============================================================================
ID3D11Device* GetDevice()
{
	return g_pDevice;
}

//=============================================================================
// デバイス コンテキスト取得
//=============================================================================
ID3D11DeviceContext* GetDeviceContext()
{
	return g_pDeviceContext;
}

//=============================================================================
//SetBlendState関数
//内容:ブレンド ステート設定
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
// 更新処理
//=============================================================================
void Update(void)
{
	// TODO: ここに更新処理を記述.
	if (GetActiveWindow() != g_hWnd)
	{
		return;
	}
		
	// 入力更新(Update関数の初めに呼び出し)
	UpdateInput();
	
	// シーン更新
	scene.Update();
	
	// サウンド更新
	CSound::Update();

}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファのクリア
	float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
	g_pDeviceContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	
	// シーン描画
	scene.Draw();
	
	// バックバッファとフロントバッファの入れ替え
	g_pSwapChain->Present(g_uSyncInterval, 0);
}

//=============================================================================
// タイマーの表示コード『Timer.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "timer.h"
#include "number.h"
#include "Texture.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義(#define)
//*****************************************************************************
#define PATH_TEXTURE_FRAMETIMER L"data/texture/frame_timer.png"
#define WIDTH_FRAMETIMER 140.0f
#define HEIGHT_FRAMETIMER 80.0f
#define POS_X_FRAMETIMER (SCREEN_WIDTH - WIDTH_FRAMETIMER) * 0.5f
#define POS_Y_FRAMETIMER (SCREEN_HEIGHT - HEIGHT_FRAMETIMER) * 0.5f

#define CHRCNT_TIMER 3
#define POS_X_TIMER (POS_X_FRAMETIMER - NUMBER_SIZE_X * CHRCNT_TIMER * 0.5f)
#define POS_Y_TIMER (POS_Y_FRAMETIMER + 16.0f)
#define START_TIMER 60

//*****************************************************************************
// グローバル変数(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static int g_nTimer;
CNumber TimeNumber;			// ナンバークラス


// 初期化
HRESULT InitTimer(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_TEXTURE_FRAMETIMER, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	//変数の初期化
	g_nTimer = START_TIMER * 60 + 59;

	return hr;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitTimer()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimer()
{
	if (g_nTimer > 0)
	{
		--g_nTimer;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTimer()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	//タイマー枠の表示
	SetPolygonSize(WIDTH_FRAMETIMER, HEIGHT_FRAMETIMER);
	SetPolygonPos(POS_X_FRAMETIMER - 400, POS_Y_FRAMETIMER);

	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);

	// タイマー表示
	TimeNumber.Draw(XMFLOAT2(POS_X_TIMER - 400, POS_Y_TIMER), (unsigned)(g_nTimer / 60), CHRCNT_TIMER);
}

//=============================================================================
//AddTimer関数
//内容:スコア表示加算処理
//引数:スコアを加算させるnScore
//戻り値:なし
//=============================================================================
void AddTimer(int nSec)
{
	g_nTimer += nSec * 60;
}

//=============================================================================
//GetTimer関数
//内容:スコア表示取得処理
//引数:なし
//戻り値:スコアを取得するためのg_nTimer
//=============================================================================
int GetTimer()
{
	return g_nTimer / 60;
}
//=============================================================================
// ナンバー専用コード『number.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "number.h"
#include "Texture.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義(#define)
//*****************************************************************************
#define PATH_TEXTURE_NUMBER L"data/texture/number000.png"
#define COUNT_X_NUMBER 10
#define COUNT_Y_NUMBER 1

//*****************************************************************************
// グローバル変数(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;


// 初期化処理
HRESULT CNumber::Init()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_TEXTURE_NUMBER, &g_pTexture);

	return hr;
}

// 終了処理
void CNumber::Uninit()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

// 描画処理
void CNumber::Draw(XMFLOAT2 vPos, unsigned uNumber, int nWidth,float fSizeX, float fSizeY)
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(fSizeX, fSizeY);
	SetPolygonTexture(g_pTexture);
	SetPolygonFrameSize(1.0f / COUNT_X_NUMBER, 1.0f / COUNT_Y_NUMBER);

	vPos.y -= fSizeY * 0.5f;
	vPos.x += fSizeX * (nWidth - 0.5f);

	for (; nWidth > 0; --nWidth, vPos.x -= fSizeX)
	{
		unsigned n = uNumber % 10;
		uNumber /= 10;
		SetPolygonPos(vPos.x, vPos.y);
		SetPolygonUV((n % COUNT_X_NUMBER) / (float)COUNT_X_NUMBER,
			(n / COUNT_X_NUMBER) / (float)COUNT_Y_NUMBER);
		DrawPolygon(pDC);
	}

	// 元に戻す
	SetPolygonUV(0.0f, 0.0f);
	SetPolygonFrameSize(1.0f, 1.0f);
}

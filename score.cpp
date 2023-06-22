//=============================================================================
// スコア表示専用コード『score.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "score.h"
#include "number.h"
#include "Texture.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義(#define)
//*****************************************************************************
#define PATH_TEXTURE_FRAMESCORE L"data/texture/frame_score.png"
#define WIDTH_FRAMESCORE 340.0f
#define HEIGHT_FRAMESCORE 80.0f
#define POS_X_FRAMESCORE (SCREEN_WIDTH - WIDTH_FRAMESCORE) * 0.5f
#define POS_Y_FRAMESCORE (SCREEN_HEIGHT - HEIGHT_FRAMESCORE) * 0.5f

#define CHRCNT_SCORE 8
#define POS_X_SCORE (POS_X_FRAMESCORE - NUMBER_SIZE_X * CHRCNT_SCORE * 0.5f)
#define POS_Y_SCORE (POS_Y_FRAMESCORE + 16.0f)

//*****************************************************************************
// グローバル変数(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static int g_nScore;

CNumber ScoreNumber;		// ナンバークラスの変数

// 初期化
HRESULT InitScore(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_TEXTURE_FRAMESCORE, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	//変数の初期化
	g_nScore = 0;

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 明るい緑に設定
	SetPolygonColor(0.0f, 1.0f, 0.0f);
	
	// スコア枠表示
	SetPolygonSize(WIDTH_FRAMESCORE, HEIGHT_FRAMESCORE);
	SetPolygonPos(POS_X_FRAMESCORE, POS_Y_FRAMESCORE);

	SetPolygonTexture(g_pTexture);
	DrawPolygon(pDC);

	// スコアの表示
	ScoreNumber.Draw(XMFLOAT2(POS_X_SCORE, POS_Y_SCORE), (unsigned)g_nScore, CHRCNT_SCORE);
	//DrawNumber(XMFLOAT2(POS_X_SCORE, POS_Y_SCORE),(unsigned)g_nScore, CHRCNT_SCORE);
	
	// 色を元に戻す
	SetPolygonColor(1.0f, 1.0f, 1.0f);

}

//=============================================================================
//AddScore関数
//内容:スコア表示加算処理
//引数:スコアを加算させるnScore
//戻り値:なし
//=============================================================================
void AddScore(int nScore)
{
	g_nScore += nScore;
}

//=============================================================================
//GetScore関数
//内容:スコア表示取得処理
//引数:なし
//戻り値:スコアを取得するためのg_nScore
//=============================================================================
int GetScore() 
{
	return g_nScore;
}
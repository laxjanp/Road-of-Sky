//=============================================================================
// 弾用ソースコード『bullet.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "bullet.h"
#include "Texture.h"
#include "polygon.h"
#include "debugproc.h"
#include "enemy.h"
#include "enemy2.h"
#include "life.h"
#include "score.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義(#define)
//*****************************************************************************
#define PATH_BULLET_TEXTURE L"data/texture/bullet.png"
#define POS_X_BULLET 0.0f
#define POS_Y_BULLET 0.0f
#define BULLET_WIDTH 32
#define BULLET_HEIGHT 32

#define SPEED_BULLET 15.0f
#define MAX_BULLET 100

#define COUNT_X_BULLET 4	//横枠
#define COUNT_Y_BULLET 4	//縦枠
#define FRAME_BULLET 3		//枠No

#define COLLISION_WIDTH 14.0f
#define COLLISION_HEIGHT 20.0f


//*****************************************************************************
// 構造体定義(struct)
//*****************************************************************************
struct BULLET
{
	XMFLOAT2 vPos;			// 位置
	XMFLOAT2 vVel;			// 速度
};

//*****************************************************************************
// グローバル変数(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static BULLET g_bullet[MAX_BULLET];		// 弾情報
static int g_nBullet;					// 使用中弾数

// 初期化
HRESULT InitBullet()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_BULLET_TEXTURE, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	// 初期化
	g_nBullet = 0;
	return hr;
}

// 終了処理
void UninitBullet()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

// 更新
void UpdateBullet()
{

	XMFLOAT2 vBox(COLLISION_WIDTH, COLLISION_HEIGHT);
	BULLET* pBullet;
	for (int i = g_nBullet - 1; i >= 0; --i)
	{
		pBullet = &g_bullet[i];

		// 移動
		pBullet->vPos.x += pBullet->vVel.x;
		pBullet->vPos.y += pBullet->vVel.y;

		// 画面外に出たら消去
		if (pBullet->vPos.x <
			(SCREEN_WIDTH + BULLET_WIDTH) * -0.5f || pBullet->vPos.x >
			(SCREEN_WIDTH + BULLET_WIDTH) * 0.5f || pBullet->vPos.y <
			(SCREEN_HEIGHT + BULLET_HEIGHT) * -0.5f || pBullet->vPos.y >
			(SCREEN_HEIGHT + BULLET_HEIGHT) * 0.5f)
		{
			--g_nBullet;
			if (g_nBullet > 0)
			{
				*pBullet = g_bullet[g_nBullet];
			}

			continue;
		}

		// TODO: 当たり判定
		if (CollisionEnemy(pBullet->vPos, vBox, 1.0f) >= 0)
		{
			--g_nBullet;
			AddScore(50);
			GetScore();
			if (g_nBullet > 0)
			{
				*pBullet = g_bullet[g_nBullet];
				continue;
			}

		}

		if (CollisionEnemy2(pBullet->vPos, vBox, 1.0f) >= 0)
		{
			--g_nBullet;
			AddScore(400);
			GetScore();
			if (g_nBullet > 0)
			{
				*pBullet = g_bullet[g_nBullet];
				continue;
			}
		}

	}
}

// 描画
void DrawBullet()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(BULLET_WIDTH, BULLET_HEIGHT);
	SetPolygonTexture(g_pTexture);
	BULLET* pBullet = g_bullet;
	for (int i = 0; i < g_nBullet; ++i, ++pBullet)
	{
		SetPolygonPos(pBullet->vPos.x, pBullet->vPos.y);
		DrawPolygon(pDC);
	}

	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);

}

// 発射
int FireBullet(XMFLOAT2 vPos, int nDir)
{
	if (g_nBullet < MAX_BULLET)
	{
		BULLET* pBullet = &g_bullet[g_nBullet];
		pBullet->vPos = vPos;

		switch (nDir)
		{
		case 0://下
			pBullet->vVel.x = SPEED_BULLET;
			pBullet->vVel.y = 0.0f;
			break;

		case 1://左
			pBullet->vVel.x = SPEED_BULLET;
			pBullet->vVel.y = 0.0f;
			break;

		case 2://右
			pBullet->vVel.x = SPEED_BULLET;
			pBullet->vVel.y = 0.0f;
			break;

		case 3://上
			pBullet->vVel.x = SPEED_BULLET;
			pBullet->vVel.y = 0.0f;
			break;
		}

		++g_nBullet;
		CSound::Play(SE_SHOT);
		return g_nBullet;
	}

	return 0;
}
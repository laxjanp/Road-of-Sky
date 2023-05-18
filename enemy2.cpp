//=============================================================================
// 敵専用コード『enemy2.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "enemy2.h"
#include "Texture.h"
#include "polygon.h"
#include "collision.h"
#include "bullet.h"
#include "effect.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義(#define)
//*****************************************************************************
#define PATH_ENEMYTEXTURE L"data/texture/Enemy2.png"
#define ENEMY2_WIDTH 68*2
#define ENEMY2_HEIGHT 30*2
#define ENEMY2_SPEED 8.5f

#define MAX_ENEMY2 3	//最大の敵数

#define COLLISION_WIDTH 30
#define COLLISION_HEIGHT 20
#define COLLISION_OFFSETX 0.0f
#define COLLISION_OFFSETY -12.0f


//*****************************************************************************
// 構造体定義(struct)
//*****************************************************************************
struct ENEMY2 // 敵情報
{
	XMFLOAT2 vPos;	// 表示位置
	XMFLOAT2 vSize;	// 座標位置
	int nDir;		// 方向(0:下,1:左,2:右,3:上)
};

//*****************************************************************************
// グローバル変数(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static ENEMY2 g_enemy2[MAX_ENEMY2]; // 敵情報
static int g_nEnemy2;				// 敵現在数


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy2()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_ENEMYTEXTURE,&g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	// 変数初期化
	g_nEnemy2 = 0;		//敵の数を保存

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy2()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy2()
{
	ENEMY2* pEnemy;

	for (int i = g_nEnemy2 - 1; i >= 0; --i)
	{
		pEnemy = &g_enemy2[i];

		//移動処理
		bool bOut = false;
		switch (pEnemy->nDir)
		{
		case 0: // 下
			pEnemy->vPos.y -= ENEMY2_SPEED;
			if (pEnemy->vPos.y < (SCREEN_HEIGHT + ENEMY2_HEIGHT) * -0.5f) 
				bOut = true;
			break;
		case 1: // 左
			pEnemy->vPos.x -= ENEMY2_SPEED;
			if (pEnemy->vPos.x < (SCREEN_WIDTH + ENEMY2_WIDTH) * -0.5f)
				bOut = true;
			break;
		case 2: // 右
			pEnemy->vPos.x += ENEMY2_SPEED;
			if (pEnemy->vPos.x > (SCREEN_WIDTH + ENEMY2_WIDTH) * 0.5f)
				bOut = true;
			break;
		case 3: // 上
			pEnemy->vPos.y += ENEMY2_SPEED;
			if (pEnemy->vPos.y > (SCREEN_HEIGHT + ENEMY2_HEIGHT) * 0.5f)
				bOut = true;
			break;
		}

		if (bOut)		//消す
		{
			--g_nEnemy2;

			if (g_nEnemy2 > 0)
			{
				*pEnemy = g_enemy2[g_nEnemy2];
				
				continue;

			}
		}
	}

	// 生成する
	static int nTimer = 0;
	++nTimer;
	if (nTimer > 120 && g_nEnemy2 < MAX_ENEMY2)
	{
		nTimer = 0;
		pEnemy = &g_enemy2[g_nEnemy2];

		// 方向選択
		pEnemy->nDir = 1;

		// 位置初期化
		switch (pEnemy->nDir)
		{
		case 0: // 下
			pEnemy->vPos.y = (SCREEN_HEIGHT + ENEMY2_HEIGHT) * 0.5f;
			pEnemy->vPos.x = rand() % SCREEN_WIDTH - SCREEN_WIDTH * 0.5f;
			break;

		case 3: // 上
			pEnemy->vPos.y = (SCREEN_HEIGHT + ENEMY2_HEIGHT) * -0.5f;
			pEnemy->vPos.x = rand() % SCREEN_WIDTH - SCREEN_WIDTH * 0.5f;
			break;

		case 1: // 左
			pEnemy->vPos.x = (SCREEN_WIDTH + ENEMY2_WIDTH) * 0.5f;
			pEnemy->vPos.y = rand() % SCREEN_HEIGHT - SCREEN_HEIGHT * 0.5f;
			break;

		case 2: // 右
			pEnemy->vPos.x = (SCREEN_WIDTH + ENEMY2_WIDTH) * -0.5f;
			pEnemy->vPos.y = rand() % SCREEN_HEIGHT - SCREEN_HEIGHT * 0.5f;
			break;
		}

		// 追加データを有効化
		++g_nEnemy2;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy2()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(ENEMY2_WIDTH, ENEMY2_HEIGHT);
	SetPolygonAngle(360);
	SetPolygonTexture(g_pTexture);
	ENEMY2* pEnemy2 = g_enemy2;

	for (int i = 0; i < g_nEnemy2; ++i, ++pEnemy2)
	{
		SetPolygonPos(pEnemy2->vPos.x, pEnemy2->vPos.y);
		DrawPolygon(pDC);
	}

	// 元に戻す
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}

//=============================================================================
// 当たり(衝突)判定
//=============================================================================
int CollisionEnemy2(XMFLOAT2 vCenter, XMFLOAT2 vBox, float fDamage)
{
	XMFLOAT2 vEnemyPos;
	XMFLOAT2 vEnemyBox(COLLISION_WIDTH, COLLISION_HEIGHT);
	ENEMY2* pEnemy = g_enemy2;
	for (int i = 0; i < g_nEnemy2; ++i, ++pEnemy)
	{
		vEnemyPos.x = pEnemy->vPos.x - 30 + COLLISION_OFFSETX;
		vEnemyPos.y = pEnemy->vPos.y + COLLISION_OFFSETY;
		if (CollisionBox(vEnemyPos, vEnemyBox, vCenter, vBox))
		{
			if (fDamage > 0.0f)	//仮
			{
				CSound::Play(SE_EXPLOSION);
				StartEffect(vEnemyPos);
				--g_nEnemy2;
				if (g_nEnemy2 > 0)
				{
					*pEnemy = g_enemy2[g_nEnemy2];
				}

			}
			return i;
		}
	}
	return -1;
}
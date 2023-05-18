//=============================================================================
// 敵専用ソースコード『enemy.cpp』
// 制作者:松本恵太
//=============================================================================

//*****************************************************************************
//インクルードガード(#include)
//*****************************************************************************
#include "enemy.h"
#include "Texture.h"
#include "polygon.h"
#include "collision.h"
#include "effect.h"
#include "Sound.h"

//*****************************************************************************
// マクロ定義(#define)
//*****************************************************************************
#define PATH_ENEMY_TEXTURE L"data/texture/enemy001.png"
#define ENEMY_WIDTH 64
#define ENEMY_HEIGHT 64
#define ENEMY_SPEED 4.0f

#define MAX_ENEMY 30	//最大の敵数

#define COLLISION_WIDTH 30
#define COLLISION_HEIGHT 20
#define COLLISION_OFFSETX 0.0f
#define COLLISION_OFFSETY -12.0f


//*****************************************************************************
// 構造体定義(struct)
//*****************************************************************************
struct ENEMY			// 敵情報
{
	XMFLOAT2 vPos;		// 表示位置
	int nDir;			// 方向(0:下,1:左,2:右,3:上)
};

//*****************************************************************************
// グローバル変数(g_)
//*****************************************************************************
static ID3D11ShaderResourceView* g_pTexture;
static ENEMY g_enemy[MAX_ENEMY];	// 敵情報
static int g_nEnemy;				// 敵現在数


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// テクスチャ読込
	hr = CreateTextureFromFile(pDevice, PATH_ENEMY_TEXTURE, &g_pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	// 変数初期化
	g_nEnemy = 0;

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy()
{
	// テクスチャ解放
	SAFE_RELEASE(g_pTexture);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy()
{
	ENEMY* pEnemy;
	for (int i = g_nEnemy - 1; i >= 0; --i)
	{
		pEnemy = &g_enemy[i];

		// 移動
		bool bOut = false;
		switch (pEnemy->nDir)
		{
		case 0: // 下
			pEnemy->vPos.y -= ENEMY_SPEED;
			if (pEnemy->vPos.y < (SCREEN_HEIGHT + ENEMY_HEIGHT) * -0.5f)
				bOut = true;
			break;
		case 1: // 左
			pEnemy->vPos.x -= ENEMY_SPEED;
			if (pEnemy->vPos.x < (SCREEN_WIDTH + ENEMY_WIDTH) * -0.5f)
				bOut = true;
			break;
		case 2: // 右
			pEnemy->vPos.x += ENEMY_SPEED;
			if (pEnemy->vPos.x > (SCREEN_WIDTH + ENEMY_WIDTH) * 0.5f)
				bOut = true;
			break;
		case 3: // 上
			pEnemy->vPos.y += ENEMY_SPEED;
			if (pEnemy->vPos.y > (SCREEN_HEIGHT + ENEMY_HEIGHT) * 0.5f)
				bOut = true;
			break;
		}

		// 消去
		if (bOut)
		{
			--g_nEnemy;

			if (g_nEnemy > 0)
			{
				*pEnemy = g_enemy[g_nEnemy];

				continue;
			}
		}

	}

	// 生成する
	static int nTimer = 0;
	++nTimer;
	if (nTimer > 20 && g_nEnemy < MAX_ENEMY)
	{
		nTimer = 0;
		pEnemy = &g_enemy[g_nEnemy];

		// 方向選択
		pEnemy->nDir = 1;

		// 位置初期化
		switch (pEnemy->nDir)
		{
		case 0: // 下
			pEnemy->vPos.y = (SCREEN_HEIGHT + ENEMY_HEIGHT) * 0.5f;
			pEnemy->vPos.x = rand() % SCREEN_WIDTH - SCREEN_WIDTH * 0.5f;
			break;

		case 3: // 上
			pEnemy->vPos.y = (SCREEN_HEIGHT + ENEMY_HEIGHT) * -0.5f;
			pEnemy->vPos.x = rand() % SCREEN_WIDTH - SCREEN_WIDTH * 0.5f;
			break;

		case 1: // 左
			pEnemy->vPos.x = (SCREEN_WIDTH + ENEMY_WIDTH) * 0.5f;
			pEnemy->vPos.y = rand() % SCREEN_HEIGHT - SCREEN_HEIGHT * 0.5f;
			break;

		case 2: // 右
			pEnemy->vPos.x = (SCREEN_WIDTH + ENEMY_WIDTH) * -0.5f;
			pEnemy->vPos.y = rand() % SCREEN_HEIGHT - SCREEN_HEIGHT * 0.5f;
			break;
		}

		// 追加データを有効化
		++g_nEnemy;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy()
{
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetPolygonSize(ENEMY_WIDTH, ENEMY_HEIGHT);
	SetPolygonAngle(-90);
	SetPolygonTexture(g_pTexture);
	ENEMY* pEnemy = g_enemy;

	for (int i = 0; i < g_nEnemy; ++i, ++pEnemy)
	{
		SetPolygonPos(pEnemy->vPos.x, pEnemy->vPos.y);
		DrawPolygon(pDC);
	}

	// 元に戻す
	SetPolygonFrameSize(1.0f, 1.0f);
	SetPolygonUV(0.0f, 0.0f);
}

//=============================================================================
// 当たり(衝突)判定
//=============================================================================
int CollisionEnemy(XMFLOAT2 vCenter, XMFLOAT2 vBox, float fDamage)
{
	XMFLOAT2 vEnemyPos;
	XMFLOAT2 vEnemyBox(COLLISION_WIDTH, COLLISION_HEIGHT);
	ENEMY* pEnemy = g_enemy;
	for (int i = 0; i < g_nEnemy; ++i, ++pEnemy)
	{
		vEnemyPos.x = pEnemy->vPos.x + COLLISION_OFFSETX;
		vEnemyPos.y = pEnemy->vPos.y + COLLISION_OFFSETY;
		if (CollisionBox(vEnemyPos, vEnemyBox, vCenter, vBox))
		{

			if (fDamage > 0.0f)	//仮
			{
				CSound::Play(SE_EXPLOSION);
				StartEffect(vEnemyPos);
				--g_nEnemy;
				if (g_nEnemy > 0)
				{
					*pEnemy = g_enemy[g_nEnemy];
				}

			}
			return i;
		}
	}
	return -1;
}
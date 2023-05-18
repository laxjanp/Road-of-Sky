//=============================================================================
// 背景描画『bullet.h』
// 制作者:松本恵太
//=============================================================================

#pragma once

#include "main.h"

//関数のプロトタイプ
HRESULT InitBullet();			// 初期化
void UninitBullet();			// 終了
void UpdateBullet();			// 更新
void DrawBullet();				// 描画
int FireBullet(XMFLOAT2, int);	// 発射
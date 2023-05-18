//=============================================================================
// 背景描画『effect.h』
// 制作者:松本恵太
//=============================================================================

#pragma once

#include "main.h"

//関数のプロトタイプ
HRESULT InitEffect();			// 初期化
void UninitEffect();			// 終了
void UpdateEffect();			// 更新
void DrawEffect();				// 描画
int StartEffect(XMFLOAT2 vPos);	// 爆発の開始
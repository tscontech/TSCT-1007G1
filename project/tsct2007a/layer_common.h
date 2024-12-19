/**
 * @file layer_common.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __LAYERCOMM_H__
#define __LAYERCOMM_H__

typedef enum {
    AUTH_WAIT_LAYER = 1,
    AUTH_MEMBERCARD_LAYER = 2,
    AUTH_QR_LAYER = 3,
    AUTH_CREADIT_LAYER = 4,
    AUTH_LAYER_END = 5,
} AUTH_LAYER;

typedef void (*HookerFunction)(void*);

void TopHomeBtnVisible(bool flag);
void TopBackBtnVisible(bool flag);
void GotoStartLayer(void);
void GotoChargeLayer(void);
void GotoNextAuthLayer(AUTH_LAYER layer);

#endif

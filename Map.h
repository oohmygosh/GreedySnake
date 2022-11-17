//
// Created by Lee on 2022/6/16.
//

#ifndef GREEDYSNAKE_MAP_H
#include <graphics.h>
#define GREEDYSNAKE_MAP_H

static int with = GetPrivateProfileInt(_T("MAP"), _T("with"), 140, _T("./setting.ini")) < 140 ? 140 : (GetPrivateProfileInt(_T("MAP"), _T("with"), 140, _T("./setting.ini")) / 10) * 10;
static int high = GetPrivateProfileInt(_T("MAP"), _T("high"), 100, _T("./setting.ini")) < 100 ? 100 : (GetPrivateProfileInt(_T("MAP"), _T("high"), 100, _T("./setting.ini")) / 10) * 10;
class Map {
public:
    Map();
    // 初始化地图
    static void initMap();
};


#endif //GREEDYSNAKE_MAP_H

//
//  AMapNaviLocation.h
//  AMapNaviKit
//
//  Created by eidan on 2018/1/18.
//  Copyright © 2018年 Amap. All rights reserved.
//

#import "AMapNaviCommonObj.h"

///当前的自车位置
@interface AMapNaviLocation : NSObject<NSCopying>

///经纬度
@property (nonatomic, strong) AMapNaviPoint *coordinate;

///精确度
@property (nonatomic, assign) double accuracy;

///高度
@property (nonatomic, assign) double altitude;

///方向
@property (nonatomic, assign) NSInteger heading;

///速度(km/h)
@property (nonatomic, assign) NSInteger speed;

///时间戳
@property (nonatomic, strong) NSDate *timestamp;

///是否匹配在道路上
@property (nonatomic, assign) BOOL isMatchNaviPath;

///当前所在的segment段的index,从0开始 since 6.7.0
@property (nonatomic, assign) int currentSegmentIndex;

///当前所在的link段的index,从0开始 since 6.7.0
@property (nonatomic, assign) int currentLinkIndex;

///当前自车位置在当前link段所在的point的index,从0开始 since 6.7.0
@property (nonatomic, assign) int currentPointIndex;

///当前定位点是否为参与导航的网络点 since 6.8.0
@property (nonatomic, assign) BOOL isNetworkNavi;

@end

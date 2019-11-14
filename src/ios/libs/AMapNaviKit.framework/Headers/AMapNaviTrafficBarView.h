//
//  AMapNaviTrafficBarView.h
//  AMapNaviKit
//
//  Created by AutoNavi on 14-7-11.
//  Copyright (c) 2014年 AutoNavi. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AMapNaviDriveDataRepresentable.h"

///导航路况光柱view. 需通过 AMapNaviDriveManager 的 -addDataRepresentative: 方法进行注册.
@interface AMapNaviTrafficBarView : UIView <AMapNaviDriveDataRepresentable>

///是否显示车图标,默认YES
@property (nonatomic, assign) BOOL showCar;

///光柱是否表示全程,即偏航后小车是否还在原来的位置(不是重新从底部开始).默认为YES. since 6.2.0
@property (nonatomic, assign) BOOL wholeCourse;

///外边框的宽度,默认为4. since 6.2.0
@property (nonatomic, assign) CGFloat borderWidth;

///外边框的颜色,默认为白色. since 6.2.0
@property (nonatomic, strong, nonnull) UIColor *borderColor;

///交通状态的颜色数组 \n 例如:{(AMapNaviRouteStatusSlow): [UIColor yellowColor],(AMapNaviRouteStatusSeriousJam): [UIColor colorWithRed:160/255.0 green:8/255.0 blue:8/255.0 alpha:1.0]}，设置空字典恢复默认颜色,例如:{}
@property (nonatomic, copy) NSDictionary<NSNumber *, UIColor *> * _Nullable statusColors;

/**
 * @brief 更新路况光柱 已废弃，请通过 AMapNaviDriveManager 的 -addDataRepresentative: 将 AMapNaviTrafficBarView 注册为数据接收者，即可自动更新光柱信息，无需再主动调用此方法，since 6.2.0
 * @param trafficStatuses 路况信息数组,可以通过 AMapNaviDriveManager 的getTrafficStatuses方法获取.
 */
- (void)updateTrafficBarWithTrafficStatuses:(nullable NSArray<AMapNaviTrafficStatus *> *)trafficStatuses __attribute__((deprecated("已废弃，请通过 AMapNaviDriveManager 的 -addDataRepresentative: 将 AMapNaviTrafficBarView 注册为数据接收者，即可自动更新光柱信息，无需再主动调用此方法，可参考官方demo中的CustomUIViewController例子，since 6.2.0")));

/**
 * @brief 更新车图标的位置 已废弃，请通过 AMapNaviDriveManager 的 -addDataRepresentative: 将 AMapNaviTrafficBarView 注册为数据接收者，即可自动更新光柱百分比，无需再主动调用此方法，since 6.2.0
 * @param posPercent 位置百分比(范围:[0,1.0])
 */
- (void)updateTrafficBarWithCarPositionPercent:(double)posPercent __attribute__((deprecated("已废弃，请通过 AMapNaviDriveManager 的 -addDataRepresentative: 将 AMapNaviTrafficBarView 注册为数据接收者，即可自动更新光柱百分比，无需再主动调用此方法，可参考官方demo中的CustomUIViewController例子，since 6.2.0")));

@end

//
//  AMapNaviCompositeUserConfig.h
//  AMapNaviKit
//
//  Created by eidan on 2017/6/23.
//  Copyright © 2017年 Amap. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AMapNaviCommonObj.h"

///导航组件的配置类 since 5.2.0
@interface AMapNaviCompositeUserConfig : NSObject

/**
 * @brief 设置起点、终点、途径点（最多支持三个途径点）来调起路线规划页面,自动完成算路. 如果不设置起点,默认使用的是“我的位置”.
 * @param type POI点的起终点类型,参考 AMapNaviRoutePlanPOIType .
 * @param locationPoint POI点的经纬度坐标,必填. SDK会使用该坐标点进行路径规划,参考 AMapNaviPoint .
 * @param name POI点的名字,选填. SDK会将该名字显示在搜索栏内. 如果不传将使用默认值,如“终点”、“途径点1”.
 * @param mid POI点的高德唯一标识ID,选填. 如果传入，SDK将会优先使用此ID对应的POI点的经纬度坐标、名字等信息进行算路和展示.
 * @return 是否设置成功
 */
- (BOOL)setRoutePlanPOIType:(AMapNaviRoutePlanPOIType)type location:(AMapNaviPoint *_Nonnull)locationPoint name:(NSString *_Nullable)name POIId:(NSString *_Nullable)mid;

/**
 * @brief 设置导航组件启动时，直接进入导航界面（跳过路径规划页面）进行算路并自动开始导航. since 5.3.0
 * @param directly 是否直接进入导航界面, 默认值为NO. 注意：如果为YES，为了保证算路成功，还需设置有效的终点，否则会报错
 */
- (void)setStartNaviDirectly:(BOOL)directly;

/**
 * @brief 设置导航组件的主题皮肤类型. since 5.4.0
 * @param themeType 主题皮肤类型，默认为 AMapNaviCompositeThemeTypeDefault（蓝色）,参考 AMapNaviCompositeThemeType .
 */
- (void)setThemeType:(AMapNaviCompositeThemeType)themeType;

/**
 * @brief 设置导航组件启动时，是否需要进行路径规划. 此设置只有在setStartNaviDirectly为YES时，才起作用. since 5.4.0
 * @param need 是否需要进行路径规划，默认为YES. 如果为NO，导航组件启动时将不再进行算路，直接使用 AMapNaviDriveManager 单例已经规划好的路径进行导航
 */
- (void)setNeedCalculateRouteWhenPresent:(BOOL)need;

/**
 * @brief 设置导航界面顶部信息是否展示随后转向图标. since 6.9.0
 * @param openNextRoadInfo 是否展示随后转向图标，默认为NO，不展示.
 */
- (void)setShowNextRoadInfo:(BOOL)showNextRoadInfo;

/**
 * @brief 设置导航组件界面dismiss时，是否调用 [AMapNaviDriveManager destroyInstance] 来尝试销毁 AMapNaviDriveManager 的单例. since 5.4.0
 * @param need 是否尝试销毁 AMapNaviDriveManager 的单例，默认为YES.
 */
- (void)setNeedDestoryDriveManagerInstanceWhenDismiss:(BOOL)need;

/**
 * @brief 设置当退出GPS导航时，是否弹出“确认退出导航”的AlertView. since 5.5.0
 * @param need 是否弹出“确认退出导航”的AlertView，默认为YES.
 */
- (void)setNeedShowConfirmViewWhenStopGPSNavi:(BOOL)need;

/**
 * @brief 设置车辆信息. since 6.0.0
 * @param vehicleInfo 车辆信息，参考 AMapNaviVehicleInfo.
 */
- (void)setVehicleInfo:(nonnull AMapNaviVehicleInfo *)vehicleInfo;

/**
 * @brief 设置驾车导航界面自定义View,该View将显示在界面的底部区域,容器View的宽度为屏幕宽度减去133,高度为46. since 6.1.0
 * @param customView 将被显示在底部区域的自定义view
 */
- (void)addCustomViewToNaviDriveView:(UIView *_Nonnull)customView;

/**
 * @brief 设置驾车导航界面左侧自定义View,该View宽度为63,高度为53,将显示在界面的左侧靠边垂直中心区域. since 6.9.0
 * @param customView 将被显示的自定义view
 */
- (void)addLeftCustomViewToNaviDriveView:(UIView *_Nonnull)customeView;

/**
 * @brief 设置驾车导航界面自定义View,该View将显示在界面的底部区域之下,容器View的宽度为屏幕宽度,高度最高为200. 注意: 设置了自定义View,导航界面将自动设置为不支持横屏 since 6.1.0
 * @param customBottomView 将被显示在底部区域之下的自定义view
 * @return 是否设置成功(高度超过200将会返回NO)
 */
- (BOOL)addCustomBottomViewToNaviDriveView:(UIView *_Nonnull)customBottomView;

/**
 * @brief 设置驾车路径规划策略. 注意：如设置，将清空用户之前选择的值。如不设置，则取用户之前选择的值，如用户之前无选择过，则取 AMapNaviDrivingStrategyMultipleDefault . since 6.1.0
 * @param driveStrategy 参考 AMapNaviDrivingStrategy .
 */
- (void)setDriveStrategy:(AMapNaviDrivingStrategy)driveStrategy;

/**
 * @brief 设置驾车导航时是否显示路口放大图. since 6.1.0
 * @param need 是否显示路口放大图，默认为YES.
 */
- (void)setShowCrossImage:(BOOL)need;

/**
 * @brief 设置路径规划偏好策略页面是否显示. since 6.1.0
 * @param need 是否显示，默认为YES.
 */
- (void)setShowDrivingStrategyPreferenceView:(BOOL)need;

/**
 * @brief 设置驾车导航界面到达目的地后是否移除路线和牵引线. since 5.5.0
 * @param need 是否移除，默认为NO.
 */
- (void)setRemovePolylineAndVectorlineWhenArrivedDestination:(BOOL)need;

/**
 * @brief 设置多路线导航模式(GPS导航中拥有若干条备选路线供用户选择，默认模式), 或单路线导航模式. 注意: 多路线导航模式还需同时满足以下4个条件才能够生效：a.路径规划时 AMapNaviDrivingStrategy 需选用多路径策略; b.起终点的直线距离需<=80KM; c.不能有途径点; d.车辆不能是货车类型. since 6.3.0
 * @param multipleRouteNaviMode YES:多路线导航模式, NO:单路线导航模式. 默认为YES.
 * @return 是否设置成功，如外界已经开始导航后调用就会返回NO
 */
- (BOOL)setMultipleRouteNaviMode:(BOOL)multipleRouteNaviMode;

/**
 * @brief 设置多路线导航模式下地图是否显示备选路线. since 6.7.0
 * @param need 是否显示，默认为YES.
 */
- (void)setShowBackupRoute:(BOOL)need;

/**
 * @brief 设置网约车模式. since 6.4.0
 * @param type 参考 AMapNaviOnlineCarHailingType. 默认为 AMapNaviOnlineCarHailingTypeNone (非网约车模式, 即正常模式)
 * @return 是否设置成功
 */
- (BOOL)setOnlineCarHailingType:(AMapNaviOnlineCarHailingType)type;

/**
 * @brief 设置地图是否展示实时路况. since 6.6.0
 * @param need 是否显示，默认为YES.
 */
- (void)setMapShowTraffic:(BOOL)need;

@end

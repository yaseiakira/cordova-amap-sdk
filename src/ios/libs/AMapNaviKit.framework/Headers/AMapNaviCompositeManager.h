//
//  AMapNaviManager.h
//  AMapNaviKit
//
//  Created by eidan on 2017/5/9.
//  Copyright © 2017年 Amap. All rights reserved.
//

#import "AMapNaviCommonObj.h"

@class AMapNaviRoute;
@class AMapNaviLocation;
@class AMapNaviCompositeUserConfig;
@class AMapNaviCompositeCustomAnnotation;
@protocol AMapNaviCompositeOverlay;

@protocol AMapNaviCompositeManagerDelegate;

///导航组件类 since 5.1.0 注意：AMapNaviCompositeManager 内部使用了单例 [AMapNaviDriveManager sharedInstance]
@interface AMapNaviCompositeManager : NSObject

///实现了 AMapNaviCompositeManagerDelegate 协议的类指针
@property (nonatomic, weak, nullable) id<AMapNaviCompositeManagerDelegate>delegate;

///当前选择的导航路径的ID
@property (nonatomic, readonly) NSInteger naviRouteID;

///当前选择的导航路径的信息,参考 AMapNaviRoute 类.
@property (nonatomic, readonly, nullable) AMapNaviRoute *naviRoute;

///路径规划后的所有路径ID,路径ID为 NSInteger 类型.
@property (nonatomic, readonly, nullable) NSArray<NSNumber *> *naviRouteIDs;

///路径规划后的所有路径信息,参考 AMapNaviRoute 类.
@property (nonatomic, readonly, nullable) NSDictionary<NSNumber *, AMapNaviRoute *> *naviRoutes;

/**
 * @brief 通过present的方式显示导航组件页面 注意：此函数涉及到UI操作，请在主线程中调用，否则无效.
 * @param options 导航组件的配置类,参考 AMapNaviCompositeUserConfig .
 */
- (void)presentRoutePlanViewControllerWithOptions:(AMapNaviCompositeUserConfig *_Nullable)options;

/**
 * @brief 退出导航组件页面 注意：此函数涉及到UI操作，请在主线程中调用，否则无效. since 5.5.0
 * @param animated 是否执行动画
 */
- (void)dismissWithAnimated:(BOOL)animated;

/**
 * @brief 在驾车导航界面添加自定义标注. 注意：每次退出导航之后,标注都会被清空. since 5.5.0
 * @param annotation 会被显示在驾车导航界面地图上, 具体参考 AMapNaviCompositeCustomAnnotation .
 */
- (void)addAnnotation:(AMapNaviCompositeCustomAnnotation *_Nonnull)annotation;

/**
 * @brief 移除驾车导航界面的自定义标注. since 5.5.0
 * @param annotation 具体参考 AMapNaviCompositeCustomAnnotation .
 */
- (void)removeAnnotation:(AMapNaviCompositeCustomAnnotation *_Nonnull)annotation;

/**
 * @brief 在驾车导航界面添加自定义Overlay. 注意：每次退出导航之后,Overlay都会被清空. since 6.7.0
 * @param customOverlay 会被显示在驾车导航界面地图上, 此对象需遵守AMapNaviCompositeOverlay协议,否则无效, 具体参考官方Demo CustomOverlayCompositeViewController 类 .
 */
- (void)addCustomOverlay:(id <AMapNaviCompositeOverlay>_Nonnull)customOverlay;

/**
 * @brief 移除驾车导航界面的自定义Overlay. since 6.7.0
 * @param customOverlay 具体参考 AMapNaviCompositeOverlay .
 */
- (void)removeCustomOverlay:(id <AMapNaviCompositeOverlay>_Nonnull)customOverlay;

@end

///AMapNaviCompositeManagerDelegate 协议 since 5.1.0
@protocol AMapNaviCompositeManagerDelegate <NSObject>

@optional

/**
 * @brief 发生错误时,会调用此方法
 * @param compositeManager 导航组件类
 * @param error 错误信息
 */
- (void)compositeManager:(AMapNaviCompositeManager *_Nonnull)compositeManager error:(NSError *_Nonnull)error;

/**
 * @brief 算路成功后的回调函数, 路径规划页面的算路、导航页面的重算等成功后均会调用此方法
 * @param compositeManager 导航组件类
 */
- (void)compositeManagerOnCalculateRouteSuccess:(AMapNaviCompositeManager *_Nonnull)compositeManager;

/**
 * @brief 算路成功后的回调函数. since 5.5.0
 * @param compositeManager 导航组件类
 * @param type 路径规划类型，参考 AMapNaviRoutePlanType .
 */
- (void)compositeManager:(AMapNaviCompositeManager *_Nonnull)compositeManager onCalculateRouteSuccessWithType:(AMapNaviRoutePlanType)type;

/**
 * @brief 算路失败后的回调函数,路径规划页面的算路、导航页面的重算等失败后均会调用此方法
 * @param compositeManager 导航组件类
 * @param error 错误信息,error.code参考 AMapNaviCalcRouteState .
 */
- (void)compositeManager:(AMapNaviCompositeManager *_Nonnull)compositeManager onCalculateRouteFailure:(NSError *_Nonnull)error;

/**
 * @brief 开始导航的回调函数
 * @param compositeManager 导航组件类
 * @param naviMode 导航类型，参考 AMapNaviMode .
 */
- (void)compositeManager:(AMapNaviCompositeManager *_Nonnull)compositeManager didStartNavi:(AMapNaviMode)naviMode;

/**
 * @brief SDK需要实时的获取是否正在进行导航信息播报，以便SDK内部控制 "导航播报信息回调函数" 的触发时机，避免出现下一句话打断前一句话的情况. 如果需要自定义"导航语音播报"功能，必须实现此代理
 * @param compositeManager 导航组件类
 * @return 返回当前是否正在进行导航信息播报,如一直返回YES，"导航播报信息回调函数"就一直不会触发，如一直返回NO，就会出现语句打断情况，所以请根据实际情况返回。
 */
- (BOOL)compositeManagerIsNaviSoundPlaying:(AMapNaviCompositeManager *_Nonnull)compositeManager;

/**
 * @brief 导航播报信息回调函数,此回调函数需要和compositeManagerIsNaviSoundPlaying:配合使用. 如果需要自定义"导航语音播报"功能，必须实现此代理
 * @param compositeManager 导航组件类
 * @param soundString 播报文字
 * @param soundStringType 播报类型,参考 AMapNaviSoundType. 注意：since 6.0.0 AMapNaviSoundType 只返回 AMapNaviSoundTypeDefault
 */
- (void)compositeManager:(AMapNaviCompositeManager *_Nonnull)compositeManager playNaviSoundString:(NSString *_Nullable)soundString soundStringType:(AMapNaviSoundType)soundStringType;

/**
 * @brief 停止导航语音播报的回调函数，当导航SDK需要停止外部语音播报时，会调用此方法. 如果需要自定义"导航语音播报"功能，必须实现此代理
 * @param compositeManager 导航组件类
 */
- (void)compositeManagerStopPlayNaviSound:(AMapNaviCompositeManager *_Nonnull)compositeManager;

/**
 * @brief 当前位置更新回调(无论是否在导航中，只要当前位置有更新就会回调)
 * @param compositeManager 导航组件类
 * @param naviLocation 当前位置信息,参考 AMapNaviLocation 类
 */
- (void)compositeManager:(AMapNaviCompositeManager *_Nonnull)compositeManager updateNaviLocation:(AMapNaviLocation *_Nullable)naviLocation;

/**
 * @brief 驾车导航页面白天夜间模式切换回调 since 6.7.0
 * @param compositeManager 导航组件类
 * @param showStandardNightType 是否为夜间模式,YES 表示夜间模式，NO 表示白天模式
 */
- (void)compositeManager:(AMapNaviCompositeManager *_Nonnull)compositeManager didChangeDayNightType:(BOOL)showStandardNightType;

/**
 * @brief 导航到达目的地后的回调函数
 * @param compositeManager 导航组件类
 * @param naviMode 导航类型，参考 AMapNaviMode .
 */
- (void)compositeManager:(AMapNaviCompositeManager *_Nonnull)compositeManager didArrivedDestination:(AMapNaviMode)naviMode;

/**
 * @brief 导航组件页面回退或者退出导航组件时会调用此函数 since 5.5.0
 * @param compositeManager 导航组件类
 * @param backwardActionType 导航组件页面回退的动作类型，参考 AMapNaviCompositeVCBackwardActionType .
 */
- (void)compositeManager:(AMapNaviCompositeManager *_Nonnull)compositeManager didBackwardAction:(AMapNaviCompositeVCBackwardActionType)backwardActionType;

/**
 * @brief 每次进入导航组件时和驾车路径规划策略改变均会调用此方法 since 6.1.0
 * @param compositeManager 导航组件类
 * @param driveStrategy 驾车路径规划策略，参考 AMapNaviDrivingStrategy .
 */
- (void)compositeManager:(AMapNaviCompositeManager *_Nonnull)compositeManager onDriveStrategyChanged:(AMapNaviDrivingStrategy)driveStrategy;

/**
 * @brief 导航到达某个途经点的回调函数 since 6.1.0
 * @param compositeManager 导航组件类
 * @param wayPointIndex 到达途径点的编号，标号从0开始. 注意：如果导航过程进行了路径重算(包含偏航、手动刷新等)，wayPointIndex会重新从0开始计数
 */
- (void)compositeManager:(AMapNaviCompositeManager *_Nonnull)compositeManager onArrivedWayPoint:(int)wayPointIndex;

@end


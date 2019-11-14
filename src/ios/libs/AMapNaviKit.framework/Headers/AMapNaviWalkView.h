//
//  AMapNaviWalkView.h
//  AMapNaviKit
//
//  Created by AutoNavi on 15/12/28.
//  Copyright © 2016年 Amap. All rights reserved.
//

#import "AMapNaviHeaderHandler.h"
#import "AMapNaviCommonObj.h"
#import "AMapNaviWalkDataRepresentable.h"

NS_ASSUME_NONNULL_BEGIN

///步行导航界面显示模式
typedef NS_ENUM(NSInteger, AMapNaviWalkViewShowMode)
{
    AMapNaviWalkViewShowModeCarPositionLocked = 1,  ///< 锁车状态
    AMapNaviWalkViewShowModeOverview = 2,           ///< 全览状态
    AMapNaviWalkViewShowModeNormal = 3,             ///< 普通状态
};

@protocol AMapNaviWalkViewDelegate;

///步行导航界面.该类实现 AMapNaviWalkDataRepresentable 协议,可通过 AMapNaviWalkManager 的addDataRepresentative:方法进行注册展示步行导航过程.
@interface AMapNaviWalkView : UIView<AMapNaviWalkDataRepresentable>

#pragma mark - Delegate

///实现了 AMapNaviWalkViewDelegate 协议的类指针
@property (nonatomic, weak) id<AMapNaviWalkViewDelegate> delegate;

#pragma mark - Options

///是否采用横屏布局,默认NO
@property (nonatomic, assign) BOOL isLandscape;

///导航界面跟随模式,默认AMapNaviViewTrackingModeMapNorth
@property (nonatomic, assign) AMapNaviViewTrackingMode trackingMode;

///导航界面显示模式,默认AMapNaviDriveViewShowModeCarPositionLocked
@property (nonatomic, assign) AMapNaviWalkViewShowMode showMode;

///是否显示界面元素,默认YES
@property (nonatomic, assign) BOOL showUIElements;

///是否黑夜模式,默认NO. 对应的地图样式为:白天模式MAMapTypeNavi,黑夜模式MAMapTypeStandardNight.
@property (nonatomic, assign) BOOL showStandardNightType;

///是否显示全览按钮,默认YES
@property (nonatomic, assign) BOOL showBrowseRouteButton;

///是否显示更多按钮,默认YES
@property (nonatomic, assign) BOOL showMoreButton;

///是否显示转向箭头,默认YES
@property (nonatomic, assign) BOOL showTurnArrow;

///是否显示传感器方向信息,默认NO.设置为YES后,自车图标方向将显示为设备方向
@property (nonatomic, assign) BOOL showSensorHeading;

#pragma mark - MapView

///是否显示指南针,默认NO
@property (nonatomic, assign) BOOL showCompass;

///锁车状态下地图cameraDegree, 默认30.0, 范围[0,60]
@property (nonatomic, assign) CGFloat cameraDegree;

///当前地图是否显示比例尺，默认NO
@property (nonatomic, assign) BOOL showScale;

///当前地图比例尺的原点位置，默认(10,10)
@property (nonatomic, assign) CGPoint scaleOrigin;

///当前地图是否开启自定义样式, 默认NO. 设置为YES，将忽略showStandardNightType的设置，并将mapType切换为MAMapTypeStandard. 设置为NO，将根据showStandardNightType恢复mapType. since 5.1.0
@property (nonatomic, assign) BOOL customMapStyleEnabled;

/**
 * @brief 自定义当前地图样式, 目前仅支持自定义标准类型. 已废弃, 请使用 setCustomMapStyleOptions: since 6.6.0
 * @param customJson 自定义的JSON格式数据.
 */
- (void)setCustomMapStyle:(NSData *)customJson __attribute((deprecated("已废弃, 请使用 setCustomMapStyleOptions: since 6.6.0")));

/**
 * @brief 根据web导出数据设置地图样式, 目前仅支持自定义标准类型. 默认不生效，调用customMapStyleEnabled=YES使生效. since 6.2.0
 * @param data 高德web端工具导出的地图样式数据.
 */
- (void)setCustomMapStyleWithWebData:(NSData*)data __attribute((deprecated("已废弃, 请使用 setCustomMapStyleOptions: since 6.6.0")));

/**
 * @brief 自定义地图样式设置,可以支持分级样式配置，如控制不同级别显示不同的颜色(自6.6.0开始使用新版样式，旧版样式无法在新版接口setCustomMapStyleOptions:(MAMapCustomStyleOptions *)styleOptions中使用，请到官网(lbs.amap.com)更新新版样式文件)
 * @param StyleOptions 自定义样式options. since 6.6.0
 */
- (void)setCustomMapStyleOptions:(MAMapCustomStyleOptions *)styleOptions;

#pragma mark - Polyline Texture

///路线polyline的宽度,设置0恢复默认宽度
@property (nonatomic, assign) CGFloat lineWidth;

///标准路线Polyline的纹理图片,设置nil恢复默认纹理.纹理图片需满足：长宽相等，且宽度值为2的次幂
@property (nonatomic, copy, nullable) UIImage *normalTexture;

#pragma mark - Image

/**
 * @brief 设置路径起点图标
 * @param startPointImage 起点图标,设置nil为默认图标
 */
- (void)setStartPointImage:(nullable UIImage *)startPointImage;

/**
 * @brief 设置路径终点图标
 * @param endPointImage 终点图标,设置nil为默认图标
 */
- (void)setEndPointImage:(nullable UIImage *)endPointImage;

/**
 * @brief 设置自车图标
 * @param carImage 自车图标,设置nil为默认图标
 */
- (void)setCarImage:(nullable UIImage *)carImage;

/**
 * @brief 设置自车罗盘图标
 * @param carCompassImage 自车罗盘图标,设置nil为默认图标
 */
- (void)setCarCompassImage:(nullable UIImage *)carCompassImage;

@end

@protocol AMapNaviWalkViewDelegate <NSObject>
@optional

/**
 * @brief 导航界面关闭按钮点击时的回调函数
 * @param walkView 步行导航界面
 */
- (void)walkViewCloseButtonClicked:(AMapNaviWalkView *)walkView;

/**
 * @brief 导航界面更多按钮点击时的回调函数
 * @param walkView 步行导航界面
 */
- (void)walkViewMoreButtonClicked:(AMapNaviWalkView *)walkView;

/**
 * @brief 导航界面转向指示View点击时的回调函数
 * @param walkView 步行导航界面
 */
- (void)walkViewTrunIndicatorViewTapped:(AMapNaviWalkView *)walkView;

/**
 * @brief 导航界面显示模式改变后的回调函数
 * @param walkView 步行导航界面
 * @param showMode 显示模式
 */
- (void)walkView:(AMapNaviWalkView *)walkView didChangeShowMode:(AMapNaviWalkViewShowMode)showMode;

@end

NS_ASSUME_NONNULL_END

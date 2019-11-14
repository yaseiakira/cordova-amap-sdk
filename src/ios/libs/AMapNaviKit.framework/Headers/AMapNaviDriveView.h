//
//  AMapNaviDriveView.h
//  AMapNaviKit
//
//  Created by AutoNavi on 15/12/28.
//  Copyright © 2016年 Amap. All rights reserved.
//

#import "AMapNaviHeaderHandler.h"
#import "AMapNaviCommonObj.h"
#import "AMapNaviDriveDataRepresentable.h"
#import "AMapNaviCompositeAnnotation.h"

NS_ASSUME_NONNULL_BEGIN

@protocol AMapNaviDriveViewDelegate;

///驾车导航界面.该类实现AMapNaviDriveDataRepresentable协议,可通过 AMapNaviDriveManager 的addDataRepresentative:方法进行注册展示驾车导航过程.
@interface AMapNaviDriveView : UIView <AMapNaviDriveDataRepresentable>

#pragma mark - Delegate

///实现了 AMapNaviDriveViewDelegate 协议的类指针
@property (nonatomic, weak) id<AMapNaviDriveViewDelegate> delegate;

#pragma mark - Mode Options

///导航界面跟随模式,默认AMapNaviViewTrackingModeMapNorth
@property (nonatomic, assign) AMapNaviViewTrackingMode trackingMode;

///导航界面显示模式,默认AMapNaviDriveViewShowModeCarPositionLocked
@property (nonatomic, assign) AMapNaviDriveViewShowMode showMode;

#pragma mark - Map Options

///当前地图Logo位置, 必须在 AMapNaviDriveView.bounds 之内，否则会被忽略. since 6.2.0
@property (nonatomic, assign) CGPoint logoCenter;

///当前地图Logo的宽高. since 6.2.0
@property (nonatomic, assign, readonly) CGSize logoSize;

///当前地图是否显示比例尺，默认NO
@property (nonatomic, assign) BOOL showScale;

///当前地图比例尺的原点位置
@property (nonatomic, assign) CGPoint scaleOrigin;

///是否显示指南针,默认NO
@property (nonatomic, assign) BOOL showCompass;

///指南针原点位置. since 6.3.0
@property (nonatomic, assign) CGPoint compassOrigin;

///指南针的宽高. since 6.3.0
@property (nonatomic, assign, readonly) CGSize compassSize;

///地图是否显示交通路况,默认YES. 注意：此值的设置不会影响“实时交通按钮”的状态. since 6.1.0
@property (nonatomic, assign) BOOL mapShowTraffic;

///当前地图的zoomLevel，修改zoomLevel会进入非锁车状态
@property (nonatomic, assign) CGFloat mapZoomLevel;

///锁车模式下是否为了预见下一导航动作自动缩放地图,默认为NO. since 6.2.0
@property (nonatomic, assign) BOOL autoZoomMapLevel;

///锁车状态下地图cameraDegree, 默认35.0, 范围[0,60]
@property (nonatomic, assign) CGFloat cameraDegree;

///地图的视图锚点. (0, 0)为左上角，(1, 1)为右下角. 可通过设置此值来改变自车图标的默认显示位置. 注意:只有showUIElements为NO时，设置此值才有效 since 6.2.0
@property (nonatomic, assign) CGPoint screenAnchor;

///当前地图最大帧数，有效的帧数为：60、30、20、10等能被60整除的数，默认为30. since 6.5.0
@property (nonatomic, assign) NSUInteger maxRenderFrame;

///导航界面日夜模式类型, 默认为 AMapNaviViewMapModeTypeDay(白天模式) since 6.7.0
@property (nonatomic, assign) AMapNaviViewMapModeType mapViewModeType;

/**
 * @brief 自定义地图样式设置,可以支持分级样式配置，如控制不同级别显示不同的颜色(自6.6.0开始使用新版样式，旧版样式无法在新版接口setCustomMapStyleOptions:(MAMapCustomStyleOptions *)styleOptions中使用，请到官网(lbs.amap.com)更新新版样式文件)
 * @param StyleOptions 自定义样式options. since 6.6.0
 */
- (void)setCustomMapStyleOptions:(MAMapCustomStyleOptions *)styleOptions;

///地图是否自动切换白天黑夜模式,默认NO.
@property (nonatomic, assign) BOOL autoSwitchDayNightType __attribute((deprecated("已废弃, 请使用 mapViewModeType 替代 since 6.7.0")));

///是否黑夜模式,默认NO.
@property (nonatomic, assign) BOOL showStandardNightType __attribute((deprecated("已废弃, 请使用 mapViewModeType 替代 since 6.7.0")));

///当前地图是否开启自定义样式, 默认NO.
@property (nonatomic, assign) BOOL customMapStyleEnabled __attribute((deprecated("已废弃, 请使用 mapViewModeType 替代 since 6.7.0")));

/**
 * @brief 自定义当前地图样式, 目前仅支持自定义标准类型. 已废弃, 请使用 setCustomMapStyleOptions: since 6.6.0
 * @param customJson 自定义的JSON格式数据.
 */
- (void)setCustomMapStyle:(NSData *)customJson __attribute((deprecated("已废弃, 请使用 setCustomMapStyleOptions: 替代 since 6.6.0")));

/**
 * @brief 根据web导出数据设置地图样式, 目前仅支持自定义标准类型. 默认不生效，调用customMapStyleEnabled=YES使生效. since 5.1.0
 * @param data 高德web端工具导出的地图样式数据.
 */
- (void)setCustomMapStyleWithWebData:(NSData*)data __attribute((deprecated("已废弃, 请使用 setCustomMapStyleOptions: 替代 since 6.6.0")));

#pragma mark - Annoation Options and Overlay Options

///是否显示实时交通图层(地图路况+路线路况),默认YES
@property (nonatomic, assign) BOOL showTrafficLayer;

///路线上是否显示摄像头,默认YES
@property (nonatomic, assign) BOOL showCamera;

///路线上是否显示转向箭头,默认YES
@property (nonatomic, assign) BOOL showTurnArrow;

///路线上转弯箭头颜色. since 6.1.0
@property (nonatomic, strong) UIColor *turnArrowColor;

///路线上转弯箭头的宽度,设置0恢复默认宽度. since 6.2.0
@property (nonatomic, assign) CGFloat turnArrowWidth;

///路线上转弯箭头是否为3D箭头线,默认为YES. since 6.6.0
@property (nonatomic, assign) BOOL turnArrowIs3D;

///路线上转弯箭头设置3D箭头的侧边颜色（当turnArrowIs3D为YES时有效). since 6.6.0
@property (nonatomic, strong) UIColor *turnArrowSideColor;

///是否显示牵引线,默认YES. since 6.2.0
@property (nonatomic, assign) BOOL showVectorline;

///是否显示红绿灯图标,默认YES. since 6.2.0
@property (nonatomic, assign) BOOL showTrafficLights;

///是否显示小车图标,默认YES. since 6.2.0
@property (nonatomic, assign) BOOL showCar;

///是否显示路线,默认YES. 注意：路线上的相关信息，如摄像头(showCamera)、红绿灯(showTrafficLights)、转向箭头(showTurnArrow)、牵引线(showVectorline)、起终点等也会一同受控制，进行显示和隐藏. since 6.2.0
@property (nonatomic, assign) BOOL showRoute;

///走过的路线是否置灰,默认为NO. since 6.2.0
@property (nonatomic, assign) BOOL showGreyAfterPass;

///路线polyline的虚线部分宽度,设置0恢复默认宽度
@property (nonatomic, assign) CGFloat dashedLineWidth;

///路线虚线部分的颜色. since 6.2.0
@property (nonatomic, strong) UIColor *dashedLineColor;

///路线虚线部分走过后置灰的颜色 since 6.2.0
@property (nonatomic, strong) UIColor *dashedLineGreyColor;

///路线polyline的宽度,设置0恢复默认宽度
@property (nonatomic, assign) CGFloat lineWidth;

///路线纹理部分走过后置灰的纹理图片,设置nil恢复默认纹理. 纹理图片需满足：长宽相等，且宽度值为2的次幂. since 6.2.0
@property (nonatomic, copy, nullable) UIImage *greyTexture;

///带路况路线Polyline的纹理图片. 纹理图片需满足: 长宽相等,且宽度值为2的次幂. 例如:@{@(AMapNaviRouteStatusSlow): [UIImage Slow路况下的Image],@(AMapNaviRouteStatusSeriousJam): [UIImage SeriousJam路况下的Image]}. 设置空字典恢复默认纹理,例如: @{}
@property (nonatomic, copy) NSDictionary<NSNumber *, UIImage *> *statusTextures;

///标准路线Polyline的纹理图片,设置nil恢复默认纹理.纹理图片需满足：长宽相等，且宽度值为2的次幂
@property (nonatomic, copy, nullable) UIImage *normalTexture;

///自定义导航界面自车图标的弹出框view, 设置为nil取消弹框. 注意:弹框功能同MAAnnotationView的customCalloutView, 弹框不会触发 mapView:didAnnotationViewCalloutTapped: 方法. since 5.1.0
@property (nonatomic, strong, nullable) MACustomCalloutView *customCalloutView;

///多路线导航模式下是否显示备选路线, 默认YES. since 6.7.0
@property (nonatomic, assign) BOOL showBackupRoute;

/**
 * @brief 设置摄像头图标
 * @param cameraImage 摄像头图标,设置nil为默认图标
 */
- (void)setCameraImage:(nullable UIImage *)cameraImage;

/**
 * @brief 设置路径起点图标
 * @param startPointImage 起点图标,设置nil为默认图标
 */
- (void)setStartPointImage:(nullable UIImage *)startPointImage;

/**
 * @brief 设置路径途经点图标
 * @param wayPointImage 途经点图标,设置nil为默认图标
 */
- (void)setWayPointImage:(nullable UIImage *)wayPointImage;

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

/**
 * @brief 在导航地图中添加自定义标注. since 6.2.0
 * @param annotation 具体参考 AMapNaviCompositeCustomAnnotation .
 */
- (void)addCustomAnnotation:(AMapNaviCompositeCustomAnnotation *)annotation;

/**
 * @brief 移除导航地图中的自定义标注. since 6.2.0
 * @param annotation 具体参考 AMapNaviCompositeCustomAnnotation .
 */
- (void)removeCustomAnnotation:(AMapNaviCompositeCustomAnnotation *)annotation;

/**
 * @brief 在导航地图中添加自定义overlay. since 6.7.0
 * @param overlay 具体参考 官方Demo中 DriveViewWithCustomOverlayViewController .
 */
- (void)addCustomOverlay:(id <AMapNaviCompositeOverlay>_Nonnull)overlay;

/**
 * @brief 移除导航地图中的自定义overlay. since 6.7.0
 * @param overlay 具体参考 AMapNaviCompositeOverlay .
 */
- (void)removeCustomOverlay:(id <AMapNaviCompositeOverlay>_Nonnull)overlay;

#pragma mark - UI Element Options

///是否显示界面元素,默认YES
@property (nonatomic, assign) BOOL showUIElements;

///是否显示路口放大图,默认YES
@property (nonatomic, assign) BOOL showCrossImage;

///是否显示实时交通按钮,默认YES
@property (nonatomic, assign) BOOL showTrafficButton;

///是否显示路况光柱,默认YES
@property (nonatomic, assign) BOOL showTrafficBar;

///是否显示全览按钮,默认YES
@property (nonatomic, assign) BOOL showBrowseRouteButton;

///是否显示更多按钮,默认YES
@property (nonatomic, assign) BOOL showMoreButton;

#pragma mark - Other

///目前是否为横屏状态. since 6.2.0 内部会自行监听 UIDeviceOrientationDidChange 进行横竖屏切换，无需再设置此值，但用户要自行保证 AMapNaviDriveView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight，让 AMapNaviDriveView 能够跟着父View一起变化. 可通过回调 -driveView:didChangeOrientation: 获取横竖屏切换时机
@property (nonatomic, assign, readonly) BOOL isLandscape;

///当显示模式为非锁车模式时，是否在6秒后自动设置为锁车模式，默认为NO. since 5.3.0
@property (nonatomic, assign) BOOL autoSwitchShowModeToCarPositionLocked;

/**
 * @brief 在全览状态下调用此函数能够让路线显示在可视区域内(排除EdgePadding后剩余的区域)，保证路线不被自定义界面元素遮挡. 比如showUIElements为NO时（自定义界面）横竖屏切换后，可以调用此函数. since 6.2.0
 */
- (void)updateRoutePolylineInTheVisualRangeWhenTheShowModeIsOverview;

@end

@protocol AMapNaviDriveViewDelegate <NSObject>
@optional

/**
 * @brief 导航界面关闭按钮点击时的回调函数
 * @param driveView 驾车导航界面
 */
- (void)driveViewCloseButtonClicked:(AMapNaviDriveView *)driveView;

/**
 * @brief 导航界面更多按钮点击时的回调函数
 * @param driveView 驾车导航界面
 */
- (void)driveViewMoreButtonClicked:(AMapNaviDriveView *)driveView;

/**
 * @brief 导航界面转向指示View点击时的回调函数
 * @param driveView 驾车导航界面
 */
- (void)driveViewTrunIndicatorViewTapped:(AMapNaviDriveView *)driveView;

/**
 * @brief 导航界面显示模式改变后的回调函数
 * @param driveView 驾车导航界面
 * @param showMode 显示模式
 */
- (void)driveView:(AMapNaviDriveView *)driveView didChangeShowMode:(AMapNaviDriveViewShowMode)showMode;

/**
 * @brief 驾车导航界面跟随模式改变后的回调函数. since 6.2.0
 * @param driveView 驾车导航界面
 * @param trackMode 跟随模式
 */
- (void)driveView:(AMapNaviDriveView *)driveView didChangeTrackingMode:(AMapNaviViewTrackingMode)trackMode;

/**
 * @brief 驾车导航界面横竖屏切换后的回调函数. since 6.2.0
 * @param driveView 驾车导航界面
 * @param isLandscape 是否时横屏
 */
- (void)driveView:(AMapNaviDriveView *)driveView didChangeOrientation:(BOOL)isLandscape;

/**
 * @brief 驾车导航界面白天黑夜模式切换后的回调函数. since 6.2.0
 * @param driveView 驾车导航界面
 * @param showStandardNightType 是否为黑夜模式
 */
- (void)driveView:(AMapNaviDriveView *)driveView didChangeDayNightType:(BOOL)showStandardNightType;

/**
 * @brief 在showUIElements为NO时，驾车导航界面需要实时的取得可视区域，比如切换成全览时、横竖屏切换时、动态计算地图的缩放级别时. 注意：此回调只在showUIElements为NO时，才会调用且比较频繁，在获取EdgePadding时请勿进行大量的计算. since 6.2.0
 * @param driveView 驾车导航界面
 * @return 如(100, 50, 80, 60)表示的是：driveView.bounds 上边留出100px，左边留出50px，底部留出80px，右边留出60px后的区域为可视区域，一般EdgePadding的值由用户的界面布局决定.
 */
- (UIEdgeInsets)driveViewEdgePadding:(AMapNaviDriveView *)driveView;

/**
 * @brief 获取导航界面上路线显示样式的回调函数. 已废弃，请使用 lineWidth、statusTextures、greyTexture、dashedLineColor 等相关属性替代, since 6.2.0
 * @param driveView 驾车导航界面
 * @param naviRoute 当前界面的路线信息
 * @return AMapNaviRoutePolylineOption 路线显示样式
 */
- (id)driveView:(AMapNaviDriveView *)driveView needUpdatePolylineOptionForRoute:(AMapNaviRoute *)naviRoute __attribute__((deprecated("已废弃，请使用 lineWidth、statusTextures、greyTexture、dashedLineColor 等相关属性替代, since 6.2.0")));

@end

NS_ASSUME_NONNULL_END

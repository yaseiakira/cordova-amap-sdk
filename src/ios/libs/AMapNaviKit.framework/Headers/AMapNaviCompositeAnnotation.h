//
//  AMapNaviCompositeAnnotation.h
//  AMapNaviKit
//
//  Created by eidan on 2018/3/26.
//  Copyright © 2018年 Amap. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AMapNaviHeaderHandler.h"

///导航页添加自定overlay，需要遵守此协议 since 6.7.0
@protocol AMapNaviCompositeOverlay <MAOverlay>

@required

///导航SDK内部会将该值传给地图进行绘制, 开发者自行实现的对象在遵守协议后, 需对该值赋值, 不能为空.
@property (nonatomic, strong, readonly) MAOverlayRenderer *overlayRender;

@optional

///添加的overlay所在层级, 参考MAOverlayLevel, 默认 MAOverlayLevelAboveRoads.
@property (nonatomic, assign, readonly) MAOverlayLevel overlayLevel;

@end

///导航界面自定义标注 since 5.5.0
@interface AMapNaviCompositeCustomAnnotation : NSObject <MAAnnotation>

///标注的中心坐标
@property (nonatomic, assign) CLLocationCoordinate2D coordinate;

///z值，大值在上，默认为0. 类似CALayer的zPosition
@property (nonatomic, assign) NSInteger zIndex;

/**
 * @brief 初始化导航界面自定义标注. since 5.5.0
 * @param coordinate 标注的中心坐标.
 * @param view 会被显示在驾车导航界面的地图上. 如需调整view的相对位置，请修改view.frame.origin
 * @return id 自定义标注对象
 */
- (id)initWithCoordinate:(CLLocationCoordinate2D)coordinate view:(UIView *)view;



@end

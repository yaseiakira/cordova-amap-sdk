//
//  AMapNaviDriveManager.h
//  AMapNaviKit
//
//  Created by 刘博 on 16/1/12.
//  Copyright © 2016年 Amap. All rights reserved.
//

#import "AMapNaviBaseManager.h"
#import "AMapNaviDriveDataRepresentable.h"

NS_ASSUME_NONNULL_BEGIN

@protocol AMapNaviDriveManagerDelegate;

#pragma mark - AMapNaviDriveManager

///驾车导航管理类
@interface AMapNaviDriveManager : AMapNaviBaseManager

#pragma mark - Singleton

/**
 * @brief AMapNaviDriveManager单例. since 5.4.0
 * @return AMapNaviDriveManager实例
 */
+ (AMapNaviDriveManager *)sharedInstance;

/**
 * @brief 销毁AMapNaviDriveManager单例. AMapNaviDriveManager内存开销比较大，建议不使用时可销毁. since 5.4.0
 * @return 是否销毁成功. 如果返回NO，请检查单例是否被强引用
 */
+ (BOOL)destroyInstance;

/**
 * @brief 请使用单例替代. since 5.4.0 init已被禁止使用，请使用单例 [AMapNaviDriveManager sharedInstance] 替代，且在调用类的 dealloc 函数或其他适当时机(如导航ViewController被pop时)，调用 [AMapNaviDriveManager destroyInstance] 来销毁单例（需要注意如未销毁成功，请检查单例是否被强引用)
 */
- (instancetype)init __attribute__((unavailable("since 5.4.0 init 已被禁止使用，请使用单例 [AMapNaviDriveManager sharedInstance] 替代，且在调用类的 dealloc 函数里或其他适当时机(如导航ViewController被pop时)，调用 [AMapNaviDriveManager destroyInstance] 来销毁单例（需要注意如未销毁成功，请检查单例是否被强引用)")));

#pragma mark - Delegate

///实现了 AMapNaviDriveManagerDelegate 协议的类指针
@property (nonatomic, weak) id<AMapNaviDriveManagerDelegate> delegate;

#pragma mark - Event Listener

/**
 * @brief 增加用于接收导航回调事件的Listener, 效果等同于delegate. 注意:该方法不会增加实例对象的引用计数(Weak Reference). since 5.4.0
 * @param aListener 实现了 AMapNaviDriveManagerDelegate 协议的实例
 */
- (void)addEventListener:(id<AMapNaviDriveManagerDelegate>)aListener;

/**
 * @brief 移除用于接收导航回调事件的Listener. since 5.4.0
 * @param aListener 实现了 AMapNaviDriveManagerDelegate 协议的实例
 */
- (void)removeEventListener:(id<AMapNaviDriveManagerDelegate>)aListener;

#pragma mark - Data Representative

/**
 * @brief 增加用于展示导航数据的DataRepresentative.注意:该方法不会增加实例对象的引用计数(Weak Reference)
 * @param aRepresentative 实现了 AMapNaviDriveDataRepresentable 协议的实例
 */
- (void)addDataRepresentative:(id<AMapNaviDriveDataRepresentable>)aRepresentative;

/**
 * @brief 移除用于展示导航数据的DataRepresentative
 * @param aRepresentative 实现了 AMapNaviDriveDataRepresentable 协议的实例
 */
- (void)removeDataRepresentative:(id<AMapNaviDriveDataRepresentable>)aRepresentative;

#pragma mark - Navi Route

///当前导航路径的ID
@property (nonatomic, readonly) NSInteger naviRouteID;

///当前导航路径的信息,参考 AMapNaviRoute 类.
@property (nonatomic, readonly, nullable) AMapNaviRoute *naviRoute;

///多路径规划时的所有路径ID,路径ID为 NSInteger 类型.
@property (nonatomic, readonly, nullable) NSArray<NSNumber *> *naviRouteIDs;

///多路径规划时的所有路径信息,参考 AMapNaviRoute 类.
@property (nonatomic, readonly, nullable) NSDictionary<NSNumber *, AMapNaviRoute *> *naviRoutes;

/**
 * @brief 多路径规划时选择路径.注意:该方法仅限于在开始导航前使用,开始导航后该方法无效.
 * @param routeID 路径ID
 * @return 是否选择路径成功
 */
- (BOOL)selectNaviRouteWithRouteID:(NSInteger)routeID;

/**
 * @brief 切换平行道路, 包括主辅路切换、高架上下切换. 该方法需要配合 AMapNaviDriveDataRepresentable 的 driveManager:updateParallelRoadStatus: 回调使用. since 5.3.0
 * @param parallelRoad 平行路切换信息，参考 AMapNaviParallelRoadInfo.
 */
- (void)switchParallelRoad:(AMapNaviParallelRoadInfo *)parallelRoadInfo;

/**
 * @brief 设置多路线导航模式(GPS导航中拥有若干条备选路线供用户选择), 或单路线导航模式(默认模式). 注意: 1、设置的导航模式会在下一次主动路径规划时生效, 导航中设置无效, 建议在 AMapNaviDriveManager 单例初始化时就进行设置. 2、多路线导航模式还需同时满足以下4个条件才能够生效：a.路径规划时 AMapNaviDrivingStrategy 需选用多路径策略; b.起终点的直线距离需<=80KM; c.不能有途径点; d.车辆不能是货车类型. since 6.3.0
 * @param multipleRouteNaviMode YES:多路线导航模式, NO:单路线导航模式(默认)
 * @return 是否设置成功，如开始导航后调用就会返回NO
 */
- (BOOL)setMultipleRouteNaviMode:(BOOL)multipleRouteNaviMode;

#pragma mark - Options

///是否播报摄像头信息,默认YES.
@property (nonatomic, assign) BOOL updateCameraInfo;

///是否播报交通信息,默认YES(需要联网).
@property (nonatomic, assign) BOOL updateTrafficInfo;

///偏航时是否重新计算路径,默认YES(需要联网). 已废弃, 默认进行重算，since 5.0.0
@property (nonatomic, assign) BOOL isRecalculateRouteForYaw __attribute__((deprecated("已废弃，默认进行重算，since 5.0.0")));

///前方拥堵时是否重新计算路径,默认YES(需要联网). 已废弃, 默认进行重算，since 5.0.0
@property (nonatomic, assign) BOOL isRecalculateRouteForTrafficJam __attribute__((deprecated("已废弃，默认进行重算，since 5.0.0")));

///巡航模式,默认为 AMapNaviDetectedModeNone. 注意：如果已经处在导航模式，要开启巡航模式时，需要先调用 stopNavi 来停止导航，再设置 detectedMode 才能生效
@property (nonatomic, assign) AMapNaviDetectedMode detectedMode;

#pragma mark - Calculate Route

// 以下算路方法需要高德坐标(GCJ02)

/**
 * @brief 不带起点的驾车路径规划
 * @param endPoints    终点坐标.终点列表的尾点为实际导航终点.
 * @param wayPoints    途经点坐标,最多支持16个途经点. 超过16个会取前16个.
 * @param strategy     路径的计算策略，建议使用 AMapNaviDrivingStrategyMultipleDefault，与[高德地图]默认策略一致 (避让拥堵+速度优先+避免收费)
 * @return 规划路径所需条件和参数校验是否成功，不代表算路成功与否
 */
- (BOOL)calculateDriveRouteWithEndPoints:(NSArray<AMapNaviPoint *> *)endPoints
                               wayPoints:(nullable NSArray<AMapNaviPoint *> *)wayPoints
                         drivingStrategy:(AMapNaviDrivingStrategy)strategy;

/**
 * @brief 带起点的驾车路径规划
 * @param startPoints  起点坐标.起点列表的尾点为实际导航起点,其他坐标点为辅助信息,带有方向性,可有效避免算路到马路的另一侧.
 * @param endPoints    终点坐标.终点列表的尾点为实际导航终点,其他坐标点为辅助信息,带有方向性,可有效避免算路到马路的另一侧.
 * @param wayPoints    途经点坐标,最多支持16个途经点. 超过16个会取前16个
 * @param strategy     路径的计算策略，建议使用 AMapNaviDrivingStrategyMultipleDefault，与[高德地图]默认策略一致 (避让拥堵+速度优先+避免收费)
 * @return 规划路径所需条件和参数校验是否成功，不代表算路成功与否
 */
- (BOOL)calculateDriveRouteWithStartPoints:(NSArray<AMapNaviPoint *> *)startPoints
                                 endPoints:(NSArray<AMapNaviPoint *> *)endPoints
                                 wayPoints:(nullable NSArray<AMapNaviPoint *> *)wayPoints
                           drivingStrategy:(AMapNaviDrivingStrategy)strategy;

/**
 * @brief 根据高德POIId进行驾车路径规划,为了保证路径规划的准确性,请尽量使用此方法. since 6.1.0
 * @param startPOIId  起点POIId,如果以“我的位置”作为起点,请传nil
 * @param endPOIId    终点POIId,必填
 * @param wayPOIIds   途经点POIId,最多支持16个途经点. 超过16个会取前16个
 * @param strategy    路径的计算策略，建议使用 AMapNaviDrivingStrategyMultipleDefault，与[高德地图]默认策略一致 (避让拥堵+速度优先+避免收费)
 * @return 规划路径所需条件和参数校验是否成功，不代表算路成功与否
 */
- (BOOL)calculateDriveRouteWithStartPointPOIId:(nullable NSString *)startPOIId
                                 endPointPOIId:(nonnull NSString *)endPOIId
                                wayPointsPOIId:(nullable NSArray<NSString *> *)wayPOIIds
                               drivingStrategy:(AMapNaviDrivingStrategy)strategy;
/**
 * @brief 根据高德POIInfo进行驾车路径规划,为了保证路径规划的准确性,请尽量使用此方法. since 6.4.0
 * @param startPOIInfo  起点POIInfo,参考 AMapNaviPOIInfo. 如果以“我的位置”作为起点,请传nil. 如果startPOIInfo不为nil,那么POIID合法,优先使用ID参与算路,否则使用坐标点.
 * @param endPOIInfo    终点POIInfo,参考 AMapNaviPOIInfo. 如果POIID合法,优先使用ID参与算路,否则使用坐标点. 注意:POIID和坐标点不能同时为空
 * @param wayPOIInfos   途经点POIInfo,最多支持16个途经点,超过16个会取前16个. 如果POIID合法,优先使用ID参与算路,否则使用坐标点. 注意:POIID和坐标点不能同时为空
 * @param strategy      路径的计算策略，建议使用 AMapNaviDrivingStrategyMultipleDefault，与[高德地图]默认策略一致 (避让拥堵+速度优先+避免收费)
 * @return 规划路径所需条件和参数校验是否成功，不代表算路成功与否
 */
- (BOOL)calculateDriveRouteWithStartPOIInfo:(nullable AMapNaviPOIInfo *)startPOIInfo
                                 endPOIInfo:(nonnull AMapNaviPOIInfo *)endPOIInfo
                                wayPOIInfos:(nullable NSArray<AMapNaviPOIInfo *> *)wayPOIInfos
                            drivingStrategy:(AMapNaviDrivingStrategy)strategy;

/**
 * @brief 导航过程中重新规划路径(起点为当前位置,途经点和终点位置不变)
 * @param strategy 路径的计算策略，建议使用 AMapNaviDrivingStrategyMultipleDefault，与[高德地图]默认策略一致 (避让拥堵+速度优先+避免收费)
 * @return 重新规划路径所需条件和参数校验是否成功, 不代表算路成功与否, 如非导航状态下调用此方法会返回NO.
 */
- (BOOL)recalculateDriveRouteWithDrivingStrategy:(AMapNaviDrivingStrategy)strategy;

#pragma mark - Manual

/**
 * @brief 设置车牌信息. 已废弃，请使用 setVehicleInfo: 替代，since 6.0.0
 * @param province 车牌省份缩写，例如："京"
 * @param number 除省份及标点之外，车牌的字母和数字，例如："NH1N11"
 */
- (void)setVehicleProvince:(NSString *)province number:(NSString *)number __attribute__((deprecated("已废弃，请使用 setVehicleInfo: 替代，since 6.0.0")));

/**
 * @brief 设置车辆信息. since 6.0.0
 * @param vehicleInfo 车辆信息，参考 AMapNaviVehicleInfo. 如果要清空已设置的车辆信息，传入nil即可.
 * @return 是否设置成功
 */
- (BOOL)setVehicleInfo:(nullable AMapNaviVehicleInfo *)vehicleInfo;

/**
 * @brief 设置播报模式,默认新手详细播报( AMapNaviBroadcastModeDetailed )
 * @return 是否成功
 */
- (BOOL)setBroadcastMode:(AMapNaviBroadcastMode)mode;

/**
 * @brief 设置网约车模式. since 6.4.0
 * @param type 参考 AMapNaviOnlineCarHailingType. 默认为 AMapNaviOnlineCarHailingTypeNone (非网约车模式, 即正常模式)
 * @return 是否设置成功
 */
- (BOOL)setOnlineCarHailingType:(AMapNaviOnlineCarHailingType)type;

/**
 * @brief 手动刷新路况信息,调用后会刷新路况光柱. 已废弃，since 5.0.0.
 */
- (void)refreshTrafficStatusesManual __attribute__((deprecated("已废弃，since 5.0.0")));

/**
 * @brief 设置TTS语音播报每播报一个字需要的时间. 已废弃，使用 driveManagerIsNaviSoundPlaying: 替代，since 5.0.0
 * @param time 每个字的播放时间(范围:[250,500]; 单位:毫秒)
 */
- (void)setTimeForOneWord:(int)time __attribute__((deprecated("已废弃，使用 driveManagerIsNaviSoundPlaying: 替代，since 5.0.0")));

#pragma mark - Traffic Status

/**
 * @brief 获取某一范围内的路况光柱信息
 * @param startPosition 光柱范围在路径中的起始位置,取值范围[0, routeLength)
 * @param distance 光柱范围的距离,startPosition + distance 和的取值范围(0, routelength]
 * @return 该范围内路况信息数组,可用于绘制光柱,参考 AMapNaviTrafficStatus 类.
 */
- (nullable NSArray<AMapNaviTrafficStatus *> *)getTrafficStatusesWithStartPosition:(int)startPosition distance:(int)distance;

/**
 *  获取当前道路的路况光柱信息
 *
 *  @return 该范围内路况信息数组,可用于绘制光柱,参考 AMapNaviTrafficStatus 类.
 */
- (nullable NSArray<AMapNaviTrafficStatus *> *)getTrafficStatuses;

#pragma mark - Xcode Simulate Location

/**
 * @brief 设置Xcode模拟定位点是否参与导航. 注意：此方法仅供开发者调试使用. since 6.7.0
 * @param enableNavi 模拟的定位点是否参与导航, 默认为NO.
 */
- (void)setXcodeSimulateLocationEnable:(BOOL)enableNavi;

#pragma mark - Statistics Information

/**
 * @brief 获取导航统计信息. 已废弃，since 5.0.0
 * @return 导航统计信息, 参考 AMapNaviStatisticsInfo 类.
 */
- (nullable AMapNaviStatisticsInfo *)getNaviStatisticsInfo __attribute__((deprecated("已废弃，since 5.0.0")));

@end

#pragma mark - Escort

@interface AMapNaviDriveManager (Escort)

/**
 * 设置一路护航任务id.  注意：此方法必须在"开始算路"之前设置, 否则无效, since 6.7.0
 * @param missonID 一路护航的任务id
 * @return 是否设置成功
 */
- (BOOL)setEscortMissonID:(NSNumber *)missonID;

@end

#pragma mark - Private

@interface AMapNaviDriveManager (Private)

///内部接口请勿调用
- (BOOL)pushDriveRouteWithData:(nonnull NSData *)routeData
                  startPOIInfo:(nonnull AMapNaviPOIInfo *)startPOIInfo
                    endPOIInfo:(nonnull AMapNaviPOIInfo *)endPOIInfo
                   wayPOIInfos:(nullable NSArray<AMapNaviPOIInfo *> *)wayPOIInfos
               drivingStrategy:(AMapNaviDrivingStrategy)strategy;

///内部接口请勿调用
- (NSString *)routeSDKVersion;

///内部接口请勿调用
- (NSString *)routeServerVersion;

///内部接口请勿调用
- (void)setRouteId:(nullable NSString *)routeId;

@end

#pragma mark - AMapNaviDriveManagerDelegate

@protocol AMapNaviDriveManagerDelegate <NSObject>
@optional

/**
 * @brief 发生错误时,会调用代理的此方法
 * @param driveManager 驾车导航管理类
 * @param error 错误信息
 */
- (void)driveManager:(AMapNaviDriveManager *)driveManager error:(NSError *)error;

/**
 * @brief 驾车路径规划成功后的回调函数，请尽量使用 -driveManager:onCalculateRouteSuccessWithType: 替代此方法
 * @param driveManager 驾车导航管理类
 */
- (void)driveManagerOnCalculateRouteSuccess:(AMapNaviDriveManager *)driveManager;

/**
 * @brief 驾车路径规划成功后的回调函数 since 6.1.0
 * @param driveManager 驾车导航管理类
 * @param type 路径规划类型,参考 AMapNaviRoutePlanType
 */
- (void)driveManager:(AMapNaviDriveManager *)driveManager onCalculateRouteSuccessWithType:(AMapNaviRoutePlanType)type;

/**
 * @brief 驾车路径规划失败后的回调函数. 从5.3.0版本起,算路失败后导航SDK只对外通知算路失败,SDK内部不再执行停止导航的相关逻辑.因此,当算路失败后,不会收到 driveManager:updateNaviMode: 回调; AMapDriveManager.naviMode 不会切换到 AMapNaviModeNone 状态, 而是会保持在 AMapNaviModeGPS or AMapNaviModeEmulator 状态.
 * @param driveManager 驾车导航管理类
 * @param error 错误信息,error.code参照 AMapNaviCalcRouteState
 */
- (void)driveManager:(AMapNaviDriveManager *)driveManager onCalculateRouteFailure:(NSError *)error;

/**
 * @brief 驾车路径规划失败后的回调函数. since 6.1.0
 * @param driveManager 驾车导航管理类
 * @param error 错误信息,error.code参照 AMapNaviCalcRouteState
 * @param type 路径规划类型,参考 AMapNaviRoutePlanType
 */
- (void)driveManager:(AMapNaviDriveManager *)driveManager onCalculateRouteFailure:(NSError *)error routePlanType:(AMapNaviRoutePlanType)type;

/**
 * @brief 启动导航后回调函数
 * @param naviMode 导航类型，参考 AMapNaviMode .
 * @param driveManager 驾车导航管理类
 */
- (void)driveManager:(AMapNaviDriveManager *)driveManager didStartNavi:(AMapNaviMode)naviMode;

/**
 * @brief 出现偏航需要重新计算路径时的回调函数.偏航后将自动重新路径规划,该方法将在自动重新路径规划前通知您进行额外的处理.
 * @param driveManager 驾车导航管理类
 */
- (void)driveManagerNeedRecalculateRouteForYaw:(AMapNaviDriveManager *)driveManager;

/**
 * @brief 前方遇到拥堵需要重新计算路径时的回调函数.拥堵后将自动重新路径规划,该方法将在自动重新路径规划前通知您进行额外的处理.
 * @param driveManager 驾车导航管理类
 */
- (void)driveManagerNeedRecalculateRouteForTrafficJam:(AMapNaviDriveManager *)driveManager;

/**
 * @brief 导航到达某个途经点的回调函数
 * @param driveManager 驾车导航管理类
 * @param wayPointIndex 到达的途径点的索引值，表示的是 AMapNaviRoute.wayPointsInfo 的索引.
 */
- (void)driveManager:(AMapNaviDriveManager *)driveManager onArrivedWayPoint:(int)wayPointIndex;

/**
 * @brief 开发者请根据实际情况返回是否正在播报语音，如果正在播报语音，请返回YES, 如果没有在播报语音，请返回NO
 * @param driveManager 驾车导航管理类
 * @return 如一直返回YES，SDK内部会认为外界一直在播报，"-driveManager:playNaviSoundString:soundStringType" 就会一直不触发，导致无文字吐出; 如一直返回NO，文字吐出的频率可能会过快，会出现语句打断情况，所以请根据实际情况返回。
 */
- (BOOL)driveManagerIsNaviSoundPlaying:(AMapNaviDriveManager *)driveManager;

/**
 * @brief 导航播报信息回调函数,此回调函数需要和driveManagerIsNaviSoundPlaying:配合使用
 * @param driveManager 驾车导航管理类
 * @param soundString 播报文字
 * @param soundStringType 播报类型,参考 AMapNaviSoundType. 注意：since 6.0.0 AMapNaviSoundType 只返回 AMapNaviSoundTypeDefault
 */
- (void)driveManager:(AMapNaviDriveManager *)driveManager playNaviSoundString:(NSString *)soundString soundStringType:(AMapNaviSoundType)soundStringType;

/**
 * @brief 模拟导航到达目的地后的回调函数
 * @param driveManager 驾车导航管理类
 */
- (void)driveManagerDidEndEmulatorNavi:(AMapNaviDriveManager *)driveManager;

/**
 * @brief GPS导航到达目的地后的回调函数
 * @param driveManager 驾车导航管理类
 */
- (void)driveManagerOnArrivedDestination:(AMapNaviDriveManager *)driveManager;

/**
 * @brief 导航(巡航)过程中播放提示音的回调函数. since 5.4.0
 * @param driveManager 驾车导航管理类
 * @param ringType 提示音类型,参考 AMapNaviRingType .
 */
- (void)driveManager:(AMapNaviDriveManager *)driveManager onNaviPlayRing:(AMapNaviRingType)ringType;

/**
 * @brief GPS信号强弱回调函数. since 5.5.0
 * @param driveManager 驾车导航管理类
 * @param gpsSignalStrength GPS信号强度类型,参考 AMapNaviGPSSignalStrength .
 */
- (void)driveManager:(AMapNaviDriveManager *)driveManager updateGPSSignalStrength:(AMapNaviGPSSignalStrength)gpsSignalStrength;

/**
 * @brief GPS导航中关于路线的‘信息通知’回调. since 6.2.0
 * @param driveManager 驾车导航管理类
 * @param notifyData 通知信息, 参考 AMapNaviRouteNotifyData .
 */
- (void)driveManager:(AMapNaviDriveManager *)driveManager postRouteNotification:(AMapNaviRouteNotifyData *)notifyData;

/**
 * @brief 多路线GPS导航模式下，建议将某备选路线切换为主导航路线的回调函数. since 6.3.0
 * @param driveManager 驾车导航管理类
 * @param suggestChangeMainNaviRouteInfo 切换主导航路线的相关信息, 参考 AMapNaviSuggestChangeMainNaviRouteInfo .
 */
- (void)driveManager:(AMapNaviDriveManager *)driveManager onSuggestChangeMainNaviRoute:(AMapNaviSuggestChangeMainNaviRouteInfo *)suggestChangeMainNaviRouteInfo;
@end

NS_ASSUME_NONNULL_END

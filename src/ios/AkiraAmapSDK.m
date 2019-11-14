/********* AkiraAmapNavigation.m Cordova Plugin Implementation *******/

#import <Cordova/CDV.h>
#import <UIKit/UIKit.h>
#import "SpeechSynthesizer.h"

#import <AMapNaviKit/AMapNaviKit.h>
#import <AMapLocationKit/AMapLocationKit.h>

@interface AkiraAmapSDK : CDVPlugin <AMapNaviDriveViewDelegate, AMapNaviDriveManagerDelegate,AMapLocationManagerDelegate> {
    // Member variables go here.
    NSString* callbackId;
    NSString* watchCallbackId;
}

@property (nonatomic, strong)NSString*                  amapApiKey;
@property (nonatomic, strong)MAMapView*                 mapView;
@property (nonatomic, strong)AMapNaviDriveManager*           naviManager;
@property (nonatomic, strong)AMapNaviDriveView*    naviDriveView;
@property (nonatomic)NSInteger totalLength; // 导航总长度
@property (nonatomic)NSInteger totalTime; // 导航总时间
@property (nonatomic)BOOL isArrive; // 是否已到达目的地
@property (nonatomic,strong)NSMutableArray<AMapNaviPoint *> *predictPoints; // 预计经纬度列表
@property (nonatomic,strong)NSMutableArray<AMapNaviPoint *> *actualPoints; // 实际经纬度列表

@property (nonatomic,strong)AMapLocatingCompletionBlock _completionBlock;
@property (nonatomic,strong)AMapLocationManager* _locationManager;
@property (nonatomic,strong)AMapLocationManager* _watchLocationManager;

@property (nonatomic,strong)NSDateFormatter* _dateFormatter;

@property (nonatomic)BOOL completionBlockInitialized;

- (void)startNavigation:(CDVInvokedUrlCommand*)command;
@end

@implementation AkiraAmapSDK

//插件对外方法 开启导航
- (void)startNavigation:(CDVInvokedUrlCommand*)command
{
    callbackId = command.callbackId;
    [AMapServices sharedServices].apiKey = [self amapApiKey];
    
    if(_predictPoints == nil){
        _predictPoints=[NSMutableArray array];
    }
    if([_predictPoints count]>0){
        [_predictPoints removeAllObjects];
    }
    
    if(_actualPoints == nil){
        _actualPoints=[NSMutableArray array];
    }
    
    if([_actualPoints count]>0){
        [_actualPoints removeAllObjects];
    }
    
    self.isArrive=false;
    
    
    CGFloat startLng=[[[command.arguments objectAtIndex:0] objectForKey:@"longitude"] doubleValue];
    CGFloat startLat=[[[command.arguments objectAtIndex:0] objectForKey:@"latitude"] doubleValue];
    
    CGFloat endLng=[[[command.arguments objectAtIndex:1] objectForKey:@"longitude"] doubleValue];
    CGFloat endLat=[[[command.arguments objectAtIndex:1] objectForKey:@"latitude"] doubleValue];
    
    
    AMapNaviPoint* startPoint = [AMapNaviPoint locationWithLatitude:startLat longitude:startLng];
    
    AMapNaviPoint* endPoint = [AMapNaviPoint locationWithLatitude:endLat longitude:endLng];
    
    [self initDriveView];
    
    [self initDriveManager];
    
    [self routePlanAction:startPoint endPoint:endPoint];
}

- (void)getCurrentLocation:(CDVInvokedUrlCommand *)command {
    
    callbackId = command.callbackId;
    
    [self initDateformat];
    
    [AMapServices sharedServices].apiKey = [self amapApiKey];
    
    [self initCompleteBlock];
    [self configLocationManager];
    
    [self._locationManager requestLocationWithReGeocode:YES completionBlock:self._completionBlock];
}

- (void)watchLocation:(CDVInvokedUrlCommand *)command {
    
    watchCallbackId = command.callbackId;
    
    [self initDateformat];
    
    [AMapServices sharedServices].apiKey = [self amapApiKey];
    
    [self initCompleteBlock];
    [self configWatchLocationManager: command.arguments];
    
    [self._watchLocationManager startUpdatingLocation];
}

-(void) configLocationManager {
    if(self._locationManager == nil){
        self._locationManager = [[AMapLocationManager alloc] init];
        [self._locationManager setDelegate:self];
        
        [self._locationManager setDesiredAccuracy:kCLLocationAccuracyHundredMeters ];
        
        [self._locationManager setPausesLocationUpdatesAutomatically:NO];
        
        [self._locationManager setAllowsBackgroundLocationUpdates:YES];
        
        [self._locationManager setLocationTimeout:10];
        
        [self._locationManager setReGeocodeTimeout:5];
    }
}

-(void) configWatchLocationManager: (NSArray *) args {
    if(self._watchLocationManager == nil){
        self._watchLocationManager = [[AMapLocationManager alloc] init];
        [self._watchLocationManager setDelegate:self];
        
        [self._watchLocationManager setDesiredAccuracy:kCLLocationAccuracyHundredMeters ];
        
        [self._watchLocationManager setPausesLocationUpdatesAutomatically:NO];
        
        [self._watchLocationManager setAllowsBackgroundLocationUpdates:YES];
        
        [self._watchLocationManager setLocationTimeout:10];
        
        [self._watchLocationManager setReGeocodeTimeout:5];
        
        // [[args objectAtIndex:0] objectForKey:@"needAddress"]
        
        [self._watchLocationManager setLocatingWithReGeocode:YES];
    }
}

-(void) initCompleteBlock{
    if(!self.completionBlockInitialized){
        __weak __typeof__(self) weakSelf = self;
        self._completionBlock = ^(CLLocation *location, AMapLocationReGeocode *regeocode, NSError *error) {
            if (error != nil && error.code == AMapLocationErrorLocateFailed)
            {
                //定位错误：此时location和regeocode没有返回值，不进行annotation的添加
                [weakSelf returnError:[@"定位错误：" stringByAppendingString:error.localizedDescription]];
                return;
            }
            else if (error != nil
                     && (error.code == AMapLocationErrorReGeocodeFailed
                         || error.code == AMapLocationErrorTimeOut
                         || error.code == AMapLocationErrorCannotFindHost
                         || error.code == AMapLocationErrorBadURL
                         || error.code == AMapLocationErrorNotConnectedToInternet
                         || error.code == AMapLocationErrorCannotConnectToHost))
            {
                //逆地理错误：在带逆地理的单次定位中，逆地理过程可能发生错误，此时location有返回值，regeocode无返回值，进行annotation的添加
                [weakSelf returnError:[@"定位错误：" stringByAppendingString:error.localizedDescription]];
            }
            else if (error != nil && error.code == AMapLocationErrorRiskOfFakeLocation)
            {
                //存在虚拟定位的风险：此时location和regeocode没有返回值，不进行annotation的添加
                [weakSelf returnError:[@"存在虚拟定位的风险：" stringByAppendingString:error.localizedDescription]];
                return;
            }
            else
            {
                //没有错误：location有返回值，regeocode是否有返回值取决于是否进行逆地理操作，进行annotation的添加
                [weakSelf returnLocationSuccess:location regeocode:regeocode callback:callbackId isOnce:true];
                
            }
        };
        
        self.completionBlockInitialized = true;
    }
}

- (void) amapLocationManager:(AMapLocationManager *)manager didUpdateLocation:(CLLocation *)location reGeocode:(AMapLocationReGeocode *)reGeocode{
    [self returnLocationSuccess:location regeocode:reGeocode callback:watchCallbackId isOnce:false];
}

// 返回导航数据结果
- (void)returnNavigationSuccess{
    
    NSMutableArray *predictPoints = [NSMutableArray array];
    for (AMapNaviPoint *item in [self predictPoints]) {
        [predictPoints addObject:@[[NSNumber numberWithFloat:item.longitude],
                                   [NSNumber numberWithFloat:item.latitude]]];
    }
    
    NSMutableArray *actualPoints = [NSMutableArray array];
    for (AMapNaviPoint *item in [self actualPoints]) {
        [predictPoints addObject:@[[NSNumber numberWithFloat:item.longitude],
                                   [NSNumber numberWithFloat:item.latitude]]];
    }
    
    NSDictionary* dictionary = @{@"totalLength":[NSNumber numberWithInt:[self totalLength]],
                                 @"totalTime":[NSNumber numberWithInt:[self totalTime]],
                                 @"isArrive":[NSNumber numberWithBool:[self isArrive]],
                                 @"predictPoints":predictPoints,
                                 @"actualPoints":actualPoints
                                 };
    
    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dictionary];
    
    [self.commandDelegate sendPluginResult:result callbackId:callbackId];
}

// 初始化日期格式化类
- (void) initDateformat {
    if(self._dateFormatter == nil){
        self._dateFormatter = [[NSDateFormatter alloc] init];
        [self._dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    }
}

// 返回插件错误结果
- (void)returnError: (NSString*) message{
    
    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:message];
    
    [self.commandDelegate sendPluginResult:result callbackId:callbackId];
}

// 返回定位数据结果
- (void)returnLocationSuccess:(CLLocation*)location regeocode:(AMapLocationReGeocode*)regeocode
                     callback:(NSString*)callback
                       isOnce:(Boolean*) isOnce{

    NSDictionary* dictionary = @{@"latitude": [NSNumber numberWithDouble:location.coordinate.latitude],
                                 @"longitude": [NSNumber numberWithDouble:location.coordinate.longitude],
                                 @"speed": [NSNumber numberWithDouble:location.speed],
                                 @"bearing": [NSNumber numberWithDouble:location.course],
                                 @"accuracy": [NSNumber numberWithDouble:location.horizontalAccuracy],
                                 @"date": [self._dateFormatter stringFromDate:location.timestamp],
                                 @"address": regeocode.formattedAddress ?: @"",
                                 @"country": regeocode.country ?: @"",
                                 @"province": regeocode.province ?: @"",
                                 @"city": regeocode.city ?: @"",
                                 @"cityCode": regeocode.citycode ?: @"",
                                 @"district": regeocode.district ?: @"",
                                 @"street": regeocode.street ?: @"",
                                 @"streetNum": regeocode.number ?: @"",
                                 @"adCode": regeocode.adcode ?: @"",
                                 @"poiName": regeocode.POIName ?: @"",
                                 @"aoiName": regeocode.AOIName ?: @""};
    
    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:dictionary];
    
    if(!isOnce){
        [result setKeepCallbackAsBool:YES];
    }
    
    [self.commandDelegate sendPluginResult:result callbackId:callback];
}

- (void)initDriveView
{
    if(self.naviDriveView == nil){
        self.naviDriveView = [[AMapNaviDriveView alloc] initWithFrame:self.webView.bounds];
        self.naviDriveView.autoresizingMask=UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight;
        [self.naviDriveView setDelegate:self];
        
        [self.naviDriveView setShowMoreButton:NO];
        
    }
    
    [self.webView addSubview:self.naviDriveView];
    
}

- (void)initDriveManager
{
    //请在 dealloc 函数中或者页面pop时，执行 [AMapNaviDriveManager destroyInstance] 来销毁单例
    [[AMapNaviDriveManager sharedInstance] setDelegate:self];
    
    [[AMapNaviDriveManager sharedInstance] addDataRepresentative:self.naviDriveView];
    
    [[AMapNaviDriveManager sharedInstance] setAllowsBackgroundLocationUpdates:YES];
    [[AMapNaviDriveManager sharedInstance] setPausesLocationUpdatesAutomatically:NO];
}



- (NSString*)amapApiKey{
    if(!_amapApiKey){
        CDVViewController* viewController = (CDVViewController*)self.viewController;
        _amapApiKey = [viewController.settings objectForKey:@"ios_key"];
    }
    return _amapApiKey;
}


-(void) showErrorAlert:(NSString*) title message:(NSString*) message{
    UIAlertController *alertVC =[UIAlertController alertControllerWithTitle:title message:message preferredStyle:UIAlertControllerStyleAlert];
    
    UIAlertAction *cancelAC=[UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action){}];
    
    [alertVC addAction:cancelAC];
    
    [self.viewController presentViewController:alertVC animated:YES completion:nil];
}

#pragma mark - AMapLocationManager Delegate

- (void)amapLocationManager:(AMapLocationManager *)manager doRequireLocationAuth:(CLLocationManager *)locationManager
{
    [locationManager requestAlwaysAuthorization];
}

#pragma mark - Actions

- (void)routePlanAction:(AMapNaviPoint*)startPoint endPoint:(AMapNaviPoint*)endPoint
{
    NSArray* startPoints = @[startPoint];
    NSArray* endPoints = @[endPoint];
    
    [[AMapNaviDriveManager sharedInstance] calculateDriveRouteWithStartPoints:startPoints endPoints:endPoints wayPoints:nil drivingStrategy:AMapNaviDrivingStrategySingleDefault];
}

#pragma mark - DriveNaviView Delegate

- (void)driveViewCloseButtonClicked:(AMapNaviDriveView *)driveView
{
    //停止导航
    [[AMapNaviDriveManager sharedInstance] stopNavi];
    
    [[AMapNaviDriveManager sharedInstance] removeEventListener:self.naviDriveView];
    [[AMapNaviDriveManager sharedInstance] setDelegate:nil];
    [AMapNaviDriveManager destroyInstance];
    
    //停止语音
    [[SpeechSynthesizer sharedSpeechSynthesizer] stopSpeak];
    
    [driveView removeFromSuperview];
    
    [self returnNavigationSuccess];
    
}

#pragma mark - AMapNaviDriveManager Delegate

- (void)driveManager:(AMapNaviDriveManager *)driveManager error:(NSError *)error
{
    [self showErrorAlert:@"初始化错误" message:error.localizedDescription];
}

- (void)driveManagerOnCalculateRouteSuccess:(AMapNaviDriveManager *)driveManager
{
    [[AMapNaviDriveManager sharedInstance] startGPSNavi];
    
    
    self.totalLength=[[[AMapNaviDriveManager sharedInstance] naviRoute] routeLength];
    self.totalTime=[[[AMapNaviDriveManager sharedInstance] naviRoute] routeTime];
    
    NSArray<AMapNaviPoint *> *points = [[[AMapNaviDriveManager sharedInstance] naviRoute] routeCoordinates];
    
    NSInteger count=[points count];
    
    for(int i=0;i<count;i++){
        [self.predictPoints addObject:[points objectAtIndex:i]];
    }
    
}

- (void)driveManager:(AMapNaviDriveManager *)driveManager onCalculateRouteFailure:(NSError *)error
{
    [self showErrorAlert:@"路径规划错误" message:error.localizedDescription];
    
}

- (void)driveManager:(AMapNaviDriveManager *)driveManager didStartNavi:(AMapNaviMode)naviMode
{
    
}

- (void)driveManagerNeedRecalculateRouteForYaw:(AMapNaviDriveManager *)driveManager
{
    
}

- (void)driveManagerNeedRecalculateRouteForTrafficJam:(AMapNaviDriveManager *)driveManager
{
    
}

- (void)driveManager:(AMapNaviDriveManager *)driveManager onArrivedWayPoint:(int)wayPointIndex
{
    
}

-(void) driveManager:(AMapNaviDriveManager *)driveManager updateNaviLocation:(nullable AMapNaviLocation *)naviLocation{
    if(naviLocation!=nil){
        AMapNaviPoint *point=[[AMapNaviPoint alloc] init];
        [point setLongitude:[[naviLocation coordinate] longitude]];
        [point setLatitude:[[naviLocation coordinate] latitude]];
        [self.actualPoints addObject:point];
    }
}

- (BOOL)driveManagerIsNaviSoundPlaying:(AMapNaviDriveManager *)driveManager
{
    return [[SpeechSynthesizer sharedSpeechSynthesizer] isSpeaking];
}

- (void)driveManager:(AMapNaviDriveManager *)driveManager playNaviSoundString:(NSString *)soundString soundStringType:(AMapNaviSoundType)soundStringType
{
    [[SpeechSynthesizer sharedSpeechSynthesizer] speakString:soundString];
}

- (void)driveManagerDidEndEmulatorNavi:(AMapNaviDriveManager *)driveManager
{
    
}

- (void)driveManagerOnArrivedDestination:(AMapNaviDriveManager *)driveManager
{
    self.isArrive=true;
}

@end

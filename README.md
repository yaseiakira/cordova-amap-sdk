# 高德地图SDK cordova 插件

> 支持ios, android
开通服务: [http://lbs.amap.com](http://lbs.amap.com)

## 安装

```
cordova plugin add https://github.com/yaseiakira/cordova-amap-sdk --variable IOS_KEY=你的KEY --variable ANDROID_KEY=你的KEY --save
```
或
```
ionic cordova plugin add https://github.com/yaseiakira/cordova-amap-sdk --variable IOS_KEY=你的KEY --variable ANDROID_KEY=你的KEY
```

## 使用方法

> 开始导航
```js

const win = window as any;

win.cordova.plugins.AkiraAmapSDK.startNavigation({
            start: {
                name: '北京路',
                longitude: 160.0012,
                latitude: 27.12
            }, end: {
                name: '澳门路',
                longitude: 160.0013,
                latitude: 27.13
            }
        }, (data) => {
            console.log(data);
			// 返回字段详情
            /*  isArrive // 是否达到目的地
                totalLength  // 路径规划线路的总长,单位: 米
                totalTime  // 预计时间,单位: 秒
                predictPoints:[[106.1.27.1],[106.2.27.2] ,...... ]  // 路径规划产生的定位点
                actualPoints :[[106.1.27.1],[106.2.27.2] ,...... ] // 实际走的定位点
			*/
        }, (err) => {
			console.log(err);
        });

```
> 持续定位
```js
const win = window as any;

win.cordova.plugins.AkiraAmapSDK.watchLocation({
                        interval: 2000, // 安卓可用
                        needAddress: true // 安卓可用,是否进行逆地址编码,ios 始终返回地址信息
                    },
                    p => {
                        console.log(p);
                        // 字段详情
                        /*  locationType //获取当前定位结果来源，如网络定位结果，详见定位类型表
                            latitude  //获取纬度
                            longitude  //获取经度
                            accuracy  //获取精度信息
                            speed  //获取速度信息
                            bearing  //获取方向信息
                            date   //定位时间
                            address  //地址，如果option中设置needAddress为false，则没有此结果
                            country  //国家信息
                            province  //省信息
                            city  //城市信息
                            district  //城区信息
                            street  //街道信息
                            streetNum  //街道门牌号
                            cityCode  //城市编码
                            adCode  //地区编码
                            poiName 
                            aoiName 
							*/
                    },
                    e => {
                        console.error(e);
                    }
                );

```
> 获取单次定位
```js
const win = window as any;

win.cordova.plugins.AkiraAmapSDK.getCurrentLocation(
                        p => {
                            console.log(p);
                        },
                        e => {
                            console.error(e);
                        }
                    );
```

## 其他注意事项

1.如果IOS集成后无法编译,请参考 [高德IOS手动部署](https://lbs.amap.com/api/ios-location-sdk/guide/create-project/manual-configuration)

2.[提交AppStore必读](https://lbs.amap.com/api/ios-location-sdk/guide/create-project/idfa-guide)

3.后台持续定位只需开启一次就可以了,无需多次调用该方法,暂时未提供关闭后台定位的方法

4.如果使用了后台持续定位,配合 [cordova-plugin-background-mode](https://github.com/katzer/cordova-plugin-background-mode) 插件效果更佳

5.如果正在使用后台持续定位,那么再次调用一次性定位貌似有点问题,奈何本人水平有限
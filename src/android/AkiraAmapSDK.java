package com.colorfulsquare.akira;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;

import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaWebView;
import org.apache.cordova.PluginResult;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.amap.api.location.AMapLocation;
import com.amap.api.location.AMapLocationClient;
import com.amap.api.location.AMapLocationClientOption;
import com.amap.api.location.AMapLocationClientOption.AMapLocationMode;
import com.amap.api.location.AMapLocationListener;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import android.Manifest;
import android.content.pm.PackageManager;

import com.colorfulsquare.akira.activity.AkiraAmapNaviActivity;

/**
 * This class echoes a string called from JavaScript.
 */
public class AkiraAmapSDK extends CordovaPlugin implements ActivityCompat.OnRequestPermissionsResultCallback {

    private static final int PERMISSON_REQUESTCODE = 0;

    /**
     * 判断是否需要检测，防止不停的弹框
     */
    private boolean isNeedCheck = true;

    String TAG = "AkiraAmapSDK";
    String[] permissions = {
            Manifest.permission.ACCESS_COARSE_LOCATION,
            Manifest.permission.ACCESS_FINE_LOCATION,
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.READ_PHONE_STATE,
            Manifest.permission.ACCESS_LOCATION_EXTRA_COMMANDS
    };

    private CallbackContext callbackContext;
    private CallbackContext navCallbackContext;
    private CallbackContext watchCallbackContext;

    private AMapLocationClient locationClient = null;
    private AMapLocationClientOption locationOption = null;

    private AMapLocationClient watchLocationClient = null;
    private AMapLocationClientOption watchlocationOption = null;

    @Override
    public void pluginInitialize() {
        if (isNeedCheck) {
            checkPermissions(permissions);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String[] permissions, int[] paramArrayOfInt) {
        if (requestCode == PERMISSON_REQUESTCODE) {
            if (!verifyPermissions(paramArrayOfInt)) {
                showMissingPermissionDialog();
                isNeedCheck = false;
            }
        }
    }

    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        if (action.equals("startNavigation")) {
            if (args == null || args.length() == 0 || args.length() < 2) {
                callbackContext.error("起点或终点数据为空或者长度不够");
                return true;
            }
            this.startNavigation(args, callbackContext);
            return true;
        }

        if (action.equals("getCurrentLocation")) {
            this.getCurrentLocation(callbackContext);
            return true;
        }

        if (action.equals("watchLocation")) {
            this.watchLocation(args, callbackContext);
            return true;
        }

        return false;
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent intent) {
        if (resultCode == Activity.RESULT_OK) {
            try {
                this.navCallbackContext.success(new JSONObject(intent.getStringExtra("result")));
            } catch (JSONException ex) {
                this.navCallbackContext.error("errMsg:" + ex.getMessage());
            }
        } else if (resultCode == Activity.RESULT_CANCELED) {
            this.navCallbackContext.error("errMsg:取消导航");
        } else {
            this.navCallbackContext.error("errMsg:未能获取调用结果");
        }
    }

    /*插件公开方法:开启导航*/
    private void startNavigation(JSONArray args, CallbackContext callbackContext) throws JSONException {
        this.navCallbackContext = callbackContext;
        Intent naviIntent = new Intent().setClass(cordova.getActivity(), AkiraAmapNaviActivity.class);
        naviIntent.putExtra("start", args.getJSONObject(0).toString());
        naviIntent.putExtra("end", args.getJSONObject(1).toString());
        cordova.startActivityForResult(this, naviIntent, 0);
    }

    /*插件公开方法:获取一次性定位*/
    private void getCurrentLocation(CallbackContext callbackContext) {
        this.callbackContext = callbackContext;
        this.initAMapLocation();
        locationClient.startLocation();
    }

    /*插件公开方法:开启持续导航*/
    private void watchLocation(JSONArray args, CallbackContext callbackContext) throws JSONException {
        this.watchCallbackContext = callbackContext;
        this.initAMapWatchLocation(args);
        watchLocationClient.startLocation();
    }

    /*内部方法:检测权限*/
    private void checkPermissions(String... permissions) {
        Activity currentActivity = this.cordova.getActivity();
        List<String> needRequestPermissonList = findDeniedPermissions(permissions);
        if (null != needRequestPermissonList
                && needRequestPermissonList.size() > 0) {
            ActivityCompat.requestPermissions(currentActivity,
                    needRequestPermissonList.toArray(
                            new String[needRequestPermissonList.size()]),
                    PERMISSON_REQUESTCODE);
        }
    }

    /**
     * 显示提示信息
     *
     * @since 2.5.0
     */
    private void showMissingPermissionDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this.cordova.getActivity().getApplicationContext());
        builder.setTitle("提示");
        builder.setMessage("当前应用缺少必要权限。\\n\\n请点击\\\"设置\\\"-\\\"权限\\\"-打开所需权限");

        // 拒绝, 退出应用
        builder.setNegativeButton("取消",
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });

        builder.setPositiveButton("设置",
                new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        startAppSettings();
                    }
                });

        builder.setCancelable(false);

        builder.show();
    }

    /**
     * 启动应用的设置
     *
     * @since 2.5.0
     */
    private void startAppSettings() {
        Intent intent = new Intent(
                Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
        intent.setData(Uri.parse("package:" + this.cordova.getActivity().getPackageName()));
        this.cordova.getActivity().startActivity(intent);
    }

    /**
     * 内部方法:获取权限集中需要申请权限的列表
     *
     * @param permissions
     * @return
     * @since 2.5.0
     */
    private List<String> findDeniedPermissions(String[] permissions) {
        Activity currentActivity = this.cordova.getActivity();
        List<String> needRequestPermissonList = new ArrayList<String>();
        for (String perm : permissions) {
            if (ContextCompat.checkSelfPermission(currentActivity.getApplicationContext(),
                    perm) != PackageManager.PERMISSION_GRANTED
                    || ActivityCompat.shouldShowRequestPermissionRationale(
                    currentActivity, perm)) {
                needRequestPermissonList.add(perm);
            }
        }
        return needRequestPermissonList;
    }

    /**
     * 内部方法:检测是否说有的权限都已经授权
     *
     * @param grantResults
     * @return
     * @since 2.5.0
     */
    private boolean verifyPermissions(int[] grantResults) {
        for (int result : grantResults) {
            if (result != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }
        return true;
    }

    /* 内部方法:初始化一次性定位 */
    private void initAMapLocation() {
        if (locationClient == null) {
            locationClient = new AMapLocationClient(this.cordova.getActivity().getApplicationContext());

            if (locationOption == null) {
                locationOption = new AMapLocationClientOption();
                AMapLocationMode accuracy = AMapLocationMode.Hight_Accuracy;
                int interval = 2000;
                boolean needAddress = true;

                locationOption.setLocationMode(accuracy);
                locationOption.setNeedAddress(needAddress);
                locationOption.setInterval(interval);

                //设置为单次定位
                locationOption.setOnceLocation(true);
            }

            locationClient.setLocationListener(new AMapLocationListener() {
                @Override
                public void onLocationChanged(AMapLocation aMapLocation) {
                    if (aMapLocation != null) {
                        if (aMapLocation.getErrorCode() == 0) {
                            SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                            Date date = new Date(aMapLocation.getTime());
                            df.format(date);//定位时间

                            JSONObject locationInfo = new JSONObject();
                            try {
                                locationInfo.put("locationType", aMapLocation.getLocationType()); //获取当前定位结果来源，如网络定位结果，详见定位类型表
                                locationInfo.put("latitude", aMapLocation.getLatitude()); //获取纬度
                                locationInfo.put("longitude", aMapLocation.getLongitude()); //获取经度
                                locationInfo.put("accuracy", aMapLocation.getAccuracy()); //获取精度信息
                                locationInfo.put("speed", aMapLocation.getSpeed()); //获取速度信息
                                locationInfo.put("bearing", aMapLocation.getBearing()); //获取方向信息
                                locationInfo.put("date", date); //定位时间
                                locationInfo.put("address", aMapLocation.getAddress()); //地址，如果option中设置isNeedAddress为false，则没有此结果
                                locationInfo.put("country", aMapLocation.getCountry()); //国家信息
                                locationInfo.put("province", aMapLocation.getProvince()); //省信息
                                locationInfo.put("city", aMapLocation.getCity()); //城市信息
                                locationInfo.put("district", aMapLocation.getDistrict()); //城区信息
                                locationInfo.put("street", aMapLocation.getStreet()); //街道信息
                                locationInfo.put("streetNum", aMapLocation.getStreetNum()); //街道门牌号
                                locationInfo.put("cityCode", aMapLocation.getCityCode()); //城市编码
                                locationInfo.put("adCode", aMapLocation.getAdCode()); //地区编码
                                locationInfo.put("poiName", aMapLocation.getPoiName());
                                locationInfo.put("aoiName", aMapLocation.getAoiName());

                                PluginResult result = new PluginResult(PluginResult.Status.OK, locationInfo);

                                callbackContext.sendPluginResult(result);

                            } catch (JSONException e) {
                                Log.e(TAG, "Locatioin json error:" + e);

                                PluginResult result = new PluginResult(PluginResult.Status.ERROR, e.getMessage());
                                callbackContext.sendPluginResult(result);
                            }

                        } else {
                            //显示错误信息ErrCode是错误码，errInfo是错误信息，详见错误码表。
                            Log.e(TAG, "Locatioin error:" + aMapLocation.getErrorCode());
                            PluginResult result = new PluginResult(PluginResult.Status.ERROR, aMapLocation.getErrorInfo());
                            callbackContext.sendPluginResult(result);
                        }

                        locationClient.stopLocation();
                    }
                }
            });
            locationClient.setLocationOption(locationOption);
        }

    }

    /* 内部方法:初始化后台持续性定位 */
    private void initAMapWatchLocation(JSONArray args) throws JSONException {
        if (watchLocationClient == null) {
            watchLocationClient = new AMapLocationClient(this.cordova.getActivity().getApplicationContext());

            if (watchlocationOption == null) {
                watchlocationOption = new AMapLocationClientOption();
                AMapLocationMode accuracy = AMapLocationMode.Hight_Accuracy;

                int interval = 2000;
                boolean needAddress = true;

                if (args.length() > 0) {
                    JSONObject options = args.getJSONObject(0);
                    if (options != null) {
                        if (options.has("interval")) {
                            interval = options.optInt("interval", 2000);
                        }
                        if (options.has("needAddress")) {
                            needAddress = options.optBoolean("needAddress", false);
                        }
                    }
                }

                watchlocationOption.setLocationMode(accuracy);
                watchlocationOption.setNeedAddress(needAddress);
                watchlocationOption.setInterval(interval);

                //设置为持续定位
                watchlocationOption.setOnceLocation(false);
            }

            watchLocationClient.setLocationListener(new AMapLocationListener() {
                @Override
                public void onLocationChanged(AMapLocation aMapLocation) {
                    if (aMapLocation != null) {
                        if (aMapLocation.getErrorCode() == 0) {
                            SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                            Date date = new Date(aMapLocation.getTime());
                            df.format(date);//定位时间

                            JSONObject locationInfo = new JSONObject();
                            try {
                                locationInfo.put("locationType", aMapLocation.getLocationType()); //获取当前定位结果来源，如网络定位结果，详见定位类型表
                                locationInfo.put("latitude", aMapLocation.getLatitude()); //获取纬度
                                locationInfo.put("longitude", aMapLocation.getLongitude()); //获取经度
                                locationInfo.put("accuracy", aMapLocation.getAccuracy()); //获取精度信息
                                locationInfo.put("speed", aMapLocation.getSpeed()); //获取速度信息
                                locationInfo.put("bearing", aMapLocation.getBearing()); //获取方向信息
                                locationInfo.put("date", date); //定位时间
                                locationInfo.put("address", aMapLocation.getAddress()); //地址，如果option中设置isNeedAddress为false，则没有此结果
                                locationInfo.put("country", aMapLocation.getCountry()); //国家信息
                                locationInfo.put("province", aMapLocation.getProvince()); //省信息
                                locationInfo.put("city", aMapLocation.getCity()); //城市信息
                                locationInfo.put("district", aMapLocation.getDistrict()); //城区信息
                                locationInfo.put("street", aMapLocation.getStreet()); //街道信息
                                locationInfo.put("streetNum", aMapLocation.getStreetNum()); //街道门牌号
                                locationInfo.put("cityCode", aMapLocation.getCityCode()); //城市编码
                                locationInfo.put("adCode", aMapLocation.getAdCode()); //地区编码
                                locationInfo.put("poiName", aMapLocation.getPoiName());
                                locationInfo.put("aoiName", aMapLocation.getAoiName());

                                PluginResult result = new PluginResult(PluginResult.Status.OK, locationInfo);
                                result.setKeepCallback(true);
                                watchCallbackContext.sendPluginResult(result);

                            } catch (JSONException e) {
                                Log.e(TAG, "Watch Locatioin json error:" + e);

                                PluginResult result = new PluginResult(PluginResult.Status.ERROR, e.getMessage());
                                result.setKeepCallback(true);
                                watchCallbackContext.sendPluginResult(result);
                            }

                        } else {
                            //显示错误信息ErrCode是错误码，errInfo是错误信息，详见错误码表。
                            Log.e(TAG, "Locatioin error:" + aMapLocation.getErrorCode());
                            PluginResult result = new PluginResult(PluginResult.Status.ERROR, aMapLocation.getErrorInfo());
                            result.setKeepCallback(true);
                            watchCallbackContext.sendPluginResult(result);
                        }
                    }
                }
            });
            watchLocationClient.setLocationOption(watchlocationOption);
        }

    }

}

package com.colorfulsquare.akira.model;

import com.amap.api.navi.model.NaviLatLng;

import java.util.List;

public class NavigationResult {
    private int totalLength;
    private int totalTime;
    private List<NaviLatLng> predictPoints;
    private List<NaviLatLng> actualPoints;
    private boolean isArrive;

    public NavigationResult() {
        this.isArrive = false;
    }

    public void setTotalLength(int value) {
        this.totalLength = value;
    }

    public void setTotalTime(int value) {
        this.totalTime = value;
    }

    public void setPredictPoints(List<NaviLatLng> value) {
        this.predictPoints = value;
    }

    public void setActualPoints(List<NaviLatLng> value) {
        this.actualPoints = value;
    }

    public void addToActualPoints(NaviLatLng value) {
        this.actualPoints.add(value);
    }

    public void setIsArrive(boolean value) {
        this.isArrive = value;
    }

    @Override
    public String toString() {
        String isArriveValue = isArrive ? "true" : "false";
        return "{"
                + "\"totalLength\":" + totalLength + ","
                + "\"totalTime\":" + totalTime + ","
                + "\"predictPoints\":" + convertListToJSONString(predictPoints) + ","
                + "\"actualPoints\":" + convertListToJSONString(actualPoints) + ","
                + "\"isArrive\":" + isArriveValue
                + "}";
    }

    private String convertListToJSONString(List<NaviLatLng> list) {
        if (list.isEmpty()) {
            return "[]";
        }
        String innerListString = "";
        for (int i = 0; i < list.size(); i++) {
            NaviLatLng point = list.get(i);
            innerListString += "[" + point.getLongitude() + "," + point.getLatitude() + "],";

        }
        innerListString = innerListString.substring(0, innerListString.length() - 1);
        return "[" + innerListString + "]";
    }
}

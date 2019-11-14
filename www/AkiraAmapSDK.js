var exec = require('cordova/exec');

exports.startNavigation = function (points, success, error) {
    exec(success, error, 'AkiraAmapSDK', 'startNavigation', [points.start, points.end]);
};

exports.getCurrentLocation = function (success, error) {
    exec(success, error, 'AkiraAmapSDK', 'getCurrentLocation', []);
};

exports.watchLocation = function (options,success, error) {
    exec(success, error, 'AkiraAmapSDK', 'watchLocation', [options]);
};



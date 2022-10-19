#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QPoint>

namespace Config
{
    const static QString url = "http://localhost:8080";
    const static QString loginUrl = url + "/login";
    const static QString homeUrl = url + "/home";
    const static QString registerUrl = url + "/register";
    const static QString enableAllUrl = url + "/home/activate";
    const static QString shutdownAllUrl = url + "/home/shutdown";
    const static QString changeDevicesStatusUrl = url + "/home/changeDevicesStatus";
    const static QString setDeviceConfigUrl = url + "/home/changeDeviceConfig";
    const static QString deviceUrl = url + "/home/device?username=";
    const static QString logoutUrl = url + "/logout";
    const static QString removeUrl = url + "/home/remove";
    static int width = 1920;
    static int height = 1080;
    static int updateInterval = 60;
    static int popupWidth = 400;
    static int popupHeight = 600;
    static QPoint popupDeviation = QPoint(0, 120);
};

#endif // CONFIG_H

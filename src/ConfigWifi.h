#ifndef CONFIGWIFI_H
#define CONFIGWIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <WebServer.h>

class ConfigWifi
{
public:
  ConfigWifi();
  bool connectWifi();
  void begin();
  void loop();
  bool useWeb;
  String AP_NAME;
  IPAddress IP;

private:
  void initSoftAP();
  void initDNS();
  void initWebServer();
  void handleRoot();
  void handleRootPost();
  bool haveSSID();

  WebServer server;
  DNSServer dnsServer;
  String page_html_template;
};

#endif // CONFIGWIFI_H
#include "ConfigWifi.h"

ConfigWifi::ConfigWifi()
    : server(80), useWeb(false), AP_NAME("连接此WIFI配网"), IP(192, 168, 4, 1),
      page_html_template(R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, user-scalable=no">
    <title>配网界面</title>
    <style>
        *{margin:0;padding:0;box-sizing:border-box}
        html,body{height:100%}
        body{background-color:#1F6F3A;display:flex;justify-content:center;align-items:center}
        .container{width:90%;max-width:400px;padding:30px;border:3px solid#fff;border-radius:20px;text-align:center;box-shadow:0 4px 8px}
        h2{color:#fff;margin-bottom:20px;font-size:24px}
        .form-item{margin-bottom:20px}
        .form-item label{color:#fff;font-size:16px}
        .form-item select{}
        .form-item input,.form-item select{width:100%;height:48px;padding:0 25px;border:2px solid#fff;border-radius:25px;font-size:18px;color:#fff;background-color:rgba(255,255,255,0.2);outline:none;transition:background-color 0.3s,border-color 0.3s,box-shadow 0.3s}
        .form-item input::placeholder{color:#ddd}
        .send_button{width:100%;height:50px;border:none;border-radius:25px;font-size:18px;color:#1F6F4A;background-color:#fff;cursor:pointer;outline:none}
        .checkbox-container{display:flex;justify-content:center;align-items:center;margin-top:20px;margin-bottom:30px}
        .checkbox-container input[type="checkbox"]{width:20px;height:20px;margin-right:10px}
        .checkbox-container label{color:#fff;font-size:16px}
        .hidden{display:none}
    </style>
    <script>
        document.addEventListener("DOMContentLoaded", function() {
            const manualSsidCheckbox = document.querySelector('input[name="manual_ssid"]');
            const ssidSelect = document.querySelector('select[name="ssid"]');
            const ssidInput = document.createElement('input');
            ssidInput.name = "manual_ssid_input";
            ssidInput.type = "text";
            ssidInput.placeholder = "WiFi名称";
            ssidInput.classList.add('hidden');
            ssidSelect.parentNode.insertBefore(ssidInput, ssidSelect.nextSibling);

            manualSsidCheckbox.addEventListener('change', function() {
                if (this.checked) {
                    ssidSelect.classList.add('hidden');
                    ssidInput.classList.remove('hidden');
                } else {
                    ssidSelect.classList.remove('hidden');
                    ssidInput.classList.add('hidden');
                }
            });
        });
    </script>
</head>
<body>
    <div class="container">
        <h2>参数配置</h2>
        <form action="/wifi" method="POST">
            <div class="form-item">
                <select name="ssid" autocomplete="off">
                    <option value="">选择WiFi网络</option>
                    {{WIFI_LIST}}
                </select>
            </div>
            <div class="form-item">
                <input type="password" name="password" autocomplete="on" placeholder="WiFi密码" required>
            </div>
            <div class="checkbox-container">
                <input type="checkbox" name="manual_ssid" id="manual_ssid">
                <label for="manual_ssid">手动填入WiFi名称</label>
            </div>
            <div class="form-item">
                <input class="send_button" type="submit" value="保存并连接">
            </div>
        </form>
    </div>
</body>
</html>
)")
{
}

bool ConfigWifi::haveSSID()
{
  Preferences prefs;
  prefs.begin("wifi");
  String ssid = prefs.getString("ssid");
  String password = prefs.getString("password");
  return !ssid.isEmpty() && password.length() >= 8;
}

bool ConfigWifi::connectWifi()
{
  if (haveSSID())
  {
    Preferences prefs;
    prefs.begin("wifi");
    String ssid = prefs.getString("ssid");
    String password = prefs.getString("password");
    Serial.println("Connecting to WiFi: " + ssid);
    Serial.println("Password: " + password);
    int i = 0;
    bool isConnect = false;

    while (i < 5)
    {
      WiFi.begin(ssid.c_str(), password.c_str());
      delay(1000);
      Serial.println("Connecting to " + ssid + " for " + i);
      isConnect = (WiFi.status() == WL_CONNECTED);
      if (!isConnect)
      {
        delay(1000);
        i++;
      }
      else
      {
        break;
      }
    }

    if (!isConnect)
    {
      Serial.println("Failed to connect to WiFi after 5 attempts.");
    }
    else
    {
      Serial.println("Connected to WiFi.");
    }

    return isConnect;
  }
  else
  {
    return false;
  }
}

void ConfigWifi::handleRoot()
{
  String wifiList;
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i)
  {
    wifiList += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
  }
  WiFi.scanDelete(); // Free memory
  String html = page_html_template;
  html.replace("{{WIFI_LIST}}", wifiList);
  server.send(200, "text/html", html);
}

void ConfigWifi::handleRootPost()
{
  String password = server.arg("password");
  String ssid = server.arg("ssid");
  String manual_ssid_input = server.arg("manual_ssid_input");
  if (!manual_ssid_input.isEmpty())
  {
    ssid = manual_ssid_input;
  }
  Serial.println("SSID: " + ssid);

  if (!ssid.isEmpty() && password.length() >= 8)
  {
    server.send(200, "text/html", "<h1>开始连接WIFI，连接成功后将重启设备</h1>");
    Preferences prefs;
    prefs.begin("wifi");
    prefs.putString("ssid", ssid);
    prefs.putString("password", password);
    if (connectWifi())
    {
      ESP.restart();
    }
  }
  else
  {
    server.send(200, "text/html", "<h1>输入WIFI账号密码异常</h1>");
  }
}

void ConfigWifi::initSoftAP()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IP, IP, IPAddress(255, 255, 255, 0));
  if (WiFi.softAP(AP_NAME))
  {
    Serial.println("ESP32 SoftAP mode set successfully");
    Serial.print("SoftAP IP address: ");
    Serial.println(WiFi.softAPIP());
  }
  else
  {
    Serial.println("Failed to set ESP32 SoftAP mode");
  }
}

void ConfigWifi::initDNS()
{
  if (dnsServer.start(53, "*", IP))
  {
    Serial.println("DNS server started successfully");
  }
  else
  {
    Serial.println("Failed to start DNS server");
  }
}

void ConfigWifi::initWebServer()
{
  server.on("/", HTTP_GET, std::bind(&ConfigWifi::handleRoot, this));
  server.on("/wifi", HTTP_POST, std::bind(&ConfigWifi::handleRootPost, this));
  server.onNotFound(std::bind(&ConfigWifi::handleRoot, this));
  server.begin();
  Serial.println("Web server started successfully!");
}

void ConfigWifi::begin()
{
  bool wifiConnected = connectWifi();
  if (wifiConnected)
  {
    Serial.println("WIFI connect successed");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    useWeb = false;
  }
  else
  {
    Serial.println("WIFI connect error, start AP mode.");
    initSoftAP();
    initWebServer();
    initDNS();
    useWeb = true;
  }
}

void ConfigWifi::loop()
{
  if (useWeb)
  {
    dnsServer.processNextRequest();
    server.handleClient();
  }
}
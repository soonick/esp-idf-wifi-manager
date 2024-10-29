# esp-idf-wifi-manager

[![CircleCI](https://dl.circleci.com/status-badge/img/gh/soonick/esp-idf-wifi-manager/tree/master.svg?style=svg)](https://dl.circleci.com/status-badge/redirect/gh/soonick/esp-idf-wifi-manager/tree/master)

Compatible with ESP-IDF. Starts an access point where users can provide WiFi credentials.

## How to use

Go to the `components` folder in your project and download this library:

```
git submodule add https://github.com/soonick/esp-idf-wifi-manager.git
```

Instantiate the esp-idf-wifi-manager and get user configuration:

```cpp
EspIdfWifiManager wm;
std::unordered_map<std::string, std::string> config = wm.getUserConfig();
```

`getUserConfig` returns the user configuration directly from `nvs` if it's available. If not, it starts an access point

# esp-idf-wifi-manager

[![CircleCI](https://dl.circleci.com/status-badge/img/gh/soonick/esp-idf-wifi-manager/tree/master.svg?style=svg)](https://dl.circleci.com/status-badge/redirect/gh/soonick/esp-idf-wifi-manager/tree/master)

Compatible with ESP-IDF. Starts an access point where users can provide WiFi credentials.

## Development

The easiest way to start developing is to start a development shell:

```
make ssh
```

If you have a device connected and want to be able to flash to the device, use:

```
make ssh-connected
```

From the shell, navigate to an example, and build it. For example:

```
cd examples/basic
. /esp-idf/export.sh
idf.py set-target esp32
idf.py build
```

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

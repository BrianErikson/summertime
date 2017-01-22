# Summertime

A quick way to see the high and low from a five-day forecast

## Installation

* Ensure the following libraries are installed on your linux distribution:
	* libcurl
  	* jsoncpp
  	* libnotify
* This application uses a notification server to provide weather information, please ensure one that supports libnotify is installed on your system

* `cd summertime`
* `mkdir build && cd build`
* `cmake ../ && make`
* `./summertime`

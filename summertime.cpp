#include "summertime.hpp"
#include <iostream>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <sstream>
#include <libnotify/notify.h>
#include <libnotify/notification.h>

using namespace std;

static size_t curlCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string fetchWeatherJSON() {
    CURL *curl;
    CURLcode res;
    string curlBuffer;

    curl = curl_easy_init();
    if (!curl) {
        throw "cURL could not be initialized";
    }
    curl_easy_setopt(curl, CURLOPT_URL, "api.openweathermap.org/data/2.5/forecast?id=5781783&units=imperial&appid=91cdcd239b9914515bdb4b9134590c11");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        throw "curl_easy_perform() failed";
    }

    //cout << curlBuffer << endl;
    //cout << "------" << endl;
    curl_easy_cleanup(curl);

    return curlBuffer;
}

string fetchNotificationContent() {
    Json::Value root;
    Json::Reader reader;
    string weatherJSON;
    try {
        weatherJSON = fetchWeatherJSON();
    } catch (const char *msg) {
        throw msg;
    }

    reader.parse(fetchWeatherJSON(), root, false);
    if (!reader.getFormattedErrorMessages().empty()) {
        throw "Could not parse fetched JSON";
    }

    int dataLen = root["cnt"].asInt();

    double maxTemp = 0;
    int maxInd = 0;
	double minTemp = 1000;
	int minInd = 0;
    for (int i = 0; i < dataLen; i++) {
        double temp = root["list"][i]["main"]["temp"].asDouble();
        if (temp > maxTemp) {
            maxInd = i;
            maxTemp = temp;
        }
		else if (temp < minTemp) {
			minInd = i;
			minTemp = temp;
		}
    }

    string maxDate = root["list"][maxInd]["dt_txt"].asString();
    unsigned long delimInd = maxDate.find_first_of(" ");
    maxDate = maxDate.substr(0, delimInd);

    string minDate = root["list"][minInd]["dt_txt"].asString();
	delimInd = minDate.find_first_of(" ");
	minDate = minDate.substr(0, delimInd);

    //cout << maxTemp << " | " << root["list"][maxInd]["dt_txt"].asString() << endl;
    //cout << minTemp << " | " << root["list"][minInd]["dt_txt"].asString() << endl;

    ostringstream content;
    content << "High:\t" << maxTemp << "f \t" << maxDate << endl;
	content << "Low:\t" << minTemp << "f \t" << minDate; 
	return content.str();
}

int main() {
	string content;
	try {
		content = fetchNotificationContent();
	} catch (const char *msg) {
		cerr << msg << endl;
		return 1;
	}

    notify_init("Summertime");

    NotifyNotification* notification = notify_notification_new(
			"Five-day Forecast", content.c_str(), "emblem-synchronizing");
    notify_notification_set_urgency(notification, NOTIFY_URGENCY_NORMAL);

    if (!notify_notification_show(notification, NULL)) {
        notify_uninit();
        return 2;
    }

    notify_uninit();
    return 0;
}

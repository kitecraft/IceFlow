#pragma once
#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>

using namespace std;
#include <vector>

static String IceFS_ReadFile(String filename)
{
	SPIFFS.begin(false);
	String ret = "";
    filename = String(PROFILE_SPIFFS_FILE_DIRECTORY) + "/" + filename;

	File file = SPIFFS.open(filename);
	if (!file) {
		Serial.print("Failed to open file: ");
		Serial.println(filename);
		return ret;
	}

	while (file.available()) {
		ret += file.readString();
	}
	file.close();
	return ret;
}

static vector<String> GetDirectoryListing(String directory)
{
	vector<String> ret;

    File dir = SPIFFS.open(directory);
    if (!dir) {
        Serial.println("- failed to open directory");
        return ret;
    }
    if (!dir.isDirectory()) {
        Serial.println(" - not a directory");
        return ret;
    }

    File file = dir.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            ret.push_back(file.name());
        }
        file = dir.openNextFile();
    }
	return ret;
}
#pragma once
#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>

static String IceFS_ReadFile(String filename)
{
	SPIFFS.begin(false);
	String ret = "";
    
    //Serial.print("Opening: ");
    //Serial.println(filename);
	fs::File file = SPIFFS.open(filename);
	if (!file) {
		Serial.print("Failed to open file: ");
		Serial.println(filename);
		return ret;
	}

	while (file.available()) {
		ret += file.readString();
	}
	file.close();
    //Serial.print("Returning: ");
    //Serial.println(ret);
	return ret;
}

static int GetDirectoryListing(String directory, String *strArray)
{
    if (directory.endsWith("/")) {
        directory.remove(directory.length() - 1);
    }
	
    fs::File dir = SPIFFS.open(directory);
    if (!dir) {
        Serial.println("- failed to open directory");
        return 0;
    }
    if (!dir.isDirectory()) {
        Serial.println(" - not a directory");
        return 0;
    }

    fs::File file = dir.openNextFile();
    int filecount = 0;
    while (file) {
        if (!file.isDirectory()) {
            filecount++;
        }
        file = dir.openNextFile();
    }

    strArray = new String[filecount];

    file = dir.openNextFile();
    int curr = 0;
    while (file) {
        if (!file.isDirectory()) {
            strArray[curr] = (file.name());
        }
        file = dir.openNextFile();
    }
	return filecount;
}

static double GetFreespaceBytes()
{
    return (SPIFFS.totalBytes() - SPIFFS.usedBytes());
}
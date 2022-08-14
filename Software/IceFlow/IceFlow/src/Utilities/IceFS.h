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

static bool IceFs_DoesFileExist(String fileName)
{
    return SPIFFS.exists(fileName);
}

static bool IceFS_WriteFile(String fileName, String contents)
{
    fs::File file = SPIFFS.open(fileName, FILE_WRITE, true);
    int ret = file.print(contents);
    file.close();
    if (ret > 0) {
        return true;
    }
    return false;
}

static int IceFS_GetDirectoryListing(String directory, String *&strArray)
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

    dir = SPIFFS.open(directory);
    file = dir.openNextFile();
    int curr = 0;
    while (file) {
        if (!file.isDirectory()) {
            strArray[curr] = (file.name());
            curr++;
        }
        file = dir.openNextFile();
    }

	return filecount;
}

static bool IceFS_DeleteFile(String fileName)
{
    SPIFFS.begin(false);

    return SPIFFS.remove(fileName);
}

static double IceFS_GetFreespaceBytes()
{
    return (SPIFFS.totalBytes() - SPIFFS.usedBytes());
}

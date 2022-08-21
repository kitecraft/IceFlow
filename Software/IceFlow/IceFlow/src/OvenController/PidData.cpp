#include "PidData.h"
#include <ArduinoJson.h>

PidData::PidData()
{

}

PidData::~PidData()
{

}

bool PidData::GetData()
{
	String jsonString = IceFS_ReadFile(String(PID_DATA_SPIFFS_FILE_DIRECTORY) + PID_DATA_FILENAME);
}
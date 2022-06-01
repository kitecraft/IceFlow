#pragma once

enum StarsideErrorCodes {
	NO_ERROR = 0,
	ERROR_SPIFFS_FAILED,
	ERROR_FAILED_TO_LOAD_SETTINGS,
	ERROR_FAILED_TO_WRITE_SETTINGS,
	ERROR_WIFI_NOT_CONFIGURED,
	ERROR_FONT_MISSING,
	ERROR_WIFI_NO_CONNECT_RESULT,
	ERROR_OTA_CURRENTLY_IN_PROGRESS,
	ERROR_ABORT_OTA_HAS_RETURNED_FALSE,

};

class StarsideError {
public:
	StarsideErrorCodes code = NO_ERROR;
	String msg = "";
	StarsideError() {};
	void Clear(){
		code = NO_ERROR;
		msg = "";
	};
	void Set(StarsideErrorCodes newCode, String newMsg){
		code = newCode;
		msg = newMsg;
	};
};

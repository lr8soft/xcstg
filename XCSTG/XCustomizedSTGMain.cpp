#include "util/AppFrameWork.h"
#include <mutex>
#include <iostream>
#include <thread>
#ifndef _DEBUG
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif
int exit_code = 0;
std::mutex mutex;
void SetExitType() {
	mutex.lock();
	exit_code=-1;
	mutex.unlock();
}
int GetExitType() {
	mutex.lock();
	int code = exit_code;
	mutex.unlock();
	return code;
}
void GameThead() {
	xc_ogl::AppFrameWork app(1280, 720, "XCSTG v0.73");
	app.init();
	app.message_loop();
	SetExitType();
}
int main() {
	std::thread oglThread(GameThead);
	oglThread.detach();
	while (!GetExitType());
	return 0;
}
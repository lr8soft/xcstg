#pragma once
#ifndef _XCTASK_LOOP_H_
#define _XCTASK_LOOP_H_
#include<map>
#include<vector>
#include<iostream>
#include<fstream>
#include "XCTask.h"
#include "../XCFont/XCFont.h"
#include "../XCSpecialEffect/XCGameInfoSlot.h"
class XCTaskLoop {
protected:	/*UUID   Task*/
	bool IsReplayMode = false,ShouldProcessRun=true,have_font_show=false,use_ogl_log=false;
	std::fstream taskLog;
	std::multimap<std::string, XCTask*> tasklist;
	std::vector<int> taskCommandList;
	XCTaskCollisionInfo CollisionInfo;
	XCTaskRenderInfo RenderInfo;
	XCTask* pPlayerTask=nullptr,*pEnemyTask=nullptr,*pBulletTask=nullptr,*pBackgroundTask=nullptr;
	XCFont taskFont;
	const float game_width = 720.0f, game_height = 730.f;
	xc_se::XCGameInfoSlot renderGroup;
	void BeforeProcess();
	void SetPlayer(XCTask* ptask);
	void SetEnemy(XCTask* ptask);
	void SetBullet(XCTask* ptask);
	void SetBackground(XCTask* ptask);
	std::map<std::string, XCTask*>::iterator DoTaskCommand(int command,std::map<std::string, XCTask*>::iterator &iter);
	void DoExtraCommand(int command, std::map<std::string, XCTask*>::iterator &iter);
public:
	enum TaskCommand {COMMAND_NONE,CLEAN_ENEMY,CLEAN_BULLET,STAGE_INIT,STAGE_RENDER,STAGE_END, 
		PROCESS_PAUSE, PROCESS_RESUME, PROCESS_CLEAN};
	bool IsProcessing();
	void SetIsReplay(bool isreplay);
	void SetScreen(GLFWwindow* screen);
	void SetWidthHeight(float w,float h);
	/*开启记录后可能会影响性能*/
	void SetUseLog(bool uselog);
	void AddTask(XCTask* task,std::string priority);
	void DeleteTask(std::string uuid);
	void ActiveTask(std::string uuid);
	void TaskProcess(float nowFrame);
	void TaskProcessCommand(int command);
	
};
#endif
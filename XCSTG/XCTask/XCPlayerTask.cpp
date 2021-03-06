#include "XCPlayerTask.h"

XCPlayerTask::XCPlayerTask()
{
	taskType = PlayerType;
	pPlayer = new PlayerEntity;
}

XCPlayerTask::~XCPlayerTask()
{
	delete pPlayer;
}

void XCPlayerTask::TaskInit()
{
	if (!have_resource_init&&have_abs_bored_set)
	{	
		pPlayer->SetBoundingBox(render_abs_height,-render_abs_height,-render_abs_width, render_abs_width);
		pPlayer->GroupInit();
		have_resource_init = true;
	}

}

void XCPlayerTask::TaskRender(XCTaskRenderInfo * pInfo)
{
	pPlayer->GroupRender(pInfo->RenderTimer.getNowFrame());
	
}

void XCPlayerTask::TaskCollisionCheck(XCTaskCollisionInfo * pInfo)
{
	pPlayer->PlayerCollisonEvent(&(pInfo->EnemyInfoGroup));
}

void XCPlayerTask::TaskKeyCheck(GLFWwindow * win)
{

	pPlayer->GroupKeyCheck(win);
}

void XCPlayerTask::TaskRelease()
{
	; 
}

PlayerEntity * XCPlayerTask::GetPlayerPointer()
{
	return pPlayer;
}

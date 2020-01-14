/**
 ***********************************************************************************************************************
 * @file    MAIN.CPP
 * @author  ZhangRan
 * @date    2019/12/24
 * @version 1.0.0
 *
 * <h2><center>&copy; COPYRIGHT 2020 SCHRODER CORPORATION</center></h2>
 *
 ***********************************************************************************************************************
 */

#include "media_player.h"
#include <QtWidgets/QApplication>

#include <BasicUsageEnvironment.hh>

#include "rtsp_client.h"

char eventLoopWatchVariable = 0;

int main(int argc, char* argv[])
{
  // QApplication a(argc, argv);
  // MediaPlayer w;
  // w.show();
  // return a.exec();

  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);

  MyRTSPClient rtsp(*env, "rtsp://192.168.1.64:554/user=admin&password=&channel=12&stream=0.sdp?real_stream", 1, "test",
                    0);

  rtsp.openURL(*env, "test", "");

  env->taskScheduler().doEventLoop(&eventLoopWatchVariable);

  return 0;
}

/**
 ***********************************************************************************************************************
 * @file    RTSP_CLIENT.H
 * @author  ZhangRan
 * @date    2019/12/24
 * @version 1.0.0
 *
 * <h2><center>&copy; COPYRIGHT 2020 SCHRODER CORPORATION</center></h2>
 *
 ***********************************************************************************************************************
 */

#include <NetAddress.hh>
#include <RTSPClient.hh>
#include <MediaSession.hh>
#include <UsageEnvironment.hh>

#ifndef QT_RTSP_CLIENT_H
#define QT_RTSP_CLIENT_H

#include <QVideoWidget>

class VideoWidget : public QVideoWidget
{
  Q_OBJECT

public:
  explicit VideoWidget(QWidget* parent = nullptr);

protected:
  void keyPressEvent(QKeyEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
};

/*
 *----------------------------------------------------------------------------------------------------------------------
 *                                                          
 *----------------------------------------------------------------------------------------------------------------------
 */
class StreamClientState
{
public:
  StreamClientState();
  virtual ~StreamClientState();

public:
  MediaSubsessionIterator* iter;
  MediaSession* session;
  MediaSubsession* subsession;
  TaskToken streamTimerTask;
  double duration;
};

/*
 *----------------------------------------------------------------------------------------------------------------------
 *                                                          
 *----------------------------------------------------------------------------------------------------------------------
 */
class MyRTSPClient : public RTSPClient
{
public:
  static MyRTSPClient* create(UsageEnvironment& env, char const* rtspURL, int verbosityLevel = 0,
                                 char const* applicationName = NULL, portNumBits tunnelOverHTTPPortNum = 0);

protected:
  MyRTSPClient(UsageEnvironment& env, char const* rtspURL, int verbosityLevel, char const* applicationName,
               portNumBits tunnelOverHTTPPortNum);

  // called only by createNew();
  virtual ~MyRTSPClient();

public:
  StreamClientState scs;
};

#endif

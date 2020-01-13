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

/**
 *
 */
class RTSPException : public std::logic_error
{
public:
  RTSPException(const std::string& what) : std::logic_error(what)
  {
  }
};

/*
 *----------------------------------------------------------------------------------------------------------------------
 *
 *----------------------------------------------------------------------------------------------------------------------
 */
class StreamClientState
{
public:
  StreamClientState() : iter(nullptr), session(nullptr), subsession(nullptr), streamTimerTask(nullptr), duration(0.0)
  {
  }

  virtual ~StreamClientState()
  {
  }

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

/**
 * 虚拟槽，用来接收 RTSP 数据
 */
class DummySink : public MediaSink
{
private:
  u_int8_t* fReceiveBuffer;
  MediaSubsession& fSubsession;
  char* fStreamId;

public:
  static DummySink* createNew(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId = NULL);

private:
  /**
   * Ctor
   */
  DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId);

  /**
   * Dtor
   */
  virtual ~DummySink();

  /**
   * Dtor
   */
  static void afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
                                struct timeval presentationTime, unsigned durationInMicroseconds);

  /**
   * Dtor
   */
  void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime,
                         unsigned durationInMicroseconds);

private:
  // redefined virtual functions:
  virtual Boolean continuePlaying();
};

/*
 *----------------------------------------------------------------------------------------------------------------------
 *
 *----------------------------------------------------------------------------------------------------------------------
 */
/**
 * RTSP 客户端
 */
class MyRTSPClient : public RTSPClient
{
private:
  StreamClientState scs;

public:
  /**
   * Ctor
   */
  MyRTSPClient(UsageEnvironment& env, char const* rtspURL, int verbosityLevel, char const* applicationName,
               portNumBits tunnelOverHTTPPortNum);

  /**
   * Dtor
   */
  virtual ~MyRTSPClient();

  /**
   * 打开 URL
   */
  void openURL(UsageEnvironment& env, char const* progName, char const* rtspURL);

private:
  void shutdownStream();
  void initMediaSession(const std::string& description);
  void setupAllSessions();

  void afterDescribeHandler(int resultCode, char* resultString);

  static inline void afterDescribeHandlerWrapper(RTSPClient* rtspClient, int resultCode, char* resultString);

  friend UsageEnvironment& operator<<(UsageEnvironment& env, const RTSPClient& rtspClient);
};

inline void MyRTSPClient::afterDescribeHandlerWrapper(RTSPClient* rtspClient, int resultCode, char* resultString)
{
  static_cast<MyRTSPClient*>(rtspClient)->afterDescribeHandler(resultCode, resultString);
}

#endif

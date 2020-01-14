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

#ifndef QT_RTSP_CLIENT_H
#define QT_RTSP_CLIENT_H

#include <NetAddress.hh>
#include <RTSPClient.hh>
#include <MediaSession.hh>
#include <UsageEnvironment.hh>

#include <stdexcept>

/*
 *----------------------------------------------------------------------------------------------------------------------
 *
 *----------------------------------------------------------------------------------------------------------------------
 */

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
  void setupNextSubsession();
  void initMediaSession(const std::string& description);
  void afterDescribeHandler(int resultCode, char* resultString);
  void continueSetupSubsession(int resultCode, char* resultString);

  static inline void afterPlayHandlerWrapper(RTSPClient* rtspClient, int resultCode, char* resultString);
  static inline void afterDescribeHandlerWrapper(RTSPClient* rtspClient, int resultCode, char* resultString);
  static inline void continueSetupSessionHandlerWrapper(RTSPClient* rtspClient, int resultCode, char* resultString);

  friend UsageEnvironment& operator<<(UsageEnvironment& env, const RTSPClient& rtspClient);
  friend UsageEnvironment& operator<<(UsageEnvironment& env, const MediaSubsession& subsession);
};

inline void MyRTSPClient::afterDescribeHandlerWrapper(RTSPClient* rtspClient, int resultCode, char* resultString)
{
  static_cast<MyRTSPClient*>(rtspClient)->afterDescribeHandler(resultCode, resultString);
}

inline void MyRTSPClient::continueSetupSessionHandlerWrapper(RTSPClient* rtspClient, int resultCode, char* resultString)
{
  static_cast<MyRTSPClient*>(rtspClient)->continueSetupSubsession(resultCode, resultString);
}

inline void MyRTSPClient::afterPlayHandlerWrapper(RTSPClient* rtspClient, int resultCode, char* resultString)
{

}

#endif

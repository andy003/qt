/**
 ***********************************************************************************************************************
 * @file    RTSP_CLIENT.CPP
 * @author  ZhangRan
 * @date    2019/12/26
 * @version 1.0.0
 *
 * <h2><center>&copy; COPYRIGHT 2020 SCHRODER CORPORATION</center></h2>
 *
 ***********************************************************************************************************************
 */

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "rtsp_client.h"
#include "dummy_sink.h"

void byeHandler(void* arg)
{
}

void playCB(void* clientData)
{
}

void closeSubsessionHandler(void* clientData)
{
  MediaSubsession* subsession = (MediaSubsession*)clientData;
  RTSPClient* rtspClient = (RTSPClient*)(subsession->miscPtr);

  // Begin by closing this subsession's stream:
  Medium::close(subsession->sink);
  subsession->sink = NULL;

  // Next, check whether *all* subsessions' streams have now been closed:
  MediaSession& session = subsession->parentSession();
  MediaSubsessionIterator iter(session);
  while ((subsession = iter.next()) != NULL)
  {
    if (subsession->sink != NULL)
      return;  // this subsession is still active
  }

  // All subsessions' streams have now been closed, so shutdown the client:
  //shutdownStream(rtspClient);
}


/*
 *----------------------------------------------------------------------------------------------------------------------
 *
 *----------------------------------------------------------------------------------------------------------------------
 */
MyRTSPClient::MyRTSPClient(UsageEnvironment& env, char const* rtspURL, int verbosityLevel, char const* applicationName,
                           portNumBits tunnelOverHTTPPortNum)
  : RTSPClient(env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1)
{
}

MyRTSPClient::~MyRTSPClient()
{
}

UsageEnvironment& operator<<(UsageEnvironment& env, const RTSPClient& rtspClient)
{
  return env << "[URL:\"" << rtspClient.url() << "\"]: ";
}

UsageEnvironment& operator<<(UsageEnvironment& env, const MediaSubsession& subsession)
{
  return env << subsession.mediumName() << "/" << subsession.codecName();
}

void MyRTSPClient::openURL(UsageEnvironment& env, char const* progName, char const* rtspURL)
{
  sendDescribeCommand(afterDescribeHandlerWrapper);
}

void MyRTSPClient::afterDescribeHandler(int resultCode, char* resultString)
{
  try
  {
    if (resultCode != 0)
    {
      envir() << *this << "Failed to get a SDP description: " << resultString << "\n";
      throw RTSPException(envir().getResultMsg());
    }

    // 根据 SDP 初始化多媒体会话
    initMediaSession(resultString);

    // 设置下个会话
    setupNextSubsession();
  }
  catch (const RTSPException& err)
  {
    shutdownStream();
    printf("%s\n", err.what());
  }

  delete[] resultString;
}

void MyRTSPClient::setupNextSubsession()
{
  UsageEnvironment& env = envir();  // alias

  scs.subsession = scs.iter->next();
  if (scs.subsession != nullptr)
  {
    if (!scs.subsession->initiate())
    {
      env << *this << "Failed to initiate the \"" << *scs.subsession << "\" subsession: " << env.getResultMsg() << "\n";
      setupNextSubsession();
    }

    env << *this << "Initiated the \"" << *scs.subsession << "\" subsession (";
    if (scs.subsession->rtcpIsMuxed())
    {
      env << "client port " << scs.subsession->clientPortNum();
    }
    else
    {
      env << "client ports " << scs.subsession->clientPortNum() << "-" << scs.subsession->clientPortNum() + 1;
    }
    env << ")\n";

    // Continue setting up this subsession, by sending a RTSP "SETUP" command:
    sendSetupCommand(*scs.subsession, continueSetupSessionHandlerWrapper, False, True);
  }
  else
  {
    // We've finished setting up all of the subsessions.  Now, send a RTSP "PLAY" command to start the streaming:
    if (scs.session->absStartTime() != NULL)
    {
      // Special case: The stream is indexed by 'absolute' time, so send an appropriate "PLAY" command:
      sendPlayCommand(*scs.session, afterPlayHandlerWrapper, scs.session->absStartTime(), scs.session->absEndTime());
    }
    else
    {
      scs.duration = scs.session->playEndTime() - scs.session->playStartTime();
      sendPlayCommand(*scs.session, afterPlayHandlerWrapper);
    }
  }
}

void MyRTSPClient::continueSetupSubsession(int resultCode, char* resultString)
{
  try
  {
    UsageEnvironment& env = envir();  // alias

    if (resultCode != 0)
    {
      env << *this << "Failed to set up the \"" << *scs.subsession << "\" subsession: " << resultString << "\n";
      throw RTSPException(env.getResultMsg());
    }

    env << *this << "Set up the \"" << *scs.subsession << "\" subsession (";
    if (scs.subsession->rtcpIsMuxed())
    {
      env << "client port " << scs.subsession->clientPortNum();
    }
    else
    {
      env << "client ports " << scs.subsession->clientPortNum() << "-" << scs.subsession->clientPortNum() + 1;
    }
    env << ")\n";

    // Having successfully setup the subsession, create a data sink for it, and call "startPlaying()" on it.
    // (This will prepare the data sink to receive data; the actual flow of data from the client won't start happening
    // until later, after we've sent a RTSP "PLAY" command.)

    scs.subsession->sink = DummySink::createNew(env, *scs.subsession, url());
    // perhaps use your own custom "MediaSink" subclass instead
    if (scs.subsession->sink == NULL)
    {
      env << *this << "Failed to create a data sink for the \"" << *scs.subsession
          << "\" subsession: " << env.getResultMsg() << "\n";
      throw RTSPException(env.getResultMsg());
    }

    env << *this << "Created a data sink for the \"" << *scs.subsession << "\" subsession\n";
    // a hack to let subsession handler functions get the "RTSPClient" from the subsession
    scs.subsession->miscPtr = this;

    scs.subsession->sink->startPlaying(*(scs.subsession->readSource()), playCB, scs.subsession);

    // Also set a handler to be called if a RTCP "BYE" arrives for this subsession:
    if (scs.subsession->rtcpInstance() != NULL)
    {
      scs.subsession->rtcpInstance()->setByeHandler(byeHandler, scs.subsession);
    }
  }
  catch (const RTSPException& err)
  {
    envir() << err.what();
  }

  delete[] resultString;
  setupNextSubsession();
}

void MyRTSPClient::initMediaSession(const std::string& description)
{
  UsageEnvironment& env = envir();

  env << *this << "Got a SDP description:\n" << description.c_str() << "\n";

  // Create a media session object from this SDP description:
  scs.session = MediaSession::createNew(env, description.c_str());
  if (scs.session == NULL)
  {
    env << *this << "Failed to create a MediaSession object from the SDP description: " << env.getResultMsg() << "\n";
    throw RTSPException(env.getResultMsg());
  }
  else if (!scs.session->hasSubsessions())
  {
    env << *this << "This session has no media subsessions (i.e., no \"m=\" lines)\n";
    throw RTSPException(env.getResultMsg());
  }

  // Then, create and set up our data source objects for the session.  We do this by iterating over the session's
  // 'subsessions', calling "MediaSubsession::initiate()", and then sending a RTSP "SETUP" command, on each one. (Each
  // 'subsession' will have its own data source.)
  scs.iter = new MediaSubsessionIterator(*scs.session);
}

void MyRTSPClient::shutdownStream()
{
  UsageEnvironment& env = envir();  // alias

  // First, check whether any subsessions have still to be closed:
  if (scs.session != NULL)
  {
    Boolean someSubsessionsWereActive = False;
    MediaSubsessionIterator iter(*scs.session);
    MediaSubsession* subsession;

    while ((subsession = iter.next()) != NULL)
    {
      if (subsession->sink != NULL)
      {
        Medium::close(subsession->sink);
        subsession->sink = NULL;

        if (subsession->rtcpInstance() != NULL)
        {
          subsession->rtcpInstance()->setByeHandler(NULL, NULL);  // in case the server sends a RTCP "BYE" while
                                                                  // handling "TEARDOWN"
        }

        someSubsessionsWereActive = True;
      }
    }

    if (someSubsessionsWereActive)
    {
      // Send a RTSP "TEARDOWN" command, to tell the server to shutdown the stream.
      // Don't bother handling the response to the "TEARDOWN".
      sendTeardownCommand(*scs.session, NULL);
    }
  }

  env << *this << "Closing the stream.\n";
  Medium::close(this);
}

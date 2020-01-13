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

    // 设置所有会话
    setupAllSessions();

    delete[] resultString;
  }
  catch (const RTSPException& err)
  {
    delete[] resultString;
    shutdownStream();
  }
}

void MyRTSPClient::setupAllSessions()
{
  UsageEnvironment& env = envir();  // alias

  scs.subsession = scs.iter->next();
  while (scs.subsession != nullptr)
  {
    if (!scs.subsession->initiate())
    {
      env << *this << "Failed to initiate the \"" << *scs.subsession << "\" subsession: " << env.getResultMsg()
          << "\n";
      continue;
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
    sendSetupCommand(*scs.subsession, continueAfterSETUP, False, True);
  }

  // We've finished setting up all of the subsessions.  Now, send a RTSP "PLAY" command to start the streaming:
  if (scs.session->absStartTime() != NULL)
  {
    // Special case: The stream is indexed by 'absolute' time, so send an appropriate "PLAY" command:
    sendPlayCommand(*scs.session, continueAfterPLAY, scs.session->absStartTime(),
                                scs.session->absEndTime());
  }
  else
  {
    scs.duration = scs.session->playEndTime() - scs.session->playStartTime();
    sendPlayCommand(*scs.session, continueAfterPLAY);
  }
}

void MyRTSPClient::initMediaSession(const std::string& description)
{
  BasicUsageEnvironment& env = envir();

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

UsageEnvironment& operator<<(UsageEnvironment& env, const RTSPClient& rtspClient)
{
  return env << "[URL:\"" << rtspClient.url() << "\"]: ";
}

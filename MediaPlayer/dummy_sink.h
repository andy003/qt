/**
 ***********************************************************************************************************************
 * @file    DUMMY_SINK.H
 * @author  ZhangRan
 * @date    2020/01/13
 * @version 1.0.0
 *
 * <h2><center>&copy; COPYRIGHT 2020 SCHRODER CORPORATION</center></h2>
 *
 ***********************************************************************************************************************
 */

#ifndef QT_DUMMY_SINK_H
#define QT_DUMMY_SINK_H

#include <MediaSink.hh>
#include <MediaSession.hh>

/**
 * 虚拟槽，用来接收 RTSP 数据
 */
class DummySink : public MediaSink
{
private:
  u_int8_t* fReceiveBuffer;
  MediaSubsession& fSubsession;
  char* fStreamId;
  static constexpr uint32_t MAX_BUF_SIZE = 1 * 1024 * 1024;

public:
  static DummySink* createNew(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId = NULL)
  {
    return new DummySink(env, subsession, streamId);
  }

protected:
  /**
   * Ctor
   */
  DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId)
    : MediaSink(env), fSubsession(subsession), fStreamId(fStreamId)
  {
    fReceiveBuffer = new uint8_t[MAX_BUF_SIZE];
  }

  /**
   * Dtor
   */
  virtual ~DummySink()
  {
  }

private:
  /**
   * Dtor
   */
  static void afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
                                struct timeval presentationTime, unsigned durationInMicroseconds)
  {
    DummySink* sink = (DummySink*)clientData;
    sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
  }

  /**
   * Dtor
   */
  void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime,
                         unsigned durationInMicroseconds)
  {
    // We've just received a frame of data.  (Optionally) print out information about it:
    if (fStreamId != NULL)
      envir() << "Stream \"" << fStreamId << "\"; ";
    envir() << fSubsession.mediumName() << "/" << fSubsession.codecName() << ":\tReceived " << frameSize << " bytes";
    if (numTruncatedBytes > 0)
      envir() << " (with " << numTruncatedBytes << " bytes truncated)";
    char uSecsStr[6 + 1];  // used to output the 'microseconds' part of the presentation time
    sprintf(uSecsStr, "%06u", (unsigned)presentationTime.tv_usec);
    envir() << ".\tPresentation time: " << (int)presentationTime.tv_sec << "." << uSecsStr;
    if (fSubsession.rtpSource() != NULL && !fSubsession.rtpSource()->hasBeenSynchronizedUsingRTCP())
    {
      envir() << "!";  // mark the debugging output to indicate that this presentation time is not RTCP-synchronized
    }
#ifdef DEBUG_PRINT_NPT
    envir() << "\tNPT: " << fSubsession.getNormalPlayTime(presentationTime);
#endif
    envir() << "\n";

    // Then continue, to request the next frame of data:
    continuePlaying();
  }

  virtual Boolean continuePlaying() override
  {
    if (fSource == NULL)
      return False;  // sanity check (should not happen)

    fSource->getNextFrame(fReceiveBuffer, MAX_BUF_SIZE, afterGettingFrame, this, onSourceClosure, this);
    return True;
  }
};

#endif

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

#include <assert.h>
#include <MediaSink.hh>
#include <MediaSession.hh>

/**
 * 虚拟槽，用来接收 RTSP 数据
 */
class DummySink : public MediaSink
{
private:
  char* stream_;
  uint8_t* recv_buffer_;
  MediaSubsession& subsession_;
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
    : MediaSink(env), subsession_(subsession)
  {
    stream_ = _strdup(streamId);
    recv_buffer_ = new uint8_t[MAX_BUF_SIZE];
  }

  /**
   * Dtor
   */
  virtual ~DummySink()
  {
    free(stream_);
    delete[] recv_buffer_;
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
    printf("\nstart\n");
    printf("%d\n", frameSize);
    printf("%d\n", numTruncatedBytes);
    printf("%d\n", durationInMicroseconds);

    continuePlaying();
  }

  virtual Boolean continuePlaying() override
  {
    assert(fSource);

    fSource->getNextFrame(recv_buffer_, MAX_BUF_SIZE, afterGettingFrame, this, onSourceClosure, this);
    return True;
  }
};

#endif

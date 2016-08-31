#pragma once

#include <LoopbackStream.h>

class PipedStream : public Stream {
  LoopbackStream& in;
  LoopbackStream& out;
public:
  PipedStream(LoopbackStream& in, LoopbackStream& out);
  
  virtual size_t write(uint8_t);
  virtual int availableForWrite(void);
  
  virtual int available();
  virtual int read();
  virtual int peek();
  virtual void flush();
};

class PipedStreamPair {
private:
  LoopbackStream buffer1, buffer2;
public:
  PipedStreamPair(uint16_t buffer_size = LoopbackStream::DEFAULT_SIZE);
  PipedStream first, second;
};

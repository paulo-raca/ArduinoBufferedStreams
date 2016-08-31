#pragma once

#include <Stream.h>

class LoopbackStream : public Stream {
  uint8_t *buffer;
  uint16_t buffer_size;
  uint16_t pos, size;
public:
  static const uint16_t DEFAULT_SIZE = 64;
  
  LoopbackStream(uint16_t buffer_size = LoopbackStream::DEFAULT_SIZE);
  ~LoopbackStream();
  
  virtual size_t write(uint8_t);
  virtual int availableForWrite(void);
  
  virtual int available();
  virtual int read();
  virtual int peek();
  virtual void flush();
};
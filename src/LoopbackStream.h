#pragma once

#include <Stream.h>

class LoopbackStream : public Stream {
  uint8_t *buffer;
  uint16_t buffer_len;
  uint16_t pos, len;
public:
  LoopbackStream(uint16_t buffer_len = 64);
  ~LoopbackStream();
  
  virtual size_t write(uint8_t);
  virtual int availableForWrite(void);
  
  virtual int available();
  virtual int read();
  virtual int peek();
  virtual void flush();
};
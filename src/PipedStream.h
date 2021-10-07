#pragma once

#include "LoopbackStream.h"

/*
 * PipedStreams are created in pairs, and all data written to one of the streams can be read on the other
 * 
 * If any of the internal buffers overflows, the last bytes written are lost.
 *
 * Alt use: pass out_stream to constructor, and data will be written if the buffer is filled, or on flush()
 * Use out_stream and buffer_size=1 to bypass buffering completely (for first PipedStream of pair)
 * Example use case: connect object that uses a single Stream to take input from one source, and output to another, without overflowing output buffer
 *
 * Use fillFromStream(in_stream) in loop() to write as much data as available from Stream into buffer
 * 
 * It can be used as a Serial or Socket-like layer between components.
 */

class PipedStream : public Stream {
  LoopbackStream& in;
  LoopbackStream& out;
public:
  PipedStream(LoopbackStream& in, LoopbackStream& out);

  /** Clear the buffers */
  void clear(); 

  virtual size_t write(uint8_t v);
  using Print::write; // pull in write(str) and write(buf, size) from Print
  virtual int availableForWrite(void);
  
  virtual int available();
  virtual int read();
  virtual int peek();
  virtual void flush();
  virtual size_t fillFromStream(Stream &in_stream);
};

class PipedStreamPair {
private:
  LoopbackStream buffer1, buffer2;
public:
  PipedStreamPair(uint16_t buffer_size = LoopbackStream::DEFAULT_SIZE);
  PipedStreamPair(Stream &out_stream1, uint16_t buffer_size2 = LoopbackStream::DEFAULT_SIZE, uint16_t buffer_size1 = 1);
  PipedStream first, second;
};

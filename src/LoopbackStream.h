#pragma once

#include <Stream.h>

/*
 * A LoopbackStream stores all data written in an internal buffer and returns it back when the stream is read.
 * 
 * If the buffer overflows, the last bytes written are lost.
 * 
 * Alt use: pass out_stream to constructor, and data will be written if the buffer is filled, or on flush()
 * Use out_stream and buffer_size=1 to bypass buffering completely
 * Example use case: connect object that uses a single Stream to take input from one source, and output to another, without overflowing output buffer
 *
 * Use fillFromStream(in_stream) in loop() to write as much data as available from Stream into buffer
 * 
 * It can be used as a buffering layer between components.
 */
class LoopbackStream : public Stream {
  uint8_t *buffer;
  uint16_t buffer_size;
  uint16_t pos, size;
  Stream * out_stream;
public:
  static const uint16_t DEFAULT_SIZE = 64;
  
  LoopbackStream(uint16_t buffer_size = LoopbackStream::DEFAULT_SIZE, Stream *out_stream = NULL);
  LoopbackStream(Stream &out_stream, uint16_t buffer_size = 1);
  ~LoopbackStream();
    
  /** Clear the buffer */
  void clear(); 
  
  virtual size_t write(uint8_t v);
  using Print::write; // pull in write(str) and write(buf, size) from Print
  virtual int availableForWrite(void);
  
  virtual int available();
  virtual bool contains(char);
  virtual int read();
  virtual int peek();
  virtual void flush();
  virtual void flushAvailableForWrite(bool flushOutStream = true);
  virtual size_t fillFromStream(Stream &in_stream);
};

// LoopbackStreamStats adds the ability to count characters written to the buffer
class LoopbackStreamStats : public LoopbackStream {
  public:
    LoopbackStreamStats(uint16_t buffer_size = LoopbackStream::DEFAULT_SIZE, Stream *out_stream = NULL) :
    LoopbackStream(buffer_size, out_stream) {
      this->writeCount = 0;
    }

    LoopbackStreamStats(Stream &out_stream, uint16_t buffer_size = 1) :
    LoopbackStream(out_stream, buffer_size) { 
      this->writeCount = 0;
    }

    void clearWriteCount() { writeCount = 0; }

    size_t getWriteCount() { return writeCount; }

    virtual size_t write(uint8_t v) {
      writeCount++;
      return LoopbackStream::write(v);
    }

  private:
    size_t writeCount;
};

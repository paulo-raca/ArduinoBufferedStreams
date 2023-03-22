#include "PipedStream.h"

PipedStream::PipedStream(LoopbackStream& in, LoopbackStream& out) 
: in(in), out(out) 
{ }

void PipedStream::clear() {
  in.clear();
  out.clear();
}

int PipedStream::read() {
  return in.read();
}

size_t PipedStream::write(uint8_t v) {
  return out.write(v);
}

int PipedStream::available() {
  return in.available();
}

int PipedStream::availableForWrite() {
  return out.availableForWrite();
}

int PipedStream::peek() {
  return in.peek();
}

void PipedStream::flush() {
  out.flush();
}

void PipedStream::flushAvailableForWrite(bool flushOutStream) {
  out.flushAvailableForWrite(flushOutStream);
}

size_t PipedStream::fillFromStream(Stream &in_stream) {
  return out.fillFromStream(in_stream);
}

PipedStreamPair::PipedStreamPair(uint16_t buffer_size)
: buffer1(buffer_size),
  buffer2(buffer_size),
  first(buffer1, buffer2),
  second(buffer2, buffer1)
{ }

PipedStreamPair::PipedStreamPair(Stream &out_stream1,
                        uint16_t buffer_size2,
                        uint16_t buffer_size1)
: buffer1(out_stream1, buffer_size1),
  buffer2(buffer_size2),
  first(buffer1, buffer2),
  second(buffer2, buffer1)
{ }


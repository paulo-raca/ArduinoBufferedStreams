#include "LoopbackStream.h"

LoopbackStream::LoopbackStream(uint16_t buffer_size, Stream *out_stream) {
  this->buffer = (uint8_t*) malloc(buffer_size);
  this->buffer_size = buffer_size;
  this->pos = 0;
  this->size = 0;
  this->out_stream = out_stream;
}

LoopbackStream::LoopbackStream(Stream &out_stream, uint16_t buffer_size) : LoopbackStream(buffer_size, &out_stream) { }

LoopbackStream::~LoopbackStream() {
  free(buffer);
}

void LoopbackStream::clear() {
  this->pos = 0;
  this->size = 0;
}

int LoopbackStream::read() {
  if (size == 0) {
    return -1;
  } else {
    int ret = buffer[pos];
    pos++;
    size--;
    if (pos == buffer_size) {
      pos = 0;
    }
    return ret;
  }
}

size_t LoopbackStream::write(uint8_t v) {
  if (size == buffer_size) {
    return 0;
  } else if (buffer_size == 1 && out_stream) {
    out_stream->write(v);  // passthrough use case, no temporary storage in buffer
    return 1;
  } else {
    int p = pos + size;
    if (p >= buffer_size) {
      p -= buffer_size;
    }
    buffer[p] = v;
    size++;
    if (out_stream && !(buffer_size - size)) // when the buffer is full, push data to out_stream to make space
      out_stream->write(read());
    return 1;
  }  
}

int LoopbackStream::available() {
  return size;
}

int LoopbackStream::availableForWrite() {
  return buffer_size - size;
}

bool LoopbackStream::contains(char ch) {
  for (int i=0; i<size; i++){
    int p = (pos + i) % buffer_size;
    if (buffer[p] == ch) {
      return true;
    }
  }
  return false;
}

int LoopbackStream::peek() {
  return size == 0 ? -1 : buffer[pos];
}

void LoopbackStream::flush() {
  if(!out_stream) return; // flush can't do anything without an out_stream to push data to and flush
  while(available())
    out_stream->write(read());
  out_stream->flush();
}

// an optionally non-blocking version of flush() - flushOutStream == false shouldn't block, and flushOutStream == true only blocks if out_stream->flush() blocks
void LoopbackStream::flushAvailableForWrite(bool flushOutStream) {
  if(!out_stream) return; // flushAvailableForWrite can't do anything without an out_stream to push data to and flush
  while(available() && out_stream->availableForWrite())
    out_stream->write(read());
  if(flushOutStream) out_stream->flush();
}

size_t LoopbackStream::fillFromStream(Stream &in_stream) {
  size_t count=0;
  while(in_stream.available() && availableForWrite()) {
    write(in_stream.read());
    count++;
  }
  return count;
}

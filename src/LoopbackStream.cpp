#include "LoopbackStream.h"

LoopbackStream::LoopbackStream(uint16_t buffer_len) {
  this->buffer = malloc(buffer_len);
  this->buffer_len = buffer_len;
  this->pos = 0;
  this->len = 0;
}
LoopbackStream::~LoopbackStream() {
  free(buffer);
}



int LoopbackStream::read() {
  int ret = buffer[pos];
  pos++;
  len--;
  if (pos == buffer_len) {
    pos = 0;
  }
  return ret;
}

size_t LoopbackStream::write(uint8_t v) {
  if (len == buffer_len) {
    return 0;
  } else {
    int p = pos + len;
    if (p >= buffer_len) {
      p -= buffer_len;
    }
    buffer[p] = v;
    len++;
    return 1;
  }  
}

int LoopbackStream::available() {
  return len;
}

int LoopbackStream::availableForWrite() {
  return buffer_len - len;
}

int LoopbackStream::peek() {
  return len == 0 ? -1 : buffer[pos];
}

void LoopbackStream::flush() {
  len = 0;
  pos = 0;
}


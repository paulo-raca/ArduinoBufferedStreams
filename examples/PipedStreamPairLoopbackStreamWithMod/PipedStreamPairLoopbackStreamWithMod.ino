/*
  This example shows how to take a Stream and modify the data before connecting it to a method that only acts on a Stream and not a data buffer
  This is a contrived example to be self-contained and simple, but a practical application was connecting the XYmodem library
  (which only acts on a Stream) through a commandline library that needs to read and parse data from Serial,
  and only send some of that data through to the XYmodem library

                       Stream Data Flow
  Serial -> (optional modification) -> streamSerialSide -> processStream()
  Serial <---------------------------- streamSerialSide <--------|
*/

#include <PipedStream.h>

// this creates two PipedStreams, and all data written to one of the streams can be read on the other
PipedStreamPair pipes(Serial, 4096);
// streamSerialSide has a 4096 byte input buffer, and is setup to output all its data to Serial when the buffer is full or flushed (and the output buffer size is default 1 byte, so any data will fill the buffer and be immediately written to Serial)
PipedStream& streamSerialSide = pipes.first;
// streamProcessSide has a 4096 byte output buffer, and is where data can be written to automatically write back to Serial
PipedStream& streamProcessSide = pipes.second;

// for this contrived example, pretend this function acting on a Stream is in a library and can't be modified
void processStream(Stream &stream) {
  while(stream.available())
    stream.write(stream.read());
}

void swapCase(Stream &inputStream, Stream &outputStream) {
  while(inputStream.available()) {
    char inputchar = inputStream.read();

    if(inputchar >= 'A' && inputchar <= 'Z')
      inputchar -= ('A' - 'a');
    else if(inputchar >= 'a' && inputchar <= 'z')
      inputchar -= ('a' - 'A');

    outputStream.write(inputchar); 
  }
}

void setup() {
  Serial.begin(9600);
  delay(100);
  streamProcessSide.print("Hello, World"); // data printed to "streamProcessSide" is output on "streamSerialSide", which is automatically output to Serial
}

void loop() {
  // half the time, send all chars from Serial straight through to streamSerialSide, where they'll be consumed by processStream()
  // the other half of the time, call swapCase, which modifieds some characters from Serial before writing to streamSerialSide
  if(millis()%2000 > 1000)  
    streamSerialSide.fillFromStream(Serial);
  else
    swapCase(Serial, streamSerialSide);

  processStream(streamProcessSide);
}

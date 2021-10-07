/* 
  This example shows the out_stream functionality connecting PipedStream to automatically output to a Stream

  The example only shows the advantage of using the PipedStream buffer if SERIAL_PORT is set to a Stream that doesn't use flow control
  (e.g. on a Teensy, use a hardware serial port and not a USB serial port)

  You can paste this sample text into a terminal window (make sure there's no flow control or pacing based on echo enabled),
  and with USE_PIPED_STREAM_BUFFER == 0 you should see dropped characters (do a diff on the echo'd output) because of the delay(50) in loop()
  but with USE_PIPED_STREAM_BUFFER == 1 you should see the echo

About 4034 chars:
1. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum feugiat nisl vitae egestas iaculis. Nam felis eros, venenatis gravida varius eu, viverra eu purus. Vestibulum et est aliquet, dignissim nunc non, rutrum risus. Cras et consequat neque. Praesent maximus venenatis nulla, at posuere leo. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Proin euismod euismod gravida. Morbi nec convallis turpis, nec euismod magna. Integer convallis, nulla ac imperdiet sodales, eros elit rhoncus diam, id egestas nunc metus et massa. Integer accumsan magna quis suscipit euismod.

2. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. In lobortis feugiat mauris in consequat. Duis tempor a urna in blandit. Phasellus malesuada sapien dolor, varius viverra diam condimentum ut. Praesent augue velit, congue vel nisl ac, maximus sodales neque. Nullam lectus orci, facilisis sed commodo ut, convallis vel ante. Pellentesque auctor feugiat risus ac imperdiet. In vel lectus nec nunc molestie posuere. Suspendisse pharetra molestie mattis. Curabitur laoreet odio ac porta elementum. Phasellus in turpis at justo consequat viverra. Sed mattis egestas odio. In convallis a nisl nec imperdiet. Duis facilisis mattis erat.

3. Morbi vitae est at mi congue aliquam in nec nulla. Vivamus vel mi consectetur, porttitor dui id, consequat est. Duis interdum hendrerit enim, at tincidunt urna consequat ut. Sed cursus maximus ex, sit amet euismod sem pretium eget. Proin lacinia mollis augue cursus vulputate. Fusce posuere enim sed mi egestas blandit. Donec eleifend elit eu condimentum commodo. Vestibulum mollis elit nec justo eleifend, vel convallis mauris porta. Sed tellus risus, feugiat eget elementum at, lacinia vitae nibh. Proin ornare dui eget orci vulputate, facilisis pellentesque purus eleifend. Nam sagittis, risus eget mollis suscipit, ipsum purus gravida massa, et finibus purus ipsum in diam. Sed sollicitudin rutrum iaculis. Vivamus ut risus lectus. Nunc sollicitudin arcu eget lectus pulvinar, at suscipit nunc aliquam.

4. Integer sed suscipit elit, sed bibendum augue. Sed dolor metus, vulputate dignissim diam at, aliquet elementum justo. Nullam viverra augue eget orci sagittis, vitae dapibus tortor scelerisque. Donec sed mauris ut tortor accumsan semper eget sed tortor. Nam porttitor ligula a volutpat hendrerit. Maecenas tempus velit nec erat mollis, sit amet fringilla arcu dapibus. Nunc at orci finibus, molestie quam et, malesuada quam. Duis interdum tortor eget diam volutpat, sed posuere sem dictum. Quisque pulvinar felis non blandit eleifend. Donec semper tellus ut nunc consectetur, quis convallis urna blandit. Fusce quis dapibus massa. In sit amet accumsan mauris.

5. Fusce dui nunc, accumsan faucibus laoreet nec, blandit in enim. Donec sed mauris vel sapien suscipit consequat. Nunc sagittis risus nulla, sed efficitur mauris viverra at. Sed sodales, velit vel vulputate ultrices, erat mauris venenatis lorem, vel malesuada neque sapien ac risus. Sed ut ultrices elit. Aliquam eu arcu hendrerit, accumsan turpis nec, vestibulum magna. Nullam pellentesque eu risus vitae pharetra. Fusce iaculis nisl quis nulla rutrum, ut egestas purus dictum. Vivamus mattis felis facilisis lectus tempus, eget viverra mi lacinia. Duis vitae ornare erat, at ultrices nisi. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Sed nec elementum eros, ut semper lacus. Nullam congue consectetur orci at pretium.

6. Ut quis tellus massa. Duis posuere lacus libero. Duis id eros non massa faucibus pulvinar. Donec iaculis eget felis nec finibus. Proin scelerisque mauris vel massa porta ultrices. Vestibulum rutrum sapien quis lacus tempor, eu aliquet velit tristique. Praesent tincidunt augue ullamcorper ex finibus imperdiet. Nulla eu tellus iaculis, venenatis velit rhoncus, posuere lacus.

7. Maecenas ornare hendrerit facilisis. Pellentesque consectetur velit vitae leo convallis efficitur. Mauris sapien.
*/

#include <PipedStream.h>

#define SERIAL_PORT Serial1

#define USE_PIPED_STREAM_BUFFER 1

// pipes automatically outputs data written to pipes.second to SERIAL_PORT, and has a 4096 byte buffer on pipes.second
PipedStreamPair pipes(SERIAL_PORT, 4096);
// pipes.first automatically outputs the data written to pipes.second to SERIAL_PORT, so doesn't need to be used directly in this example
PipedStream& streamSerialOut = pipes.first;
// pipes.second can be used to print data to SERIAL_PORT, with a 4096 byte buffer between
PipedStream& streamSerialIn = pipes.second;

void setup() {
  SERIAL_PORT.begin(9600); // purposely set to a slow baud rate for this example
  delay(100);

  streamSerialIn.println("Hello, World");
}

void loop() {
  #if (USE_PIPED_STREAM_BUFFER == 1)
    // this looks for any available data on SERIAL_PORT, and writes it into streamSerialIn, taking advantage of the 4096 byte buffer on pipes.second
    streamSerialIn.fillFromStream(SERIAL_PORT);
  #else
    // this code runs using on the buffers available to SERIAL_PORT, which are likely to lose characters over time because of the delay below
    while(SERIAL_PORT.available() && SERIAL_PORT.availableForWrite())
      SERIAL_PORT.write(SERIAL_PORT.read());
  #endif

  delay(50); // this long delay simulates other code running in the loop between any SERIAL_PORT read/writes
}

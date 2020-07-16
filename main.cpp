/* mbed TM1638 Test program, for TM1638 LED controller
 * Copyright (c) 2015, v01: WH, Initial version
 *               2016, v02: WH, Added ASCII alphabet test
 *               2016, v03: WH, Added QYF-TM1638 and LKM1638, refactoring of
 * writeData()
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "TM1638.h"
#include "mbed.h"
static BufferedSerial pc(USBTX, USBRX, 115200);

//                          01234567
const char NATO[][10] = {
    {"Alpha   "}, {"Bravo   "}, {"Charlie "}, {"Delta   "},
    {"Echo    "}, {"Foxtrot "}, {"Golf    "}, {"Hotel   "},
    {"India   "}, {"Juliet  "}, {"Kilo    "}, {"Lima    "},
    {"Mike    "}, {"November"}, {"Oscar   "}, {"Papa    "},
    {"Quebec  "}, {"Romeo   "}, {"Sierra  "}, {"Tango   "},
    {"Uniform "}, {"Victor  "}, {"Whiskey "}, {"X-ray   "},
    {"Yankee  "}, {"Zulu    "}};

#if (LEDKEY8_TEST == 1)
// LEDKEY8 TM1638 Test
#include "Font_7Seg.h"

DigitalOut myled(LED1);

// DisplayData_t size is 16 bytes (8 Grids @ 10 Segments)
TM1638::DisplayData_t all_str = {0xFF, 0x03, 0xFF, 0x03, 0xFF, 0x03,
                                 0xFF, 0x03, 0xFF, 0x03, 0xFF, 0x03,
                                 0xFF, 0x03, 0xFF, 0x03};
TM1638::DisplayData_t cls_str = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00};
TM1638::DisplayData_t hello_str = {C7_H, 0x00, C7_E, 0x00, C7_L, 0x00,
                                   C7_L, 0x00, C7_O, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00};
TM1638::DisplayData_t bye_str = {C7_B, 0x00, C7_Y, 0x00, C7_E, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00};

TM1638::DisplayData_t animate[] = {
    {S7_A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, S7_A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, S7_A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, S7_A, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, S7_A, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, S7_A, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     S7_A, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, S7_A, 0x00},

    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, S7_B, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, S7_C, 0x00},

    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, S7_D, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     S7_D, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, S7_D, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, S7_D, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, S7_D, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0x00, S7_D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, S7_D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {S7_D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},

    {S7_E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
    {S7_F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00},
};
// KeyData_t size is 4 bytes
TM1638::KeyData_t keydata;

// TM1638_LEDKEY8 declaration (mosi, miso, sclk, cs SPI bus pins)
TM1638_LEDKEY8 LEDKEY8(D11, D12, D13, D10);

char cmd0, bits;
char displayBuffer[40] = "Hello World";
uint8_t displayPos = 0;
char currentDisplay[48] = "";
DigitalOut blueLED(LED2);
void fancy_clear()
{     
      float delay = 0.1;
      // Icons on
      LEDKEY8.setIcon(TM1638_LEDKEY8::LD1);
      LEDKEY8.setIcon(TM1638_LEDKEY8::DP1);
      ThisThread::sleep_for(100ms);
      LEDKEY8.setIcon(TM1638_LEDKEY8::LD2);
      LEDKEY8.setIcon(TM1638_LEDKEY8::DP2);
      ThisThread::sleep_for(100ms);
      LEDKEY8.setIcon(TM1638_LEDKEY8::LD3);
      LEDKEY8.setIcon(TM1638_LEDKEY8::DP3);
      ThisThread::sleep_for(100ms);
      LEDKEY8.setIcon(TM1638_LEDKEY8::LD4);
      LEDKEY8.setIcon(TM1638_LEDKEY8::DP4);
      ThisThread::sleep_for(100ms);
      LEDKEY8.setIcon(TM1638_LEDKEY8::LD5);
      LEDKEY8.setIcon(TM1638_LEDKEY8::DP5);
      ThisThread::sleep_for(100ms);
      LEDKEY8.setIcon(TM1638_LEDKEY8::LD6);
      LEDKEY8.setIcon(TM1638_LEDKEY8::DP6);
      ThisThread::sleep_for(100ms);
      LEDKEY8.setIcon(TM1638_LEDKEY8::LD7);
      LEDKEY8.setIcon(TM1638_LEDKEY8::DP7);
      ThisThread::sleep_for(100ms);
      LEDKEY8.setIcon(TM1638_LEDKEY8::LD8);
      LEDKEY8.setIcon(TM1638_LEDKEY8::DP8);
      ThisThread::sleep_for(100ms);

      sprintf(displayBuffer, "%s", "        ");
      // Icons off
      LEDKEY8.clrIcon(TM1638_LEDKEY8::LD1);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::DP1);
      ThisThread::sleep_for(100ms);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::LD2);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::DP2);
      ThisThread::sleep_for(100ms);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::LD3);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::DP3);
      ThisThread::sleep_for(100ms);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::LD4);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::DP4);
      ThisThread::sleep_for(100ms);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::LD5);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::DP5);
      ThisThread::sleep_for(100ms);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::LD6);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::DP6);
      ThisThread::sleep_for(100ms);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::LD7);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::DP7);
      ThisThread::sleep_for(100ms);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::LD8);
      LEDKEY8.clrIcon(TM1638_LEDKEY8::DP8);
      ThisThread::sleep_for(100ms);

}
Thread thread;
void display_thread()
{
  static uint8_t lastPos = 0;
  static uint8_t screenScroller = 0;

  while (1) {
    // If string in display buffer is longer than 8 characters then scroll to
    // end of line once per second
    screenScroller = (screenScroller + 1) % 10;
    if (screenScroller == 0) {
        if ((strlen(displayBuffer) > 8))
            displayPos = (displayPos + 1) % (strlen(displayBuffer) - 7);
   
        else
            displayPos = 0;
    }
    // Update the actual display if displayBuffer has changed or we are scrolling
    if (strcmp(currentDisplay, displayBuffer) || lastPos != displayPos) {
//        printf("%d %d - %s\r\n", strlen(displayBuffer), displayPos, displayBuffer);
        lastPos = displayPos;
        strcpy(currentDisplay, displayBuffer);
        LEDKEY8.displayStringAt((char *)(currentDisplay + displayPos), 0);
        blueLED = !blueLED;
    }

    ThisThread::sleep_for(100ms);
  }
}
// void displayStringAt(char * inString, int startLoc = 0) {
//    char outString[22];
//    for (int i = 0; i < (strlen( inString )) && i < 9; i++) {
//        outString[2*i] = FONT_7S[0x5f&inString[i]];
//        outString[2*i+1]= 0;
//    }
//    LEDKEY8.locate(startLoc);
//    LEDKEY8.writeData(outString);
//}
int main() {


  // printf("Hello World\r\n"); //
  char msg[] = "Hello World!\r\n";
  char *buff = new char[1];
  pc.write(msg, sizeof(msg));

  LEDKEY8.cls();
//  fancy_clear();
  LEDKEY8.writeData(all_str);
  ThisThread::sleep_for(2ms);
  LEDKEY8.setBrightness(TM1638_BRT3);
  ThisThread::sleep_for(1ms);
  LEDKEY8.setBrightness(TM1638_BRT0);
  ThisThread::sleep_for(1ms);
  LEDKEY8.setBrightness(TM1638_BRT4);

  ThisThread::sleep_for(1ms);
  fancy_clear();
//  LEDKEY8.cls(true);
  LEDKEY8.writeData(hello_str);
  thread.start(display_thread);
  strcpy(displayBuffer,"Hello World!");

  while (1) {

    // Check and read keydata
    if (LEDKEY8.getKeys(&keydata)) {
      printf("Keydata 0..3 = 0x%02x 0x%02x 0x%02x 0x%02x\r\n", keydata[0],
             keydata[1], keydata[2], keydata[3]);

      if (keydata[LEDKEY8_SW1_IDX] == LEDKEY8_SW1_BIT) { // sw1
        LEDKEY8.cls();
        LEDKEY8.writeData(all_str);

        ThisThread::sleep_for(500ms);
        for (int i = 0; i < 20; i++) {
          LEDKEY8.writeData(animate[i]);
          ThisThread::sleep_for(100ms);
        }
        fancy_clear();
      }

      if (keydata[LEDKEY8_SW2_IDX] == LEDKEY8_SW2_BIT) { // sw2

        LEDKEY8.writeData(hello_str);
        // test to show all segs

        printf("Show all segs\r\n");
        ThisThread::sleep_for(1000ms);
        LEDKEY8.cls();

        for (int i = 0; i < TM1638_DISPLAY_MEM; i++) {
          for (int bit = 0; bit < 8; bit++) {
            LEDKEY8.cls();

            bits = 0x01 << bit;
            LEDKEY8.writeData(bits, i);
            ThisThread::sleep_for(200ms);

            //              printf("Idx = %d, Bits = 0x%02x\r\n", i, bits);
            ////              wait_us(500);
            //              pc.read(buff, sizeof(buff));
            //              pc.write(buff, sizeof(buff));
            //
            //              cmd0 = buff[0]; // wait for key
          }
        }
        printf("\r\nShow all segs done\r\n");

#endif
      }

      if (keydata[LEDKEY8_SW3_IDX] == LEDKEY8_SW3_BIT) { // sw3
        //          LEDKEY8.cls();
        //          LEDKEY8.writeData(mbed_str);

        // test to show all alpha characters
        fancy_clear();
        printf("Show all NATO Alpha chars\r\n");
        sprintf(displayBuffer,"NATOChar");
//        LEDKEY8.cls(); // clear all, preserve Icons
        while (1) {
          LEDKEY8.locate(0);
          //            outputChar = NATO[i];
          pc.read(buff, sizeof(buff));// wait for key
          pc.write(buff, sizeof(buff));// echo
        LEDKEY8.cls();

          if (buff[0] == 0x1b)  // escape pressed?
            break;
           cmd0 = 0x1f & buff[0] - 1; // convert key to array lookup index
         //            pc.write(NATO[cmd0],10);
          sprintf(displayBuffer, "%c - %s", buff[0], (NATO[cmd0]));

          wait_us(500);
        }
        printf("Show all NATO Alpha chars done\r\n");

        // test to show all chars
        printf("Show all alpha chars\r\n");
        ThisThread::sleep_for(1ms);
        fancy_clear();
        for (char letter = 65; letter < 65 + 26; letter++) {
          sprintf(displayBuffer, "%c", letter);
          //            LEDKEY8.printf("%c", char(i + 'a'));
          ThisThread::sleep_for(250ms);
        }
        printf("Show all alpha chars done\r\n");

        // test to show all chars
        printf("Show all chars\r\n");
        ThisThread::sleep_for(1ms);
        fancy_clear();

        for (char i = FONT_7S_START; i < FONT_7S_END; i++) {
          sprintf(displayBuffer, "%c", i);
          ThisThread::sleep_for(250ms);
          //            wait(0.25);
          //            cmd = getc(); // wait for key
        }
        printf("Show all chars done\r\n");
      }

#if (0)
      // test to show all digits (base is 10)
      printf("Show all digits\r\n");
      ThisThread::sleep_for(1ms);
      LEDKEY8.cls();

      for (int i = 0; i < LEDKEY8_NR_DIGITS; i++) {

        for (int cnt = 0; cnt < 10; cnt++) {
          LEDKEY8.locate(i);
          LEDKEY8.printf("%0d", cnt);

          //              wait_us(500);
          cmd = getc(); // wait for key
        }
      }
      printf("\r\nShow all digits done\r\n");
#endif

#if (1)
// test to show all digits (base is 0x10)
/*
          printf("Show all hex digits\r\n");
          ThisThread::sleep_for(1ms);
          LEDKEY8.cls();

          LEDKEY8.printf("%08x", 0x012345AB);
          cmd = getc(); // wait for key
          LEDKEY8.printf("%08x", 0x00);

          for (int i=0; i<LEDKEY8_NR_DIGITS; i++) {

            for (int cnt=0; cnt<0x10; cnt++) {
              LEDKEY8.locate(i);
              LEDKEY8.printf("%0x", cnt);

//              wait_us(500);
              cmd = getc(); // wait for key
            }

          }
          printf("\r\nShow all hex digits done\r\n");
          */
#endif
    }

    if (keydata[LEDKEY8_SW4_IDX] == LEDKEY8_SW4_BIT) { // sw4
//          LEDKEY8.cls();
//          LEDKEY8.writeData(mbed_str);
#if (1)
      // test to show all icons
      printf("Show all icons\r\n");
      LEDKEY8.cls(true); // Also clear all Icons
      fancy_clear();

      //          ThisThread::sleep_for(1ms);
      //          LEDKEY8.cls(); // clear all, preserve Icons
      printf("Show all icons done\r\n");
#endif
    }
    if (keydata[LEDKEY8_SW5_IDX] == LEDKEY8_SW5_BIT) { // sw5
      fancy_clear();
      printf("Decimal Counting\r\n");
      LEDKEY8.cls(); // clear all, preserve Icons
      displayPos = 0;
                     //          LEDKEY8.writeData(outputChar);
      for (int cnt = 0; cnt <= 0xFF; cnt++) {
        if (cnt < 10) sprintf(displayBuffer, "Count  %d", cnt);
        else if (cnt < 100) sprintf(displayBuffer, "Count %d", cnt);
        else sprintf(displayBuffer, "Count%d", cnt);
        ThisThread::sleep_for(200ms);
      }
      printf("Decimal Counting complete\r\n");
    }
#if (0)

#endif

    /*
              LEDKEY8.printf("hello");
            }
    */
    if (keydata[LEDKEY8_SW6_IDX] == LEDKEY8_SW6_BIT) { // sw6
//      LEDKEY8.cls(); // clear all, preserve Icons
      fancy_clear();
      LEDKEY8.writeData(hello_str);
      printf("Hello");
      ThisThread::sleep_for(1000ms);
      LEDKEY8.cls(); // clear all, preserve Icons
      LEDKEY8.writeData(bye_str);
      printf("Bye");
      ThisThread::sleep_for(1000ms);
      //          LEDKEY8.printf("Bye");
    }

    if (keydata[LEDKEY8_SW7_IDX] == LEDKEY8_SW7_BIT) { // sw7
      printf("floating point");
      fancy_clear();
//      LEDKEY8.cls();                         // clear all, preserve Icons
      sprintf(displayBuffer, " %2.3f", -0.1234); // test decimal point display
      ThisThread::sleep_for(1000ms);
      fancy_clear();
//      LEDKEY8.cls();                         // clear all, preserve Icons
      sprintf(displayBuffer, "%2.3f", -012.345); // test decimal point display
      ThisThread::sleep_for(2000ms);
      printf("floating point complete");
    }

    if (keydata[LEDKEY8_SW8_IDX] == LEDKEY8_SW8_BIT) { // sw8
//      LEDKEY8.cls();                         // clear all, preserve Icons
      fancy_clear();
    } // if Key

    myled = !myled;
    ThisThread::sleep_for(250ms);

  } // while
}

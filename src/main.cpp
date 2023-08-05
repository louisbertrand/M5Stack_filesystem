/** @brief Test concurrent operations on M5Stack Core/Core2 uSD cards 
 * @author Louis Bertrand <louis@bertrandtech.ca>
 * 
*/

#ifdef M5_CORE2
#include <M5Core2.h>
#elif M5_BASIC
#include <M5Stack.h>
#endif

#include <Arduino.h>
#include <RBD_Timer.h>

RBD::Timer activity{1000};  // Write to SD card for this long (ms)

// Declare test functions
void test_one_then_two_alternating();
void test_one_two_together();


void setup()
{
  Serial.begin(115200);
  Serial.println("Hello M5Stack");
  /// Hardware configurations
#ifdef M5_CORE2
  Wire.begin();
  M5.begin();
#elif M5_BASIC
  Wire.begin();
  M5.begin();
#endif

  M5.Lcd.clear();
  M5.lcd.setRotation(3);
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setCursor(0, 55);

  Serial.println("SD.begin(4)");
  M5.Lcd.println("SD.begin(4)");
  auto status = SD.begin(4);  // GPIO 4
  if(!status) {
    Serial.println("Cannot open SD card.");
    M5.Lcd.println("Cannot open SD card.");
    return;
  }
  test_one_then_two_alternating();
  test_one_two_together();

}

// Do nothing after tests are done
void loop()
{
    Serial.println("Idle");
    delay(2000);
}

// Open file1, write, close file 1, then the same for file2.
// The values in each file should be a pair of integers,
// monotonically increasing but not necessarily consecutive.
// File 1   File 2
// 0001-1234     0004-2567
// 0007-2790     0010-0200
const char DIR1[] = "/TEST1";
const char LOGGING1[] = "/TEST1/LOGGING.TXT"; 
const char JOURNAL1[] = "/TEST1/JOURNAL.TXT"; 
void test_one_then_two_alternating()
{
  if(!SD.exists(DIR1)) {
    SD.mkdir(DIR1);
  }
  M5.Lcd.println("Running test 1");
  Serial.print(DIR1); Serial.print("@"); Serial.println(micros());
  activity.restart();
  while(!activity.isExpired()) {
    auto fd1 = SD.open(LOGGING1, "a");
    fd1.printf("%u-%u\n", millis(), micros());
    fd1.close();
    auto fd2 = SD.open(JOURNAL1, "a");
    fd2.printf("%u-%u\n", millis(), micros());
    fd2.close();
  }
}

// Open file1 and file 2, write file1 and file2, close file1 and file2.
// The values in each file should be a pair of integers, the same or
// monotonically increasing, but not necessarily consecutive.
// File 1   File 2
// 0001     0003
// 0007     0008
const char DIR2[] = "/TEST2";
const char LOGGING2[] = "/TEST2/LOGGING.TXT"; 
const char JOURNAL2[] = "/TEST2/JOURNAL.TXT"; 
void test_one_two_together()
{
  if(!SD.exists(DIR2)) {
    SD.mkdir(DIR2);
  }
  M5.Lcd.println("Running test 2");
  Serial.print(DIR2); Serial.print("@"); Serial.println(micros());
  activity.restart();
  while(!activity.isExpired()) {
    auto fd1 = SD.open(LOGGING2, "a");
    auto fd2 = SD.open(JOURNAL2, "a");
    fd1.printf("%u-%u\n", millis(), micros());
    fd2.printf("%u-%u\n", millis(), micros());
    fd1.close();
    fd2.close();
  }
}

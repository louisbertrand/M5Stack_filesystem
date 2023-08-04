# M5Stack_filesystem
Test ability to read/write uSD card filesystem in various open/close and concurrent operations.

In the bGeigieZen weekly meeting Mon July 31, questions came up about the capabilities of the M5Stack Core and Core2 to write to two open files concurrently. The intention is to write "drive" data (CPM, location, time) to the log file for eventual submission to the Safecast dataset, but also write any received data to a journal file, including those readings made in "survey" mode, for example, stopping to survey a spot on the ground.

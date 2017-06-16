This is the solution for the Top Career Code Summer 2017

It contains two problems as the following:

1. A file contains a sequence of integers, stored one per line. Implement a class that facilitates iterator over these integers.
A valid integer is a sequence of one or more digits(without leading zeros), optionally preceded by a plus or minus sign, representing a number within the range [-1,000,000,000...1,000,000,000]. We allow spaces to appear in a line before and/or after a valid integer. Lines are separated with the line-feed character(ASCII code 10).
There might be lines that do not represent valid integers, e.g. 2u1, 23.9, #12, 00, ++1, 2000000000. Such lines are considered to be comments and should be discarded.
Define a class Solution with an input iterator(as defined by C++ 03 and 11) that interates over integers from an input stream compliant with the above format

2. James is a businessman. He is on a tight schedule this week. The week starts on Monday at 00:00 and ends on Sunday at 24:00. His schedule consists of M meetings he needs to take part in. Each of them will take place in a period of time, beginning and ending on the same day(there are no two ongoing meetings at the same time). James is very tired thus he needs to find the longest period of time there are no ongoing meetings. The sleeping break can begin and end on different days and should begin and end in the same week.
You are given a string containing M lines. Each line is a substring representing one meeting in the schedule, in the format "Ddd hh:mm-hh:mm". "Ddd" is a three-letter abbreviation for the day of the week when the meeting takes place: "Mon"(Monday), "Tue", "Web", "Thu", "Fri", "Sat", "Sun". "hh:mm-hh:mm" means the beginning time and the ending time of the meeting(from 00:00 to 24:00 inclusive).
THe given times represent exact moments of time. So, there are exactly five minutes between 13:40 and 13:45.

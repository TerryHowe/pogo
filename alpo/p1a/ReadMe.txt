Part 1a: Fixed Point Class

Design and implement a class or set of functions to work with the
following encoding of a fixed-point number. We often work with client
hardware that uses proprietary or unusual protocols or data formats.
For this exercise, assume that we are developing software that
receives data values from a piece of client-provided hardware in the
binary-encoded format described below. The task at hand is to create a
class that can correctly encode and decode those values and make them
usable by other software.

In this instance, a fixed-point number is packed into a 32-bit
unsigned integer as follows:

s i i i i i i i   i i i i i i i i   i f f f f f f f   f f f f f f f f

where:

   s : sign bit (1 == negative)

   i : 16 integer bits

   f : 15 fraction bits

Examples:

   Value       Fixed Point encoding
   1.0         0x00008000
   -1.0        0x80008000
   2.0         0x00010000
   -2.5        0x80014000
   3.14        0x000191eb
   100.99      0x00327eb8

Your class should support at least the following operations:

   * Create Fixed Point object from floating point value
   * Create Fixed Point object from packed 32-bit value
   * Idiomatic conversion to string (as <<optional sign>><<integer
part>>.<<fractional part>>) For example, an instance of your Fixed
Point class initialized from an encoded value of 0x80008000 would be
converted to the string "-1.0".
   * Convert value to closest floating point equivalent



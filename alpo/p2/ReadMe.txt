Part 2: Text File Parser

Design and implement a class that can parse the following configuration file format:

[section name 1]
key: value
key2 : value

[section name 2]
key3: value that is too long for one line, so we wrap onto a second line
  and continue by using any whitespace character in the first column.

Or more specifically:

    *

      The file is broken up into named sections. The name of each section is enclosed by square brackets, and the opening bracket must be placed in column 0.
    *

      The first non-blank line in a file must be a section heading.
    *

      Each line is terminated by a CR/LF pair (on all platforms).
    *

      Whitespace between the opening bracket and the first non-whitespace character of the section name is discarded, as is any whitespace between the last character of the section name and the closing bracket. The following section names are all identical:

      [Section demonstration]
      [   Section demonstration]
      [ Section demonstration     ]

    *

      Each non blank line of the file inside a section consists of a key and a value, separated by a colon. Whitespace surrounding the colon is discarded, so the following lines are identical:

      key:value
      key :value
      key :  value

    *

      Key names within each section must be unique (but key names may be reused in different sections).
    *

      Each section name must be unique within the file.
    *

      Each key must begin in column zero.
    *

      Long lines may be wrapped by continuing them onto the next line and placing one or more whitespace characters in column zero. When reading the file in, the combination CR/LF followed by whitespace is treated as if it were just the whitespace. Only the value component of a line may be wrapped.

Your class must support at least the following operations:

    * Create a parser object given a file name.
    * Get a string value associated with a given section and key names.
    * Get an integer value associated with a given section and key names.
    * Get a floating point value associated with a given section and key names.
    * Set a string value for a given section and key names, writing the new file to disk.
    * Set an integer value for a given section and key names, writing the new file to disk.
    * Set a floating point value for a given section and key names, writing the new file to disk.

You may use the following test data for your development:

[header]
project: Programming Test
budget : 4.5
accessed :205

[meta data]
description : This is a tediously long description of the Art & Logic
 programming test that you are taking. Tedious isn't the right word, but
 it's the first word that comes to mind.

correction text: I meant 'moderately,' not 'tediously,' above.

[ trailer ]
budget:all out of budget.



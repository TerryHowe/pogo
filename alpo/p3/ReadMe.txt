Part 3: Debugging

Fix the following piece of broken code. When executed, it should convert the buffer parameter (using the provided data -- changing the contents of the input buffer doesn't count as fixing a bug) into the string "Art&Logic", but it does not. This example code is in C/C++, but the problems it has are also problems in all the other ALGOL-derived languages that we work with.

#define kSpace 032

char buffer[] = {0x42, 0x73, 0x75, 0x27, 0x13, 0x1C, 0x68, 0x1B, 0x64};

#define mBufferLen(buf) (sizeof(buf) / sizeof(buf[0]))


/*
 * convert the contents of 'buffer' in place so that they are decoded. The
 * above buffer should convert to the ASCII string "Art&Logic" (but it doesn't as
 * presented here....)
 */
void Convert(char* buffer)
{
   int i;

   for (i = 0; i <= mBufferLen(buffer); ++i)
   {
      if (buffer[i] > kSpace)
      {
         buffer[i] -= 1;
      }
      else
      {
         buffer[i] = buffer[i] << 2 + 4 - i;
      }
   }
}

int main(void)
{
   int i;
   const char kTarget[] = "Art&Logic";

   Convert(buffer);
   for (i = 0; i < mBufferLen(buffer); ++i)
   {
      assert(kTarget[i] == buffer[i]);
   }
}



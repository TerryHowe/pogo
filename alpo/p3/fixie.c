#include <stdio.h>
#include <assert.h>

#define kSpace 32

char buffer[] = {0x42, 0x73, 0x75, 0x27, 0x13, 0x1C, 0x68, 0x1B, 0x64};

#define mBufferLen(buf) (sizeof(buf) / sizeof(buf[0]))


/*
 * convert the contents of 'buffer' in place so that they are decoded. The
 * above buffer should convert to the ASCII string "Art&Logic" (but it doesn't as
 * presented here....)
 */
void Convert(char* buffer, unsigned int siz)
{
   int i;

   for (i = 0; i < siz; ++i)
   {
      if (buffer[i] > kSpace)
      {
         buffer[i] -= 1;
      }
      else
      {
         buffer[i] = (int)(buffer[i] << 2) + 4 - i;
      }
   }
}

int main(void)
{
   int i;
   const char kTarget[] = "Art&Logic";

   Convert(buffer, mBufferLen(buffer));
   for (i = 0; i < mBufferLen(buffer); ++i)
   {
      assert(kTarget[i] == buffer[i]);
   }
   return(0);
}

#include <assert.h>
#include <iostream>

#include "AFileParser.h"

using namespace std;

main()
{
   AFileParser fileParser("testConfigFile");
   if(!fileParser.Read())
   {
      cerr << "Error in testConfigFile or it doesn't exist\n";
      return 1;
   }

   int index = fileParser.GetSection("header");
   assert(index == 0);

   index = fileParser.GetSection("not there");
   assert(index == -1);

   string value = fileParser.GetStringValue("header", "project");
   assert(!value.empty());

   fileParser.SetValue("header", "project", "aced", "newOne");
   value = fileParser.GetStringValue("header", "project");
   assert(!value.empty());

   int intVal = fileParser.GetIntValue("header", "accessed");
   assert(intVal == 205);
   try
   {
      int intVal = fileParser.GetIntValue("header", "notThere");
      assert(false);
   }
   catch(...)
   {
   }

   fileParser.SetValue("header", "accessed", 206, "newOne");
   intVal = fileParser.GetIntValue("header", "accessed");
   assert(intVal == 206);

   float floatVal = fileParser.GetFloatValue("header", "budget");
   assert(floatVal == 4.5f);

   fileParser.SetValue("header", "budget", 6.5f, "newOne");
   floatVal = fileParser.GetFloatValue("header", "budget");
   assert(floatVal == 6.5f);

   index = fileParser.GetSection("meta data");
   assert(index == 1);

   value = fileParser.GetStringValue("meta data", "correction text");
   assert(!value.empty());

   value = fileParser.GetStringValue("meta data", "non existent");
   assert(value.empty());
 
   index = fileParser.GetSection("trailer");
   assert(index == 2);

   value = fileParser.GetStringValue("trailer", "budget");
   assert(!value.empty());

   AFileParser parser2("newOne");
   if(!parser2.Read())
   {
      cerr << "Error in newOne or it doesn't exist\n";
      return 1;
   }
   cout << "Done\n";
}

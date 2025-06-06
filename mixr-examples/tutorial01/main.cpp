// OBJECTIVE: 
// A simple "Hello World" program introducing MIXR.

#include <iostream>

#include "mixr/base/String.hpp"

int main(int argc, char* argv[])
{
   // CONST marks unchanging pointer, AUTO marks the type is derived from the created object (mixr::base::String)
   const auto str = new mixr::base::String("Hello world\n"); // Objects are POINTERS to objects in memory

   std::cout << str->getString();

   str->setStr("Goodbye world\n");

   std::cout << str->getString();

   str->unref(); // Ref/Unref increments/decrements reference counter, if ref counter == 0, deletes the object.

   return 0;
}

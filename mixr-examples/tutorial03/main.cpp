// OBJECTIVE:
// A program to demonstrate Software Design Patterns (Factories, Builders) and introduce SLOTTABLE.

#include <iostream>
#include <string>

#include "mixr/base/Pair.hpp"
#include "mixr/base/edl_parser.hpp"

// factories
#include "mixr/base/factory.hpp"

#include "Rng.hpp"

// our class factory
// According to Refactoring.guru
// "Factory Method is a creational design pattern that provides an interface for creating objects in a superclass, 
// but allows subclasses to alter the type of objects that will be created."
mixr::base::Object* factory(const std::string& name)
{
   mixr::base::Object* obj{};

   // look in application's classes
   if ( name == Rng::getFactoryName() ) {
      obj = new Rng;
   }

   // look in base classes
   if (obj == nullptr) obj = mixr::base::factory(name);
   return obj;
}

// random builder
// According to Refactoring.guru...
// Builder is a creational design pattern that lets you construct complex objects step by step. 
// The pattern allows you to produce different types and representations of an object using the same construction code.
Rng* builder(const std::string& filename)
{
   // read configuration file
   int num_errors{};
   mixr::base::Object* obj{mixr::base::edl_parser(filename, factory, &num_errors)};
   if (num_errors > 0) {
      std::cerr << "File: " << filename << ", number of errors: " << num_errors << std::endl;
      std::exit(EXIT_FAILURE);
   }

   // test to see if an object was created
   if (obj == nullptr) {
      std::cerr << "Invalid configuration file, no objects defined!" << std::endl;
      std::exit(EXIT_FAILURE);
   }

   // do we have a base::Pair, if so, point to object in Pair, not Pair itself
   const auto pair = dynamic_cast<mixr::base::Pair*>(obj);
   if (pair != nullptr) {
      obj = pair->object();
      obj->ref();
      pair->unref();
   }

   // try to cast to proper object, and check
   const auto random = dynamic_cast<Rng*>(obj);
   if (random == nullptr) {
      std::cerr << "Invalid configuration file!" << std::endl;
      std::exit(EXIT_FAILURE);
   }
   return random;
}

int main(int argc, char* argv[])
{
   // default configuration filename
   std::string configFilename = "file0.edl";

   // build random
   Rng* rng{builder(configFilename)};

   for (int i=0; i<10; i++) {
      std::cout << rng->num() << std::endl;
   }

   rng->unref();

   return 0;
}

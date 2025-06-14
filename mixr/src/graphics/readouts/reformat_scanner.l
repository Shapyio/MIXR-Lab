
%{

//------------------------------------------------------------------------------
// Description: Scans format specifications embedded in EDL files and builds
//              a sprintf-style format string (i.e., it reformats one specification
//              into another)
//------------------------------------------------------------------------------

#include <iostream>
#include <cstring>
#include <cstdio>
#include <sstream>

#include "ReformatScanner.hpp"

#define YY_BREAK  /* We'll put in the break commands to stop the warnings */

%}

/*** Flex declarations and Options ***/

/* generate a C++ scanner class */
%option c++

/* no calls to yywrap */
%option noyywrap

/* suppress inclusion of unistd.h file */
%option nounistd

/* change the name of the scanner class - results in "rfFlexLexer" */
%option prefix="rf"

/* derived 'mixr::graphics::ReformatScanner' is a subclass of rfFlexLexer */
%option yyclass="mixr::graphics::ReformatScanner"

%%

\+?0*#+\+?                    {   // "+0#" (Integer w/sign and leading zeros)
                                  return mixr::graphics::ReformatScanner::processInteger(yytext, yyleng);
                              }

\+?0*#+\.#+\+?                {   // "+0#.#" (Floating w/sign and leading zeros)
                                  return mixr::graphics::ReformatScanner::processFloat(yytext, yyleng);
                              }

\+?(0|H)H+:?MM:?SS(\.S+)?\+?  {   // HH:MM:SS (Hours, minutes and seconds)
                                  return mixr::graphics::ReformatScanner::processTime(mixr::graphics::TimeReadout::TimeMode::hhmmss, yytext, yyleng);
                              }

\+?(0|H)H+:?MM(\.M+)?\+?      {   // HH:MM (Hours and minutes)
                                  return mixr::graphics::ReformatScanner::processTime(mixr::graphics::TimeReadout::TimeMode::hhmm, yytext, yyleng);
                              }

\+?(0|H)H+(\.H+)?\+?          {   // HH (Hours)
                                  return mixr::graphics::ReformatScanner::processTime(mixr::graphics::TimeReadout::TimeMode::hh, yytext, yyleng);
                              }

\+?(0|M)M+:?SS(\.S+)?\+?      {   // MM:SS (Minutes and seconds)
                                  return mixr::graphics::ReformatScanner::processTime(mixr::graphics::TimeReadout::TimeMode::mmss, yytext, yyleng);
                              }

\+?(0|M)M+(\.M+)?\+?          {   // MM (Minutes)
                                  return mixr::graphics::ReformatScanner::processTime(mixr::graphics::TimeReadout::TimeMode::mm, yytext, yyleng);
                              }

\+?(0|S)S+(\.S+)?\+?          {  // SS (Seconds)
                                 return mixr::graphics::ReformatScanner::processTime(mixr::graphics::TimeReadout::TimeMode::ss, yytext, yyleng);
                              }

\+?(0|D)D{1,2}@?MM\'?SS(\.S+)?\"?\+?    {   // +DDMMSS (Degrees, minutes and seconds)
                                            return mixr::graphics::ReformatScanner::processDirection(mixr::graphics::DirectionReadout::DirMode::ddmmss, yytext, yyleng);
                                        }

\+?(0|D)D{1,2}@?MM(\.M+)?\'?\+?         {   // +DDMM (Degrees and minutes)
                                            return mixr::graphics::ReformatScanner::processDirection(mixr::graphics::DirectionReadout::DirMode::ddmm, yytext, yyleng);
                                        }

\+?(0|D)D{1,2}(\.D+)?@?\+?    {   // +DD (Degrees)
                                  return mixr::graphics::ReformatScanner::processDirection(mixr::graphics::DirectionReadout::DirMode::dd, yytext, yyleng);
                              }

\+?D(\.D+)?@?\+?              {   // +DD (Degrees)
                                  return mixr::graphics::ReformatScanner::processDirection(mixr::graphics::DirectionReadout::DirMode::dd, yytext, yyleng);
                              }

%%

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// yylex()
//------------------------------------------------------------------------------
int ReformatScanner::yylex(const DataType dt)
{
   postSign = false;
   dataType = dt;
   return yylex();
}

//------------------------------------------------------------------------------
// processInteger() -- process a integer number string
//    Take in a format such as "+0##" and return "%+04d".
//------------------------------------------------------------------------------
int ReformatScanner::processInteger(const char* text, const int len)
{
   switch (dataType) {
   case DataType::number:         // We're looking for a number,
   case DataType::hex:            //   a Hexadecimal number, or
   case DataType::octal:          //   an Octal number.
      break;
   default:                       // No, we didn't want any of these
      return formatError(text);
   }

   // ---
   // Scan the example string
   // ---

   // Check for the sign
   int i {};
   int s {};

   if (text[i] == '+') {
      s = 1;
      i++;
   }

   // Check for an leading zeros
   int lz {};
   while (text[i] == '0' && i < len) {
      lz++;
      i++;
   }

   // Check for an leading numbers
   int ln {};
   while (text[i] == '#' && i < len) {
      ln++;
      i++;
   }

   // Check for the post sign
   if (text[i] == '+') {
      postSign = true;
      s = 1;
      i++;
   }

   // ---
   // Build the format statement
   // ---

   // Computer total number of characters in numberic field:
   //    sign, leading zeros, and leading numbers.
   int fw {s + lz + ln};

   // Create the sprintf() format string
   int j {};

   format[j++] = '%';				// Start with %

   if (s)
      format[j++] = '+';			// Add a '+' if signed

   if (lz > 0)
      format[j++] = '0';			// Add a '0' if leading zeros

   j += std::sprintf(&format[j], "%d", fw);		// Add total field size

   if (dataType == DataType::number) {
      format[j++] = '.';
      format[j++] = '0';
      format[j++] = 'f';
   }
   else if (dataType == DataType::octal) format[j++] = 'o';
   else if (dataType == DataType::hex) format[j++] = 'X';

   format[j] = '\0';

   return 1;
}

//------------------------------------------------------------------------------
// processFloat() -- process a floating point number string
//    Take in a format such as "+0###.###" and return "%+09.3f".
//------------------------------------------------------------------------------
int ReformatScanner::processFloat(const char* text, const int len)
{
   // Check valid type
   if (dataType != DataType::number)
      return formatError(text);

   // ---
   // Scan the example string
   // ---

   // Check for the sign
   int i {};
   int s {};

   if (text[i] == '+') {
      s = 1;
      i++;
   }

   // Check for an leading zeros
   int lz {};
   while (text[i] == '0' && i < len) {
      lz++;
      i++;
   }

   // Check for an leading numbers
   int ln {};
   while (text[i] == '#' && i < len) {
      ln++;
      i++;
   }

   // Skip the decimal point
   i++;

   // Check for the trailing numbers
   int nr {};
   while (text[i] == '#' && i < len) {
      nr++;
      i++;
   }

   // Check for the post sign
   if (text[i] == '+') {
      postSign = true;
      s = 1;
      i++;
   }


   // ---
   // Build the format statement
   // ---

   // Computer total number of characters in numberic field:
   //    sign, leading zeros, leading numbers, decimal point, and
   //    trailing numbers.
   int fw {s + lz + ln + 1 + nr};

   // Create the sprintf() format string
   int j {};

   format[j++] = '%';                           // Start with %

   if (s)
      format[j++] = '+';                        // Add a '+' if signed

   if (lz > 0)
      format[j++] = '0';                        // Add a '0' if leading zeros

   j += std::sprintf(&format[j], "%d", fw);     // Add total field size
   j += std::sprintf(&format[j], ".%d", nr);    // Add trailing numbers

   format[j++] = 'f';                           // Add the data type

   format[j] = '\0';

   return 1;
}

//------------------------------------------------------------------------------
// processTime() -- process a time format string
//    Take in a format such as "0MSS.S" and return "%02d%04.1f".
//------------------------------------------------------------------------------
int ReformatScanner::processTime(const TimeReadout::TimeMode tm, const char* text, const int len)
{
   // If not a time data type, exit with an error
   if (dataType != DataType::time)
      return formatError(text);

   // Check sign
   int i {};
   int s {};

   if (text[i] == '+') {
      s = 1;
      i++;
   }

   // Check leading zeros
   int lz {};
   if (text[i] == '0') {
      lz = 1;
      i++;
   }

   // Handle hours
   int hh {};
   while (text[i] == 'H' && i < len) {
      hh++;
      i++;
   }

   int hc {};
   int hd {};
   int hr {};

   // A colon or a decimal point is optional after hours
   if (text[i] == ':') {
      hc = 1;
      i++;
   }
   else if (text[i] == '.') {
      hd = 1;
      i++;

      while (text[i] == 'H' && i < len) {
         hr++;
         i++;
      }
   }

   // Handle minutes
   int mm {};
   while (text[i] == 'M' && i < len) {
      mm++;
      i++;
   }

   int mc {};
   int md {};
   int mr {};

   // A colon or a decimal point is optional after minutes
   if (text[i] == ':') {
      mc = 1;
      i++;
   }
   else if (text[i] == '.') {
      md = 1;
      i++;

      while (text[i] == 'M' && i < len) {
         mr++;
         i++;
      }
   }

   // Handle seconds
   int ss {};
   while (text[i] == 'S' && i < len) {
      ss++;
      i++;
   }

   int sr {};

   // A decimal point is optional with seconds
   if (text[i] == '.') {
      i++;

      while (text[i] == 'S' && i < len) {
         sr++;
         i++;
      }
   }

   // Check for the post sign
   if (text[i] == '+') {
      postSign = true;
      s = 1;
      i++;
   }

   // Build format string
   int j {};

   format[j++] = '%';

   if (s)
      format[j++] = '+';

   if (lz)
      format[j++] = '0';

   // Build the format for "HH[:.]".  Add in s and lz into fw.
   if (hh > 0) {
      if (s) hh++;
      if (lz) hh++;

      if (hd || mm == 0)
         j+= std::sprintf(&format[j], "%d.%df", hh+hr+(hr>0), hr);
      else
         j+= std::sprintf(&format[j], "%dd", hh);

      if (hc)
         format[j++] = ':';

      if (mm > 0) {
         format[j++] = '%';
         format[j++] = '0';
      }
   }

   // Build the format for "MM[:.]".  Add in s and lz into fw if
   // there are NO hours.
   if (mm > 0) {
      if (s  && hh <= 0) mm++;
      if (lz && hh <= 0) mm++;

      if (md || ss == 0)
         j+= std::sprintf(&format[j], "%d.%df", mm+mr+(mr>0), mr);
      else
         j+= std::sprintf(&format[j], "%dd", mm);

      if (mc)
         format[j++] = ':';

      if (ss > 0) {
         format[j++] = '%';
         format[j++] = '0';
      }
   }

   // Build the format for "SS[.]".  Add in s and lz into fw if
   // there are NO hours or minutes.
   if (ss > 0) {
      if (s  && hh <= 0 && mm <= 0) ss++;
      if (lz && hh <= 0 && mm <= 0) ss++;

      j+= std::sprintf(&format[j], "%d.%df", ss+sr+(sr>0), sr);
   }

   format[j] = '\0';

   return static_cast<int>(tm);
}


//------------------------------------------------------------------------------
// processDirection() -- process a directional format string
//    Take in a format such as "+120@23'43.2"" and return
//    "+%03d@%02d'%4.1f""
//------------------------------------------------------------------------------
int ReformatScanner::processDirection(const DirectionReadout::DirMode dm, const char* text, const int len)
{
   // If not a directional type, return error
   if (dataType != DataType::dir)
      return formatError(text);

   // Check sign
   int i {};
   int s {};

   if (text[i] == '+') {
      s = 1;
      i++;
   }

   // Check leading zeros
   int lz {};
   if (text[i] == '0') {
      lz = 1;
      i++;
   }

   // Handle degrees
   int dd {};
   while (text[i] == 'D' && i < len) {
      dd++;
      i++;
   }

   char dc  = '\0';
   int  ddc {};
   int  dr  {};

   if (text[i] == '.' && i < len) {
      ddc = 1;
      i++;

      while (text[i] == 'D' && i < len) {
         dr++;
         i++;
      }
   }
   if (text[i] != 'M' && text[i] != '+' && i < len)
      dc = text[i++];

   // Handle minutes
   int mm {};
   while (text[i] == 'M' && i < len) {
      mm++;
      i++;
   }

   char mc = '\0';
   int  md {};
   int  mr {};

   if (text[i] == '.' && i < len) {
      md = 1;
      i++;

      while (text[i] == 'M' && i < len) {
         mr++;
         i++;
      }
   }
   if (text[i] != 'S' && text[i] != '+' && i < len)
      mc = text[i++];

   // Handle seconds
   int ss {};
   while (text[i] == 'S' && i < len) {
      ss++;
      i++;
   }

   char sc = '\0';
   int  sr {};

   if (text[i] == '.' && i < len) {
      i++;

      while (text[i] == 'S' && i < len) {
         sr++;
         i++;
      }
   }
   if (i < len && text[i] != '+')
      sc = text[i++];

   // Check for the post sign
   if (text[i] == '+') {
      postSign = true;
      s = 1;
      i++;
   }

   // Build format string
   int j {};

   format[j++] = '%';

   if (s)
      format[j++] = '+';

   if (lz)
      format[j++] = '0';

   // Build the format for "DDD[.]".  Add in s and lz into fw.
   if (dd > 0) {
      if (s) dd++;
      if (lz) dd++;
      if (ddc || mm == 0)
         j+= std::sprintf(&format[j], "%d.%df", dd+dr+(dr>0), dr);
      else
         j+= std::sprintf(&format[j], "%dd", dd);

      if (dc)
         format[j++] = dc;

      if (mm > 0) {
         format[j++] = '%';
         format[j++] = '0';
      }
   }

   // Build the format for "MM[.]".  Add in s and lz into fw if
   // there are NO hours.
   if (mm > 0) {
      if (s  && dd <= 0) mm++;
      if (lz && dd <= 0) mm++;

      if (md || ss == 0)
         j+= std::sprintf(&format[j], "%d.%df", mm+mr+(mr>0), mr);
      else
         j+= std::sprintf(&format[j], "%dd", mm);

      if (mc)
         format[j++] = mc;

      if (ss > 0) {
         format[j++] = '%';
         format[j++] = '0';
      }
   }

   // Build the format for "SS[.]".  Add in s and lz into fw if
   // there are NO hours or minutes.
   if (ss > 0) {
      if (s  && dd <= 0 && mm <= 0) ss++;
      if (lz && dd <= 0 && mm <= 0) ss++;

      j+= std::sprintf(&format[j], "%d.%df", ss+sr+(sr>0), sr);

      if (sc)
         format[j++] = sc;
   }

   format[j] = '\0';

   return static_cast<int>(dm);
}

//------------------------------------------------------------------------------
// formatError() -- process an error
//    Display the error to wherever lex wants it and return invalid (0).
//------------------------------------------------------------------------------
int ReformatScanner::formatError(const char* text)
{
   switch (dataType) {
   case DataType::hex:
   case DataType::octal:
      *yyout << "error: " << text << " is an invalid integer format." << std::endl;
      break;
   case DataType::number:
      *yyout << "error: " << text << " is an invalid number format." << std::endl;
      break;
   case DataType::time:
      *yyout << "error: " << text << " is an invalid time format." << std::endl;
      break;
   case DataType::dir:
      *yyout << "error: " << text << " is an invalid degree format." << std::endl;
      break;
   default:
      *yyout << "error in formatError" << std::endl;
      break;
   }

   return 0;
}


// Explicitly convert an integer or floating point number.  This
// expects a format such as "#.###" as input.  And returns "%5.3f"
// as output.
ReformatScanner::DataType ReformatScanner::convertNumber(const char* s)
{
   std::istringstream str(s);
   yyin = &str;
   yyrestart(yyin);
   return DataType(yylex(DataType::number));

// for flex 2.6.0
// std::istringstream iss(s);
// yyin.rdbuf(iss.rdbuf());
// yyrestart(yyin);
// return DataType(yylex(DataType::number));
}


// Explicitly convert an octal integer.  This expects a format
// such as "0##" as input.  And returns "%03o" as output.
ReformatScanner::DataType ReformatScanner::convertOctal(const char* s)
{
   std::istringstream str(s);
   yyin = &str;
   yyrestart(yyin);
   return DataType(yylex(DataType::octal));

// for flex 2.6.0
// std::istringstream iss(s);
// yyin.rdbuf(iss.rdbuf());
// yyrestart(yyin);
// return DataType(yylex(DataType::octal));
}


// Explicitly convert a hexadecimal integer.  This expects a format
// such as "###" as input.  And returns "%3X" as output.
ReformatScanner::DataType ReformatScanner::convertHex(const char* s)
{
   std::istringstream str(s);
   yyin = &str;
   yyrestart(yyin);
   return DataType(yylex(DataType::hex));

// for flex 2.6.0
// std::istringstream iss(s);
// yyin.rdbuf(iss.rdbuf());
// yyrestart(yyin);
// return DataType(yylex(DataType::hex));
}


// Explicitly convert a time value.  This expects a format such
// as "0H:MM:SS.S" as input.  And returns "%02d:%02d:%04.1f" as
// output.
TimeReadout::TimeMode ReformatScanner::convertTime(const char* s)
{
   std::istringstream str(s);
   yyin = &str;
   yyrestart(yyin);
   return TimeReadout::TimeMode(yylex(DataType::time));

// for flex 2.6.0
// std::string str(s);
// std::cout << "convertTime str: " << str << std::endl;
// std::istringstream iss(s);
// yyin.rdbuf(iss.rdbuf());
// yyrestart(yyin);
// return TimeReadout::TimeMode(yylex(DataType::time));
}


// Explicitly convert a directional value. This expects a format
// such as "+DDD@MM.M" as input.  And returns "+%03d@%04.1f" as
// output.
DirectionReadout::DirMode ReformatScanner::convertDirection(const char* s)
{
   std::istringstream str(s);
   yyin = &str;
   yyrestart(yyin);
   return DirectionReadout::DirMode(yylex(DataType::dir));

// for flex 2.6.0
// std::string str(s);
// std::cout << "convertDirection str: " << str << std::endl;
// std::istringstream iss(s);
// yyin.rdbuf(iss.rdbuf());
// yyrestart(yyin);
// return DirectionReadout::DirMode(yylex(DataType::dir));
}

}
}

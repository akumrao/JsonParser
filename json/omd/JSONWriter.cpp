//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright 2009, 2010 Object Modeling Designs
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define OMD_JSON_SOURCE

#include <fstream>
#include <sstream>
#include <boost/phoenix.hpp>

#include "JSONWriter.h"


bool write_stream( std::ostream& stream, omd::var &object )
{
   
   stream << object;
   return true;
}



OMD_JSON_DECL bool omd::json::write_file( std::string filename, omd::var &object )
{
   std::ofstream file( filename.c_str() );

   if( !file ) 
   {
      return false;
   }

   return write_stream( file, object );
}


OMD_JSON_DECL std::string omd::json::write_string( omd::var &object )
{
   std::stringstream stream;

   if( write_stream( stream, object ) )
   {
      return stream.str();
   }
   else
   {
      return "";
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright 2009, 2010 Object Modeling Designs
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#ifndef JSONREADER_H
#define JSONREADER_H

#include <string>
#include <istream>
#include "OMDTypes.h"
#include "omd/var.h"

/// xml reader lite abstraction
/**
 *  The xml lite version of the reader can handle simple xml constructs and no command
 *  or fancy directives.
 *  
 */
namespace omd{ 
  LIBEXPORT std::istream& operator>>( std::istream& stream, omd::var& object );

namespace json{
  /**
   *
   */
  //      bool read( InputIterator begin, InputIterator end, var object );

  /**
   *
   */
    
  LIBEXPORT void stream_to_string(std::istream& is, std::string& s);
     
  LIBEXPORT bool read_file( std::string filename, var& object );

  /**
   *
   */
  LIBEXPORT bool read_stream( std::istream& stream, var& object );

  /**
   *  Uses the specified xmlData buffer to populate var objects.
   *  The method will create additional var objects as needed and attach
   *  them to the top object.
   *
   *  @param xmlData the buffer containing the xml data to parse into the var
   *  @param object the var object that becomes the top object for the xml tree
   */
  LIBEXPORT bool parse_buffer( std::string& json_data, var& object );
  
  LIBEXPORT var& parse( var& object );

}}

#endif  //JSONREADER_H 

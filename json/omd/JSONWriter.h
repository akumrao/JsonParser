//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright 2009, 2010 Object Modeling Designs
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#ifndef JSONWRITER_H
#define JSONWRITER_H

#include <string>

#include "OMDTypes.h"
#include "omd/var.h"

#ifdef OMD_JSON_SOURCE
#define OMD_JSON_DECL LIBEXPORT
#else
#define OMD_JSON_DECL LIBIMPORT
#endif

/// xml writer abstraction
namespace omd
{
    namespace json
    {

        /**
         *
         */
        //      bool write( OutputIterator begin, var object );

        /**
         *
         */
        OMD_JSON_DECL bool write_file(std::string filename, var &object);

        /**
         *
         */
        OMD_JSON_DECL std::string write_string(var &object);

    }
}

#endif  // JSONWRITER_H

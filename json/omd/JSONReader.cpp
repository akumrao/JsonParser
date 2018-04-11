//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright 2009, 2010 Object Modeling Designs
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define OMD_JSONNode_SOURCE

#include "JSONReader.h"
#include "omd/detail/json_grammar.h"
#include <fstream>
#include <sstream>
#include <ios>

#define USE_SPIRIT_STREAM

#ifdef USE_SPIRIT_STREAM
#include <boost/spirit/include/support_istream_iterator.hpp>
#endif


namespace omd
{

    std::istream& operator>>(std::istream& stream, omd::var& object)
    {
        if (!omd::json::read_stream(stream, object))
        {
            //	 stream.setstate( io_base::failbit );
        }

        return stream;
    }

    namespace json
    {

        namespace spirit = boost::spirit;

        bool read_file(std::string filename, var& object)
        {
            std::ifstream stream(filename.c_str());

            if (!stream.is_open())
            {
                std::cout << "file could not be opened" << std::endl;
                return false;
            }

#ifdef USE_SPIRIT_STREAM
            return read_stream(stream, object);
#else

            std::stringstream buffer;
            buffer << stream.rdbuf();

            return parse_buffer(buffer.str(), object);

#endif
        }

        bool read_stream(std::istream& is, var& object)
        {

             std::string s;
            stream_to_string(is, s);
            return parse_buffer(s, object);
        }
        
        void stream_to_string(std::istream& is, std::string& s)
        {
            is.unsetf(std::ios::skipws);

            copy(std::istream_iterator< char>(is), std::istream_iterator< char >(), back_inserter(s));
        }

        var& parse(var& object)
        {

            std::string json_data;
            
            object.getValue( json_data ) ;
            
            object.erase();
            
            parse_buffer( json_data, object  );
            return object;
            
        }
        

        bool parse_buffer(std::string& json_data, var& object)
        {
            typedef std::string::const_iterator iterator_type;
            typedef omd::json::detail::json_grammar<iterator_type> json_grammar1;


            json_grammar1 xml; // Our grammar
            using boost::spirit::ascii::space;
            std::string::const_iterator iter = json_data.begin();
            std::string::const_iterator end = json_data.end();

            // client::var test;

            bool r = phrase_parse(iter, end, xml, space, object);
            if (r && iter == end)
            {
                std::cout << "-------------------------\n";
                std::cout << "Parsing succeeded\n";

                std::cout << object;

                std::cout << "-------------------------\n";


                return true;
            } else
            {
                std::cout << "-------------------------\n";
                std::cout << "Parsing failed\n";
                std::cout << "-------------------------\n";
                return false;
            }


        }
    }
}




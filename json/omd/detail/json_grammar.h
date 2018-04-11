/**
 *   \file json_grammar.h
 *   Copyright (c) 2014 Arvind Umrao< akumrao@yahoo.com>
 *
 *   \brief parser for minimal json
 */

#ifndef OMD_JSON_DETAIL_JSON_GRAMMAR_H
#define OMD_JSON_DETAIL_JSON_GRAMMAR_H

// --------- for testing --------
//#define BOOST_SPIRIT_DEBUG


#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp> 

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/function.hpp>
#include <boost/phoenix/object.hpp>

#include <boost/foreach.hpp>

#include <boost/phoenix/core/argument.hpp>


#include "omd/var.h"

namespace omd
{
    namespace json
    {
        namespace detail
        {
            namespace fusion = boost::fusion;
            namespace phoenix = boost::phoenix;
            namespace qi = boost::spirit::qi;
            namespace ascii = boost::spirit::ascii;
    
            template <typename Iterator>
            struct json_grammar
            : qi::grammar<Iterator, ascii::space_type, var()>
            {

                json_grammar()
                : json_grammar::base_type(value, "value")
                {
                    using qi::lit;
                    using qi::lexeme;
                    using qi::on_error;
                    using qi::fail;
                    using qi::char_;
                    using qi::int_;
                    using qi::double_;
                    using qi::bool_;
                    using qi::_val;
                    using qi::_1;
                    using qi::_2;
                    //using ascii::string;
                    using namespace qi::labels;
                    using phoenix::construct;
                    using phoenix::val;

                    nil_ = qi::attr_cast(lit("null"));

                    pair = (string > ":" >> value)
                            [pairit(_val, _1, _2)];
                    string %= lexeme['"' > +(char_ - '"') > '"' ];
                    value %= bool_ | nil_ | dbl |  number| string | array | object;
                    dbl %= double_;
                    number %= int_;
                    array = ('[' > value [arrayit(_val, _1)] % ',' > ']');

                    object = ("{" > -(pair [objectit(_val, _1)] % ',') > "}");


                    object.name("object");
                    pair.name("pair");
                    string.name("string");
                    value.name("value");
                    array.name("array");

                    BOOST_SPIRIT_DEBUG_NODE(object);
                    BOOST_SPIRIT_DEBUG_NODE(pair);
                    BOOST_SPIRIT_DEBUG_NODE(string);
                    BOOST_SPIRIT_DEBUG_NODE(value);
                    BOOST_SPIRIT_DEBUG_NODE(array);
                    
                    BOOST_SPIRIT_DEBUG_NODE(number);
                    BOOST_SPIRIT_DEBUG_NODE(dbl);

                    on_error<fail >
                            (
                            value
                            , std::cout
                            << val("Error! Expecting ")
                            << _4 // what failed?
                            << val(" here: \"")
                            << construct<std::string > (_3, _2) // iterators to error-pos, end
                            << val("\"")
                            << std::endl
                            );
                }

                qi::rule<Iterator, ascii::space_type, var() > number;
                qi::rule<Iterator, ascii::space_type, var() > dbl;
                qi::rule<Iterator, ascii::space_type, std::string() > string;
                qi::rule<Iterator, ascii::space_type, var() > value;
                qi::rule<Iterator, ascii::space_type, var() > pair;
                qi::rule<Iterator, ascii::space_type, var() > object;
                qi::rule<Iterator, ascii::space_type, var() > array;

                qi::rule<Iterator, var::nil_type() > nil_;

            };

        }
    }
}

#endif // OMD_JSON

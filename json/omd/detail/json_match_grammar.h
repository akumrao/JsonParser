/**
 *   \file json_match_grammar.h
 *   Copyright (c) 2014 Arvind Umrao< akumrao@yahoo.com>
 *

 *   \brief parser definition for json path commands
 */

#ifndef JSON_MATCH_GRAMMAR_H
#define JSON_MATCH_GRAMMAR_H


// --------- for testing --------
//#define BOOST_SPIRIT_DEBUG
//#include <iostream>
// ------------------------------

#include <vector>
#include <string>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/phoenix/stl/container.hpp>
#include <boost/phoenix/bind/bind_member_function.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>

#include "omd/var.h"
using namespace omd;

using boost::phoenix::function;
using boost::spirit::qi::unused_type;

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;
namespace ascii = boost::spirit::ascii;
namespace fusion = boost::fusion;
namespace phx = boost::phoenix;

typedef var_container::iterator node_container_iter;

void print(var i)
{
    std::cout << " print :" << i << std::endl << std::flush;
}

struct find_next_set_of_nodes
{

    find_next_set_of_nodes(var_container & node_vector)
    : node_vector_(node_vector),
    special_command_('\0')
    {
    }

    find_next_set_of_nodes(var_container& node_vector, char special_command)
    : node_vector_(node_vector),
    special_command_(special_command)
    {
    }

    void check_and_add(var &node, var_container& result_container, const std::string & node_name) const
    {

        if (node.tag() == 102)
        {
            var_container tmp = node.getNodes(node_name);

            std::transform(tmp.begin(), tmp.end(),
                    std::back_inserter(result_container), boost::ref<var>);
        }

         // std::cout << "find_next_set_of_nodes::check_and_add2 " << node_name << " " << result_container[0]  << std::endl << std::flush;

    }

    void operator()(const std::string& value, qi::unused_type, qi::unused_type) const
    {
        using namespace boost::phoenix;
        using namespace boost::phoenix::arg_names;

        var_container result_container;

        std::for_each(node_vector_.begin(),
                node_vector_.end(),
                bind(&find_next_set_of_nodes::check_and_add, this, arg1, boost::ref(result_container), value));

        node_vector_.clear();
        node_vector_ = result_container;

        //std::for_each(node_vector_.begin(), node_vector_.end(), print);

    }

    void operator()(unsigned int index, qi::unused_type, qi::unused_type) const
    {

        //std::cout << " index0 :" << index << " size " << node_vector_.size() << std::endl << std::flush;

        var_container result_container;

        node_container_iter iter = node_vector_.begin();
        node_container_iter iter_end = node_vector_.end();
        while (iter != iter_end)
        {
            var& obj = (var&) (*iter);

            if (obj.tag() == 101) // if array
            {
                //std::cout << " indexOuter :" << obj << " object.size " << obj.size() << std::endl << std::flush;

                if (index < obj.size())
                {
                    var& tmp_node = obj[ index ];
                    //std::cout << " index_inner :" << tmp_node << std::endl << std::flush;
                    result_container.push_back(boost::ref(tmp_node));
                }

            }
            ++iter;

        }


        node_vector_.clear();
        node_vector_ = result_container;

    }

    void operator()(qi::unused_type, qi::unused_type, qi::unused_type) const
    {
        using namespace boost::phoenix;
        using namespace boost::phoenix::arg_names;
        using namespace boost::phoenix::local_names;

        var_container result_container;

        switch (special_command_)
        {
            case '*':

                std::for_each(node_vector_.begin(),
                        node_vector_.end(),
                        bind(&find_next_set_of_nodes::check_and_add, this, arg1, boost::ref(result_container), "*"));

                node_vector_.clear();
                node_vector_ = result_container;
                break;

            default:
                break;
        }
    }

    var_container& node_vector_;
    const char special_command_;
};

struct fillTopMatchingNode
{

    fillTopMatchingNode(var_container& node_vector,
            var & top_node)
    : node_vector_(node_vector),
    top_node_(top_node)
    {
    }

    void operator()(qi::unused_type, qi::unused_type, qi::unused_type) const
    {

        node_vector_.clear();
        node_vector_.push_back(boost::ref(top_node_));
        //std::cout << " print :" << node_vector_[0] << std::endl << std::flush;
    }

    var_container& node_vector_;
    var& top_node_;
};

struct populate_relative_path
{

    populate_relative_path(var_container& node_vector,
            var & top_node)
    : node_vector_(node_vector),
    top_node_(top_node)
    {
    }

    void operator()(qi::unused_type, qi::unused_type, qi::unused_type) const
    {
       // std::cout << " populate_relative_path * :" << node_vector_.size() << std::endl << std::flush;

        node_vector_.clear();
        parse_utree(top_node_, "*");
    }

    void operator()(const std::string& value, qi::unused_type, qi::unused_type) const
    {
        //std::cout << " populate_relative_path :" << node_vector_.size() << std::endl << std::flush;
        node_vector_.clear();
        parse_utree(top_node_, value);
    }

    void parse_list(var& list, const std::string & node_name) const
    {
        for (var::size_type j = 0; j < list.size(); ++j)
        {
            var& vr = list[j];
            parse_utree(vr, node_name);
        }
    }

    void parse_utree(var& ut, const std::string & node_name) const
    {
        typedef spirit::utree utree;
        typedef spirit::utree_type utree_type;
        utree_type::info which = ut.which();
        short tag = ut.tag();

        //for complex data 
        if (tag == 100) // pair
        {
            if (ut.getKey() == node_name || node_name == "*")
            {
                if (ut.getValue().tag() != 101 && node_name != "*")
                    node_vector_.push_back(boost::ref(ut.getValue()));
                //std::cout << " key :" << ut.getKey() << " value: " << ut.getValue() << std::endl << std::flush;
            }
            parse_utree(ut.getValue(), node_name);
        } else if (tag == 101 || tag == 102) // array and object
        {
           if (tag == 102 && node_name == "*")
           {
               node_vector_.push_back(boost::ref(ut));
              // std::cout << " key object :" << ut  << std::endl << std::flush;
           }
            parse_list(ut, node_name);
        } else // for simple data
        {
            switch (which)
            {
                case utree_type::list_type:
                {
                    parse_list(ut, node_name);
                    break;
                }
		default:
		break;
            }
        }
    }


    var_container& node_vector_;
    var& top_node_;
};

struct attribute_handler
{

    attribute_handler(var_container & node_vector)
    : node_vector_(node_vector)
    {
    }

    void insert_pair(const std::pair< std::string, std::string>& value, var& pair, var_container& result_container) const
    {

        if (pair.size() == 2 && pair.tag() == 100)
        {
            var&  attribute_value =  pair.getValue();
            std::string attribute = pair.getKey();
            //std::cout << " attribute_handler insert2 :" << attribute_value << " attribute: " << value.second << std::endl << std::flush;

            if ((attribute == value.first))
            {
                if ( value.second.empty()  || attribute_value.equal( value.second) )
                {
                    result_container.push_back(boost::ref(pair));
                   // std::cout << " attribute_handler insert3 :" << " Pair : " << pair << std::endl << std::flush;
                }
            }
        }
    }

    void operator()(const std::pair< std::string, std::string>& value, qi::unused_type, qi::unused_type) const
    {
        var_container result_container;

        node_container_iter iter = node_vector_.begin();
        node_container_iter iter_end = node_vector_.end();
        
       // std::cout << " attribute_handler insert0 :"  << " size: " << node_vector_.size() << std::endl << std::flush;
        
        
        while (iter != iter_end)
        {

            var &obj = (var &) * iter;

            if (obj.tag() == 102) // if only if object
            {
               // std::cout << " attribute_handler insert1_object :" << obj << " attribute: " << value.first << std::endl << std::flush;

                for (var::size_type j = 0; j < obj.size(); ++j)
                {
                    var &pair = (var &) obj[j];
                    insert_pair(value, pair, result_container);
                }
            } else if (obj.tag() == 100) // if only if object
            {
              //  std::cout << " attribute_handler insert1_pair :" << obj << " attribute: " << value.first << std::endl << std::flush;

                insert_pair(value, obj, result_container);
            }

            ++iter;
        }

        node_vector_.clear();
        node_vector_ = result_container;
    }

    var_container& node_vector_;
};

/**
 *  Grammar definition
 */

template <typename Iterator>
struct json_match_grammar
: qi::grammar< Iterator, ascii::space_type >
{

    json_match_grammar(var &top_node,
            var_container & matched_nodes)
    : json_match_grammar::base_type(path_command),
    top_node_(top_node),
    matched_nodes_(matched_nodes)
    {
        using namespace qi::labels;
        using namespace boost::phoenix::arg_names;
        using namespace boost::phoenix::local_names;
        using qi::lit;
        using qi::on_error;
        using qi::lexeme;
        using qi::raw;
        //         using qi::eps;
        using qi::int_;
        using ascii::alpha;
        using ascii::alnum;
        using ascii::char_;

        using boost::spirit::_val;
        using boost::spirit::_1;
        using boost::spirit::_2;
        using boost::spirit::_3;
        using boost::spirit::_4;

        using phoenix::val;
        using phoenix::ref;
        using phoenix::push_back;
        using phoenix::clear;
        using phoenix::construct;
        using phoenix::let;
        using phoenix::if_;
        using phoenix::end;
        using phoenix::at_c;
        using phoenix::size;



        path_command =
                (
                absolute_path
                |
                relative_path
                )
                ;


        absolute_path =
                lit("/")                                        [ fillTopMatchingNode(matched_nodes_, top_node_) ]
                >>
                path_segment
                >>
                *(
                lit("/")
                >>
                path_segment
                )
                ;


         relative_path =
            ( lit( "*" )                                        [ populate_relative_path( matched_nodes_, top_node_ ) ]
              |
              segment_name                                      [ populate_relative_path( matched_nodes_, top_node_ ) ]
               )
            >>
            -( attribute_selection                              [ attribute_handler( matched_nodes_ ) ]      )
            >>
            *( 
               lit( "/" )
               >>
               path_segment 
               )
            ;

        path_segment =
                (lit("*")                                       [ find_next_set_of_nodes(matched_nodes_, '*') ]
                |
                segment_name                                    [ find_next_set_of_nodes(matched_nodes_) ]
                )
                >>
                -(indexing                                      [ find_next_set_of_nodes(matched_nodes_) ])
                >>
                -(attribute_selection                           [ attribute_handler(matched_nodes_) ])
                ;



        segment_name %=
                raw[ lexeme[ alpha >> *(alnum | '_' | ':' | '-') ] ]
                ;


        indexing =
                lit("[")
                >>
                (
                int_                                            [ _val = _1 ]
                |
                lit("$")                                        [ _val = matched_nodes_.size() - 1  ]
                )
                >>
                lit("]")
                ;

        attribute_selection =
                lit("@")
                >>
                attribute_name                                  [ at_c < 0 > (_val) = _1 ]
                >>
                -(
                lit("='")
                >>
                attribute_value                                 [ at_c < 1 > (_val) = _1 ]
                >>
                lit("'")
                )
                ;

        attribute_name %=
                raw[ lexeme[ alpha >> *(alnum | '_' | '-') ] ]
                ;

        attribute_value %=
                raw[ lexeme[ *(char_ - '\'') ] ]
                ;



        // --------------------- debug  ----------------------
        BOOST_SPIRIT_DEBUG_NODE(path_command);
        BOOST_SPIRIT_DEBUG_NODE(absolute_path);
        BOOST_SPIRIT_DEBUG_NODE(relative_path);
        BOOST_SPIRIT_DEBUG_NODE(path_segment);
        BOOST_SPIRIT_DEBUG_NODE(segment_name);
        BOOST_SPIRIT_DEBUG_NODE(indexing);
        //BOOST_SPIRIT_DEBUG_NODE( attribute_selection );
        BOOST_SPIRIT_DEBUG_NODE(attribute_name);
        BOOST_SPIRIT_DEBUG_NODE(attribute_value);

        path_command.name("path_command");
        absolute_path.name("absolute_path");
        relative_path.name("relative_path");
        path_segment.name("path_segment");
        segment_name.name("segment_name");
        attribute_selection.name("attribute_selection");
        attribute_name.name("attribute_name");
        attribute_value.name("attribute_value");
        // ---------------------------------------------------


        // ---------------- failure handler ------------------
        on_error< qi::fail >
                (
                path_command
                , std::cout
                << val("Error! Expecting ")
                << _4 // what failed?
                << val(" here: \"")
                << construct<std::string > (_3, _2) // iterators to error-pos, end
                << val("\"")
                << std::endl
                );

        on_error< qi::fail >
                (
                absolute_path
                , std::cout
                << val("Error! Expecting ")
                << _4 // what failed?
                << val(" here: \"")
                << construct<std::string > (_3, _2) // iterators to error-pos, end
                << val("\"")
                << std::endl
                );

        // ---------------------------------------------------
    }


    qi::rule< Iterator, ascii::space_type >
            path_command,
            absolute_path,
            relative_path,
            path_segment;

    qi::rule< Iterator, std::pair< std::string, std::string > (), ascii::space_type >
            attribute_selection;

    qi::rule< Iterator, int(), ascii::space_type >
            indexing;

    qi::rule< Iterator, std::string(), ascii::space_type >
            segment_name,
            attribute_name,
            attribute_value;


    var& top_node_;
    var_container& matched_nodes_;
};


#endif  // JSON_MATCH_GRAMMAR_H

/**
 *   \file json_grammar.h
 *   Copyright (c) 2014 Arvind Umrao< akumrao@yahoo.com>
 *
 *   \brief json utility class for json path
 */

#ifndef OMD_JSON_UTIL_IMPL_H
#define OMD_JSON_UTIL_IMPL_H

#define BOOST_SPIRIT_DONT_USE_MPL_ASSERT_MSG 1

#include <vector>
#include <string>
#include <boost/exception/error_info.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>

#include "OMDTypes.h"
#include "omd/Exception.h"
#include "omd/detail/json_match_grammar.h"


namespace omd
{
   namespace json
   {
      typedef boost::error_info< struct json_error_path, std::string > json_path_info;

      /**
       *   \brief jsonPath like matching for omd::json AST variations
       *
       *   Performs jsonPath like matching with an var.
       *
       *   Supports the following constructs:
       *
       *         "/path"                 -  absolute path from root
       *         "path"                  -  relative paths that match any occurance
       *         "/path/ *"               -  wildcard matching
       *         "/path/foo[2]           -  indexing (third child of path named foo)
       *         "/path/foo[$]           -  last of ( last child of path named foo)
       *         "/path/ *[0]             -  first child of path
       *         "/path/ *[$]             -  last child of path
       *         "foo/bar"               -  all bar nodes that have a parent foo
       *         "foo/bar@gorp           -  attribute matching (all bar nodes that have a 
       *                                    parent foo and an attribute of gorp)
       *         "foo/bar@gorp='floop'"  -  attribute matching (all bar nodes that have a 
       *                                    parent foo and an attribute of gorp that is set to floop )
       *         "*@xid"                 -  all nodes that have an attribute of xid
       *         "*@xid='1234'"          -  all nodes that have an attribute of xid equal to 1234
       *
       *
       *  @param path the jsonPath like description of the path to the desired node(s)
       *  @param top_node the parent node to start searching in
       *  @return a container of the matched nodes
       */

     
      var_container match_( const std::string& path,
                                     var& top_node )
      {
         var_container matched_nodes;
         json_match_grammar<std::string::const_iterator> grammar( top_node, matched_nodes );

         std::string::const_iterator path_begin = path.begin();
         std::string::const_iterator path_end = path.end();
         
         /*bool parse_result =*/  boost::spirit::qi::phrase_parse( path_begin, path_end, 
                                                                   grammar, 
                                                                   boost::spirit::ascii::space );
         
         
        // std::cout << "match_: "  << matched_nodes[0]  << std::endl << std::flush;
         return matched_nodes;
      }



      /**
       *  Removes all matching nodes. Nodes are actually unlinked.
       *
       *  @param path the jsonPath like description of the nodes to remove
       *  @param top_node the parent node to apply path to
       */
     
      void remove_( const std::string& path,
                   var &top_node)
        {
            using namespace boost::phoenix;
            using namespace boost::phoenix::arg_names;

            var_container matched_nodes = omd::json::match(path, top_node);
            
            node_container_iter iter = matched_nodes.begin();
            node_container_iter iter_end = matched_nodes.end();
            while (iter != iter_end)
            {
                var& obj = (var&) (*iter);
                
                obj.erase();
                ++iter;
            }
      }


      /**
       *  Adds the specified node to the parent described by path. Failsmatched_nodes
       *  with an exception if path does not describe a singular node.
       *
       *  If the parent already has children the new node will be added
       *  at the end of the child list.
       *
       *  @param path jsonPath like description to the parent node
       *  @param top_node the parent node to apply path to
       *  @param node the node to add
       */

        void add_(const std::string& path,
                var& top_node,
                var& node_to_add)
        {
            using namespace boost::phoenix;
            using namespace boost::phoenix::arg_names;

            var_container matched_nodes = omd::json::match(path, top_node);

            if (matched_nodes.size() != 1)
            {
                throw omd::Exception() << omd::json::json_path_info(path);
            }


            node_container_iter iter = matched_nodes.begin();
            node_container_iter iter_end = matched_nodes.end();
            while (iter != iter_end)
            {
                var& obj = (var&) (*iter);

                //obj.erase();
                ++iter;
            }

        }



      /**
       *  Moves the specified node to the parent described by path. Fails
       *  with an exception if path does not describe a singular node.
       *
       *
       *  @param path jsonPath like description to the parent node
       *  @param top_node the parent node to apply path to
       *  @param node the node to move
       */
     
      void move_( const std::string& path, 
                 var& top_node,
                 var& node_to_move)
        {
            using namespace boost::phoenix;
            using namespace boost::phoenix::arg_names;

            var_container matched_nodes = omd::json::match(path, top_node);

            if (matched_nodes.size() != 1)
            {
                throw omd::Exception() << omd::json::json_path_info(path);
            }

            node_container_iter iter = matched_nodes.begin();
            node_container_iter iter_end = matched_nodes.end();
            while (iter != iter_end)
            {
                var& obj = (var&) (*iter);

                obj.swap(node_to_move);
                ++iter;
            }


        }


      /**
       *  Will insert the specified node before the node described by path. Fails
       *  with an exception if path does not describe a singular node.
       *
       *  @param path jsonPath like description to the node to be inserted before
       *  @param top_node the parent node to apply path to
       *  @param node the node to insert
       */
     
      void insert_before_( const std::string& path, 
                          var top_node,
                          var node_to_insert )
      {
         using namespace boost::phoenix;
         using namespace boost::phoenix::arg_names;

         var_container matched_nodes = omd::json::match( path, top_node );

         if( matched_nodes.size() != 1 )
         {
            throw omd::Exception() << omd::json::json_path_info( path );
         }

         var& matched_node = matched_nodes[ 0 ];

         // if the name is empty it is and indicator to add children
         if( node_to_insert.getKey() == "" )
         {
            /*
             * /var_container child_nodes = node_to_insert.getNodes();
            if( ! child_nodes.empty() )
            {
               std::for_each( child_nodes.begin(),
                              child_nodes.end(),
                              bind( &var::insertBefore, matched_node, arg1 ) );
            }
             */
         }
         else
         {
            //matched_nodes[ 0 ].insertBefore( node_to_insert );
         }
      }


      /**
       *  Will insert the specified node after the node described by path. Fails
       *  with an exception if path does not describe a singular node.
       *
       *  @param path jsonPath like description to the node to be inserted after
       *  @param top_node the parent node to apply path to
       *  @param node the node to insert
       */
     
      void insert_after_( const std::string& path, 
                         var top_node,
                         var node_to_insert )
      {
         using namespace boost::phoenix;
         using namespace boost::phoenix::arg_names;

         var_container matched_nodes = omd::json::match( path, top_node );

         if( matched_nodes.size() != 1 )
         {
            throw omd::Exception() << omd::json::json_path_info( path );
         }

         var& matched_node = matched_nodes[ 0 ];

         // if the name is empty it is and indicator to add children
         if( node_to_insert.getKey() == "" )
         {
           /*
            
             var_container child_nodes = node_to_insert.getNodes();
            std::for_each( child_nodes.begin(),
                           child_nodes.end(),
                           (
                              bind( &var::insertAfter, ref( matched_node ), arg1 ),
                              std::cout << arg1 << "\n"
                              )
                              );
            */
         }
         else
         {
            //matched_nodes[ 0 ].insertAfter( node_to_insert );
         }
      }
   }
}


#endif  // OMD_JSON_UTIL_IMPL_H



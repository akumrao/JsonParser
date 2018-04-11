/**
 *   \file json_util
 *   Copyright (C) 2009, 2010 Object Modeling Designs
 */

#ifndef OMD_JSON_UTIL_H
#define OMD_JSON_UTIL_H

#include <vector>
#include <string>
#include "OMDTypes.h"
#include "omd/Exception.h"

#include "omd/var.h"



#ifdef OMD_JSON_SOURCE
#define OMD_JSON_DECL LIBEXPORT
#else
#define OMD_JSON_DECL LIBIMPORT
#endif


namespace omd
{
   namespace json
   {
      typedef boost::error_info< struct json_error_path, std::string > json_path_info;

      /**
       *   \brief xpath like matching for omd::json AST variations
       *
       *   Performs XPath like matching with an var.
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
       *  @param path the xpath like description of the path to the desired node(s)
       *  @param top_node the parent node to start searching in
       *  @return a container of the matched nodes
       */

      OMD_JSON_DECL var_container match( const std::string& path, omd::var& top_node );
      //OMD_JSON_DECL std::vector< omd::XMLRefNode > match( const std::string& path, omd::XMLRefNode top_node );


      /**
       *  Removes all matching nodes. Nodes are actually unlinked.
       *
       *  @param path the xpath like description of the nodes to remove
       *  @param top_node the parent node to apply path to
       */
      OMD_JSON_DECL void remove( const std::string& path, omd::var& top_node );
      //OMD_JSON_DECL void remove( const std::string& path, omd::XMLRefNode top_node );



      /**
       *  Adds the specified node to the parent described by path. Fails
       *  with an exception if path does not describe a singular node.
       *
       *  If the parent already has children the new node will be added
       *  at the end of the child list.
       *
       *  @param path xpath like description to the parent node
       *  @param top_node the parent node to apply path to
       *  @param node the node to add
       */
      OMD_JSON_DECL void add( const std::string& path, omd::var& top_node, omd::var& node_to_add );
      //OMD_JSON_DECL void add( const std::string& path, omd::XMLRefNode top_node, omd::XMLRefNode node_to_add );



      /**
       *  Moves the specified node to the parent described by path. Fails
       *  with an exception if path does not describe a singular node.
       *
       *
       *  @param path xpath like description to the parent node
       *  @param top_node the parent node to apply path to
       *  @param node the node to move
       */
      OMD_JSON_DECL void move( const std::string& path, omd::var& top_node, omd::var& node_to_move );
      //OMD_JSON_DECL void move( const std::string& path, omd::XMLRefNode top_node, omd::XMLRefNode node_to_move );



      /**
       *  Will insert the specified node before the node described by path. Fails
       *  with an exception if path does not describe a singular node.
       *
       *  @param path xpath like description to the node to be inserted before
       *  @param top_node the parent node to apply path to
       *  @param node the node to insert
       */
      OMD_JSON_DECL void insert_before( const std::string& path, omd::var& top_node, omd::var& node_to_insert );
      //OMD_JSON_DECL void insert_before( const std::string& path, omd::XMLRefNode top_node, omd::XMLRefNode node_to_insert );



      /**
       *  Will insert the specified node after the node described by path. Fails
       *  with an exception if path does not describe a singular node.
       *
       *  @param path xpath like description to the node to be inserted after
       *  @param top_node the parent node to apply path to
       *  @param node the node to insert
       */
      OMD_JSON_DECL void insert_after( const std::string& path, omd::var& top_node, omd::var& node_to_insert );
      //OMD_JSON_DECL void insert_after( const std::string& path, omd::XMLRefNode top_node, omd::XMLRefNode node_to_insert );

   }
}


#endif  // OMD_XML_UTIL_H



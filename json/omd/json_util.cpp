/**
 *   \file json_grammar.h
 *   Copyright (c) 2014 Arvind Umrao< akumrao@yahoo.com>
 *
 *   \brief json utility class for json path
 */

#define OMD_JSON_SOURCE


#include "omd/json_util.h"
#include "omd/detail/json_util_impl.h"
#include "omd/var.h"


omd::var_container omd::json::match( const std::string& path, omd::var& top_node )
{
   return omd::json::match_( path, top_node );
}

// std::vector< omd::XMLRefNode > omd::json::match( const std::string& path, omd::XMLRefNode top_node )
// {
//    return omd::json::match< omd::XMLRefNode >( path, top_node );
// }


void omd::json::remove( const std::string& path, omd::var& top_node )
{
   omd::json::remove_( path, top_node );
}

// void omd::json::remove( const std::string& path, omd::XMLRefNode top_node )
// {
//    omd::json::remove< omd::XMLRefNode >( path, top_node );
// }


void omd::json::add( const std::string& path, omd::var& top_node, omd::var& node_to_add )
{
   omd::json::add_( path, top_node, node_to_add );
}

// void omd::json::add( const std::string& path, omd::XMLRefNode top_node, omd::XMLRefNode node_to_add )
// {
//    omd::json::add< omd::XMLRefNode >( path, top_node, node_to_add );
// }


void omd::json::move( const std::string& path, omd::var& top_node, omd::var& node_to_move )
{
   omd::json::move_( path, top_node, node_to_move );
}


void omd::json::insert_before( const std::string& path, omd::var& top_node, omd::var& node_to_insert )
{
   omd::json::insert_before_( path, top_node, node_to_insert );
}



void omd::json::insert_after( const std::string& path, omd::var& top_node, omd::var& node_to_insert )
{
   omd::json::insert_after_( path, top_node, node_to_insert );
}




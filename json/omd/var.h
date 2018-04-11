/**
 *  \file var.h
 *
 *  Copyright (c) 2014 Arvind Umrao<akumrao@yahoo.com>
 */

#ifndef VARNODE_H
#define VARNODE_H



#include "OMDTypes.h"

#include <boost/spirit/include/support_utree.hpp>

/*
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
*/

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/function.hpp>



#ifdef WIN32
#pragma warning( push )
#pragma warning( disable : 4251 4231 4275 )
#endif


#define OMD_JSON_ENABLE_BOOST_SERIALIZATION



#ifdef OMD_JSON_SOURCE
#define OMD_JSON_DECL LIBEXPORT
#else
#define OMD_JSON_DECL LIBIMPORT
#endif

namespace omd
{

    /// Class supporting a nice easy way to access json like data
    /**
     *  The var provides an easy way to access json like data. The var
     *  has attributes, a parent node and children nodes. The object, array and attributes
     *  can be accessed in a variety of ways.
     *
     *
     */




    namespace spirit = boost::spirit;

    using boost::spirit::nil;
    using boost::spirit::utree;

    class var;
    typedef std::vector<  boost::reference_wrapper <var> > var_container;
    
    class OMD_JSON_DECL var : public spirit::utree
    {
    public:

       
        void tab(int indent, std::ostream& os);
        void print_list(utree &list, int indent, const char* delimeter, std::ostream& os, bool tab = false);

        std::ostream& print_utree(var& ut, int indent, std::ostream& os);

        var& operator=(nil_type const& other)
        {
            spirit::utree::operator=(other);
            return *this;
        }

        var& operator=(bool const& other)
        {
            spirit::utree::operator=(other);
            return *this;
        }

        var& operator=(char const* other)
        {
            spirit::utree::operator=(other);
            return *this;
        }

        var& operator=(std::string const& other)
        {
            spirit::utree::operator=(other);
            return *this;
        }

        var& operator=(spirit::utree const& other)
        {
            spirit::utree::operator=(other);
            return *this;
        }

        var& operator=(int const& other)
        {
            spirit::utree::operator=(other);
            return *this;
        }
        
        var& operator=(double const& other)
        {
            spirit::utree::operator=(other);
            return *this;
        }


        var(nil_type const& other) : spirit::utree(other)
        {
        }

        var(bool const& other) : spirit::utree(other)
        {
        }

        var(char const* other) : spirit::utree(other)
        {
        }

        var(std::string const& other) : spirit::utree(other)
        {
        }

        var(spirit::utree const& other) : spirit::utree(other)
        {
        }

        var(int const& other) : spirit::utree(other)
        {
        }
        
         var(double const& other) : spirit::utree(other)
        {
        }

        var() : spirit::utree()
        {
        }

        
        bool operator==(const var& other) const
        {
            if( other.tag() >= 100 ||  this->tag() >= 100 )
            {
               return (other.string() == this->string());
            }
           
            else return ( (utree&) *this ==  (utree&) other) ;
           
        }
        

        var& getNode(const std::string name) const;
        
        
        var_container getNodes(const std::string name) const;

        var& operator[](int const& other);

        var& operator[](std::string const& other);
        bool equal1(const var& other) const;
         bool equal(const std::string other) const;
        
        void erase()
        {
            spirit::utree::clear();
        }

       /* void push_back( var& other)
        {
            spirit::utree::push_back(other);
        }*/
        
        std::string getKey()  const;;
        var& getValue()  const;;
        
        
        int& getValue( int& val);
        std::string& getValue( std::string& val);
        
       

        void deleteNode()
        {
            spirit::utree::clear();
        }

        void addtoArray(var node);
        void addtoArray( var node, int pos);
        
        void addtoObject(std::string name, var node);
        
        void addtoObject(std::string name, var node, int pos);

        std::string string() const
        {
            std::stringstream s;
            s << (var&)*this;
            return s.str();
        }


    private:
        //friend std::ostream& operator<<(std::ostream& out, var& x);

    };

    class operate1
    {
    public:

	typedef void result_type;


        operate1(int op) : op(op)
        {
        }

        void operator()(var& v, std::string &first, var& vt1) const
        {
            v.tag(op);
            v.push_back(first);
            v.push_back(vt1);
        }

        void pair(var& v, std::string &first, var& vt1) const
        {
            v.tag(op);
            v.push_back(first);
            v.push_back(vt1);
        }


        int const op;
    };

    class operate2
    {
    public:

	 typedef void result_type;


        operate2(int op) : op(op)
        {
        }


        void operator()(var& v, var& vt1) const
        {
            v.tag(op);
            v.push_back(vt1);
        }
        int const op;
    };


    int const tabsize = 4;

    boost::phoenix::function<operate1> const pairit = operate1(100);
    boost::phoenix::function<operate2> const arrayit = operate2(101);
    boost::phoenix::function<operate2> const objectit = operate2(102);

    
    OMD_JSON_DECL std::ostream& operator<<(std::ostream& out, var& node);
    

}


#include "JSONReader.h"




#ifdef WIN32
#pragma warning( pop )
#endif

#endif  // VARNODE_H

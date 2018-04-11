
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright 2014 Arvind Umrao<akumrao.yahoo.com>
//
// File: var.cpp
#define OMD_JSON_SOURCE
/************************************************************************************
object  =       "{" member ("," member)* "}"
member  =       string ":" value
string  =       '"' character* '"'
value   =       string | number | object | array | "true" | "false" | "null"
number  =       integer | real
array   =       "[" value ("," value)* "]"
character =       "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z"

        
****************************************************************************************/
#ifdef OMD_JSON_SOURCE
#define OMD_JSON_DECL LIBEXPORT
#else
#define OMD_JSON_DECL LIBIMPORT
#endif

#include "var.h"

namespace omd
{

     using boost::spirit::utf8_string_range_type;

   
    
     var& var::operator[](std::string const& other)
     {
         return getNode(other);
     }
    
    var& var::operator[](int const& other)
    {
        
        if (this->tag() != 100) // arvind need to work
        {
            using boost::spirit::get;
            var& b =  (var&)  get( *this, other);

            return  b;
         }
        
        throw  "Use getKey and getValue function to read object's member";
  
        
    }
    
    void var::addtoArray( var node)
    {
        if ( this->tag() ==  101 )
        {
        
            this->push_back( node );
        }
        else
        {
              throw "Wrong node, check the path";
        }
        
    }
    
    void var::addtoArray( var node, int pos)
    {
        
        utree::iterator i = this->begin();
        for( int count = 0; count < pos ; ++count)
        ++i;
         
        if ( this->tag() ==  101 )
        {
            this->insert(i, node);
        }
        else
        {
              throw "Wrong node, check the path";
        }
        
    }
    
    void var::addtoObject( std::string name,  var node)
    {
        using namespace boost::phoenix::arg_names;
            
        if ( this->tag() ==  102 )
        {
             var pair;
             pairit(arg1, arg2, arg3)(pair, name , node);
             
             this->push_back(pair);
        }
        else
        {
              throw "Wrong node. It is not the object node path";// << node.string();
        }
    }
    
    void var::addtoObject(std::string name, var node, int pos)
    {
        
        utree::iterator i = this->begin();
        
        for( int count = 0; count < pos ; ++count)
        ++i;
        
        using namespace boost::phoenix::arg_names;
            
        if ( this->tag() ==  102 )
        {
             var pair;
             pairit(arg1, arg2, arg3)(pair, name , node);
             
             this->insert(i, pair);
        }
        else
        {
              throw "Wrong node. It is not the object node path";// << node.string();
        }
    
    }

    std::string var::getKey() const
    {
        if (this->tag() == 100)
        {
            var &pair = (var &) (*this);
            
            using boost::spirit::get;
            var& key =  (var&)  get( pair, 0);
            utf8_string_range_type rt = key.get<utf8_string_range_type > ();
            std::string str(rt.begin(), rt.end()); 
            return str;
        }
        
          throw "Not a valid object member";
        return "";
    }

    var& var::getValue() const
    {
        if (this->tag() == 100)
        {
            var &pair = (var &) (*this);
            
            using boost::spirit::get;
            var& b =  (var&)  get( pair, 1);
            return  b;

        }
        
        throw "Not a valid object member";
        
        return (var&) nil;
    }

    var_container var::getNodes(const std::string name) const
    {
        var_container ret;
        var &obj = (var &) * this;
        for (var::size_type j = 0; j < obj.size(); ++j)
        {
            var &pair = (var &) obj[j];
            if( pair.size() == 2 && pair.tag() ==  100 )
            {
                var& attribute_value = pair.getValue();
                std::string attribute = pair.getKey();
                if (  attribute ==  name || name == "*" )
                {
                    ret.push_back(boost::ref(attribute_value));
                }
            }
        }
        return ret;
    }
    

        
    var& var::getNode(const std::string name) const
    {
        var &obj = (var &) * this;
        for (var::size_type j = 0; j < obj.size(); ++j)
        {
            var &pair = (var &) obj[j];

            
            if( pair.size() == 2 && pair.tag() ==  100 )
            {   
                var& attribute_value = pair.getValue();
                std::string attribute = pair.getKey();
            
                if (attribute ==  name )
                {
                     return attribute_value ;
                }
            }
        }
        throw "Wrong node, check the path";
        return (var&) nil;
    }
 
   
    void var::tab( int indent,  std::ostream& os)
    {
        for (int i = 0; i < indent; ++i)
            os << ' ';
    }

    int& var::getValue(int& val)
    {
        val =  this->get<int>();
        return val;
    }

    std::string& var::getValue(std::string& val)
    {
          utf8_string_range_type rt = this->get<utf8_string_range_type > ();
          val = std::string(rt.begin(), rt.end()); 
        return val;

    }
       
        
    void var::print_list( utree &list, int indent, const char* delimeter, std::ostream& os , bool tb)
    {
        utree::ref_iterator it = list.ref_begin();
        utree::ref_iterator end = list.ref_end();
        while (true)
        {
            var vr(*it);
            vr.print_utree(vr, indent , os);
            
            ++it;
            if(it != end )
            {
                os <<  delimeter ;
                if(tb )
                tab(indent , os);
            }
            else
                break;
        }
    }

     
     bool var::equal(const std::string other) const
    {
        
        if (this->string() == other )
        {
            return true;
        }
        else
        {
           return false;
        }
    }
    std::ostream& var::print_utree(var& ut , int indent,  std::ostream& os)
    {
        typedef spirit::utree utree;
        typedef spirit::utree_type utree_type;
        utree_type::info which = ut.which();
        short tag = ut.tag();
        
        //for complex data 
        if(tag == 100) // pair
        {
            print_list(ut , indent, ": ", os);
        }
        else if(tag == 101) // array
        {
            os << '[';
            print_list(ut , indent, ", " , os);
            os << ']';
        }
        else if(tag == 102) // object
        {
           
           os << '{' << std::endl;
           tab(indent + tabsize , os);
           
            print_list(ut , indent + tabsize, ",\n" , os, true);
            
            os  << std::endl;
          tab(indent , os);
          os << '}' ;//<< std::endl;
        } 
        else // for simple data
        {
            switch (which)
            {
                case utree_type::invalid_type:
                {
                    break;
                }

                case utree_type::nil_type:
                {
                    os << "null";
                    break;
                }

                case utree_type::list_type:
                {
                    print_list(ut, indent, " ",  os);
                    break;
                }

                case utree_type::range_type:
                {
                    break;
                }

                case utree_type::reference_type:
                {
                    break;
                }

                case utree_type::any_type:
                {
                    break;
                }

                case utree_type::function_type:
                {
                    break;
                }

                case utree_type::string_range_type:
                {
                    break;
                }

                case utree_type::bool_type:
                {
                    os << (ut.get<bool>()? "true" : "false");
                    break;
                }

                case utree_type::int_type:
                {
                    os << ut.get<int>();
                    break;
                }

                case utree_type::double_type:
                {
                    os << ut.get<double>();
                    break;
                }

                case utree_type::binary_type:
                {
                    break;
                }

                case utree_type::string_type:
                {
                    utf8_string_range_type r = ut.get<utf8_string_range_type > ();
                    os<< "\"" << r << '\"' ;
                    break;
                }

                case utree_type::symbol_type:
                {
                    break;
                }
            }
        }
        return os;
    }

    
OMD_JSON_DECL std::ostream& operator<<(std::ostream& out, var& node)
{
    int indent =  0;
    node.print_utree(node, indent, out);
    return out;
}



 
        

}  // namespace omd





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

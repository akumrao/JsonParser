/**
 *   \file json_grammar.h
 *   Copyright (c) 2014 Arvind Umrao< akumrao@yahoo.com>
 *
 *   \brief test case for json parser 
 */

#include "omd/var.h"

using namespace  omd;

int main(int argc, char **argv)
{
   char const* filename;
   if (argc > 1)
   {
      filename = argv[1];
   }
   else
   {
      std::cerr << "Error: No input file provided." << std::endl;
      return 1;
   }


    var val=99;
    
    bool b =  (val == 99);
    
   std::cout << val << std::endl;
        
    val.erase();
    val = "Arvind";
    b =  (val == "Arvind");
    
    b =  (val == "Umrao");
    
    std::string test;
    
    b= test.empty();
    
    test = "sdfds";
    b = test.empty();
    
    
    val.erase();
         
    val.push_back(1);
    val.push_back(2);
    val.push_back(3);
    val.push_back(4);
    
     std::cout << val[0] << std::endl;

    val.erase();
    
     val=99;
    
     val.erase();
    
    val.push_back(1);
    val.push_back(2);
    

    
    val[0] =   "testing";
    
 

    std::cout << val << std::endl;
    
   
}



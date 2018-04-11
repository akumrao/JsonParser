/**
 *   \file json_grammar.h
 *   Copyright (c) 2014 Arvind Umrao< akumrao@yahoo.com>
 *
 *   \brief test case for json parser 
 */

#include "omd/var.h"
#include "omd/json_util.h"

using namespace omd;

int main(int argc, char **argv)
{



    char const* filename;
    if (argc > 1)
    {
        filename = argv[1];
    } else
    {
        std::cerr << "Error: No input file provided." << std::endl;
        return 1;
    }

    var ast;

    if (!omd::json::read_file(filename, ast))
    {
        return -1;
    }





    var obj = "{ \"result\":true,\"count\":1 }";
    var &jsonObj = json::parse(obj);


    std::cout << jsonObj << std::endl << std::flush;

    var arr = "[1,2,3]";

    var &jsonArr = json::parse(arr);

    std::cout << jsonArr << std::endl << std::flush;

    jsonObj.addtoObject("arr", jsonArr);

    std::cout << jsonObj << std::endl << std::flush;

    std::string jsonPath = "*@count='1'";

    const var_container& xid_nodes = json::match(jsonPath, jsonObj);

    var& values = xid_nodes[0];

    std::cout << "Matched" << values <<  std::endl << std::flush;


   




    return 0;

}



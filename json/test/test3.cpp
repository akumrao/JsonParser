/**
 *   \file json_grammar.h
 *   Copyright (c) 2014 Arvind Umrao< akumrao@yahoo.com>
 *
 *   \brief test case for json parser 
 */


#include "omd/var.h"
#include "omd/json_util.h"
using namespace omd;

void print(var i)
{
    std::cout << " print :" << i << std::endl;
}

void testPrint(const var_container& test)
{
    // (var_container&)test.push_back( boost::ref("test"));
    std::cout << " print :" << test[0] << std::endl;
}

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


    var store;


    var &test = ast["store"];

    store.push_back(boost::ref(ast));

    std::cout << "test[0]" << test[0] << "test[1]" << test[1] << std::endl << std::flush;

    var_container vtb = ast.getNodes("store");

    var& vtStore = vtb[0];

    vtb.clear();

    vtb = vtStore.getNodes("bicycle");

    std::cout << "store" << vtb[0] << std::endl << std::flush;

    const var_container& vtb1 = ast["store"].getNodes("bicycle");

    testPrint(vtb1);

    ast.addtoObject("test", "test");

    ast["store"]["bicycle"].addtoObject("test", "test");

    std::cout << "ast" << ast << std::endl << std::flush;

    std::cout << "store" << vtb[0] << std::endl << std::flush;


    {

        std::cout << "About to Match 1" << std::endl << std::flush;
        std::string path = "/store/bicycle";

        const var_container& xid_nodes = omd::json::match(path, ast);


        std::cout << "Matched" << (int) xid_nodes.size() << xid_nodes[0] << std::endl << std::flush;

        var& values = xid_nodes[0];

        std::cout << "values" << values[0] << std::endl << std::flush;

        values[2].getValue() = "working1";

        std::cout << "ast" << ast << std::endl << std::flush;

    }


    {
        std::cout << "About to Match2" << std::endl << std::flush;


        std::string path = "bicycle";

        const var_container& xid_nodes = omd::json::match(path, ast);

        std::cout << "Matched" << (int) xid_nodes.size() << xid_nodes[0] << std::endl << std::flush;

        var& values = xid_nodes[0];

        std::cout << "values" << values;

        values[2].getValue() = "working2";

        std::cout << "ast" << ast << std::endl << std::flush;

    }


    {
        std::cout << "About to Match3" << std::endl << std::flush;


        std::string path = "price";

        const var_container& xid_nodes = omd::json::match(path, ast);

        std::cout << "size " << (int) xid_nodes.size() << " value " << xid_nodes[0] << std::endl << std::flush;

        var& values = xid_nodes[0];

        std::cout << "ast" << ast << std::endl << std::flush;

    }

    {
        std::cout << "About to Match4" << std::endl << std::flush;
        std::string path = "/store/bicycle[2]";

        const var_container& xid_nodes = omd::json::match(path, ast);

        var& values = xid_nodes[0];

        std::cout << "values" << values << " [[1] " << values[1];

        values[2].getValue() = "working4";

        std::cout << "ast" << ast << std::endl << std::flush;

    }



    {
        std::cout << "About to Match5" << std::endl << std::flush;
        std::string path = "/store/bicycle@price";

        const var_container& xid_nodes = omd::json::match(path, ast);

        var& values = xid_nodes[0];

        std::cout << "values" << values << " [1] " << values << std::endl << std::flush;
        ;

        values.getValue() = 99.01;

        std::cout << "ast" << ast << std::endl << std::flush;

    }



    {
        std::cout << "About to Match6" << std::endl << std::flush;
        std::string path = "*@price";

        const var_container& xid_nodes = omd::json::match(path, ast);

        var& values = xid_nodes[0];

        std::cout << "values" << values << " [size] " << values.size() << std::endl << std::flush;

        values.getValue() = 14.01;

        std::cout << "ast" << ast << std::endl << std::flush;

    }

    {

        std::cout << "About to Match 7" << std::endl << std::flush;
        std::string path = "*/*[1]/price";
        const var_container& xid_nodes = omd::json::match(path, ast);
        std::cout << "Matched" << (int) xid_nodes.size() << xid_nodes[0] << std::endl << std::flush;
        var& values = xid_nodes[0];
        values = 9999;
        std::cout << "ast" << ast << std::endl << std::flush;

    }


    {

        std::cout << "About to Match8" << std::endl << std::flush;
        std::string path = "*@price";

        const var_container& xid_nodes = omd::json::match(path, ast);

        var& values = xid_nodes[0];

        std::cout << "values" << values << " [size] " << xid_nodes.size() << std::endl << std::flush;

        values.getValue() = 99.99;

        std::cout << "ast" << ast << std::endl << std::flush;

    }


    {

        std::cout << "About to Match9" << std::endl << std::flush;
        std::string path = "*@price='99.99'";

        const var_container& xid_nodes = omd::json::match(path, ast);

        var& values = xid_nodes[0];

        std::cout << "values" << values << " [size] " << xid_nodes.size() << std::endl << std::flush;

        values.getValue() = 88.88;

        std::cout << "ast" << ast << std::endl << std::flush;

    }

    return 0;

}



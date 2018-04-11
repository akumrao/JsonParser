
#include "omd/var.h"

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

    if( !omd::json::read_file(filename, ast))
    {
        return -1;
    }


    var node = ast.getNode("glossary");

    std::cout << node << std::endl;

    node.erase();

    node = ast["glossary"]["title"];

    std::cout << node << std::endl;

    node.erase();

    node = ast["glossary"]["GlossDiv"]["title"];

    std::cout << node << std::endl;
    
    
   node.erase();

    node = ast["glossary"]["GlossDiv"][0];

    std::cout <<"[glossary][GlossDiv][0] = " << node << std::endl;


    node.erase();

    var& node1 = ast["glossary"]["GlossDiv"]["title"];

    node1 = "M";

    std::cout << ast << std::endl;

    node.erase();

    var& node2 = ast["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"]["GlossDef"]["GlossSeeAlso"];

    node2.addtoArray("test");
    node2.addtoArray("beginTest", 1);

    var &prt = ast["glossary"]["GlossDiv"]["GlossList"]["GlossEntry"];

    std::cout << std::endl << prt << std::endl;

    prt.addtoObject("newID", "ID_12343");
    prt.addtoObject("newID", "ID_000", 0);

    std::cout << ast;
    
    return 0;

}



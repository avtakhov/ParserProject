#include "params_parser.h"
#include "parser_exceptions.h"

using namespace std;

map<string, string> ParseParams(const string& params)
{
    // Your implementation here
    throw ParsingException("Not implemented yet", params, ParamsChunk{0, params.size()});
}


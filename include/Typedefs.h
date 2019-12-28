#ifndef TYPEDEFS_H_INCLUDED
#define TYPEDEFS_H_INCLUDED

#include <utility>  // std::pair
#include <cstdint>  // uint32_t
#include <memory>   // smart pointers
#include <list>
#include <map>

using namespace std;

class FunctionBlock;
class FunctionCall;
class FunctionDefinition;
class ParsedFile;
class RowedFile;
class Token;


using Pos                           = pair<uint32_t, uint32_t>;

using FunctionDefinitionPtr         = shared_ptr<FunctionDefinition>;

using FunctionCallPtr               = shared_ptr<FunctionCall>;
using FunctionCallListPtr           = shared_ptr<list<FunctionCallPtr>>;

using ParsedFilePtr                 = shared_ptr<ParsedFile>;
using ParsedFileListPtr             = shared_ptr<list<ParsedFilePtr>>;

using RowedFilePtr                  = shared_ptr<RowedFile>;

using StringListPtr                 = shared_ptr<list<string>>;

using TokenListIter                 = list<Token>::iterator;
using TokenListPtr                  = shared_ptr<list<Token>>;

using FunctionBlockListPtr          = shared_ptr<list<FunctionBlock>>;

using CallsQueuePair                = pair<string, ParsedFilePtr>;

#endif // TYPEDEFS_H_INCLUDED

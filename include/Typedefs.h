#ifndef TYPEDEFS_H_INCLUDED
#define TYPEDEFS_H_INCLUDED

#include <utility>  // std::pair
#include <cstdint>  // uint32_t
#include <memory>   // smart pointers
#include <list>
#include <map>

using namespace std;

class FunctionBlock;
class FunctionInfo;
class ParsedFile;
class RowedFile;
class Token;


using Pos                   = pair<uint32_t, uint32_t>;

using FunctionInfoPtr       = shared_ptr<FunctionInfo>;
using FunctionInfoListPtr   = shared_ptr<list<FunctionInfoPtr>>;

using ParsedFilePtr         = shared_ptr<ParsedFile>;
using ParsedFileListPtr     = shared_ptr<list<ParsedFilePtr>>;

using RowedFilePtr          = shared_ptr<RowedFile>;

using StringListPtr         = shared_ptr<list<string>>;

using TokenListIter         = list<Token>::iterator;
using TokenListPtr          = shared_ptr<list<Token>>;

using FunctionBlockListPtr  = shared_ptr<list<FunctionBlock>>;

#endif // TYPEDEFS_H_INCLUDED

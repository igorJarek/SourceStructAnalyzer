#include "FunctionInfo.h"

FunctionInfo::~FunctionInfo()
{
    //dtor
}

void FunctionInfo::setFunctionList(FunctionInfoListPtr list)
{
    functions = list;
}

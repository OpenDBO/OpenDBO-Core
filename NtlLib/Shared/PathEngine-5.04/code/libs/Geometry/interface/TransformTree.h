//**********************************************************************
//
// Copyright (c) 2003-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/STL/vector.h"

class iPointLike;

namespace nTransformTree
{

class iOperation
{
public:

    virtual ~iOperation() {}

    virtual int resultFor(const iPointLike& p) const = 0;
    virtual int sourceFor(const iPointLike& p) const = 0;
};

class cTree
{
    // negative values indicate no operations performed in a given element
    std::vector<long> _firstOperation;
    std::vector<long> _lastOperation; // indexed by current state

    // per operation
    std::vector<iOperation*> _operations;
    //... optimise replace vector of vectors with something more efficient and cache friendly
    std::vector<std::vector<long> > _prevOperation;
    std::vector<std::vector<long> > _nextOperation;

//    void validate() const;

public:

    cTree(long topSourceElement);
    ~cTree();

    void addOperation(iOperation* operation, const std::vector<long>& sources, const std::vector<long>& results);
    void addNullOperation(long source, long result);

    void remap(const std::vector<long>& elementRemaps, long elementsAfterRemap);

    long transformDown(long startElement, const iPointLike& p) const;
    long transformUp(long startElement, const iPointLike& p) const;
};

}

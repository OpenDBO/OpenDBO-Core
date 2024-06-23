//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BSP_STATS_INCLUDED
#define BSP_STATS_INCLUDED

class cBSPStats
{
public:

  // tree state
    long _splitNodes;
    long _obstructedLeaves;
    long _maximumDepth;

  // construction costs
    long _operandSplits;
//    long _edgeSplits;

    cBSPStats()
    {
        _splitNodes = 0;
        _obstructedLeaves = 0;
        _maximumDepth = 0;
        _operandSplits = 0;
//        _edgeSplits = 0;
    }
};

#endif

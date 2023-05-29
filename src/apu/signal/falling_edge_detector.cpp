#include "falling_edge_detector.hpp"
FallingEdgeDetector::FallingEdgeDetector()
{
}

bool FallingEdgeDetector::detectFallingEdge(int value)
{
    bool ret = value < _previousValue;
    _previousValue = value;
    return ret;
}
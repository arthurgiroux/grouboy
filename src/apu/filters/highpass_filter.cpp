#include "highpass_filter.hpp"

HighpassFilter::HighpassFilter(float coefficient) : _coefficient(coefficient)
{
}

float HighpassFilter::filterValue(float value)
{
    float out = value - _accumulator;
    _accumulator = value - out * _coefficient;

    return out;
}

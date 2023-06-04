#include "apu/signal/falling_edge_detector.hpp"
#include <gtest/gtest.h>

TEST(FallingEdgeDetectorTest, DetectFallingEdgeShouldReturnFalseWithFirstValue)
{
    FallingEdgeDetector detector;
    int value = 0x40;
    ASSERT_FALSE(detector.detectFallingEdge(value));
}

TEST(FallingEdgeDetectorTest, DetectFallingEdgeShouldReturnTrueIfValueFell)
{
    FallingEdgeDetector detector;
    int firstValue = 0x30;
    int secondValue = 0x20;
    ASSERT_FALSE(detector.detectFallingEdge(firstValue));
    ASSERT_TRUE(detector.detectFallingEdge(secondValue));
}

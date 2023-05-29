#ifndef GROUBOY_FALLING_EDGE_DETECTOR_HPP
#define GROUBOY_FALLING_EDGE_DETECTOR_HPP

class FallingEdgeDetector
{
  public:
    explicit FallingEdgeDetector();
    bool detectFallingEdge(int value);

  private:
    int _previousValue = 0;
};

#endif // GROUBOY_FALLING_EDGE_DETECTOR_HPP

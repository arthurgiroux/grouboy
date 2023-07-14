#ifndef GROUBOY_FALLING_EDGE_DETECTOR_HPP
#define GROUBOY_FALLING_EDGE_DETECTOR_HPP

/**
 * Detects when a signal goes from a high to a low value.
 */
class FallingEdgeDetector
{
  public:
    /**
     * Create a new falling edge detector
     */
    explicit FallingEdgeDetector();

    /**
     * Detect if the current signal value is a falling edge event
     * @param value the new signal value
     * @return true if a falling edge was detected, false otherwise
     */
    bool detectFallingEdge(int value);

  private:
    /**
     * The previous value of the signal
     */
    int _previousValue = 0;
};

#endif // GROUBOY_FALLING_EDGE_DETECTOR_HPP

#ifndef GROUBOY_HIGHPASS_FILTER_HPP
#define GROUBOY_HIGHPASS_FILTER_HPP

/**
 * A floating-point highpass filter class.
 * This will effectively remove the DC component of a signal over time.
 * The coefficient will define how fast the DC component will be removed.
 */
class HighpassFilter
{
  public:
    /**
     * Create a high-pass filter with a given filter coefficient
     * @param coefficient the filter coefficient
     */
    explicit HighpassFilter(float coefficient);

    /**
     * Filter a new value
     * @param value the value to filter
     * @return  the filtered value
     */
    float filterValue(float value);

  private:
    /**
     * The accumulator that will be subtracted from a new value
     */
    float _accumulator = 0.0f;

    /**
     * The coefficient that will be applied to a new value
     */
    float _coefficient = 0.0f;
};

#endif // GROUBOY_HIGHPASS_FILTER_HPP

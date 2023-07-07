#ifndef GROUBOY_VOLUME_SWEEP_HPP
#define GROUBOY_VOLUME_SWEEP_HPP

/**
 * A volume sweep will adjust the volume periodically by increasing or decreasing it.
 * It's possible to control the start volume, the direction of the sweep (inc or dec),
 * and how often the volume is updated.
 */
class VolumeSweep
{
  public:
    /**
     * Create a new volume sweep
     */
    explicit VolumeSweep();

    /**
     * Set the volume that will be adjusted by the sweep
     * @param volume
     */
    void setVolume(int volume);

    /**
     * Set the direction of the sweep
     * @param sign 1 for increasing, -1 for decreasing
     */
    void setDirection(int sign);

    /**
     * Set the period of the sweep.
     * Everytime the number of ticks of the period elapses, the volume is adjusted.
     * @param period a period in ticks
     */
    void setPeriod(int period);

    /**
     * Get the period
     * @return the number of ticks
     */
    int getPeriod() const;

    /**
     * Get the current volume
     * @return a value between [0, 15]
     */
    int getVolume() const;

    /**
     * Tick the volume sweep.
     * If the period elapsed, the volume will be either increased or decreased by 1,
     * depending on the direction.
     * The volume value is stayed between [0, 15] and will loop if it over/underflows.
     */
    void tick();

    /**
     * Reset the sweep to its default state
     */
    void reset();

  private:
    /**
     * The current volume value, between [0, 15]
     */
    int _volume = 0;

    /**
     * The direction of the sweep, 1 is increasing, -1 is decreasing.
     */
    int _direction = 1;

    /**
     * How many ticks will elapse before updating the volume
     */
    int _period = 0;

    /**
     * The current number of ticks elapsed
     */
    int _timer = 0;
};

#endif // GROUBOY_VOLUME_SWEEP_HPP

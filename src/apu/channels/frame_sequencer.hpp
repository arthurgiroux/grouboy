#ifndef GROUBOY_FRAME_SEQUENCER_HPP
#define GROUBOY_FRAME_SEQUENCER_HPP

#include <functional>
#include <vector>

/**
 * A Frame sequencer executes frames at a different frequencies.
 * The frame sequencer runs at a fixed based frequency, for example 512Hz.
 * The frequency of the frame determines when they should be executed.
 * So for example if there's a Frame that has a frequency of 128Hz, it will be executed every 4 ticks.
 */
class FrameSequencer
{
  public:
    /**
     * A Frame is an arbitrary piece of code that needs to run at a specific frequency.
     */
    class Frame
    {
      public:
        /**
         * Create a new Frame
         * @param func the code to execute
         * @param frequency The frequency at which to run the frame
         */
        Frame(std::function<void()> func, int frequency);

        /**
         * Get the frequency of the frame
         * @return a value in Hz
         */
        int getFrequency() const;

        /**
         * Execute the code of the frame
         */
        void execute();

      private:
        std::function<void()> _func;
        int _frequency = 0;
    };

    /**
     * Create a new frame sequencer with a base frequency
     * @param baseFrequency The frequency at which `tick` will be called
     */
    explicit FrameSequencer(int baseFrequency);

    /**
     * Add a new frame to the sequencer
     * @param frame the frame to add
     */
    void addFrame(const Frame& frame);

    /**
     * Tick the sequencer, this will execute any frames that needs to be executed based on
     * its frequency and the base frequency.
     */
    void tick();

    /**
     * Get the frequency at which the frame sequencer runs.
     *
     * @return the frequency in Hz.
     */
    int getBaseFrequency() const;

  private:
    /**
     * The list of frames that are handled by the sequencer
     */
    std::vector<Frame> _frames = {};

    /**
     * The base frequency of the sequencer in Hz
     */
    int _baseFrequency = 0;

    /**
     * The current number of ticks
     */
    int _tickCounter = 0;
};

#endif // GROUBOY_FRAME_SEQUENCER_HPP

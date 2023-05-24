#ifndef GROUBOY_FRAME_SEQUENCER_HPP
#define GROUBOY_FRAME_SEQUENCER_HPP

#include <functional>
#include <vector>

class FrameSequencer
{
  public:
    class Frame
    {
      public:
        Frame(std::function<void()> func, int frequency);
        int getFrequency() const;
        void execute();

      private:
        std::function<void()> _func;
        int _frequency = 0;
    };

    explicit FrameSequencer(int baseFrequency);
    void addFrame(const Frame& frame);
    void tick();
    int getBaseFrequency() const;

  private:
    std::vector<Frame> _frames = {};
    int _baseFrequency = 0;
    int _tickCounter = 0;
};

#endif // GROUBOY_FRAME_SEQUENCER_HPP

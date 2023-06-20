#ifndef GROUBOY_AUDIO_MIXER_HPP
#define GROUBOY_AUDIO_MIXER_HPP

#include "channel.hpp"
#include "memory/mmu.hpp"
#include <array>

class AudioMixer
{
  public:
    class Sample
    {
      public:
        float left;
        float right;
    };

    AudioMixer(Channel* channel1, Channel* channel2, Channel* channel3, Channel* channel4);
    Sample getSample();
    void setVolumeScaleLeft(int scale);
    void setVolumeScaleRight(int scale);
    int getVolumeScaleLeft() const;
    int getVolumeScaleRight() const;
    void setPanningControlLeft(int panningControl);
    void setPanningControlRight(int panningControl);
    int getPanningControlLeft() const;
    int getPanningControlRight() const;
    void enableVinLeft(bool enable);
    void enableVinRight(bool enable);
    bool isVinLeftEnabled() const;
    bool isVinRightEnabled() const;

  private:
    static const int NBR_CHANNELS = 4;
    static constexpr float MAX_VOLUME_SCALE_VALUE = 7.f;
    static constexpr float MIN_VOLUME_FACTOR = 0.1f;
    static constexpr float MAX_VOLUME_FACTOR = 1.f;
    std::array<Channel*, NBR_CHANNELS> _channels = {};
    float panAudioSample(int panningControl);
    float scaleVolume(float amplitude, int scaleValue);
    int _volumeScaleLeft = 0;
    int _volumeScaleRight = 0;
    int _panningControlLeft = 0;
    int _panningControlRight = 0;
    bool _mixVinLeft = false;
    bool _mixVinRight = false;
};

#endif // GROUBOY_AUDIO_MIXER_HPP

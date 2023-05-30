#include "audio_mixer.hpp"

AudioMixer::AudioMixer(Channel* channel1, Channel* channel2, Channel* channel3, Channel* channel4)
{
    _channels[0] = channel1;
    _channels[1] = channel2;
    _channels[2] = channel3;
    _channels[3] = channel4;
}

AudioMixer::Sample AudioMixer::getSample()
{
    AudioMixer::Sample sample;
    sample.right = scaleVolume(panAudioSample(_panningControlRight), _volumeScaleRight);
    sample.left = scaleVolume(panAudioSample(_panningControlLeft), _volumeScaleLeft);

    return sample;
}

float AudioMixer::panAudioSample(int panningControl)
{
    float amplitude = 0;
    int nbrActivatedChannels = 0;
    for (int i = 0; i < NBR_CHANNELS; ++i)
    {
        // TODO: Remove nullptr check when all channels are implemented
        if (_channels[i] != nullptr && utils::isNthBitSet(panningControl, i) && _channels[i]->isEnabled())
        {
            amplitude += _channels[i]->getAudioSample();
            nbrActivatedChannels++;
        }
    }
    if (nbrActivatedChannels > 1)
    {
        amplitude /= nbrActivatedChannels;
    }

    return amplitude;
}

float AudioMixer::scaleVolume(float amplitude, int scaleValue)
{
    return amplitude *
           (MIN_VOLUME_FACTOR + (scaleValue / MAX_VOLUME_SCALE_VALUE) * (MAX_VOLUME_FACTOR - MIN_VOLUME_FACTOR));
}
void AudioMixer::setVolumeScaleLeft(int scale)
{
    _volumeScaleLeft = scale;
}

void AudioMixer::setVolumeScaleRight(int scale)
{
    _volumeScaleRight = scale;
}

void AudioMixer::setPanningControlLeft(int panningControl)
{
    _panningControlLeft = panningControl;
}

void AudioMixer::setPanningControlRight(int panningControl)
{
    _panningControlRight = panningControl;
}

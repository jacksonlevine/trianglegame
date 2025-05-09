//
// Created by jack on 5/8/2025.
//

#ifndef AUDIOCONTEXT_H
#define AUDIOCONTEXT_H



class AudioContext {
public:
  float bpm = 120.0f;
  AudioContext()
  {
    device = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
  };
  ~AudioContext()
  {
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
  };

  void load(const char* path)
  {
    if (buffer) {
      alDeleteBuffers(1, &buffer);
      buffer = 0;
    }

    SF_INFO sfinfo;
    SNDFILE* sndfile = sf_open(path, SFM_READ, &sfinfo);
    if (!sndfile) {
      std::cerr << "Failed to open audio file: " << path << std::endl;
      return;
    }

    std::vector<short> samples(sfinfo.frames * sfinfo.channels);
    sf_readf_short(sndfile, samples.data(), sfinfo.frames);
    sf_close(sndfile);

    ALenum format = (sfinfo.channels == 1) ? AL_FORMAT_MONO16 :
                    (sfinfo.channels == 2) ? AL_FORMAT_STEREO16 : 0;
    if (format == 0) {
      std::cerr << "Unsupported channel count: " << std::to_string(sfinfo.channels) << std::endl;
      return;
    }

    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, samples.data(),
                 static_cast<ALsizei>(samples.size() * sizeof(short)),
                 sfinfo.samplerate);

    //Aubio bpm detection starts here
    uint_t hop_size = 512;
    uint_t win_size = hop_size * 2;


    std::vector<float> mono(sfinfo.frames);
    if (sfinfo.channels == 1) {
      for (sf_count_t i = 0; i < sfinfo.frames; ++i)
        mono[i] = samples[i] / 32768.0f;
    } else {
      for (sf_count_t i = 0; i < sfinfo.frames; ++i) {
        int left = samples[i * 2];
        int right = samples[i * 2 + 1];
        mono[i] = (left + right) / (2.0f * 32768.0f);
      }
    }

    aubio_tempo_t* tempo = new_aubio_tempo("default", win_size, hop_size, sfinfo.samplerate);
    fvec_t* input = new_fvec(hop_size);

    fvec_t* tempo_output = new_fvec(1);

    for (sf_count_t i = 0; i + hop_size < sfinfo.frames; i += hop_size) {
      for (uint_t j = 0; j < hop_size; ++j)
        input->data[j] = mono[i + j];
      aubio_tempo_do(tempo, input, tempo_output);
    }

    del_fvec(tempo_output);


    bpm = aubio_tempo_get_bpm(tempo);
    std::cout << "Detected BPM: " << bpm << std::endl;

    del_aubio_tempo(tempo);
    del_fvec(input);
    aubio_cleanup();
  }

  void play()
  {
    if (source) {
      alSourceStop(source);
      alDeleteSources(1, &source);
    }
    if (buffer)
    {
      alGenSources(1, &source);
      alSourcei(source, AL_BUFFER, buffer);
      alSourcePlay(source);
    }
  }

  AudioContext(const AudioContext &other) = delete;
  AudioContext &operator=(const AudioContext &other) = delete;
  AudioContext(AudioContext &&other) = delete;
  AudioContext &operator=(AudioContext &&other) = delete;
private:
  ALuint buffer = 0;
  ALuint source = 0;
  ALCdevice* device;
  ALCcontext* context;
};



#endif //AUDIOCONTEXT_H

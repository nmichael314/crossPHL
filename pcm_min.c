/*
 *  This extra small demo sends a random samples to your speakers.
 */
 
#include <alsa/asoundlib.h>
#include <math.h>
 
static char *device = "default";            /* playback device */
short int buffer[2*1024];              /* some random data */
 
int main(void)
{
    int err,bufsize;
    unsigned int i;
    float phase;
    float freq=1000;
    float delta_ph;
    int rate = 16000;
    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_uframes_t p_size;
    unsigned int val;

    snd_pcm_hw_params_alloca(&hw_params);
    
    bufsize = sizeof(buffer)/sizeof(buffer[0]);
    printf("bufsize: %d\n",bufsize); 
    delta_ph = (2*M_PI*freq)/(float)rate;
    phase=0;
    i=0;
    for (int j = 0; j < bufsize/2; j++){
        buffer[i] = lround(2048*sin(phase)) & 0xffff;
        i++;
        buffer[i] = lround(2048*sin(phase))&0xffff;
        i++;
        phase = phase + delta_ph;
    } 
    printf("Initialized buffer.\n");


    if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    if ((err = snd_pcm_set_params(handle,
                      SND_PCM_FORMAT_S16_LE,
                      SND_PCM_ACCESS_RW_INTERLEAVED,
                      1,
                      48000,
                      1,
                      500000)) < 0) {   /* 0.5sec */
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
 
        printf("Writei buffer.\n");
        for (i = 0; i < 16; i++) {
        frames = snd_pcm_writei(handle, buffer, bufsize);
        if (frames < 0)
            frames = snd_pcm_recover(handle, frames, 0);
        if (frames < 0) {
            printf("snd_pcm_writei failed: %s\n", snd_strerror(frames));
            break;
        }
        if (frames > 0 && frames < (long)bufsize)
            printf("Short write (expected %li, wrote %li)\n", (long)bufsize, frames);
    }
 
    /* pass the remaining samples, otherwise they're dropped in close */
    err = snd_pcm_drain(handle);
    if (err < 0)
        printf("snd_pcm_drain failed: %s\n", snd_strerror(err));
    snd_pcm_close(handle);
    return 0;
}

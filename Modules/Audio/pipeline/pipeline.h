#ifndef __MODULES_AUDIO_PIPELINE_PIPELINE_H__
#define __MODULES_AUDIO_PIPELINE_PIPELINE_H__

#include <gst/gst.h>

class Pipeline {
 public:
  Pipeline();
  virtual ~Pipeline(){};

  void Play();
  void Pause();
  void Ready();
  void Unload();

  // TODO : implments Preload

 protected:
  GstElement *pipe;

 private:
  virtual void createPipeline() = 0;
};

#endif  // __MODULES_AUDIO_PIPELINE_PIPELINE_H__
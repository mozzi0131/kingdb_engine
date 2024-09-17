#include "effectpipeline.h"

EffectPipeline::EffectPipeline() { createPipeline(); }

EffectPipeline::~EffectPipeline() { Unload(); }

void EffectPipeline::AddEffect(std::string &sourcePath) {
  // created src & dec, and add it to the audiomixer
  // 이 경우 pipeline의 status가 play기만 하면 상관없이 무조건 재생되나? -> 확인
  // 필요

  // filesrc로 대체해도 될 것 같음 loop같은 기능은 딱히 안 쓰니까(아닌가?? 쓸
  // 일도 있나??)
  GstElement *src = gst_element_factory_make("filesrc", nullptr);
  // decoder 바로 붙여도 되나? 내가 왜 원래 코드는 parser 다음에 dec 붙였지?
  // interleaved처리해줘야했나??
  GstElement *parser = gst_element_factory_make("mpegaudioparse", nullptr);
  GstElement *dec = gst_element_factory_make("avdec_mp3", nullptr);
  GstElement *converter = gst_element_factory_make("audioconvert", nullptr);
  GstElement *resampler = gst_element_factory_make("audioresample", nullptr);

  if (!pipe) {
    // exception handling
  }

  gst_bin_add_many(GST_BIN(pipe), src, parser, dec, converter, resampler,
                   nullptr);
  gst_element_link_many(src, parser, dec, converter, resampler, mixer, nullptr);

  // TODO : then set "location" to the src
  g_object_set(G_OBJECT(src), "location", sourcePath.c_str(), nullptr);

  return;
}

// the pipeline should be a singleton object
void EffectPipeline::createPipeline() {
  pipe = gst_pipeline_new("effectpipeline");

  // create pipeline -> multiple source,
  mixer = gst_element_factory_make("audiomixer", "audiomixer");
  GstElement *sink = gst_element_factory_make("directsoundsink", nullptr);

  gst_bin_add_many(GST_BIN(pipe), mixer, sink, nullptr);
  gst_element_link(mixer, sink);
}
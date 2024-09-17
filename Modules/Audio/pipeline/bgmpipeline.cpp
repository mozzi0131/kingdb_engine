#include "bgmpipeline.h"

#include <iostream>

BgmPipeline::BgmPipeline() : bus(nullptr) { createPipeline(); }

BgmPipeline::~BgmPipeline() { Unload(); }

void BgmPipeline::createPipeline() {
  pipe = gst_pipeline_new("bgmpipeline");

  // with multifilesrc
  GstElement *src = gst_element_factory_make("multifilesrc", "src");
  GstElement *parser = gst_element_factory_make("mpegaudioparse", nullptr);
  GstElement *dec =
      gst_element_factory_make("avdec_mp3", nullptr);  // mpg123audiodec?
  GstElement *converter = gst_element_factory_make("audioconvert", nullptr);
  GstElement *resampler = gst_element_factory_make("audioresample", nullptr);
  GstElement *sink = gst_element_factory_make("directsoundsink", nullptr);

  gst_bin_add_many(GST_BIN(pipe), src, parser, dec, converter, resampler, sink,
                   nullptr);

  if (!gst_element_link_many(src, parser, dec, converter, resampler, sink,
                             nullptr)) {
    // update error status
  }

  g_object_set(G_OBJECT(src), "loop", true, nullptr);

  bus = gst_pipeline_get_bus(GST_PIPELINE(pipe));
  // set callback to the pipeline
  // busID = gst_bus_add_watch(bus, (GstBusFunc)busCallback, this);
  gst_object_unref(bus);
}

// bgmpipeline 말고는 딱히 필요한 경우가 없나?
void BgmPipeline::flush() {
  // send flush-start and flush-stop,
  // or seek the element for flushing the element

  // 이 정도면 되나?
  gst_element_send_event(GST_ELEMENT(pipe), gst_event_new_flush_start());
  gst_element_send_event(GST_ELEMENT(pipe), gst_event_new_flush_stop(TRUE));
}

void BgmPipeline::SetBGM(std::string &location) {
  // flush the pipeline before doing it
  flush();
  Ready();

  // change "location" of the src element
  GstElement *src = gst_bin_get_by_name(GST_BIN(pipe), "src");

  g_object_set(G_OBJECT(src), "location", location.c_str(), nullptr);
  g_object_unref(src);

  Play();
}
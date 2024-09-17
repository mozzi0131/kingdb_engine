#include "pipeline.h"

Pipeline::Pipeline() : pipe(nullptr) { gst_init(nullptr, nullptr); }

// 여기 이 부분들 pipeline.cpp/h로 옮기고
// test-set들 강화하기
void Pipeline::Play() {
  gst_element_set_state(GST_ELEMENT(pipe), GST_STATE_PLAYING);
}

void Pipeline::Pause() {
  gst_element_set_state(GST_ELEMENT(pipe), GST_STATE_PAUSED);
}

void Pipeline::Ready() {
  gst_element_set_state(GST_ELEMENT(pipe), GST_STATE_READY);
}

void Pipeline::Unload() {
  gst_element_set_state(GST_ELEMENT(pipe), GST_STATE_NULL);
  gst_object_unref(pipe);
}
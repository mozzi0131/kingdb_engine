#include <gst/gst.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "effectpipeline.h"

int main() {
  std::string mp3dir =
      "C:"
      "\\Users\\User\\Desktop\\workspace\\kingdbengine\\tests\\resource\\kingdb"
      "1_resource.mp3";

  std::string newMp3dir =
      "C:"
      "\\Users\\User\\Desktop\\workspace\\kingdbengine\\tests\\resource\\freeja"
      "zz.mp3";

  EffectPipeline *obj = new EffectPipeline();
  obj->AddEffect(mp3dir);

  sleep(10);

  obj->AddEffect(newMp3dir);
  obj->Play();

  sleep(10);

  obj->Unload();
  delete obj;

  return 0;
}

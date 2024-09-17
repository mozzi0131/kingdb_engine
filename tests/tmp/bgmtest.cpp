#include <gst/gst.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "bgmpipeline.h"

int main() {
  std::string mp3dir =
      "C:"
      "\\Users\\User\\Desktop\\workspace\\kingdbengine\\tests\\resource\\kingdb"
      "1_resource.mp3";

  std::string newMp3dir =
      "C:"
      "\\Users\\User\\Desktop\\workspace\\kingdbengine\\tests\\resource\\freeja"
      "zz.mp3";

  BgmPipeline *obj = new BgmPipeline();
  obj->SetBGM(mp3dir);

  sleep(10);

  obj->SetBGM(newMp3dir);

  sleep(10);

  obj->Unload();

  delete obj;

  return 0;
}

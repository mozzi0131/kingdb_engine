#include <string>

#include "pipeline.h"

class BgmPipeline : public Pipeline {
 public:
  BgmPipeline();
  ~BgmPipeline();

  void SetBGM(std::string &location);

 private:
  guint busID;
  GstBus *bus;

  void createPipeline();

  void flush();
};
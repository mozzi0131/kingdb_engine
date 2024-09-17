#include <string>

#include "pipeline.h"

class EffectPipeline : public Pipeline {
 public:
  EffectPipeline();
  ~EffectPipeline();

  void AddEffect(std::string &sourcePath);

 private:
  GstElement *mixer;

  void createPipeline();
};
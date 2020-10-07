#pragma once

#include <harunobu/integrator/integrator_base.h>

namespace harunobu {

class PathTracer : public IntegratorBase {
public:
    int max_depth;

public:
    PathTracer(sptr<Scene> scene_, int max_depth_);

    virtual cv::Mat integrate() override;
}; // class PathTracer

} // namespace harunobu
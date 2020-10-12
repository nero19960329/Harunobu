#include <harunobu/integrator/direct_illumination.h>
#include <harunobu/integrator/integrator_base.h>
#include <harunobu/integrator/path_tracer.h>

HARUNOBU_NAMESPACE_BEGIN

sptr<IntegratorBase> IntegratorBase::factory(std::string name,
                                             sptr<Scene> scene,
                                             ParamSet param_set) {
    if (name == "direct") {
        return std::make_shared<DirectIllumination>(
            scene, param_set.get<int>("max_depth", 65),
            param_set.get<int>("light_sample_num", 1));
    } else if (name == "path_tracer") {
        return std::make_shared<PathTracer>(
            scene, param_set.get<int>("max_depth", 65));
    } else {
        HARUNOBU_CHECK(false, "Unsupported integrator type '{}'!", name);
    }
}

HARUNOBU_NAMESPACE_END
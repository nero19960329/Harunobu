#include <harunobu/sampler/random_sampler.h>
#include <harunobu/sampler/sampler_base.h>
#include <harunobu/sampler/stratified_sampler.h>

HARUNOBU_NAMESPACE_BEGIN

sptr<SamplerBase> SamplerBase::factory(std::string name, ParamSet &param_set) {
    if (name == "random") {
        return std::make_shared<RandomSampler>(
            param_set.get<int>("sample_count", 64));
    } else if (name == "stratified") {
        return std::make_shared<StratifiedSampler>(
            param_set.get<int>("sample_count", 64),
            param_set.get<int>("dim_valid", 4));
    } else {
        HARUNOBU_CHECK(false, "Unsupported sampler type '{}'!", name);
    }
}

HARUNOBU_NAMESPACE_END
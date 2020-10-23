#include <harunobu/sampler/sampler_base.h>

HARUNOBU_NAMESPACE_BEGIN

class RandomSampler : public SamplerBase {
public:
    RandomSampler() : SamplerBase() {}

    virtual real next_1D() const override { return rng.random_real(); }

    virtual vec2 next_2D() const override {
        return vec2(rng.random_real(), rng.random_real());
    }
}; // class RandomSampler

HARUNOBU_NAMESPACE_END
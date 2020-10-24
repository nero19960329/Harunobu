#include <harunobu/sampler/sampler_base.h>

HARUNOBU_NAMESPACE_BEGIN

class RandomSampler : public SamplerBase {
public:
    RandomSampler(int sample_count) : SamplerBase(sample_count) {}

    virtual sptr<SamplerBase> copy() {
        return std::make_shared<RandomSampler>(sample_count);
    }

    virtual real next_1D() override { return rng.random_real(); }

    virtual vec2 next_2D() override {
        return vec2(rng.random_real(), rng.random_real());
    }
}; // class RandomSampler

HARUNOBU_NAMESPACE_END
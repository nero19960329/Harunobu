#include <harunobu/sampler/sampler_base.h>

HARUNOBU_NAMESPACE_BEGIN

class RandomSampler : public SamplerBase {
public:
    RandomSampler() : SamplerBase() {}

    virtual vec2 sample_screen_coor(int i, int j) const override {
        return vec2(i + rng.random_real(-0.5, 0.5),
                    j + rng.random_real(-0.5, 0.5));
    }
}; // class RandomSampler

HARUNOBU_NAMESPACE_END
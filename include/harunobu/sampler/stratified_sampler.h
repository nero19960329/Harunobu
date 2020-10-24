#pragma once

#include <algorithm>

#include <harunobu/sampler/sampler_base.h>

HARUNOBU_NAMESPACE_BEGIN

class StratifiedSampler : public SamplerBase {
private:
    int dim_valid;
    int dim_idx;

    std::vector<std::vector<real>> cache_1D;
    std::vector<std::vector<vec2>> cache_2D;

public:
    StratifiedSampler(int sample_count, int dim_valid_)
        : SamplerBase(sample_count), dim_valid(dim_valid_), dim_idx(0) {
        cache_1D = std::vector<std::vector<real>>(
            dim_valid, std::vector<real>(sample_count));
        cache_2D = std::vector<std::vector<vec2>>(
            dim_valid, std::vector<vec2>(sample_count));
    
        for (int i = 0; i < dim_valid; ++i) {
            for (int j = 0; j < sample_count; ++j) {
                real lower_bound = static_cast<real>(j) / sample_count;
                real upper_bound =
                    static_cast<real>(j + 1) / sample_count;
                cache_1D[i][j] = rng.random_real(lower_bound, upper_bound);
            }
            std::shuffle(cache_1D[i].begin(), cache_1D[i].end(), rng.rng);
        }

        // Latin Hypercube Sampling
        for (int i = 0; i < dim_valid; ++i) {
            for (int j = 0; j < sample_count; ++j) {
                cache_2D[i][j] = vec2(rng.random_real(), rng.random_real());
            }
            std::vector<int> idxs(sample_count);
            for (int j = 0; j < sample_count; ++j) {
                idxs[j] = j;
            }
            std::shuffle(idxs.begin(), idxs.end(), rng.rng);
            for (int j = 0; j < sample_count; ++j) {
                cache_2D[i][j] =
                    (cache_2D[i][j] + vec2(j, idxs[j])) / static_cast<real>(sample_count);
            }
            std::shuffle(cache_2D[i].begin(), cache_2D[i].end(), rng.rng);
        }
    }

    virtual sptr<SamplerBase> copy() {
        return std::make_shared<StratifiedSampler>(sample_count, dim_valid);
    }

    virtual void next_pixel() {
        SamplerBase::next_pixel();
        dim_idx = 0;
    }

    virtual real next_1D() override {
        if (dim_idx < dim_valid) {
            return cache_1D[dim_idx++][sample_idx];
        } else {
            return rng.random_real();
        }
    }

    virtual vec2 next_2D() override {
        if (dim_idx < dim_valid) {
            return cache_2D[dim_idx++][sample_idx];
        } else {
            return vec2(rng.random_real(), rng.random_real());
        }
    }
}; // class StratifiedSampler

HARUNOBU_NAMESPACE_END
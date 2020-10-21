#pragma once

#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

// Probability Mass Function
class PMF {
public:
    std::vector<real> pdf, cdf;

public:
    PMF(size_t n) {
        pdf = std::vector<real>(n);
        cdf = std::vector<real>(n + 1);
    }

    void build(const std::vector<real> &pdf_) {
        size_t n = pdf.size();
        for (size_t i = 0; i < n; ++i) {
            pdf[i] = pdf_[i];
        }

        real sum = std::accumulate(pdf.begin(), pdf.end(), 0.);
        if (sum != 0) {
            for (auto &w : pdf) {
                w /= sum;
            }
        } else {
            real w_avg = 1.0 / n;
            for (auto &w : pdf) {
                w = w_avg;
            }
        }

        cdf[0] = 0;
        for (size_t i = 1; i <= n; ++i) {
            cdf[i] = cdf[i - 1] + pdf[i - 1];
        }
    }

    size_t sample() const {
        size_t n = pdf.size();
        real random = rng.random_real();
        size_t idx;
        for (idx = 1; idx <= n; ++idx) {
            if (random <= cdf[idx]) {
                return idx - 1;
            }
        }
        return n - 1;
    }

    real get_pdf(size_t idx) const { return pdf[idx]; }
}; // class PMF

HARUNOBU_NAMESPACE_END
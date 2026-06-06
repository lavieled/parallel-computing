#ifndef CDF_H
#define CDF_H

#include "functor.h"

// sum functor for cdf
class Accumulator : public IAccumulator {
public:
    Accumulator() : m_total(0.0f) {}

    virtual float operator()(const float& a) {
        m_total += a;
        return m_total;
    }

private:
    float m_total;
};

// make cdf using pdf and accumulator functor
template<typename F>
std::vector<float> cdf(const std::vector<float>& pdfv, F& acc) {
    std::vector<float> cdfv;
    cdfv.reserve(pdfv.size());

    for (std::size_t i = 0; i < pdfv.size(); ++i) {
        cdfv.push_back(acc(pdfv[i]));
    }

    return cdfv;
}

#endif

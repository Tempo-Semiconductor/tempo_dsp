
#include <cmath>

#include "biquad.hpp"

using namespace tempo;

/**
@brief Generates a biquad of doubles from filter parameters

@param freq center frequency
@param gain gain at center frequency for peaking and shelving filters
@param fs sample rate of the filter
@param opt filter option
@param optval filter option value. If opt is Q then optval will be the value of
Q in calculations.
@param type filter type
@param normalize whether to normalize of not
@param quirk filter quirk

@returns A biquad of doubles computed from filter parameters
*/
bqd_t tempo::biquad_filter(double freq, double gain, double fs,
                            filt_opt_t opt, double optval,
                            filt_t type,
                            bool normalize,
                            filt_quirk_t quirk)
{
    bqd_t bq;
    double a;
    double w0 = 2 * M_PI * freq / fs;
    double sino = sin(w0);
    double coso = cos(w0);
    double alpha;

    switch(type) {
    case LPF:
    case HPF:
    case BPFQ:
    case BPF:
    case NOTCH:
    case APF:
        a = pow(10.0, gain / 20);
        break;
    case PEAKING:
    case LOWSHELF:
    case HIGHSHELF:
    default:
        a = pow(10.0, gain / 40);
        break;
    }

    switch (opt) {
    case Q:
        alpha = sin(w0) / (2 * optval);
        break;
    case S:
        alpha = sin(w0) / 2 * sqrt((a + 1 / a) * (1 / optval - 1) + 2);
        break;
    case BW:
    default:
        alpha = sino * sinh((log(2.0) / 2.0) * optval * (w0 / sino));
        break;
    }

    double two_sqrt_a_alpha;

    switch (type) {
    case LPF:
        bq.b0 = (1 - cos(w0)) / 2;
        bq.b1 = 1 - cos(w0);
        bq.b2 = (1 - cos(w0)) / 2;
        bq.a0 = 1 + alpha;
        bq.a1 = -2 * cos(w0);
        bq.a2 = 1 - alpha;
        break;
    case HPF:
        bq.b0 = (1 + cos(w0)) / 2;
        bq.b1 = -(1 + cos(w0));
        bq.b2 = (1 + cos(w0)) / 2;
        bq.a0 = 1 + alpha;
        bq.a1 = -2 * cos(w0);
        bq.a2 = 1 - alpha;
        break;
    case BPFQ:
        bq.b0 = sin(w0) / 2;
        bq.b1 = 0;
        bq.b2 = -sin(w0) / 2;
        bq.a0 = 1 + alpha;
        bq.a1 = -2 * cos(w0);
        bq.a2 = 1 - alpha;
        break;
    case BPF:
        bq.b0 = alpha;
        bq.b1 = 0;
        bq.b2 = -alpha;
        bq.a0 = 1 + alpha;
        bq.a1 = -2 * cos(w0);
        bq.a2 = 1 - alpha;
        break;
    case NOTCH:
        bq.b0 = 1;
        bq.b1 = -2 * cos(w0);
        bq.b2 = 1;
        bq.a0 = 1 + alpha;
        bq.a1 = -2 * cos(w0);
        bq.a2 = 1 - alpha;
        break;
    case APF:
        bq.b0 = 1 - alpha;
        bq.b1 = -2 * cos(w0);
        bq.b2 = 1 + alpha;
        bq.a0 = 1 + alpha;
        bq.a1 = -2 * cos(w0);
        bq.a2 = 1 - alpha;
        break;
    case LOWSHELF:
        two_sqrt_a_alpha = 2 * sqrt(a) * alpha;
        bq.b0 = a * ((a + 1) - (a - 1) * cos(w0) + two_sqrt_a_alpha);
        bq.b1 = 2 * a * ((a - 1) - (a + 1) * cos(w0));
        bq.b2 = a * ((a + 1) - (a - 1) * cos(w0) - two_sqrt_a_alpha);
        bq.a0 = (a + 1) + (a - 1) * cos(w0) + two_sqrt_a_alpha;
        bq.a1 = -2 * ((a - 1) + (a + 1) * cos(w0));
        bq.a2 = (a + 1) + (a - 1) * cos(w0) - two_sqrt_a_alpha;
        break;
    case HIGHSHELF:
        two_sqrt_a_alpha = 2 * sqrt(a) * alpha;
        bq.b0 = a * ((a + 1) + (a - 1) * cos(w0) + two_sqrt_a_alpha);
        bq.b1 = -2 * a * ((a - 1) + (a + 1) * cos(w0));
        bq.b2 = a * ((a + 1) + (a - 1) * cos(w0) - two_sqrt_a_alpha);
        bq.a0 = (a + 1) - (a - 1) * cos(w0);
        bq.a1 = 2 * ((a - 1) - (a + 1) * cos(w0));
        bq.a2 = (a + 1) - (a - 1) * cos(w0) - two_sqrt_a_alpha;
        break;
    case PEAKING:
    default:
        bq.a0 = 1.0 + alpha / a;
        bq.a1 = -2.0 * coso;
        bq.a2 = 1.0 - alpha / a;

        bq.b0 = 1.0 + alpha * a;
        bq.b1 = -2.0 * coso;
        bq.b2 = 1.0 - alpha * a;
        break;
    }

    // We are using the form:
    //        y[n] = (b0/a0)*x[n] + (b1/a0)*x[n-1] + (b2/a0)*x[n-2] 
    //                - (a1/a0)*y[n-1] - (a2/a0)*y[n-2] 
    // So we need to reverse the sign of a1 and a2
    bq.a1 = bq.a1 * -1;
    bq.a2 = bq.a2 * -1;

    switch (quirk) {
    case NO_QUIRK:
    default:
        break;
    }

    if (normalize) {
        bq.b2 = bq.b2 / bq.a0;
        bq.b1 = bq.b1 / bq.a0;
        bq.b0 = bq.b0 / bq.a0;
        bq.a2 = bq.a2 / bq.a0;
        bq.a1 = bq.a1 / bq.a0;
    }

    return bq;
}

static constexpr double mult = pow(2, 22);

static inline uint32_t coeff_d2i(double c)
{
    return static_cast<uint32_t>(floor(c * mult)) & 0x00ffffff;
}

/**
@brief Converts biquad of doubles into a biquad of unsigned ints

@param bq The biquad of doubles used in the conversion

@returns A biquad of unsigned ints
*/
bqi_t tempo::bqd_2_bqi(bqd_t& bq)
{
    bqi_t bqi;
    bqi.a0 = coeff_d2i(bq.a0);
    bqi.a1 = coeff_d2i(bq.a1);
    bqi.a2 = coeff_d2i(bq.a2);
    bqi.b0 = coeff_d2i(bq.b0);
    bqi.b1 = coeff_d2i(bq.b1);
    bqi.b2 = coeff_d2i(bq.b2);
    return bqi;
}

static inline uint8_t coeff_low(uint32_t c)
{
    return static_cast<uint8_t>(c);
}

static inline uint8_t coeff_mid(uint32_t c)
{
    return static_cast<uint8_t>(c >> 8);
}

static inline uint8_t coeff_hi(uint32_t c)
{
    return static_cast<uint8_t>(c >> 16);
}

/**
@brief Converts biquad of doubles into a biquad of unsigned bytes

@param bq The biquad of doubles used in the conversion

@returns A biquad of unsigned bytes
*/
bqb_t tempo::bqd_2_bqb(bqd_t& bq)
{
    bqi_t bqi = bqd_2_bqi(bq);
    bqb_t bqb;
    bqb.a0_l = coeff_low(bqi.a0);
    bqb.a0_m = coeff_mid(bqi.a0);
    bqb.a0_h = coeff_hi(bqi.a0);
    bqb.a1_l = coeff_low(bqi.a1);
    bqb.a1_m = coeff_mid(bqi.a1);
    bqb.a1_h = coeff_hi(bqi.a1);
    bqb.a2_l = coeff_low(bqi.a2);
    bqb.a2_m = coeff_mid(bqi.a2);
    bqb.a2_h = coeff_hi(bqi.a2);
    bqb.b0_l = coeff_low(bqi.b0);
    bqb.b0_m = coeff_mid(bqi.b0);
    bqb.b0_h = coeff_hi(bqi.b0);
    bqb.b1_l = coeff_low(bqi.b1);
    bqb.b1_m = coeff_mid(bqi.b1);
    bqb.b1_h = coeff_hi(bqi.b1);
    bqb.b2_l = coeff_low(bqi.b2);
    bqb.b2_m = coeff_mid(bqi.b2);
    bqb.b2_h = coeff_hi(bqi.b2);
    return bqb;
}

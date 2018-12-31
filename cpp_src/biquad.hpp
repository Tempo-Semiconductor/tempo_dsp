#ifndef __BIQUAD_HPP__
#define __BIQUAD_HPP__

/**
@file biquad.hpp 
@brief Library biquad types and functions
*/

#include <cstdint>

namespace tempo {

/**
@brief Enumeration of filter types
*/
enum filt_t {
    LPF,         ///< Low Pass
    HPF,         ///< High Pass
    BPFQ,        ///< Band Pass, peak gain = Q
    BPF,         ///< Band Pass, peak gain 0 dB
    NOTCH,       ///< Notch
    APF,         ///< All Pass
    PEAKING,     ///< Peaking
    LOWSHELF,    ///< Low Shelf
    HIGHSHELF,   ///< High Shelf
};

/**
@brief Enumeration of filter options
*/
enum filt_opt_t {
    /**
    the EE kind of definition, except for peakingEQ in which A*Q is
    the classic EE Q. That adjustment in definition was made so that
    a boost of N dB followed by a cut of N dB for identical Q and
    f0/Fs results in a precisely flat unity gain filter or "wire".
    */
    Q,
    /**
    BW, the bandwidth in octaves (between -3 dB frequencies for BPF
    and notch or between midpoint (dBgain/2) gain frequencies for
    peaking filter.
    */
    BW,
    /**
    a "shelf slope" parameter (for shelving EQ only). When S = 1,
    the shelf slope is as steep as it can be and remain monotonically
    increasing or decreasing gain with frequency. The shelf slope, in
    dB/octave, remains proportional to S for all other values for a
    fixed f0/Fs and dBgain.
    */
    S,
};

/**
@brief Enumeration of filter quirks
*/
enum filt_quirk_t {
    NO_QUIRK, ///< Indicates no quirks
};

/**
@brief Six contiguous biquad coefficient doubles
*/
typedef struct {
    double a0; ///< coefficient a0
    double a1; ///< coefficient a1
    double a2; ///< coefficient a2
    double b0; ///< coefficient b0
    double b1; ///< coefficient b1
    double b2; ///< coefficient b2
} bqd_t;

/**
@brief Six contiguous biquad coefficient unsigned ints
*/
typedef struct {
    uint32_t a0; ///< coefficient a0
    uint32_t a1; ///< coefficient a1
    uint32_t a2; ///< coefficient a2
    uint32_t b0; ///< coefficient b0
    uint32_t b1; ///< coefficient b1
    uint32_t b2; ///< coefficient b2
} bqi_t;

/**
@brief 18 contiguous biquad coefficient unsigned bytes
*/
typedef struct {
    uint8_t a0_l; ///< coefficient a0 low byte
    uint8_t a0_m; ///< coefficient a0 mid byte
    uint8_t a0_h; ///< coefficient a0 hi byte
    uint8_t a1_l; ///< coefficient a1 low byte
    uint8_t a1_m; ///< coefficient a1 mid byte
    uint8_t a1_h; ///< coefficient a1 hi byte
    uint8_t a2_l; ///< coefficient a2 low byte
    uint8_t a2_m; ///< coefficient a2 mid byte
    uint8_t a2_h; ///< coefficient a2 hi byte
    uint8_t b0_l; ///< coefficient b0 low byte
    uint8_t b0_m; ///< coefficient b0 mid byte
    uint8_t b0_h; ///< coefficient b0 hi byte
    uint8_t b1_l; ///< coefficient b1 low byte
    uint8_t b1_m; ///< coefficient b1 mid byte
    uint8_t b1_h; ///< coefficient b1 hi byte
    uint8_t b2_l; ///< coefficient b2 low byte
    uint8_t b2_m; ///< coefficient b2 mid byte
    uint8_t b2_h; ///< coefficient b2 hi byte
} bqb_t;

bqd_t biquad_filter(double freq, double gain, double fs,
                    filt_opt_t opt, double optval,
                    filt_t type,
                    bool normalize = true,
                    filt_quirk_t quirk = NO_QUIRK);

bqi_t bqd_2_bqi(bqd_t& bq);

bqb_t bqd_2_bqb(bqd_t& bq);

} // namespace tempo

#endif // __BIQUAD_HPP__

#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    // n += isn.raw_value();
    // uint32_t seq_no = n & 0x0000ffff;
    // return WrappingInt32{seq_no};
    return isn + uint32_t(n);
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    uint64_t abs_seq;
    if(n - isn < 0){
        abs_seq = uint64_t(n - isn + (1L << 32));
    }
    else{
        abs_seq = uint64_t(n - isn);
    }
    if(abs_seq >= checkpoint)
        return abs_seq;
    abs_seq |= (checkpoint >> 32) << 32;
    
    if(abs_seq <= checkpoint)
        abs_seq += (1UL) << 32;
    uint64_t abs_seq_small = abs_seq - (1UL << 32);
    return (checkpoint - abs_seq_small) < (abs_seq - checkpoint) ? abs_seq_small : abs_seq;

    // uint64_t cur = (checkpoint & ~((1UL << 32) - 1)) + abs_seq;
    // uint64_t small = (checkpoint & ~((1UL << 32) - 1)) - (1UL << 32) + abs_seq;
    // uint64_t big = (checkpoint & ~((1UL << 32) -1)) + (1UL << 32) + abs_seq;
    // if((checkpoint & ~((1UL << 32) - 1)) == 0){
    //     small = 0 + abs_seq;
    // }
    // if(cur > checkpoint){
    //     return (checkpoint - small) < (cur - checkpoint) ? small : cur;
    // }
    // else
    //     return (checkpoint - cur) < (big - checkpoint) ? cur : big;
}

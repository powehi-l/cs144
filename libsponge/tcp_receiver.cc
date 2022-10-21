#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    TCPHeader head = seg.header();
    if(!head.syn && !syn_arrive)
        return;

    auto segdata = seg.payload().copy();
    bool eof = false;
    if(head.syn && !syn_arrive){
        isn = seg.header().seqno;
        syn_arrive = true;
        if(head.fin){
            fin_arrive = true;
            eof = true;
        }
        _reassembler.push_substring(segdata, 0, eof);
        return;
    }

    if(head.fin){
        fin_arrive = eof = true;
    }

    uint64_t checkpoint = _reassembler.next_unreassembly();
    uint64_t index = unwrap(head.seqno, isn, checkpoint)-1;
    _reassembler.push_substring(segdata, index, eof);

}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if(!syn_arrive){
        return nullopt;
    }
    if(fin_arrive && _reassembler.empty())
        return wrap(_reassembler.next_unreassembly() + 2, isn);
    else
        return wrap(_reassembler.next_unreassembly() + 1, isn);
}

size_t TCPReceiver::window_size() const { return _reassembler.stream_out().remaining_capacity(); }

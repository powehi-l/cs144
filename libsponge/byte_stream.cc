#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) {
    this->capacity_ = capacity;
    this->size = 0;
}

size_t ByteStream::write(const string &data) {
    // if (data.length() <= (capacity_ - size)) {
    //     buffer.append(data);
    //     size += data.length();
    //     total_in += data.length();
    //     return data.length();
    // } else {
    //     buffer.append(data.substr(0, capacity_ - size));
    //     size_t add = capacity_ - size;
    //     size = capacity_;
    //     total_in += add;
    //     return add;
    // }
    int number = min(data.length(), capacity_-size);
    buffer.append(data.substr(0,number));
    size += number;
    total_in += number;
    return number;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    // if (len < size)
    //     return buffer.substr(len - 1);
    // else
    //     return buffer;

    return buffer.substr(0, (len<size)?len:size);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    // if (len < size) {
    //     buffer = buffer.substr(len, buffer.length());
    //     total_out += len;
    //     size -= len;
    // } else {
    //     buffer = "";
    //     total_out += size;
    //     size = 0;
    // }
    int number = (len<size) ? len : size;
    buffer = buffer.substr(number, size-1);
    total_out += number;
    size -= number;

}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string out;
    if (size < len) {
        size = 0;
        out = peek_output(size);
        pop_output(len);
    } else {
        size -= len;
        out = peek_output(len);
        pop_output(len);
    }
    return out;
}

void ByteStream::end_input() { stop = 1; }

bool ByteStream::input_ended() const { return stop; }

size_t ByteStream::buffer_size() const { return size; }

bool ByteStream::buffer_empty() const { return size == 0; }

bool ByteStream::eof() const { return (stop && size == 0); }

size_t ByteStream::bytes_written() const { return total_in; }

size_t ByteStream::bytes_read() const { return total_out; }

size_t ByteStream::remaining_capacity() const { return capacity_ - size; }

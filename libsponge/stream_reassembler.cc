#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity),
                        next_index(0),eof_index(0), eof_sign(false), un_reas() {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if(eof){
        this->eof_sign = true;
        eof_index += data.length() + index; 
    }

    if(index <= next_index && (index + data.length()) >= next_index){
        insert_data(data, index);
        write_next_index();
    }
    else if(index > next_index){
        insert_data(data, index);
    }
    
    return;
}

void StreamReassembler::insert_data(const string &data, const size_t index){
    size_t data_length = data.length();
    if(un_reas[index].length() < data_length)
        un_reas[index] = data;
}

void StreamReassembler::write_next_index(){
    auto it = un_reas.begin();
    for(; it!=un_reas.end(); ++it){
        size_t data_length = it->second.length();
        if(it->first <= next_index){
            if(it->first + data_length < next_index)
                continue;
            else
                next_index += _output.write(it->second.substr(next_index - it->first, it->first+data_length-next_index));
        }
        else
            break;
    }
    un_reas.erase(un_reas.begin(), it);
    if(eof_sign && eof_index <= next_index){
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const {
    auto it = un_reas.begin();
    size_t cur_index = it->first;
    size_t count = 0;
    // size_t index = it->first;
    for(;it != un_reas.end(); it++){
        if(it->first + it->second.length() <= cur_index){
            continue;
        }
        else if(it->first <= cur_index){
            count += it->first + it->second.length() - cur_index;
            cur_index = it->first + it->second.length();
        }
        else{
            count += it->second.length();
            cur_index = it->first + it->second.length();
        }   
    }
    return count;
}

bool StreamReassembler::empty() const { return un_reas.empty(); }


// void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
//     string cut_post = data;
//     size_t cur_index = index;
//     if(data.length() + un_reassemble_byte > _capacity - _output.buffer_size()){
//         return;
//     }
//     if(!un_reas.empty()){
//         auto first_larger = un_reas.upper_bound(index);
//         if(first_larger == un_reas.end() && first_larger->first < index){
//             cut_post  = cut_post.substr(first_larger->first+first_larger->second.length()-index-1,
//                                 cut_post.length()-1);
//         }
//         else{
//             if((index + data.length()) > first_larger->first)
//                 cut_post = data.substr(0, first_larger->first-index);
//             if(first_larger != un_reas.begin()){
//                 auto first_smaller = --first_larger;
//                 if((first_smaller->first + first_smaller->second.length()) > index){
//                         cut_post  = cut_post.substr(first_smaller->first+first_smaller->second.length()-index-1,
//                                 cut_post.length()-1);
//                         cur_index = first_smaller->first+first_smaller->second.length();
//                 }
//             }
//         }
//     }
//     un_reas.insert({cur_index, cut_post});
//     un_reassemble_byte += cut_post.length();

//     while(un_reas.lower_bound(next_index)->first == next_index){
//         //can be optimization, put part of the string into buffer
//         if(_output.remaining_capacity() < un_reas[next_index].length())
//             break;
//         _output.write(un_reas[next_index]);
//         un_reassemble_byte -= un_reas[next_index].length();
//         un_reas.erase(next_index);
//         next_index += un_reas[next_index].length();
//     }

//     if(eof){
//         stream_end = true;
//         _output.end_input();
//     }
//     return;   
// }

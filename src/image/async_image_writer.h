#pragma once

#include "image_writer.h"
#include "type/graphics_types.h"

#include <memory>
#include <mutex>
#include <queue>
#include <vector>

class Async_Image_Writer
{
public:
    struct Buf_Entry {
        unsigned index;     //!< Write sequence number. Writes sequentially in acending order.
        RbgFrame::Row data; //!< Image row data
    };

    Async_Image_Writer(std::unique_ptr<Image_Writer> writer) : m_writer(std::move(writer)), m_priority_buf() {}
    void open(std::string const& filename);
    void write(Config const& config);
    void write_header(RbgFrame const& frame);    
    void buf_write(Buf_Entry&& entry);
    void close();

private:
    std::mutex m_mutex = {};
    std::unique_ptr<Image_Writer> m_writer = {};

    unsigned m_next_index = 0;

    class Cmp_Min_Priority
    {
    public:
        bool operator()(Buf_Entry const& x, Buf_Entry const& y) {
            return x.index > y.index;
        }
    };
    std::priority_queue<Buf_Entry, std::vector<Buf_Entry>, Cmp_Min_Priority> m_priority_buf;

    void write_from_buf();
    void clear_buf();
};

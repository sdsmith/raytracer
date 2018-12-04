#include "async_image_writer.h"

#include <cassert>
#include <iostream>

void Async_Image_Writer::open(std::string const& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_next_index = 0;

    if (m_writer->is_open()) {
        //assert(!"Writer already open");
    } else {
        m_writer->open(filename);
    }
}

void Async_Image_Writer::write(Config const& config) {
    m_writer->write(config);
}

void Async_Image_Writer::write_header(Rbg_Frame const& frame) {
    m_writer->write_header(frame);
}

void Async_Image_Writer::buf_write(Buf_Entry&& entry) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (entry.index == m_next_index) {
        // Got next row to write
        m_writer->write(entry.data);
        m_next_index++;

        // Write as many buffered rows as possible
        write_from_buf();
    } else {
        // Row not next in sequence, buffer it
        m_priority_buf.push(std::move(entry));
    }
}

void Async_Image_Writer::close() {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_priority_buf.empty()) {
        std::cerr << "Closing buffered writer with " << m_priority_buf.size() << " entries in buffer\n";
        clear_buf();
    }

    m_writer->close();
}

void Async_Image_Writer::write_from_buf() {
    while (!m_priority_buf.empty() && m_priority_buf.top().index == m_next_index) {
        m_writer->write(m_priority_buf.top().data);
        m_priority_buf.pop();
        m_next_index++;
    }
}

void Async_Image_Writer::clear_buf() {
    while (!m_priority_buf.empty()) { m_priority_buf.pop(); }
}

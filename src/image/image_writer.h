#pragma once

#include "type/graphics_types.h"

#include <fstream>
#include <string>

class Image_Writer
{
public:
    virtual ~Image_Writer() {}
    void open(std::string const& filename);
    void close();
    bool is_open() const;

    void write(Rbg_Frame const& frame);
    virtual void write(Config const& config) = 0;
    virtual void write_header(Rbg_Frame const& frame) = 0;
    virtual void write(Rbg_Frame::Row const& row) = 0;

protected:
    std::ofstream m_image_file = {};

    std::string to_file_format(char const* ln_prefix, Config const& cfg) const;
};

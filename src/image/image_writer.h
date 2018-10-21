#pragma once

#include "type/graphics_types.h"

#include <fstream>
#include <string>

class Image_Writer
{
public:
    void open(std::string const& filename);
    virtual void write(Config const& config) = 0;
    virtual void write(RbgFrame const& frame) = 0;
    void close();

protected:
    std::ofstream m_image_file;

    std::string to_file_format(char const* ln_prefix, Config const& cfg) const;
};

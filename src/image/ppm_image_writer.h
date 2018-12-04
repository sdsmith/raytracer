#pragma once

#include "image_writer.h"

class Ppm_Image_Writer : public Image_Writer
{
public:
    void write(Config const& cfg) override;
    void write_header(Rbg_Frame const& frame) override;
    void write(Rbg_Frame::Row const& row) override;

private:
    static constexpr char const* s_ln_prefix = "#";
};

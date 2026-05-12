
#pragma once


namespace rol {

enum class horizontal_alignment {
    left,
    centre,
    right
};

enum class vertical_alignment {
    top,
    centre,
    bottom
};

struct alignment {
    horizontal_alignment x;
    vertical_alignment y;
};

} // namespace rol


namespace rol::defaults {

constexpr rol::alignment alignment {
    .x = horizontal_alignment::left,
    .y = vertical_alignment::top
};

} // namespace rol::defaults

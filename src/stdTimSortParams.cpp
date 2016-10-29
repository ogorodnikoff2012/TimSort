#include <params.h>

typedef std::uint32_t ui32;

ui32 timsort::StdTimSortParams::minRun(ui32 length) const {
    ui32 r = 0;
    while (length >= 64) {
        r |= length & 1;
        length >>= 1;
    }
    return length + r;
}
        
bool timsort::StdTimSortParams::needMerge(ui32 lenX, ui32 lenY) const {
    return lenY >= lenX;
}

timsort::EWhatMerge timsort::StdTimSortParams::whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const {
    if (lenZ >= lenY) {
        return timsort::WM_MergeYZ;
    } else if (lenZ + lenY >= lenX) {
        return timsort::WM_MergeXY;
    } else {
        return timsort::WM_NoMerge;
    }
}

ui32 timsort::StdTimSortParams::getGallop() const {
    return 7;
}

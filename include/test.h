#ifndef __TIMSORT_TEST_H__
#define __TIMSORT_TEST_H__

#include <cstdint>

namespace timsort {
    enum EWhatMerge {
        WM_NoMerge, WM_MergeXY, WM_MergeYZ
    };

    class ITimSortParams {
    public:
        typedef std::uint32_t ui32;
        virtual ui32 minRun(ui32 n) const = 0;
        virtual bool needMerge(ui32 lenX, ui32 lenY) const = 0;
        virtual EWhatMerge whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const = 0;
        virtual ui32 getGallop() const = 0;
    };
}

#endif // __TIMSORT_TEST_H__

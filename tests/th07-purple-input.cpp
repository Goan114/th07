#include "PurpleInput.hpp"
#include <cassert>
#include <cstdio>

int main()
{
    // Every combination, including force-Shift overriding disable-Shift.
    for (unsigned options = 0; options < 32; ++options)
    {
        PracticeRuntime::PurpleInput input;
        input.disable_xkey = options & 1;
        input.disable_shiftkey = options & 2;
        input.disable_zkey = options & 4;
        input.force_shiftkey = options & 8;
        input.disable_Ckey_at_same_time = options & 16;
        for (unsigned buttons = 0; buttons < 65536; ++buttons)
        {
            assert(input.filter(buttons, false, false) == buttons);
            assert(input.filter(buttons, true, true) == buttons);
            auto result = input.filter(buttons, true, false);
            assert((result & ~7u) == (buttons & ~7u));
            assert(bool(result & 1) == (bool(buttons & 1) && !input.disable_zkey));
            assert(bool(result & 2) == (bool(buttons & 2) && !input.disable_xkey));
            assert(bool(result & 4) == (input.force_shiftkey || (bool(buttons & 4) && !input.disable_shiftkey)));
        }
        std::uint8_t keys[256]{};
        keys[67] = 0x80;
        input.apply(keys);
        assert(bool(keys[67]) == !(input.disable_xkey && input.disable_Ckey_at_same_time));
    }
    std::puts("TH07 purple input: 2097152 logical samples, OFF/replay passthrough, C/Shift precedence PASS");
}

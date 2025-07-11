/**
 *               _
 *     /\       | |
 *    /  \   ___| |__   ___ _ __ ___  _ __
 *   / /\ \ / __| '_ \ / _ \ '__/ _ \| '_ \
 *  / ____ \ (__| | | |  __/ | | (_) | | | |
 * /_/    \_\___|_| |_|\___|_|  \___/|_| |_|
 *
 **/

#include "_acheron_pch.h"

namespace acs {

    void SetCapacity( const uint32_t storate_size, const uint32_t storage_offset ) {
        StorageSize   = storate_size;
        StorageOffset = storage_offset;
    }

};

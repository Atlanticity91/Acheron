/**
 *               _
 *     /\       | |
 *    /  \   ___| |__   ___ _ __ ___  _ __
 *   / /\ \ / __| '_ \ / _ \ '__/ _ \| '_ \
 *  / ____ \ (__| | | |  __/ | | (_) | | | |
 * /_/    \_\___|_| |_|\___|_|  \___/|_| |_|
 *
 * MIT License
 *
 * Copyright (c) 2025 Alves Quentin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/

#include "_acheron_pch.h"

namespace acs {

    ////////////////////////////////////////////////////////////////////////////////////////////
    //		===	PUBLIC ===
    ////////////////////////////////////////////////////////////////////////////////////////////
    AcheronComponentEntry::AcheronComponentEntry( )
        : AcheronComponentEntry{ { }, { }, { } }
    {
    }

    AcheronComponentEntry::AcheronComponentEntry(
        std::function<void( const uint32_t )> resize,
        std::function<void( const bool )> clear,
        std::function<void( const std::vector<AcheronUUID>& )> sweep
    )
        : Resize{ resize },
        Clear{ clear },
        Sweep{ sweep } 
    { 
    }

};

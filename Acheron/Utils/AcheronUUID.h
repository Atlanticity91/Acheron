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

#pragma once 

#include "AcheronTraits.h"

namespace acs {

    struct ACS_API AcheronUUID final {

        uint64_t Value;

        /**
         * Constructor
         **/
        AcheronUUID( );

        /**
         * Constructor
         * @note : Used internaly to build UUID from uint64_t.
         * @param other : Target UUID value.
         **/
        AcheronUUID( const uint64_t uuid );

        /**
         * Constructor
         * @note : Used to build entity UUID based on index and generation.
         * @param index : Base entity index.
         * @param generation : Base entity generation.
         **/
        AcheronUUID( const uint32_t index, const uint32_t generation );

        /**
         * GetIsValid constant function
         * @note : Get if the UUID is valid aka UUID < UINT64_MAX.
         * @return True when UUID is valid, false otherwise.
         **/
        bool GetIsValid( ) const;

        /**
         * Make static template function
         * @note : Get AcheronUUID for any type.
         * @template Type : Target type for UUID generation.
         * @return Return the AcheronUUID of the targeted Type.
         **/
        template<typename... Types>
        static AcheronUUID Make( ) {
            const auto uuid = ( uint64_t( typeid( Types ).hash_code( ) ) ^ ... );

            return { uuid };
        };

        /**
         * bool cast operator
         * @note : Explicit cast operator for speed GetIsValid call.
         * @return True when UUID is valid, false otherwise.
         **/
        operator bool( ) const;

        /**
         * uint64_t cast operator
         * @note : Explicit cast operator for the underlaying type.
         * @return The current UUID Value member value.
         **/
        operator uint64_t ( ) const;

        bool operator<( const AcheronUUID& other ) const;

        bool operator<=( const AcheronUUID& other ) const;

        bool operator>( const AcheronUUID& other ) const;

        bool operator>=( const AcheronUUID& other ) const;

        bool operator==( const AcheronUUID& other ) const;

        bool operator!=( const AcheronUUID& other ) const;

    };

};

namespace std { 

    template<>
    struct hash<acs::AcheronUUID> {

        std::size_t operator()( const acs::AcheronUUID& uuid ) const noexcept { 
            return std::size_t( uuid.Value );
        };

    };

};

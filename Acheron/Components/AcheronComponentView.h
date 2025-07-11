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

#include "AcheronComponentViewIterator.h"

namespace acs {

    template<typename... CompTypes>
    class AcheronComponentView final {

    private:
        AcheronComponentManager& m_component_manager;
        const std::vector<AcheronUUID>& m_entities;

    public:
        /**
         * Constructor
         * @param component_manager : Reference to current component manager instance.
         * @param component_cache : Reference to current component cache instance.
         **/
        AcheronComponentView(
            AcheronComponentManager& component_manager,
            AcheronComponentCache& component_cache
        )
            : m_component_manager{ component_manager },
            m_entities{ component_cache.Get<CompTypes...>( component_manager ) }
        { };

        /**
         * Destructor
         **/
        ~AcheronComponentView( ) = default;

        /**
         * begin function
         * @note : Get iterator to the first view iterator.
         * @return Return view iterator for the first entitiy uuid. 
         **/
        auto begin( ) -> AcheronComponentViewIterator<CompTypes...> {
            return { m_component_manager, m_entities, 0 };
        };

        /**
         * end function
         * @note : Get iterator to the last view iterator.
         * @return Return view iterator for the last entitiy uuid.
         **/
        auto end( )  -> AcheronComponentViewIterator<CompTypes...> {
            const auto count = uint32_t( m_entities.size( ) );

            return { m_component_manager, m_entities, count };
        };

    };

};

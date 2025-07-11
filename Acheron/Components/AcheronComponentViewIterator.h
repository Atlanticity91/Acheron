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

#include "AcheronComponentCache.h"

namespace acs {

    template<typename... CompTypes>
    class AcheronComponentViewIterator final {

    private:
        AcheronComponentManager& m_component_manager;
        const std::vector<AcheronUUID>& m_entities;
        uint32_t m_index;

    public:
        /**
         * Constructor
         * @param component_manager : Current Reference to current component
         *                            manager instance.
         * @param entities : Reference to current view entity uui'd cache 
         *                   instance.
         * @param index : Index of the current iterator in range :
         *                [ begin, last [.
         **/
        AcheronComponentViewIterator(
            AcheronComponentManager& component_manager,
            const std::vector<AcheronUUID>& entities,
            const uint32_t index
        )
            : m_component_manager{ component_manager },
            m_entities{ entities },
            m_index{ index }
        {
        };

        /**
         * Destructor
         **/
        ~AcheronComponentViewIterator( ) = default;

        /**
         * Dereferencing operator 
         * @note : Access the current iterator view tuple.
         * @return Tuple to current view iterator entity uuid and components 
         *         pointers.
         **/
        auto operator*( ) -> std::tuple<AcheronUUID, CompTypes*...> {
            const auto entity = m_entities[ m_index ];

            return std::make_tuple( entity, m_component_manager.GetComponent<CompTypes>( entity )... );
        };

        /**
         * Dereferencing const operator
         * @note : Access the current iterator view tuple.
         * @return Tuple to current view iterator entity uuid and components
         *         constant pointers.
         **/
        auto operator*( ) const -> std::tuple<AcheronUUID, const CompTypes*...> {
            const auto entity = m_entities[ m_index ];

            return std::make_tuple( entity, m_component_manager.GetComponent<CompTypes>( entity )... );
        };

        /**
         * Increment operator
         * @note : Move to the next view iterator entry.
         * @return Reference to current view iterator instance.
         **/
        AcheronComponentViewIterator& operator++( ) {
            ++m_index;

            return *this;
        };

        /**
         * Check operator
         * @note : Check if two view iterator are identical.
         * @param other : The other view iterator instance.
         * @return True when the two view ierators don't match.
         **/
        bool operator!=( const AcheronComponentViewIterator& other ) {
            return m_index != other.m_index;
        };

    };

};

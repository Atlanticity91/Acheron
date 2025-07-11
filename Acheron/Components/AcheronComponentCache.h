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

#include "AcheronComponentManager.h"

namespace acs {

    class ACS_API AcheronComponentCache final {

        using EntityCache = std::vector<AcheronUUID>;

    private:
        std::unordered_map<AcheronUUID, EntityCache> m_caches;

    public:
        /**
         * Constructor
         **/
        AcheronComponentCache( );

    public:
        /**
         * Get template function
         * @note : Get the cached entity list for the component list.
         * @template CompTypes : Variadic list of all component types 
         *                       required from the cache.
         * @param component_manager : Current component manager instance.
         * @return Constant reference to cached entity list that owns all
         *         the targeted components from the template.
         **/
        template<typename... CompTypes>
        const EntityCache& Get( AcheronComponentManager& component_manager ) {
            const auto component_uuid = AcheronUUID::Make<CompTypes...>( );
            auto& entry = m_caches[ component_uuid ];

            if ( component_manager.GetComponentAreDirty<CompTypes...>( ) ) {
                entry = ComputeCache( component_manager );

                component_manager.UnMarkDirty<CompTypes...>( );
            }

            return entry;
        };

    private:
        /**
         * ComputeCache template function
         * @note : Compute actual entity uuid's cache for view access.
         * @template CompTypes : Variadic template of all component in the view.
         * @param component_manager : Reference to component manager instance.
         * @return Return acutal entity uuid's cache vector.
         **/
        template<typename... CompTypes>
        EntityCache ComputeCache( AcheronComponentManager& component_manager ) {
            auto lists = std::vector<const EntityCache*>( 
                &component_manager.GetComponentEntities<CompTypes>( )...
            );

            if ( lists.empty( ) )
                return { };

            auto lists_start = lists.begin( );
            auto lists_stop  = lists.end( );
            auto sort_algo   = []( const EntityCache* a, const EntityCache* b ) -> bool {
                return a->size( ) < b->size( );
            };
            std::sort( lists_start, lists_stop, sort_algo );

            auto entity_cache  = *lists.front( );
            auto temp_cache    = EntityCache{ };
            auto temp_capacity = entity_cache.size( );

            temp_cache.reserve( temp_capacity );

            for ( auto index = 1; index < lists.size( ); index++ ) {
                temp_cache.clear( );

                std::set_intersection(
                    entity_cache.begin( ), entity_cache.end( ),
                    lists[ index ]->begin( ), lists[ index ]->end( ),
                    std::back_inserter( temp_cache )
                );

                std::swap( entity_cache, temp_cache );
            }

            return std::move( entity_cache );
        };

    };

};

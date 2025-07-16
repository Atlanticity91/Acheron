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

#include "../Entities/AcheronEntityManager.h"

namespace acs {

    template<typename CompType>
    class AcheronComponentStorage final {

    private:
        std::vector<AcheronUUID> m_entities;
        std::vector<CompType> m_components;
        bool m_is_dirty;

    public:
        /**
         * Constructor
         * @param storage_capacity : Target default component capacity ( used 
         *                           so if resize before the storage init, init
         *                           with resize capacity ).
         * @param register_callback : Callback for storage registration.
         **/
        AcheronComponentStorage(
            const uint32_t storage_capacity,
            std::function<void( const AcheronUUID, AcheronComponentStorage* )> register_callback
        )
            : m_entities{ },
            m_components{ },
            m_is_dirty{ false }
        {
            ACS_ASSERT( acs::StorageSize > 0, "Component storage size must always be non zero." );
            ACS_ASSERT( acs::StorageOffset > 0, "Component storage offset must always be non zero." );
            ACS_ASSERT( (bool)register_callback, "Component storage register callback must be populated." );

            const auto capacity = acs::GetStorageCapacity( storage_capacity );

            m_entities.reserve( capacity );
            m_components.reserve( capacity );

            const auto component_uuid = AcheronUUID::Make<CompType>( );

            std::invoke( register_callback, component_uuid, this );
        };

        /**
         * Destructor
         **/
        ~AcheronComponentStorage( ) = default;

        /**
         * MarkDirty method
         * @note : Mark the storage as dirty.
         **/
        inline void MarkDirty( ) {
            m_is_dirty = true;
        };

        /**
         * UnMarkDirty method
         * @note : Un mark the storage as dirty.
         **/
        inline void UnMarkDirty( ) {
            m_is_dirty = false;
        };

        /**
         * Resize method
         * @note : Resize the component storage to target capacity.
         * @param capacity : Target new storage capacity.
         **/
        void Resize( const uint32_t capacity ) {
            const auto current_capacity = uint32_t( m_entities.capacity( ) );

            m_entities.clear( );
            m_components.clear( );

            if ( current_capacity < capacity )
                Reallocate( capacity );
            else {
                const auto target_capacity = uint32_t( acs::GetStorageCapacity( capacity ) );

                Reallocate( target_capacity );
            }

            UnMarkDirty( );
        };

        /**
         * Clear method
         * @note : Clear all components and reset to defaut capacity if specified.
         * @param reset_capacity : True to reset the capacity to default.
         **/
        void Clear( const bool reset_capacity ) {
            m_entities.clear( );
            m_components.clear( );
            
            if ( !reset_capacity )
                return;

            Reallocate( acs::StorageSize );
            UnMarkDirty( );
        };

        /**
         * Append method
         * @note : Append a new component for the specified entity.
         * @param entity : Component owning entity.
         * @param component : New component instance to append.
         **/
        void Append( const AcheronUUID entity, CompType&& component ) {
            if ( m_components.size( ) == m_components.capacity( ) )
                Expand( );

            auto iterator = std::vector<AcheronUUID>::const_iterator( );
            auto index    = size_t( 0 );

            if ( FindEntityIndex( entity, iterator, index ) )
                return;
          
            const auto component_start = m_components.begin( );

            m_entities.insert( iterator, entity );
            m_components.insert( component_start + index, std::move( component ) );

            MarkDirty( );
        };

        /**
         * Remove method
         * @note : Remove a component for the specified entity.
         * @param entity : Component owning entity.
         **/
        void Remove( const AcheronUUID entity ) {
            auto iterator = std::vector<AcheronUUID>::const_iterator( );
            auto index    = size_t( 0 );

            if ( !FindEntityIndex( entity, iterator, index ) )
                return;

            const auto component_start = m_components.begin( );

            m_entities.erase( iterator );
            m_components.erase( component_start + index );

            MarkDirty( );
        };

        /**
         * Sweep method
         * @note : Destroy component of the defered entity destruction vector.
         * @param entities : Constant reference to current sweep vector.
         **/
        void Sweep( const std::vector<AcheronUUID>& entities ) {
            for ( const auto entity : entities )
                Remove( entity );
        };

    private:
        /**
         * Reallocate method
         * @note : Reallocate interal vector using the following policy,
         *		   capacity = acs::StorageSize < capacity ? capacity : StorageSize.
         * @param capacity : New storage capacity.
         **/
        inline void Reallocate( const uint32_t capacity ) { 
            auto temp_entities = std::vector<AcheronUUID>{ };
            temp_entities.reserve( size_t( capacity ) );

            m_entities.swap( temp_entities );

            auto temp_components = std::vector<CompType>{ };
            temp_components.reserve( size_t( capacity ) );

            m_components.swap( temp_components );
        };

        /**
         * Expand method
         * @note : Expand internal vector by acs::StorageOffset when
         *		   the actual vectors can't hold more entities.
         **/
        void Expand( ) {
            const auto old_capacity = m_entities.capacity( );
            const auto capacity = old_capacity + size_t( acs::StorageOffset );
            
            m_entities.reserve( capacity );
            m_components.reserve( capacity );
        };

    public:
        /**
         * GetCount const function
         * @note : Get valid component count.
         * @return Valid component count as uint32_t.
         **/
        uint32_t GetCount( ) const { 
            return uint32_t( m_components.size( ) );
        };

        /**
         * GetCapacity const function
         * @note : Get maximum allocated component count.
         * @return Maximum allocated component count as uint32_t.
         **/
        uint32_t GetCapacity( ) const {
            return uint32_t( m_components.capacity( ) );
        };

        /**
         * GetIsDirty const function
         * @note : Get if the storage is dirty.
         * @return True when the storage is dirty.
         **/
        bool GetIsDirty( ) const {
            return m_is_dirty;
        };

        /**
         * GetEntities const function
         * @note : Get current component entities uuids vector.
         * @return Constant reference to current component entities uuids vector.
         **/
        const std::vector<AcheronUUID>& GetEntities( ) const {
            return m_entities;
        };

        /**
         * GetVector const function
         * @note : Get current component vector.
         * @return Constant reference to current component vector.
         **/
        auto GetVector( ) const -> const std::vector<CompType>& {
            return m_components;
        };

        /**
         * Get function
         * @note : Get the component instance for an entity.
         * @param entity : Entity uuid.
         * @return Pointer to component instance or nullptr when not found.
         **/
        auto Get( const AcheronUUID entity ) -> CompType* {
            auto* component = (CompType*)nullptr;
            auto iterator   = std::vector<AcheronUUID>::const_iterator( );
            auto index      = size_t( 0 );

            if ( FindEntityIndex( entity, iterator, index ) )
                component = &m_components[ index ];

            return component;
        };

        /**
         * Get const function
         * @note : Get the component instance for an entity.
         * @param entity : Entity uuid.
         * @return Constant pointer to component instance or nullptr when not found.
         **/
        auto Get( const AcheronUUID entity ) const -> const CompType* {
            auto* component = (CompType*)nullptr;
            auto iterator   = std::vector<AcheronUUID>::const_iterator( );
            auto index      = size_t( 0 );

            if ( FindEntityIndex( entity, iterator, index ) )
                component = &m_components[ index ];

            return component;
        };

    private:
        /**
         * FindEntityIndex function
         * @note : Find component index from entity uuid.
         * @param entity : Entity uuid.
         * @param iterator : Reference to entities uuids iterator.
         * @param index : Reference to component index.
         * @return True when the entity as a component instance, false otherwise.
         **/
        bool FindEntityIndex(
            const AcheronUUID entity,
            std::vector<AcheronUUID>::iterator& iterator,
            size_t& index
        ) {
            const auto iterator_start = m_entities.begin( );
            const auto iterator_stop  = m_entities.end( );

            iterator = std::lower_bound( iterator_start, iterator_stop, entity );
            index    = std::distance( iterator_start, iterator );

            return iterator != iterator_stop && *iterator != entity;
        };
        
        /**
         * FindEntityIndex const function
         * @note : Find component index from entity uuid.
         * @param entity : Entity uuid.
         * @param iterator : Reference to entities uuids iterator.
         * @param index : Reference to component index.
         * @return True when the entity as a component instance, false otherwise.
         **/
        bool FindEntityIndex(
            const AcheronUUID entity,
            std::vector<AcheronUUID>::const_iterator& iterator,
            size_t& index
        ) const {
            const auto iterator_start = m_entities.cbegin( );
            const auto iterator_stop  = m_entities.cend( );

            iterator = std::lower_bound( iterator_start, iterator_stop, entity );
            index    = std::distance( iterator_start, iterator );

            return iterator != iterator_stop && *iterator != entity;
        };

    };

};

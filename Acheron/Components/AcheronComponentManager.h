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

#include "AcheronComponentEntry.h"

namespace acs {

    class ACS_API AcheronComponentManager final {

    private:
        uint32_t m_capacity;
        mutable std::unordered_map<AcheronUUID, AcheronComponentEntry> m_component_callbacks;

    public:
        /**
         * Constructor
         **/
        AcheronComponentManager( );

        /**
         * Destructor
         **/
        ~AcheronComponentManager( ) = default;

        /**
         * Resize method
         * @note : Resize all component storage to a target capacity.
         * @param capacity : New capacity for the storage but can't be 
         *                   smaller then acs::StorageSize.
         **/
        void Resize( const uint32_t capacity );

        /**
         * Clear method
         * @note : Clear all component storage and reset is capacity 
         *         if queried.
         * @param reset_capacity : True to reset storage capacity
         *                         to acs::StorageSize.
         **/
        void Clear( const bool reset_capacity );

        /**
         * Sweep method
         * @note : Perform actual component removing for dead entities.
         * @param entities : Reference to current entities to sweep.
         **/
        void Sweep( const std::vector<AcheronUUID>& entities );

    public:
        /**
         * Clear template method
         * @note : Clear component storage for CompType and reset is 
         *         capacity if queried.
         * @template CompTypes : Collection of component type to clear.
         * @param reset_capacity : True to reset storage capacity
         *                         to acs::StorageSize.
         **/
        template<typename... CompTypes>
        void Clear( const bool reset_capacity ) {
            ( [ & ]( )-> void {
                auto& storage = GetStorage<CompTypes>( );

                storage.Clear( reset_capacity );
            }( ), ... );
        };

        /**
         * Append template method
         * @note : Add a component to an entity.
         * @template CompType : Component type to append.
         * @param entity : Entity uuid.
         * @param component : Component instance to emplace.
         **/
        template<typename CompType>
        void Append( const AcheronUUID entity, const CompType& component ) {
            auto& storage       = GetStorage<CompType>( );
            auto temp_component = CompType{ component };

            storage.Append( entity, std::move( temp_component ) );
        };

        /**
         * Append template method
         * @note : Add a component to an entity.
         * @template CompType : Component type to append.
         * @param entity : Entity uuid.
         * @param component : Component instance to move.
         **/
        template<typename CompType>
        void Append( const AcheronUUID entity, CompType&& component ) {
            auto& storage = GetStorage<CompType>( );

            storage.Append( entity, std::move( component ) );
        };

        /**
         * Remove template method
         * @note : Remove a components from an entity.
         * @template CompTypes : Collection of component type to remove.
         * @param entity : Entity uuid.
         **/
        template<typename... CompTypes>
        void Remove( const AcheronUUID entity ) {
            ( [ & ]( )-> void {
                auto& storage = GetStorage<CompTypes>( );

                storage.Remove( entity );
            }( ), ... );
        };

        /**
         * UnMarkDirty method
         * @note : Unmark component collection dirty.
         * @template CompType : Component type to unmark dirty.
         **/
        template<typename... CompTypes>
        void UnMarkDirty( ) {
            ( [ & ]( )-> void {
                auto& storage = GetStorage<CompTypes>( );

                storage.UnMarkDirty( );
            }( ), ... );
        };

    public:
        /**
         * GetStorage template function
         * @note : Get actual component storage instance.
         * @template CompType : Storage component type.
         * @return Reference to component storage.
         **/
        template<typename CompType>
        auto GetStorage( ) const -> AcheronComponentStorage<CompType>& {
            static auto storage = AcheronComponentStorage<CompType>{
                m_capacity,
                [ this ]( const AcheronUUID component_uuid, AcheronComponentStorage<CompType>* storage ) {
                    const auto component_entry = AcheronComponentEntry{
                        [ storage ]( const uint32_t capacity ) { storage->Resize( capacity ); },
                        [ storage ]( const bool reset_capacity ) { storage->Clear( reset_capacity ); },
                        [ storage ]( const std::vector<AcheronUUID>& entities ) { storage->Sweep( entities ); }
                    };
                    const auto pair = std::make_pair( component_uuid, component_entry );

                    m_component_callbacks.emplace( pair );
                }
            };

            return storage;
        };

        /**
         * GetComponentCount const function
         * @note : Get component count.
         * @template CompType : Storage component type.
         * @return Component count as uint32_t.
         **/
        template<typename CompType>
        uint32_t GetComponentCount( ) const {
            auto& storage = GetStorage<CompType>( );

            return storage.GetCount( );
        };

        /**
         * GetComponentCapacity const function
         * @note : Get max alloacted component count.
         * @template CompType : Storage component type.
         * @return Component max allocated count as uint32_t.
         **/
        template<typename CompType>
        uint32_t GetComponentCapacity( ) const {
            auto& storage = GetStorage<CompType>( );

            return storage.GetCapacity( );
        };

        /**
         * GetComponentIsDirty const function
         * @note : Get if a component is dirty.
         * @template CompType : Storage component type.
         * @return True when the component is dirty.
         **/
        template<typename CompType>
        bool GetComponentIsDirty( ) const {
            auto& storage = GetStorage<CompType>( );

            return storage.GetIsDirty( );
        };

        /**
         * GetComponentAreDirty const function
         * @note : Get if a collection of components are dirty.
         * @template CompType : Storage component type.
         * @return True when at least one component is dirty.
         **/
        template<typename... CompTypes>
        bool GetComponentAreDirty( ) const {
            return ( GetComponentIsDirty<CompTypes>( ) || ... );
        };

        /**
         * GetComponentEntities const function
         * @note : Get all entities uuid's that use the component.
         * @template CompType : Storage component type.
         * @return Constant reference to component storage entity uuid's vector.
         **/
        template<typename CompType>
        const std::vector<AcheronUUID>& GetComponentEntities( ) const {
            auto& storage = GetStorage<CompType>( );

            return storage.GetEntities( );
        };

        /**
         * GetComponentVector template function
         * @note : Get all component stored inside the storage.
         * @template CompType : Storage component type.
         * @return Constant reference to component vector.
         **/
        template<typename CompType>
        auto GetComponentVector( ) const -> const std::vector<CompType>& {
            auto& storage = GetStorage<CompType>( );

            return storage.GetVector( );
        };

        /**
         * GetComponent template function
         * @note : Get component for a specific entity.
         * @template CompType : Storage component type.
         * @param entity : Target entity uuid.
         * @return Pointer the component or nullptr if not found.
         **/
        template<typename CompType>
        auto GetComponent( const AcheronUUID entity ) -> CompType* {
            auto& storage = GetStorage<CompType>( );

            return storage.Get( entity );
        };

        /**
         * GetComponent const template function
         * @note : Get component for a specific entity.
         * @template CompType : Storage component type.
         * @param entity : Target entity uuid.
         * @return Constant pointer the component or nullptr if not found.
         **/
        template<typename CompType>
        auto GetComponent( const AcheronUUID entity ) const -> const CompType* {
            auto& storage = GetStorage<CompType>( );

            return storage.Get( entity );
        };

    };

};

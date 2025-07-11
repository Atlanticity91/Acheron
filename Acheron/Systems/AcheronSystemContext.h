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

#include "../Components/Standard/AcheronHierarchy.h"

namespace acs { 

	class ACS_API AcheronContext {

	private:
		AcheronEntityManager m_entity_manager;
		AcheronComponentManager m_component_manager;
		AcheronComponentCache m_component_cache;

	public:
		/**
		 * Constructor
		 **/
		AcheronContext( );

		/**
		 * Destructor
		 **/
		~AcheronContext( ) = default;

		/**
		 * Resize method
		 * @note : Resize the entity and component storage to target capacity.
		 * @param capacity : Target new storage capacity.
		 **/
		void Resize( const uint32_t capacity );

		/**
		 * Clear method
		 * @note : Clear entities and all components and reset to defaut capacity
		 *		   if specified.
		 * @param reset_capacity : True to reset the capacity to default.
		 **/
		void Clear( const bool reset_capacity );

		/**
		 * Create function
		 * @note : Create an new entity.
		 * @return Return new entity uuid.
		 **/
		AcheronUUID Create( );

		/**
		 * Create function
		 * @note : Create an new entity.
		 * @param tags : Default entity tags.
		 * @return Return new entity uuid.
		 **/
		AcheronUUID Create( const uint64_t tags );

		/**
		 * Create function
		 * @note : Create an new entity.
		 * @param tags : Default entity tags.
		 * @return Return new entity uuid.
		 **/
		AcheronUUID Create( std::initializer_list<Tags> tags );

		/**
		 * Create function
		 * @note : Create an new entity.
		 * @param tags : Default entity tag component to emplace.
		 * @return Return new entity uuid.
		 **/
		AcheronUUID Create( const AcheronTag& tag_component );

		/**
		 * Create function
		 * @note : Create an new entity.
		 * @param tags : Default entity tag component to move.
		 * @return Return new entity uuid.
		 **/
		AcheronUUID Create( AcheronTag&& tag_component );

		/**
		 * Destroy method
		 * @note : Destroy an entity using is uuid.
		 * @param entity : Entity uuid.
		 * @param use_sweep_destroy : True to use sweep destroy on the entity.
		 **/
		void Destroy( const AcheronUUID entity, const bool use_sweep_destroy );
		
		/**
		 * Destroy method
		 * @note : Destroy an entity using is uuid.
		 * @param entity : Entity uuid.
		 * @param use_sweep_destroy : True to use sweep destroy on the entity.
		 * @param callback : Destruction callback when entity is destroyed.
		 **/
		void Destroy(
			const AcheronUUID entity,
			const bool use_sweep_destroy,
			std::function<void( const AcheronUUID, void* )> callback
		);

		/**
		 * Destroy method
		 * @note : Destroy an entity using is uuid.
		 * @param entity : Entity uuid.
		 * @param use_sweep_destroy : True to use sweep destroy on the entity.
		 * @param user_data : Pointer to external data that can be pass to 
		 *					  the entity destruction callback.
		 * @param callback : Destruction callback when entity is destroyed.
		 **/
		void Destroy(
			const AcheronUUID entity,
			const bool use_sweep_destroy,
			void* user_data,
			std::function<void( const AcheronUUID, void* )> callback
		);

		/**
		 * Sweep method
		 * @note : Clear the defered entity destruction vector.
		 **/
		void Sweep( );

	private:
		/**
		 * DestroyEntities method
		 * @note : Execute the sweep actions on entities.
		 **/
		void DestroyEntities( );

		/**
		 * DestroyEntities method
		 * @note : Execute the sweep actions on components.
		 **/
		void DestroyComponents( );

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
			m_component_manager.Clear<CompTypes...>( reset_capacity );
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
			m_component_manager.Append<CompType>( entity, component );
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
			m_component_manager.Append<CompType>( entity, std::move( component ) );
		};

		/**
		 * Remove template method
		 * @note : Remove a components from an entity.
		 * @template CompTypes : Collection of component type to remove.
		 * @param entity : Entity uuid.
		 **/
		template<typename... CompTypes>
		void Remove( const AcheronUUID entity ) {
			m_component_manager.Remove<CompTypes...>( entity );
		};

	public:
		/**
		 * GetEntityManager function
		 * @note : Get curent entity manager instance.
		 * @return Reference to current entity manager instance
		 **/
		AcheronEntityManager& GetEntityManager( );
		
		/**
		 * GetEntityCount const function
		 * @note : Get actual valid entitiy count.
		 * @return Return the count of valid entity as uint32_t.
		 **/
		uint32_t GetEntityCount( ) const;

		/**
		 * GetIsAlive function
		 * @note : Check if an entity is alive.
		 * @param entity : Entity uuid to check.
		 * @return True when the entity uuid point to a valid entity.
		 **/
		bool GetIsAlive( const AcheronUUID entity ) const;

		/**
		 * GetComponentManager function
		 * @note : Get curent component manager instance.
		 * @return Reference to current component manager instance
		 **/
		AcheronComponentManager& GetComponentManager( );

		/**
		 * GetComponentCache function
		 * @note : Get curent component cache instance.
		 * @return Reference to current component cache instance
		 **/
		AcheronComponentCache& GetComponentCache( );

	public:
		/**
         * GetStorage template function
         * @note : Get actual component storage instance.
         * @template CompType : Storage component type.
         * @return Reference to component storage.
         **/
		template<typename CompType>
		auto GetStorage( ) const -> AcheronComponentStorage<CompType>& {
			return m_component_manager.GetStorage<CompType>( );
		};

		/**
		 * GetComponentVector template function
		 * @note : Get all component stored inside the storage.
		 * @template CompType : Storage component type.
		 * @return Constant reference to component vector.
		 **/
		template<typename CompType>
		auto GetComponentVector( ) const -> const std::vector<CompType>& {
			return m_component_manager.GetComponentVector<CompType>( );
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
			return m_component_manager.GetComponent<CompType>( entity );
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
			return m_component_manager.GetComponent<CompType>( entity );
		};

	public:
		/**
		 * Cast operator
		 * @note : Get curent entity manager instance.
		 * @return Reference to current entity manager instance
		 **/
		operator AcheronEntityManager& ( );

		/**
		 * Cast operator
		 * @note : Get curent component manager instance.
		 * @return Reference to current component manager instance
		 **/
		operator AcheronComponentManager& ( );

		/**
		 * Cast operator
		 * @note : Get curent component cache instance.
		 * @return Reference to current component cache instance
		 **/
		operator AcheronComponentCache& ( );

	};

};

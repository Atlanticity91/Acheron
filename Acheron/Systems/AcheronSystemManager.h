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

#include "AcheronSystemInstance.h"

namespace acs {

	template<typename... SysTypes>
	concept IsSystem = ( std::is_base_of_v<IAcheronSystem, SysTypes>, ... );

	class ACS_API AcheronSystemManager final {

	private:
		std::vector<AcheronUUID> m_uuids;
		std::vector<AcheronSystemInstance> m_systems;

	public:
		/**
		 * Constructor
		 **/
		AcheronSystemManager( );

		/**
		 * Destructor
		 **/
		~AcheronSystemManager( ) = default;

		/**
		 * Resize method
		 * @note : Resize system pool to targeted capacity.
		 * @param capacity : New system pool capacity.
		 **/
		void Resize( const uint32_t capacity );

		/**
		 * Process method
		 * @note : Process all activated system.
		 * @param context : Reference to current acheron context instance.
		 * @param user_data : Pointer to external data that can be pass to 
		 *					  the process logic.
		 **/
		void Process( AcheronContext& context, void* user_data );

	public:
		/**
		 * Register template function
		 * @note : Register a new system.
		 * @template SysType : System type.
		 * @template ArgsTypes : System constructor arguments types.
		 * @param immediate_start : True to make is enabled at creation.
		 * @param args : System constructor aguments.
		 * @return Pointer to system instance.
		 **/
		template<typename SysType, typename... ArgTypes>
			requires IsSystem<SysType>
		auto Register( const bool immediate_start, ArgTypes&&... args ) -> SysType* {
			auto instance = AcheronSystemInstance{ };
			auto iterator = std::vector<AcheronUUID>::const_iterator( );
			auto index	  = size_t( 0 );
			auto uuid	  = AcheronUUID::Make<SysType>( );
			
			ACS_ASSERT( !FindIndex( uuid, iterator, index ), "This system type is already instantiate." );

			instance.Instance = std::make_unique<SysType>( std::forward<ArgTypes>( args )... );

			ACS_ASSERT( system != nullptr, "System creation failed." );

			const auto systems_start = m_systems.begin( );

			m_uuids.insert( iterator, std::move( uuid ) );
			m_systems.insert( systems_start + index, std::move( instance ) );

			return instance.Instance.get( );
		};

		/**
		 * Enable method
		 * @note : Enable a collection of system.
		 * @template SysTypes : Collection of system type.
		 * @param context : Reference to current acheron context instance.
		 * @param user_data : Pointer to external data that can be pass to 
		 *					  the process logic.
		 **/
		template<typename... SysTypes>
			requires IsSystem<SysTypes...>
		void Enable( AcheronContext& context, void* user_data ) {
			( [ & ]( ) -> void {
				if ( auto* instance = Get<SysTypes>( ) ) {
					instance->IsActive = true;
					instance->Hooks.Enable( context, user_data );
				}
			}( ), ... );
		};

		/**
		 * Disable method
		 * @note : Disable a collection of system.
		 * @template SysTypes : Collection of system type.
		 * @param context : Reference to current acheron context instance.
		 * @param user_data : Pointer to external data that can be pass to 
		 *					  the process logic.
		 **/
		template<typename... SysTypes>
			requires IsSystem<SysTypes...>
		void Disable( AcheronContext& context, void* user_data ) {
			( [ & ]( ) -> void {
				if ( auto* instance = Get<SysTypes>( ) ) {
					instance->IsActive = false;
					instance->Hooks.Disable( context, user_data );
				}
			}( ), ... );
		};

		/**
		 * ManualProcess method
		 * @note : Process a collection of system manually.
		 * @template SysTypes : Collection of system type.
		 * @param context : Reference to current acheron context instance.
		 * @param user_data : Pointer to external data that can be pass to 
		 *					  the process logic.
		 **/
		template<typename... SysTypes>
			requires IsSystem<SysTypes...>
		void ManualProcess( AcheronContext& context, void* user_data ) {
			( [ & ]( )-> void {
				if ( auto* instance = Get<SysTypes>( ) )
					Process( *instance, context, user_data );
			}( ), ... );
		};

	private:
		/**
		 * Process method 
		 * @note : Process a system.
		 * @param instance : Reference to the system instance to process.
		 * @param context : Reference to current acheron context instance.
		 * @param user_data : Pointer to external data that can be pass to 
		 *					  the process logic.
		 **/
		void Process(
			AcheronSystemInstance& instance,
			AcheronContext& context,
			void* user_data
		);

	public:
		/**
		 * GetIsActive const function
		 * @note : Get if a system is running.
		 * @template SysType : System type.
		 * @return True when system is active.
		 **/
		template<typename SysType>
			requires IsSystem<SysType>
		bool GetIsActive( ) const {
			auto is_active = false;

			if ( auto* instance = Get<SysType>( ) )
				is_active = instance->IsActive;

			return is_active;
		};

		/**
		 * GetAreActive const function
		 * @note : Get if a collection of system is running.
		 * @template SysType : Collection of system type.
		 * @return True when all systems are active.
		 **/
		template<typename... SysTypes>
			requires IsSystem<SysTypes...>
		bool GetAreActive( ) const {
			return ( GetIsActive<SysTypes>( ) && ... );
		};

		/**
		 * Get template function
		 * @note : Get system instance. 
		 * @template SysType : System type.
		 * @return Pointer to system instance.
		 **/
		template<typename SysType>
			requires IsSystem<SysType>
		AcheronSystemInstance* Get( ) {
			auto* instance = (AcheronSystemInstance*)nullptr;
			auto iterator  = std::vector<AcheronUUID>::const_iterator( );
			auto index	   = size_t( 0 );
			auto uuid	   = AcheronUUID::Make<SysType>( );

			if ( FindIndex( uuid, iterator, index ) )
				instance = &m_systems[ index ];

			return instance;
		};

		/**
		 * Get const template function
		 * @note : Get system instance.
		 * @template SysType : System type.
		 * @return Constant pointer to system instance.
		 **/
		template<typename SysType>
			requires IsSystem<SysType>
		const AcheronSystemInstance* Get( ) const {
			auto* instance = (AcheronSystemInstance*)nullptr;
			auto iterator  = std::vector<AcheronUUID>::const_iterator( );
			auto index	   = size_t( 0 );
			auto uuid	   = AcheronUUID::Make<SysType>( );

			if ( FindIndex( uuid, iterator, index ) )
				instance = &m_systems[ index ];

			return instance;
		};

	private:
		/**
		 * FindIndex const function
		 * @note : Find system index from system uuid.
		 * @param uuid : System uuid.
		 * @param iterator : Reference to entities uuids iterator.
		 * @param index : Reference to system index.
		 * @return True when the system as an instance, false otherwise.
		 **/
		bool FindIndex(
			const AcheronUUID uuid,
			std::vector<AcheronUUID>::const_iterator& iterator,
			size_t& index
		) const;

	};

};

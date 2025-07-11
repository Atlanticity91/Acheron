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

#include "Systems/AcheronSystemManager.h"

namespace acs { 

	class ACS_API AcheronComponentSystem final 
		: public AcheronContext
	{

	private:
		AcheronSystemManager m_system_manager;

	public:
		/**
		 * Constructor
		 **/
		AcheronComponentSystem( );

		/**
		 * Destructor
		 **/
		~AcheronComponentSystem( ) = default;

		/**
		 * ResizeSystemPool method
		 * @note : Resize system pool to targeted capacity.
		 * @param capacity : New system pool capacity.
		 **/
		void ResizeSystemPool( const uint32_t capacity );

		/**
		 * Process method
		 * @note : Process all activated system.
		 * @param user_data : Pointer to external data that can be pass to 
		 *					  the process logic.
		 **/
		void Process( void* user_data );

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
			return m_system_manager.Register<SysType, ArgTypes...>( immediate_start, std::forward<ArgTypes>( args )... );
		};

		/**
		 * Enable method
		 * @note : Enable a collection of system.
		 * @template SysTypes : Collection of system type.
		 * @param user_data : Pointer to external data that can be pass to
		 *					  the process logic.
		 **/
		template<typename... SysTypes>
			requires IsSystem<SysTypes...>
		void Enable( void* user_data ) {
			m_system_manager.Enable<SysTypes...>( *this, user_data );
		};

		/**
		 * Disable method
		 * @note : Disable a collection of system.
		 * @template SysTypes : Collection of system type.
		 * @param user_data : Pointer to external data that can be pass to
		 *					  the process logic.
		 **/
		template<typename... SysTypes>
			requires IsSystem<SysTypes...>
		void Disable( void* user_data ) {
			m_system_manager.Disable<SysTypes...>( *this, user_data );
		};

		/**
		 * ManualProcess method
		 * @note : Process a collection of system manually.
		 * @template SysTypes : Collection of system type.
		 * @param user_data : Pointer to external data that can be pass to
		 *					  the process logic.
		 **/
		template<typename... SysTypes>
			requires IsSystem<SysTypes...>
		void ManualProcess( void* user_data ) {
			m_system_manager.ManualProcess<SysTypes...>( *this, user_data );
		};

	public:
		/**
		 * GetSystemManager function
		 * @note : Get current system manager instance.
		 * @return Reference to current system manager instance.
		 **/
		AcheronSystemManager& GetSystemManager( );

		/**
		 * GetSystemManager const function
		 * @note : Get current system manager instance.
		 * @return Constant reference to current system manager instance.
		 **/
		const AcheronSystemManager& GetSystemManager( ) const;

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
			return m_system_manager.GetIsActive<SysType>( );
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
			return m_system_manager.GetAreActive<SysTypes...>( );
		};

		/**
		 * Get template function
		 * @note : Get system instance.
		 * @template SysType : System type.
		 * @return Pointer to system instance.
		 **/
		template<typename SysType>
			requires IsSystem<SysType>
		AcheronSystemInstance* Get( ) const {
			return m_system_manager.Get<SysType>( );
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
			return m_system_manager.Get<SysType>( );
		};

	public:
		/**
		 * Cast operator
		 * @note : Get current system manager instance.
		 * @return Reference to current system manager instance.
		 **/
		operator AcheronSystemManager& ( );

		/**
		 * Cast const operator
		 * @note : Get current system manager instance.
		 * @return Reference to current system manager instance.
		 **/
		operator const AcheronSystemManager& ( ) const;

	};

};

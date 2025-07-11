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

#include "AcheronSystem.h"

namespace acs {

	enum AcheronSystemHookTypes : uint32_t { 

		ACS_Hook_Enable = 0,
		ACS_Hook_Disable,
		ACS_Hook_PreProcess,
		ACS_Hook_PostProcess

	};

	class AcheronSystemHook final { 

		using AcheronSystemCallback = std::function<void( AcheronContext&, void* )>;

	private:
		std::array<std::vector<AcheronSystemCallback>, 4> m_callbacks;

	public:
		/**
		 * Constructor
		 **/
		AcheronSystemHook( );

		/**
		 * Destructor
		 **/
		~AcheronSystemHook( ) = default;

		/**
		 * Attach method
		 * @note : Attach a hook callback.
		 * @param type : Hook type.
		 * @param callback : Hook execution callback.
		 **/
		void Attach( const AcheronSystemHookTypes type, AcheronSystemCallback callback );

		/**
		 * Enable method
		 * @note : Call all enable hooks.
		 * @param context : Reference to current acheron context instance.
		 * @param user_data : Pointer to external data that can be pass to 
		 *					  the process logic.
		 **/
		void Enable( AcheronContext& context, void* user_data );

		/**
		 * Disable method
		 * @note : Call all disable hooks.
		 * @param context : Reference to current acheron context instance.
		 * @param user_data : Pointer to external data that can be pass to
		 *					  the process logic.
		 **/
		void Disable( AcheronContext& context, void* user_data );
		
		/**
		 * PreProcess method
		 * @note : Call all pre process hooks.
		 * @param context : Reference to current acheron context instance.
		 * @param user_data : Pointer to external data that can be pass to
		 *					  the process logic.
		 **/
		void PreProcess( AcheronContext& context, void* user_data );

		/**
		 * PostProcess method
		 * @note : Call all post process hooks.
		 * @param context : Reference to current acheron context instance.
		 * @param user_data : Pointer to external data that can be pass to
		 *					  the process logic.
		 **/
		void PostProcess( AcheronContext& context, void* user_data );

	};

};

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

#include "AcheronSystemContext.h"

namespace acs {

	struct IAcheronSystem {

		/**
		 * Process pure-virtual method
		 * @note : Process the system component group.
		 * @param context : Reference to current acheron context instance.
		 * @param user_data : Pointer to external data that can be pass to 
		 *					  the process logic.
		 **/
		virtual void Process( AcheronContext& context, void* user_data ) = 0;

	};

	template<typename... CompTypes>
	class AcheronSystem
		: public IAcheronSystem
	{

	public:
		/**
		 * Constructor
		 **/
		AcheronSystem( ) = default;

		/**
		 * Destructor
		 **/
		virtual ~AcheronSystem( ) = default;

		/**
		 * Process override method
		 * @note : Process the system component group.
		 * @param context : Reference to current acheron context instance.
		 * @param user_data : Pointer to external data that can be pass to
		 *					  the process logic.
		 **/
		virtual void Process( AcheronContext& context, void* user_data ) override {
			for ( auto components : AcheronComponentView<CompTypes...>( context, context ) )
				OnProcess( context, user_data, components );
		};

	protected:
		/**
		 * OnProcess method
		 * @note : Process a single entity and is component.
		 * @param context : Reference to current acheron context instance.
		 * @param user_data : Pointer to external data that can be pass to
		 *					  the process logic.
		 * @param components : Actual component view iterator tuple
		 **/
		virtual void OnProcess(
			AcheronContext& context,
			void* user_data,
			std::tuple<AcheronUUID, CompTypes*...>& components
		) = 0;

	};

};

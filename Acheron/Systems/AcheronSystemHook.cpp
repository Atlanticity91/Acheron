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

#include "_acheron_pch.h"

namespace acs {

	////////////////////////////////////////////////////////////////////////////////////////////
	//		===	PUBLIC ===
	////////////////////////////////////////////////////////////////////////////////////////////
	AcheronSystemHook::AcheronSystemHook( ) 
		: m_callbacks{ }
	{
	}

	void AcheronSystemHook::Attach(
		const AcheronSystemHookTypes type,
		AcheronSystemCallback callback
	) {
		m_callbacks[ type ].emplace_back( std::move( callback ) );
	}

	void AcheronSystemHook::Enable( AcheronContext& context, void* user_data ) {
		for ( auto callback : m_callbacks[ ACS_Hook_Enable ] )
			std::invoke( callback, context, user_data );
	}

	void AcheronSystemHook::Disable( AcheronContext& context, void* user_data ) {
		for ( auto callback : m_callbacks[ ACS_Hook_Disable ] )
			std::invoke( callback, context, user_data );
	}

	void AcheronSystemHook::PreProcess( AcheronContext& context, void* user_data ) {
		for ( auto callback : m_callbacks[ ACS_Hook_PreProcess ] )
			std::invoke( callback, context, user_data );
	}

	void AcheronSystemHook::PostProcess( AcheronContext& context, void* user_data ) {
		for ( auto callback : m_callbacks[ ACS_Hook_PostProcess ] )
			std::invoke( callback, context, user_data );
	}

};

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
	AcheronSystemManager::AcheronSystemManager( )
		: m_uuids{ },
		m_systems{ } 
	{
	}

	void AcheronSystemManager::Resize( const uint32_t system_count ) {
		ACS_ASSERT( system_count > 0, "You can't make a system manager without systems." );

		m_uuids.reserve( size_t( system_count ) );
		m_systems.reserve( size_t( system_count ) );
	}

	void AcheronSystemManager::Process( AcheronContext& context, void* user_data ) {
		for ( auto& instance : m_systems )
			Process( instance, context, user_data );
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//		===	PRIVATE ===
	////////////////////////////////////////////////////////////////////////////////////////////
	void AcheronSystemManager::Process(
		AcheronSystemInstance& instance,
		AcheronContext& context,
		void* user_data
	) {
		instance.Hooks.PreProcess( context, user_data );
		instance.Instance->Process( context, user_data );
		instance.Hooks.PostProcess( context, user_data );
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//		===	PRIVATE GET ===
	////////////////////////////////////////////////////////////////////////////////////////////
	bool AcheronSystemManager::FindIndex(
		const AcheronUUID uuid,
		std::vector<AcheronUUID>::const_iterator& iterator,
		size_t& index
	) const {
		const auto iterator_start = m_uuids.begin( );
		const auto iterator_stop  = m_uuids.end( );

		iterator = std::lower_bound( iterator_start, iterator_stop, uuid );

		if ( iterator != iterator_stop && *iterator != uuid ) {
			index = std::distance( iterator_start, iterator );

			return true;
		}

		return false;
	}

};

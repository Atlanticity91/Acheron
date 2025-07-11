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
	AcheronEntityManager::AcheronEntityManager( )
		: m_entities( size_t( acs::StorageSize ) ),
		m_free_ids( size_t( acs::StorageSize ) ),
		m_sweep_entities( size_t( StorageSize ) ),
		m_entity_count{ 0 }
	{
		auto iterator_start = m_free_ids.begin( );
		auto iterator_stop  = m_free_ids.end( );

		std::iota( iterator_start, iterator_stop, 0 );
	}

	void AcheronEntityManager::Resize( const uint32_t capacity ) {
		const auto current_capacity = (uint32_t)m_entities.capacity( );

		m_free_ids.clear( );
		m_sweep_entities.clear( );

		m_entity_count = 0;

		if ( current_capacity < capacity )
			Reallocate( capacity );
		else {
			const auto target_capacity = uint32_t( acs::GetStorageCapacity( capacity ) );

			Reallocate( target_capacity );
		}
	}

	void AcheronEntityManager::Clear( const bool reset_capacity ) {
		m_free_ids.clear( );
		m_sweep_entities.clear( );

		m_entity_count = 0;

		if ( reset_capacity )
			Reallocate( acs::StorageSize );
	}

	AcheronUUID AcheronEntityManager::Create( ) {
		if ( m_free_ids.size( ) == 0 )
			Expand( );

		const auto iterator = m_free_ids.begin( );
		const auto index	= m_free_ids[ 0 ];

		m_free_ids.erase( iterator );

		m_entity_count += 1;

		return { index, m_entities[ index ] };
	}

	void AcheronEntityManager::Destroy(
		const AcheronUUID& entity,
		const bool use_sweep_destroy
	) {
		if ( !entity.GetIsValid( ) )
			return;

		const auto index = GetIndex( entity );
		
		m_entities[ index ] += 1;

		m_entity_count -= 1;

		m_free_ids.emplace_back( index );

		if ( use_sweep_destroy )
			m_sweep_entities.emplace_back( entity );
	}

	void AcheronEntityManager::Sweep( ) {
		m_sweep_entities.clear( );
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//		===	PRIVATE ===
	////////////////////////////////////////////////////////////////////////////////////////////
	void AcheronEntityManager::Reallocate( const uint32_t capacity ) {
		auto temp_entities = std::vector<AcheronEntity>{ };
		temp_entities.resize( size_t( capacity ) );

		m_entities.swap( temp_entities );

		auto temp_free_ids = std::vector<uint32_t>{ };
		temp_free_ids.resize( size_t( capacity ) );

		m_free_ids.swap( temp_free_ids );

		auto iterator_start = m_free_ids.begin( );
		auto iterator_stop  = m_free_ids.end( );

		std::iota( iterator_start, iterator_stop, 0 );
	}
	
	void AcheronEntityManager::Expand( ) {
		const auto old_capacity  = m_entities.capacity( );
		const auto free_capacity = m_free_ids.size( );
		const auto capacity		 = old_capacity + size_t( acs::StorageOffset );

		m_entities.resize( capacity, 0 );

		if ( free_capacity < capacity )
			m_free_ids.resize( free_capacity + size_t( acs::StorageOffset ) );
		
		auto iterator_start = m_free_ids.begin( );
		auto iterator_stop  = m_free_ids.end( );

		std::iota( iterator_start, iterator_stop, uint32_t( old_capacity ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//		===	PUBLIC GET ===
	////////////////////////////////////////////////////////////////////////////////////////////
	uint32_t AcheronEntityManager::GetCount( ) const {
		return m_entity_count;
	}
	
	bool AcheronEntityManager::GetIsAlive( const AcheronUUID& entity ) const {
		const auto index = GetIndex( entity );

		return m_entities[ index ] == uint32_t( entity.Value >> 32 );
	}

	const std::vector<AcheronUUID>& AcheronEntityManager::GetSweeEntities( ) const {
		return m_sweep_entities;
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//		===	PRIVATE GET ===
	////////////////////////////////////////////////////////////////////////////////////////////
	uint32_t AcheronEntityManager::GetIndex( const AcheronUUID entity ) const {
		return ( entity.Value & 0xFFFFFFFF );
	}

};

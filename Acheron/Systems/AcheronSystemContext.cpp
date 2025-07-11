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
	AcheronContext::AcheronContext( )
		: m_entity_manager{ },
		m_component_manager{ },
		m_component_cache{ } 
	{
	}

	void AcheronContext::Resize( const uint32_t capacity ) {
		m_entity_manager.Resize( capacity );
		m_component_manager.Resize( capacity );
	}

	void AcheronContext::Clear( const bool reset_capacity ) {
		m_entity_manager.Clear( reset_capacity );
		m_component_manager.Clear( reset_capacity );
	}

	AcheronUUID AcheronContext::Create( ) {
		auto temp_component = AcheronTag{ };

		return Create( std::move( temp_component ) );
	}

	AcheronUUID AcheronContext::Create( const uint64_t tags ) {
		auto temp_component = AcheronTag{ tags };

		return Create( std::move( temp_component ) );
	}

	AcheronUUID AcheronContext::Create( std::initializer_list<Tags> tags ) {
		auto temp_component = AcheronTag{ tags };

		return Create( std::move( temp_component ) );
	}

	AcheronUUID AcheronContext::Create( const AcheronTag& tag_component ) {
		auto temp_component = AcheronTag{ tag_component };

		return Create( std::move( temp_component ) );
	}

	AcheronUUID AcheronContext::Create( AcheronTag&& tag_component ) {
		auto hierarchy = AcheronHierarchy{ };
		auto entity    = m_entity_manager.Create( );
		
		m_component_manager.Append( entity, std::move( entity ) );
		m_component_manager.Append( entity, std::move( hierarchy ) );
		m_component_manager.Append( entity, std::move( tag_component ) );

		return entity;
	}

	void AcheronContext::Destroy( const AcheronUUID entity, const bool use_sweep_destroy ) {
		Destroy( entity, use_sweep_destroy, nullptr, { } );
	}

	void AcheronContext::Destroy(
		const AcheronUUID entity,
		const bool use_sweep_destroy,
		std::function<void( const AcheronUUID, void* )> callback
	) {
		Destroy( entity, use_sweep_destroy, nullptr, callback );
	}

	void AcheronContext::Destroy(
		const AcheronUUID entity,
		const bool use_sweep_destroy,
		void* user_data,
		std::function<void( const AcheronUUID, void* )> callback
	) {
		m_entity_manager.Destroy( entity, use_sweep_destroy );

		if ( callback ) {
			auto destructor = AcheronDestructor{ user_data, callback };

			m_component_manager.Append( entity, std::move( destructor ) );
		}
	}

	void AcheronContext::Sweep( ) {
		DestroyEntities( );
		DestroyComponents( );
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//		===	PRVIVATE ===
	////////////////////////////////////////////////////////////////////////////////////////////
	void AcheronContext::DestroyEntities( ) {
		for ( auto [ entity, destructor ] : AcheronComponentView<AcheronDestructor>{ m_component_manager, m_component_cache } ) {
			if ( !destructor )
				continue;

			std::invoke( destructor->Callback, entity, destructor->UserData );
		}
	}

	void AcheronContext::DestroyComponents( ) {
		const auto& entities = m_entity_manager.GetSweeEntities( );

		m_component_manager.Sweep( entities );
		m_entity_manager.Sweep( );
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//		===	PUBLIC GET ===
	////////////////////////////////////////////////////////////////////////////////////////////
	AcheronEntityManager& AcheronContext::GetEntityManager( ) {
		return m_entity_manager;
	}

	uint32_t AcheronContext::GetEntityCount( ) const {
		return m_entity_manager.GetCount( );
	}

	bool AcheronContext::GetIsAlive( const AcheronUUID entity ) const {
		return m_entity_manager.GetIsAlive( entity );
	}

	AcheronComponentManager& AcheronContext::GetComponentManager( ) {
		return m_component_manager;
	}

	AcheronComponentCache& AcheronContext::GetComponentCache( ) {
		return m_component_cache;
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	//		===	OPERATOR ===
	////////////////////////////////////////////////////////////////////////////////////////////
	AcheronContext::operator AcheronEntityManager& ( ) {
		return GetEntityManager( );
	}

	AcheronContext::operator AcheronComponentManager& ( ) {
		return GetComponentManager( );
	}

	AcheronContext::operator AcheronComponentCache& ( ) {
		return GetComponentCache( );
	}

};

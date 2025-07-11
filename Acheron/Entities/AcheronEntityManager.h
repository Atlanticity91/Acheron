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

#include "../Utils/AcheronUUID.h"

namespace acs { 

	typedef uint32_t AcheronEntity;

	class ACS_API AcheronEntityManager final {

	private:
		std::vector<AcheronEntity> m_entities;
		std::vector<uint32_t> m_free_ids;
		std::vector<AcheronUUID> m_sweep_entities;
		uint32_t m_entity_count;

	public:
		/**
		 * Constructor
		 **/
		AcheronEntityManager( );

		/**
		 * Destructor
		 **/
		~AcheronEntityManager( ) = default;

		/**
		 * Resize method
		 * @note : Resize the entity storage to target capacity.
		 * @param capacity : Target new storage capacity.
		 **/
		void Resize( const uint32_t capacity );

		/**
		 * Clear method
		 * @note : Clear entities and reset to defaut capacity if specified.
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
		 * Destroy method
		 * @note : Destroy an entity using is uuid.
		 * @param entity : Entity uuid.
		 * @param use_sweep_destroy : True to use sweep destroy on the entity.
		 **/
		void Destroy( const AcheronUUID& entity, const bool use_sweep_destroy );

		/**
		 * Sweep method
		 * @note : Clear the defered entity destruction vector.
		 **/
		void Sweep( );

	private:
		/**
		 * Reallocate method
		 * @note : Reallocate interal vector using the following policy,
		 *		   capacity = acs::StorageSize < capacity ? capacity : StorageSize.
		 * @param capacity : New storage capacity.
		 **/
		void Reallocate( const uint32_t capacity );

		/**
		 * Expand method
		 * @note : Expand internal vector by acs::StorageOffset when 
		 *		   the actual vectors can't hold more entities.
		 **/
		void Expand( );

	public:
		/**
		 * GetCount const function
		 * @note : Get actual valid entitiy count.
		 * @return Return the count of valid entity as uint32_t.
		 **/
		uint32_t GetCount( ) const;

		/**
		 * GetIsAlive function
		 * @note : Check if an entity is alive.
		 * @param entity : Entity uuid to check.
		 * @return True when the entity uuid point to a valid entity.
		 **/
		bool GetIsAlive( const AcheronUUID& entity ) const;

		/**
		 * GetSweeEntities const function
		 * @note : Get the defered entity destruction vector.
		 * @return Constant reference to the sweep entity vector.
		 **/
		const std::vector<AcheronUUID>& GetSweeEntities( ) const;

	private:
		/**
		 * GetIndex const function
		 * @note : Binary search a entity index from it's uuid.
		 * @param entity : Entity uuid.
		 * @return Index of the entity in the vector.
		 **/
		uint32_t GetIndex( const AcheronUUID entity ) const;
		
	};

};

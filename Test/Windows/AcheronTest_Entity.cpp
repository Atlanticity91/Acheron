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

#include "AcheronTest_Utils.h"

////////////////////////////////////////////////////////////////////////////////////////////
//		===	TEST ===
////////////////////////////////////////////////////////////////////////////////////////////
namespace UnitTest {

	TEST_CLASS( Entities ) {

	public:
		TEST_METHOD( Create ) {
			auto acheron = acs::AcheronContext{ };

			Assert::AreEqual( acheron.GetEntityCount( ), uint32_t( 0 ) );
			Assert::AreNotEqual( acheron.Create( ), acs::AcheronUUID{ } );
			Assert::AreEqual( acheron.GetEntityCount( ), uint32_t( 1 ) );
		};

		TEST_METHOD( CreateMultiple ) {
			auto acheron = acs::AcheronContext{ };

			auto entity_1 = acheron.Create( );
			auto entity_2 = acheron.Create( );

			Assert::AreNotEqual( entity_1, acs::AcheronUUID{ } );
			Assert::AreNotEqual( entity_2, acs::AcheronUUID{ } );
			Assert::AreNotEqual( entity_1, entity_2 );
		};

		TEST_METHOD( ReUse ) {
			auto acheron = acs::AcheronContext{ };
			auto count = acs::StorageSize;
			auto entity = acs::AcheronUUID{ std::rand( ) % count, 0 };

			while ( count-- > 0 )
				acheron.Create( );

			auto get_index = []( const acs::AcheronUUID entity ) -> uint32_t {
				return uint32_t( entity.Value & 0xffffffff );
			};

			for ( auto count = 0; count < 8; count++ ) {
				const auto index = std::invoke( get_index, entity );

				acheron.Destroy( entity, false );

				entity = acheron.Create( );
				
				Assert::AreEqual( index, std::invoke( get_index, entity ) );
				Assert::AreNotEqual( entity, acs::AcheronUUID{ } );
			}
		};

		TEST_METHOD( Destroy ) {
			auto acheron = acs::AcheronContext{ };

			Assert::AreEqual( acheron.GetEntityCount( ), uint32_t( 0 ) );
			
			auto entity = acheron.Create( );
			
			Assert::AreNotEqual( entity, acs::AcheronUUID{ } );
			Assert::AreEqual( acheron.GetEntityCount( ), uint32_t( 1 ) );
			
			acheron.Destroy( entity, false );
			
			Assert::AreEqual( acheron.GetEntityCount( ), uint32_t( 0 ) );
		};

		TEST_METHOD( IsAlive ) {
			auto acheron = acs::AcheronContext{ };

			auto entity_1 = acheron.Create( );
			auto entity_2 = acheron.Create( );

			Assert::IsTrue( acheron.GetIsAlive( entity_1 ) );
			Assert::IsTrue( acheron.GetIsAlive( entity_2 ) );

			acheron.Destroy( entity_1, false );

			Assert::IsFalse( acheron.GetIsAlive( entity_1 ) );
			Assert::IsTrue( acheron.GetIsAlive( entity_2 ) );
		};

		TEST_METHOD( Bench_Create ) {
			auto acheron = acs::AcheronContext{ };

			for ( auto count : { 1000, 10000, 100000 } ) {
				acheron.Resize( count );

				acs_test::Bench( L"Bulk Create", [ & ]( ) {
					while ( count-- > 0 )
						acheron.Create( );
				} );
			}
		};

	};

};

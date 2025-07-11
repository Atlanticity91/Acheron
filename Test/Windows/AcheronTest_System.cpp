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

	struct TestComp {

		std::string Text = "";

		TestComp( )
			: TestComp{ "" }
		{ };

		TestComp( const std::string& text )
			: Text{ text }
		{ };

	};

	class TestSystem 
		: public acs::AcheronSystem<TestComp>
	{

	protected:
		virtual void OnProcess(
			acs::AcheronContext& context,
			void* user_data,
			std::tuple<acs::AcheronUUID, TestComp*>& component
		) {
			auto text = std::get<1>( component );

			Assert::IsTrue( text->Text.size( ) > 0 );
		};

	};

	TEST_CLASS( Systems ) {

	public:
		TEST_METHOD( CustomComponent ) {
			auto context = acs::AcheronContext{ };
			auto test_system = TestSystem{ };
			auto entity = context.Create( );
			auto test_comp = TestComp{ "Test Test !" };

			context.Append( entity, std::move( test_comp ) );

			test_system.Process( context, nullptr );
		};

	};

};

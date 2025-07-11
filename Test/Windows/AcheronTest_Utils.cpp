/**
 *               _
 *     /\       | |
 *    /  \   ___| |__   ___ _ __ ___  _ __
 *   / /\ \ / __| '_ \ / _ \ '__/ _ \| '_ \
 *  / ____ \ (__| | | |  __/ | | (_) | | | |
 * /_/    \_\___|_| |_|\___|_|  \___/|_| |_|
 *
 **/

#include "AcheronTest_Utils.h"

namespace acs_test {
	
	void Bench( const std::wstring& name, std::function<void( )> executor ) {
		const auto start = std::chrono::high_resolution_clock::now( );

		std::invoke( executor );

		const auto stop = std::chrono::high_resolution_clock::now( );
		const auto duration = std::chrono::duration_cast<std::chrono::microseconds>( stop - start );
		const auto bench_message = std::format( L"Bench {} executed in : {} microseconds.", name, duration.count( ) );
		const auto* message_str = bench_message.c_str( );

		Logger::WriteMessage( message_str );
	}

};

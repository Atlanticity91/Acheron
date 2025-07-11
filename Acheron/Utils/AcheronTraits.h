/**
 *               _
 *     /\       | |
 *    /  \   ___| |__   ___ _ __ ___  _ __
 *   / /\ \ / __| '_ \ / _ \ '__/ _ \| '_ \
 *  / ____ \ (__| | | |  __/ | | (_) | | | |
 * /_/    \_\___|_| |_|\___|_|  \___/|_| |_|
 *
 **/

#pragma once 

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <vector>
#include <source_location>


// Ensure C++20 use
#define ACS_MINIMAL_STD 202002L
#if __cplusplus < ACS_MINIMAL_STD
#   error "[ ACS ] You need at least C++20 for compiling and use this library."
#endif

// Current Acherion Component System (acs) version.
#define ACS_VERSION "1.2.0"

// Don't export symbols when is not defined.
#ifndef ACS_API
#   define ACS_API
#endif

/**
 * ACS_ASSERT macro
 * @note Defined the assertion macro if user don't provide one.
 * @param COND : Assertion condition.
 * @param MESSAGE : Assertion message when fail.
 **/
#ifndef ACS_ASSERT
#   if defined( _DEBUG ) || defined( ACS_FORCE_ASSERT )
#       define ACS_ASSERT( COND, MESSAGE )\
            do {\
                if ( !( COND ) ) {\
                    const auto location = std::source_location::current( );\
                    std::cerr << "Assertion failed : " << #COND\
                        << "\nMessage : " << MESSAGE\
                        << "\nFile : " << location.file_name( )\
                        << "\nLine : " << location.line( )\
                        << "\nColumn : " << location.column( )\
                        << "\nFunction : " << location.function_name( );\
                    std::abort( );\
                }\
            } while( false );
#   else
#       define ACS_ASSERT( COND, MESSAGE ) ( (void)COND );
#   endif
#endif

namespace acs {
    
    // Defined minimum entity/component pool size.
    static uint32_t StorageSize = 32;

    // Defined offset for internal vector resize operation.
    static uint32_t StorageOffset = StorageSize / 2;

    /**
     * SetCapacity method
     * @note : Call this function before any acs object creation to 
     *         change default buffer size and resize offset.
     * @param storace_size : Targeted default buffer size.
     * @param storage_offset : Targeted default buffer resize offset.
     **/
    void SetCapacity( const uint32_t storate_size, const uint32_t storage_offset );

    /**
     * GetStorageCapacity function
     * @note : Get the component storage capacity using acs rules.
     * @return Component storage capacity as size_t for vectors.
     **/
    inline size_t GetStorageCapacity( const uint32_t capacity ) {
        return size_t( ( StorageSize < capacity ) ? capacity : StorageSize );
    };

};

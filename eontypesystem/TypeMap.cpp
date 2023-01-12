#include "TypeMap.h"
#include "Action.h"
#include "Tuple.h"
#include <eonregex/RegEx.h>
#include <eonstring/NamePath.h>
#include <eonfilesys/Path.h>
#include "Expression.h"


namespace eon
{
	namespace type
	{
		std::unordered_map<std::type_index, std::set<name_t>> CppToName;
		void addType( std::type_index type, name_t name )
		{
			auto found = CppToName.find( type );
			if( found != CppToName.end() )
				found->second.insert( name );
			else
				CppToName.insert( { type, { name } } );
		}
		void _populateTypes()
		{
			addType( std::type_index( typeid( bool ) ), name_bool );
			addType( std::type_index( typeid( byte_t ) ), name_byte );
			addType( std::type_index( typeid( char ) ), name_byte );
			addType( std::type_index( typeid( char_t ) ), name_char );
			addType( std::type_index( typeid( int_t ) ), name_int );
			addType( std::type_index( typeid( short_t ) ), name_short );
			addType( std::type_index( typeid( long_t ) ), name_long );
			addType( std::type_index( typeid( flt_t ) ), name_float );
			addType( std::type_index( typeid( low_t ) ), name_low );
			addType( std::type_index( typeid( high_t ) ), name_high );
			addType( std::type_index( typeid( index_t ) ), name_index );
			addType( std::type_index( typeid( unsigned int ) ), name_index );
			addType( std::type_index( typeid( name_t ) ), name_name );
			addType( std::type_index( typeid( handle_t ) ), name_handle );
			addType( std::type_index( typeid( b8_t ) ), name_b8 );
			addType( std::type_index( typeid( b16_t ) ), name_b16 );
			addType( std::type_index( typeid( b32_t ) ), name_b32 );
			addType( std::type_index( typeid( b64_t ) ), name_b64 );
			addType( std::type_index( typeid( char* ) ), name_bytes );
			addType( std::type_index( typeid( std::string ) ), name_bytes );
			addType( std::type_index( typeid( string ) ), name_string );
			addType( std::type_index( typeid( regex ) ), name_regex );
			addType( std::type_index( typeid( namepath ) ), name_namepath );
			addType( std::type_index( typeid( path ) ), name_path );
			addType( std::type_index( typeid( Tuple ) ), name_tuple );
			addType( std::type_index( typeid( TypeTuple ) ), name_typetuple );
			addType( std::type_index( typeid( Expression ) ), name_expression );
			addType( std::type_index( typeid( Action ) ), name_action );
		}

		name_t mapCppType( std::type_index cpp_type, hint type_hint )
		{
			if( CppToName.empty() )
				_populateTypes();
			auto found = CppToName.find( cpp_type );
			if( found == CppToName.end() )
				return no_name;
			if( found->second.size() == 1 )
				return *found->second.begin();
			static std::unordered_map<name_t, hint> base{
				{ name_b8, hint::bits },
				{ name_b16, hint::bits },
				{ name_b32, hint::bits },
				{ name_b64, hint::bits },
				{ name_byte, hint::byte },
				{ name_index, hint::index }
			};
			if( type_hint != hint::none )
			{
				for( auto& option : found->second )
				{
					auto found = base.find( option );
					if( found != base.end() && found->second == type_hint )
						return option;
				}
			}

			// If we have no hints then assume it's not a bits type!
			for( auto& option : found->second )
			{
				auto found = base.find( option );
				if( found != base.end() && found->second != hint::bits )
					return option;
			}
			throw BadCppMapping();
		}



		std::unordered_map<name_t, index_t> Sizes;
		void _populateSizes()
		{
			Sizes = {
				{ name_bool, sizeof( bool ) },
				{ name_byte, sizeof( byte_t ) },
				{ name_char, sizeof( char_t ) },
				{ name_int , sizeof( int_t ) },
				{ name_short, sizeof( short_t ) },
				{ name_long, sizeof( long_t ) },
				{ name_float, sizeof( flt_t ) },
				{ name_low, sizeof( low_t ) },
				{ name_high, sizeof( high_t ) },
				{ name_index, sizeof( index_t ) },
				{ name_name, sizeof( name_t ) },
				{ name_handle, sizeof( handle_t ) },
				{ name_b8, sizeof( b8_t ) },
				{ name_b16, sizeof( b16_t ) },
				{ name_b32, sizeof( b32_t ) },
				{ name_b64, sizeof( b64_t ) },
				{ name_bytes, sizeof( std::string ) },
				{ name_string, sizeof( string ) },
				{ name_regex, sizeof( regex ) },
				{ name_namepath, sizeof( namepath ) },
				{ name_path, sizeof( path ) },
				{ name_tuple, sizeof( Tuple ) },
				{ name_typetuple, sizeof( TypeTuple ) },
				{ name_expression, sizeof( Expression ) },
				{ name_action, sizeof( Action ) }
			};
		}

		index_t sizeOf( name_t cpp_type ) noexcept
		{
			if( Sizes.empty() )
				_populateSizes();
			auto found = Sizes.find( cpp_type );
			return found != Sizes.end() ? found->second : 0;
		}
	}
}

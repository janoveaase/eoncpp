#include "TypeHandler.h"
#include "Name.h"
#include <eonregex/RegEx.h>
#include <eonfilesys/Path.h>
#include "Scope.h"
#include "Bool.h"
#include "Byte.h"
#include "Char.h"
#include "Integer.h"
#include "Floatingpt.h"
#include "Index.h"
#include "Handle.h"
#include "Name.h"
#include "Bytes.h"
#include "String.h"
#include "Regex.h"
#include "NamePath.h"
#include "Path.h"
#include "DataTuple.h"
#include "DynamicTuple.h"
#include "Tuple.h"


namespace eon
{
	namespace type
	{
		std::unordered_set<name_t> Handler::LegalDataTupleAttributeTypes;
		std::unordered_map<std::type_index, name_t> Handler::RawToEonTypeMap;
		std::unordered_map<name_t, name_t> Handler::GenericToDataTupleAttributeTypeMap;


		void Handler::init()
		{
			if( LegalDataTupleAttributeTypes.empty() )
			{
				LegalDataTupleAttributeTypes = {
					name_bool, name_byte, name_char, name_int, name_short, name_long,
					name_float, name_low, name_high, name_name, name_bytes, name_string,
					name_regex, name_namepath, name_path, name_typetuple, name_data };
				RawToEonTypeMap = {
					{ std::type_index( typeid( bool ) ), name_bool },
					{ std::type_index( typeid( byte_t ) ), name_byte },
					{ std::type_index( typeid( char_t ) ), name_char },
					{ std::type_index( typeid( int_t ) ), name_int },
					{ std::type_index( typeid( short_t ) ), name_short },
					{ std::type_index( typeid( long_t ) ), name_long },
					{ std::type_index( typeid( flt_t ) ), name_float },
					{ std::type_index( typeid( low_t ) ), name_low },
					{ std::type_index( typeid( high_t ) ), name_high },
					{ std::type_index( typeid( name_t ) ), name_name },
					{ std::type_index( typeid( std::string ) ), name_bytes },
					{ std::type_index( typeid( string ) ), name_string },
					{ std::type_index( typeid( regex ) ), name_regex },
					{ std::type_index( typeid( namepath ) ), name_namepath },
					{ std::type_index( typeid( path ) ), name_path },
					{ std::type_index( typeid( EonType ) ), name_typetuple },
					{ std::type_index( typeid( DataTuple ) ), name_data }
				};
				GenericToDataTupleAttributeTypeMap = {
					{ name_bool, name_bool },
					{ name_byte, name_byte },
					{ name_char, name_char },
					{ name_int, name_long },
					{ name_short, name_long },
					{ name_long, name_long },
					{ name_float, name_high },
					{ name_low, name_high },
					{ name_high, name_high },
					{ name_name, name_name },
					{ name_bytes, name_bytes },
					{ name_string, name_string },
					{ name_regex, name_regex },
					{ name_namepath, name_namepath },
					{ name_path, name_path },
					{ name_typetuple, name_typetuple },
					{ name_data, name_data }
				};
			}
		}




		Object* Handler::__copyConstruct( name_t type, name_t target_type, const void* value, source::Ref source )
		{
			if( type == name_bool )
				return new BoolInstance( *(const bool*)value, source );
			else if( type == name_byte )
				return new ByteInstance( *(const byte_t*)value, source );
			else if( type == name_char )
				return new CharInstance( *(const char_t*)value, source );
			else if( type == name_int )
			{
				if( type == target_type )
					return new IntegerInstance<int_t>( *(const int_t*)value, source );
				else
				{
					int_t source_val = *(const int_t*)value;
					long_t target_val = static_cast<long_t>( source_val );
					return __copyConstruct( target_type, target_type, &target_val, source );
				}
			}
			else if( type == name_short )
			{
				if( type == target_type )
					return new IntegerInstance<short_t>( *(const short_t*)value, source );
				else
				{
					short_t source_val = *(const short_t*)value;
					long_t target_val = static_cast<long_t>( source_val );
					return __copyConstruct( target_type, target_type, &target_val, source );
				}
			}
			else if( type == name_long )
				return new IntegerInstance<long_t>( *(const long_t*)value, source );
			else if( type == name_float )
			{
				if( type == target_type )
					return new FloatingptInstance<flt_t>( *(const flt_t*)value, source );
				else
				{
					flt_t source_val = *(const flt_t*)value;
					high_t target_val = static_cast<high_t>( source_val );
					return __copyConstruct( target_type, target_type, &target_val, source );
				}
			}
			else if( type == name_low )
			{
				if( type == target_type )
					return new FloatingptInstance<low_t>( *(const low_t*)value, source );
				else
				{
					short_t source_val = *(const short_t*)value;
					high_t target_val = static_cast<high_t>( source_val );
					return __copyConstruct( target_type, target_type, &target_val, source );
				}
			}
			else if( type == name_high )
				return new FloatingptInstance<high_t>( *(const high_t*)value, source );
			else if( type == name_index )
				return new IndexInstance( *(const index_t*)value, source );
			else if( type == name_name )
				return new NameInstance( *(const name_t*)value, source );
			else if( type == name_handle )
				return new HandleInstance( *(const handle_t*)value, source );
			else if( type == name_bytes )
				return new BytesInstance( *(const std::string*)value, source );
			else if( type == name_string )
				return new StringInstance( *(const string*)value, source );
			else if( type == name_regex )
				return new RegexInstance( *(const regex*)value, source );
			else if( type == name_namepath )
				return new NamePathInstance( *(const namepath*)value, source );
			else if( type == name_path )
				return new PathInstance( *(const path*)value, source );
			else if( type == name_data )
				return new DataTuple( *(const DataTuple*)value );
			else if( type == name_dynamic )
				return new DynamicTuple( *(const DynamicTuple*)value );
			else if( type == name_tuple )
				return new Tuple( *(const Tuple*)value );
			else
				throw IncompatibleType();
		}
		Object* Handler::__moveConstruct( name_t type, name_t target_type, const void* value, source::Ref source )
		{
			if( type == name_bool )
				return new BoolInstance( *(bool*)value, source );
			else if( type == name_byte )
				return new ByteInstance( *(byte_t*)value, source );
			else if( type == name_char )
				return new CharInstance( *(char_t*)value, source );
			else if( type == name_int )
			{
				if( type == target_type )
					return new IntegerInstance<int_t>( *(int_t*)value, source );
				else
				{
					int_t source_val = *(int_t*)value;
					long_t target_val = static_cast<long_t>( source_val );
					return __copyConstruct( target_type, target_type, &target_val, source );
				}
			}
			else if( type == name_short )
			{
				if( type == target_type )
					return new IntegerInstance<short_t>( *(short_t*)value, source );
				else
				{
					short_t source_val = *(short_t*)value;
					long_t target_val = static_cast<long_t>( source_val );
					return __copyConstruct( target_type, target_type, &target_val, source );
				}
			}
			else if( type == name_long )
				return new IntegerInstance<long_t>( *(long_t*)value, source );
			else if( type == name_float )
			{
				if( type == target_type )
					return new FloatingptInstance<flt_t>( *(flt_t*)value, source );
				else
				{
					flt_t source_val = *(flt_t*)value;
					high_t target_val = static_cast<high_t>( source_val );
					return __copyConstruct( target_type, target_type, &target_val, source );
				}
			}
			else if( type == name_low )
			{
				if( type == target_type )
					return new FloatingptInstance<low_t>( *(low_t*)value, source );
				else
				{
					short_t source_val = *(short_t*)value;
					high_t target_val = static_cast<high_t>( source_val );
					return __copyConstruct( target_type, target_type, &target_val, source );
				}
			}
			else if( type == name_high )
				return new FloatingptInstance<high_t>( *(high_t*)value, source );
			else if( type == name_index )
				return new IndexInstance( *(index_t*)value, source );
			else if( type == name_name )
				return new NameInstance( *(name_t*)value, source );
			else if( type == name_handle )
				return new HandleInstance( *(handle_t*)value, source );
			else if( type == name_bytes )
				return new BytesInstance( std::move( *(std::string*)value ), source );
			else if( type == name_string )
				return new StringInstance( std::move( *(string*)value ), source );
			else if( type == name_regex )
				return new RegexInstance( std::move( *(regex*)value ), source );
			else if( type == name_namepath )
				return new NamePathInstance( std::move( *(namepath*)value ), source );
			else if( type == name_path )
				return new PathInstance( std::move( *(path*)value ), source );
			else if( type == name_data )
				return new DataTuple( std::move( *(DataTuple*)value ) );
			else if( type == name_dynamic )
				return new DynamicTuple( std::move( *(DynamicTuple*)value ) );
			else if( type == name_tuple )
				return new Tuple( std::move( *(Tuple*)value ) );
			else
				throw IncompatibleType();
		}
	}
}

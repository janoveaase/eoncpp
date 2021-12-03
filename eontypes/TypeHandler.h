#pragma once

#include "TypeSystem.h"
#include <eonscopes/Scope.h>
#include <typeinfo>
#include <unordered_set>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace type
	{
		class Handler
		{
		public:

			// Initialize type handler
			static void init();

			// Construct an instance object for the specified raw C++ type
			// Throws [eon::type::IncompatibleType] if type is not supported
			template<typename T>
			static Object* copyConstruct( const T& value ) { return _copyConstruct( mapType<T>(), &value, false ); }
			template<typename T>
			static Object* copyConstructData( const T& value ) { return _copyConstruct( mapType<T>(), &value, true ); }
			template<typename T>
			static Object* moveConstruct( T&& value ) { return _moveConstruct( mapType<T>(), &value, false ); }
			template<typename T>
			static Object* moveConstructData( T&& value ) { return _moveConstruct( mapType<T>(), &value, true ); }

			// Construct an instance object for use as data tuple attribute
			// Throws [eon::type::IncompatibleType] if type is not supported
			template<typename T>
			static Object* constructForDataTuple( scope::Scope& scope, T value ) {
				return _construct( scope, mapToLegalData( mapType<T>() ), &value ); }


			// Given a raw C++ type, get the Æon type name
			// Returns no_name if there is no mapping
			template<typename T>
			static inline name_t mapType() noexcept {
				auto found = RawToEonTypeMap.find( std::type_index( typeid( T ) ) );
				return found != RawToEonTypeMap.end() ? found->second : no_name; }

			// Check if the specified type can be used as attribute in data tuples
			static bool legalForDataTuple( const TypeTuple& type ) noexcept;

			// Given an Æon type name, get the corresponding supported type for data tuples
			// Throws [eon::type::IncompatibleType] if no possible mapping to supported type!
			static inline name_t mapToLegalForDataTuple( name_t type_name ) {
				auto found = GenericToDataTupleAttributeTypeMap.find( type_name );
				if( found != GenericToDataTupleAttributeTypeMap.end() )
					return found->second; else throw IncompatibleType(); }



		private:

			// Construct a new object for the specified type
			static inline Object* _copyConstruct( name_t type, const void* value, bool data_tuple ) {
				return __copyConstruct( type, data_tuple ? mapToLegalForDataTuple( type ) : type, value ); }
			static Object* __copyConstruct( name_t type, name_t target_type, const void* value );
			static inline Object* _moveConstruct( name_t type, const void* value, bool data_tuple ) {
				return __moveConstruct( type, data_tuple ? mapToLegalForDataTuple( type ) : type, value ); }
			static Object* __moveConstruct( name_t type, name_t target_type, const void* value );




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:

			// Attribute types supported by data tuples
			static std::unordered_set<name_t> LegalDataTupleAttributeTypes;

			// Mapping between raw C++ types and Æon attribute type names
			static std::unordered_map<std::type_index, name_t> RawToEonTypeMap;

			// Mapping between general Æon attribute type names and those supported by data tuples
			static std::unordered_map<name_t, name_t> GenericToDataTupleAttributeTypeMap;
		};
	}
}

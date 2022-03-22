#pragma once

#include "TypeDefinitions.h"
#include "EonType.h"
#include <typeinfo>
#include <unordered_set>
#include <eonsource/SourceRef.h>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	namespace scope
	{
		class Scope;
	}




	namespace type
	{
		class Object;


		class Handler
		{
		public:

			// Initialize type handler
			static void init();

			// Construct an instance object for the specified raw C++ type
			// Throws [eon::type::IncompatibleType] if type is not supported
			template<typename T>
			static Object* copyConstruct( const T& value, source::Ref source ) {
				return _copyConstruct( mapType<T>(), &value, false, source ); }
			template<typename T>
			static Object* copyConstructData( const T& value, source::Ref source ) {
				return _copyConstruct( mapType<T>(), &value, true, source ); }
			template<typename T>
			static Object* moveConstruct( T&& value, source::Ref source ) {
				return _moveConstruct( mapType<T>(), &value, false, source ); }
			template<typename T>
			static Object* moveConstructData( T&& value, source::Ref source ) {
				return _moveConstruct( mapType<T>(), &value, true, source ); }

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
			static inline bool legalForDataTuple( const EonType& type ) noexcept {
				return LegalDataTupleAttributeTypes.find( type.name() ) != LegalDataTupleAttributeTypes.end(); }

			// Given an Æon type name, get the corresponding supported type for data tuples
			// Throws [eon::type::IncompatibleType] if no possible mapping to supported type!
			static inline name_t mapToLegalForDataTuple( name_t type_name ) {
				auto found = GenericToDataTupleAttributeTypeMap.find( type_name );
				if( found != GenericToDataTupleAttributeTypeMap.end() )
					return found->second; else throw IncompatibleType(); }



		private:

			// Construct a new object for the specified type
			static inline Object* _copyConstruct( name_t type, const void* value, bool data_tuple, source::Ref source ) {
				return __copyConstruct( type, data_tuple ? mapToLegalForDataTuple( type ) : type, value, source ); }
			static Object* __copyConstruct( name_t type, name_t target_type, const void* value, source::Ref source );
			static inline Object* _moveConstruct( name_t type, const void* value, bool data_tuple, source::Ref source ) {
				return __moveConstruct( type, data_tuple ? mapToLegalForDataTuple( type ) : type, value, source ); }
			static Object* __moveConstruct( name_t type, name_t target_type, const void* value, source::Ref source );




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

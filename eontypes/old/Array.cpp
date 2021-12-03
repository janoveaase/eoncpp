#include "Array.h"
#include "Instance.h"


namespace eon
{
	namespace tup
	{
		Array::Array( const typespec* element_type, varscope& scope )
		{
			ElementType = element_type;
		}




		void Array::append( const Array& other )
		{
			for( auto elm : other )
				Elements.push_back( elm->copy() );
		}




		Array* Array::copy() const
		{
			auto cpy = new Array();
			for( auto elm : Elements )
				cpy->Elements.push_back( elm->copy() );
			return cpy;
		}
		string Array::str() const
		{
			string s = "[";
			bool first = true;
			for( auto elm : Elements )
			{
				if( first )
					first = false;
				else
					s += ", ";
				s += elm->str();
			}
			s += "]";
			return s;
		}
		int Array::compare( const Array& other ) const
		{
			size_t i = 0;
			for( ; i < Elements.size() && i < other.Elements.size(); ++i )
			{
				auto cmp = Elements.at( i )->compare( *other.Elements.at( i ) );
				if( cmp != 0 )
					return cmp;
			}
			if( i < Elements.size() )
				return -1;
			else
				return i < other.Elements.size() ? 1 : 0;
		}




		void Array::registerActions( varscope& scope )
		{
			actionspec size;
			size.returnType( scope.tuple( tuplespec( { scope.primitive( name_position ) } ) ) );
			scope.registerAction( name_array, std::move( size ), new eonarray_size() );

			actionspec at_pos;
			at_pos.returnType( scope.tuple( tuplespec( { ElementType } ) ) );
			at_pos.arguments( scope.tuple( tuplespec( { scope.primitive( name_position ) } ) ) );
			scope.registerAction( name_array, std::move( at_pos ), new eonarray_at_pos() );

			actionspec at_int;
			at_int.returnType( scope.tuple( tuplespec( { ElementType } ) ) );
			at_int.arguments( scope.tuple( tuplespec( { scope.primitive( name_int ) } ) ) );
			scope.registerAction( name_array, std::move( at_int ), new eonarray_at_int() );

			actionspec append;
			append.returnType( scope.tuple( tuplespec( { scope.standard( name_array ) } ) ) );
			append.arguments( scope.tuple( tuplespec( { ElementType } ) ) );
			scope.registerAction( name_array, std::move( append ), new eonarray_append() );

			actionspec insert_pos;
			insert_pos.returnType( scope.tuple( tuplespec( { scope.standard( name_array ) } ) ) );
			insert_pos.arguments( scope.tuple( tuplespec( { scope.primitive( name_position ), ElementType } ) ) );
			scope.registerAction( name_array, std::move( insert_pos ), new eonarray_insert_pos() );

			actionspec insert_int;
			insert_int.returnType( scope.tuple( tuplespec( { scope.standard( name_array ) } ) ) );
			insert_int.arguments( scope.tuple( tuplespec( { scope.primitive( name_int ), ElementType } ) ) );
			scope.registerAction( name_array, std::move( insert_int ), new eonarray_insert_int() );
		}




		instance* eonarray_size::operator()( instance* me, varscope* scope, tuple* args ) {
			return new instance( *scope, me->arrayValue()->elements().size(), 1 ); }
		instance* eonarray_at_pos::operator()( instance* me, varscope* scope, tuple* args ) {
			return new instance( *scope, me->arrayValue()->elements().at( args->at( 0 )->positionValue() ), 1 ); }
		instance* eonarray_at_int::operator()( instance* me, varscope* scope, tuple* args ) {
			return new instance( *scope, me->arrayValue()->at( args->at( 0 )->intValue() ) ); }
		instance* eonarray_append::operator()( instance* me, varscope* scope, tuple* args ) {
			me->arrayValue()->elements().push_back( args->at( 0 ) ); return me; }
		instance* eonarray_insert_pos::operator()( instance* me, varscope* scope, tuple* args ) {
			me->arrayValue()->insert( args->at( 0 )->positionValue(), args->at( 1 ) ); return me; }
		instance* eonarray_insert_int::operator()( instance* me, varscope* scope, tuple* args ) {
			me->arrayValue()->insert( args->at( 0 )->intValue(), args->at( 1 ) ); return me; }
	}
}

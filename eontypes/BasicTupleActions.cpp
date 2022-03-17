#include "Tuple.h"
#include "DataTuple.h"
#include "BasicTupleActions.h"


namespace eon
{
	void registerBasicTupleActions( scope::Global& scope )
	{
		scope.addOperator( type::operators::code::cmp, new actions::TupleCmp<Tuple, Tuple>() );
		scope.addOperator( type::operators::code::cmp, new actions::TupleCmp<Tuple, DataTuple>() );
		scope.addOperator( type::operators::code::cmp, new actions::TupleCmp<Tuple, DynamicTuple>() );

		scope.addOperator( type::operators::code::cmp, new actions::TupleCmp<DataTuple, Tuple>() );
		scope.addOperator( type::operators::code::cmp, new actions::TupleCmp<DataTuple, DataTuple>() );
		scope.addOperator( type::operators::code::cmp, new actions::TupleCmp<DataTuple, DynamicTuple>() );

		scope.addOperator( type::operators::code::cmp, new actions::TupleCmp<DynamicTuple, Tuple>() );
		scope.addOperator( type::operators::code::cmp, new actions::TupleCmp<DynamicTuple, DataTuple>() );
		scope.addOperator( type::operators::code::cmp, new actions::TupleCmp<DynamicTuple, DynamicTuple>() );



		scope.addOperator( type::operators::code::assign, new actions::TupleAssign<Tuple, Tuple>() );
		scope.addOperator( type::operators::code::assign, new actions::TupleAssign<Tuple, DataTuple>() );
		scope.addOperator( type::operators::code::assign, new actions::TupleAssign<Tuple, DynamicTuple>() );
		
		scope.addOperator( type::operators::code::assign, new actions::TupleAssign<DataTuple, Tuple>() );
		scope.addOperator( type::operators::code::assign, new actions::TupleAssign<DataTuple, DataTuple>() );
		scope.addOperator( type::operators::code::assign, new actions::TupleAssign<DataTuple, DynamicTuple>() );
		
		scope.addOperator( type::operators::code::assign, new actions::TupleAssign<DynamicTuple, Tuple>() );
		scope.addOperator( type::operators::code::assign, new actions::TupleAssign<DynamicTuple, DataTuple>() );
		scope.addOperator( type::operators::code::assign, new actions::TupleAssign<DynamicTuple, DynamicTuple>() );


		scope.addOperator( type::operators::code::plus, new actions::TuplePlus<Tuple, Tuple, DynamicTuple>() );
		scope.addOperator( type::operators::code::plus, new actions::TuplePlus<Tuple, DataTuple, DynamicTuple>() );
		scope.addOperator( type::operators::code::plus, new actions::TuplePlus<Tuple, DynamicTuple, DynamicTuple>() );
		
		scope.addOperator( type::operators::code::plus, new actions::TuplePlus<DataTuple, DataTuple, DataTuple>() );
		
		scope.addOperator( type::operators::code::plus, new actions::TuplePlus<DynamicTuple, Tuple, DynamicTuple>() );
		scope.addOperator( type::operators::code::plus, new actions::TuplePlus<DynamicTuple, DataTuple, DynamicTuple>() );
		scope.addOperator( type::operators::code::plus, new actions::TuplePlus<DynamicTuple, DynamicTuple, DynamicTuple>() );
		

		scope.addOperator( type::operators::code::minus, new actions::TuplePlus<Tuple, Tuple, DynamicTuple>() );
		scope.addOperator( type::operators::code::minus, new actions::TuplePlus<Tuple, DataTuple, DynamicTuple>() );
		scope.addOperator( type::operators::code::minus, new actions::TuplePlus<Tuple, DynamicTuple, DynamicTuple>() );
		
		scope.addOperator( type::operators::code::minus, new actions::TupleMinus<DataTuple, DataTuple, DataTuple>() );
		
		scope.addOperator( type::operators::code::minus, new actions::TupleMinus<DynamicTuple, Tuple, DynamicTuple>() );
		scope.addOperator( type::operators::code::minus, new actions::TupleMinus<DynamicTuple, DataTuple, DynamicTuple>() );
		scope.addOperator( type::operators::code::minus, new actions::TupleMinus<DynamicTuple, DynamicTuple, DynamicTuple>() );
		

		scope.addOperator( type::operators::code::element, new actions::TupleElementIndex<Tuple>() );
		scope.addOperator( type::operators::code::element, new actions::TupleElementIndex<DataTuple>() );
		scope.addOperator( type::operators::code::element, new actions::TupleElementIndex<DynamicTuple>() );
		
		scope.addOperator( type::operators::code::element, new actions::TupleElementInt<Tuple>() );
		scope.addOperator( type::operators::code::element, new actions::TupleElementInt<DataTuple>() );
		scope.addOperator( type::operators::code::element, new actions::TupleElementInt<DynamicTuple>() );
		
		scope.addOperator( type::operators::code::element, new actions::TupleElementName<Tuple>() );
		scope.addOperator( type::operators::code::element, new actions::TupleElementName<DataTuple>() );
		scope.addOperator( type::operators::code::element, new actions::TupleElementName<DynamicTuple>() );
		
		scope.addOperator( type::operators::code::slice, new actions::TupleSlice<Tuple>() );
		scope.addOperator( type::operators::code::slice, new actions::TupleSlice<DataTuple>() );
		scope.addOperator( type::operators::code::slice, new actions::TupleSlice<DynamicTuple>() );
	}
}

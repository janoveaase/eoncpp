#include "Expression.h"
#include "TupleFactory.h"
#include "TypeTupleFactory.h"


namespace eon
{
	Expression::Expression( string&& infix, Tuple& scope )
	{
		Scope = &scope;

		Tokenizer tokenizer; _prepTokenizer( tokenizer );
		ReTokenizer retokenizer; _prepReTokenizer( retokenizer );

		source::String src( "string", std::move( infix ) );
		TokenParser temp( tokenizer( src ) );
		TokenParser parser( retokenizer( temp ) );

		source::Reporter issues;
		issues.defaultTargets();
		ParseData data( parser, issues, { ";" } );
		_parseLargeExpression( data );
	}
	Expression::Expression( TokenParser& parser, Tuple& scope, source::Reporter& reporter, std::set<string> terminate_on )
	{
		Scope = &scope;
		ParseData data( parser, reporter, terminate_on );
		_parseLargeExpression( data );
	}




	Expression& Expression::operator=( const Expression& other )
	{
		Scope = other.Scope;
		Roots = other.Roots;
		return *this;
	}
	Expression& Expression::operator=( Expression&& other ) noexcept
	{
		Scope = other.Scope; other.Scope = nullptr;
		Roots = std::move( other.Roots );
		return *this;
	}

	void Expression::str( Stringifier& strf ) const
	{
		bool first = true;
		for( auto& node : Roots )
		{
			if( first )
				first = false;
			else
				strf.punct( ";" );
			node.str( strf );
		}
	}
	string Expression::str() const
	{
		Stringifier strf;
		str( strf );
		return strf.generateString();
	}




	std::vector<Attribute> Expression::execute()
	{
		if( !Scope )
			throw type::NotFound( "Cannot execute void expression!" );
		std::vector<Attribute> results;
		for( auto& node : Roots )
			results.push_back( node.evaluate( *Scope ) );
		return results;
	}








	void Expression::_prepTokenizer( Tokenizer& tokenizer )
	{
		tokenizer.registerEonNamesAsTokens();
		tokenizer.registerSingleCharSequenceAsToken( ' ', name_space );
		tokenizer.registerSingleCharAsToken( '"', name_doublequote );
		tokenizer.registerSingleCharAsToken( '\'', name_singlequote );
		tokenizer.registerCharSequenceAsToken( charcat::number_ascii_digit, name_digits );
		tokenizer.registerSingleCharAsToken( '_', name_underscore );
		tokenizer.registerCharSequenceAsToken( charcat::letter_lowercase, name_letters );
		tokenizer.registerCharSequenceAsToken( charcat::letter_uppercase, name_letters );
		tokenizer.registerCharSequenceAsToken( charcat::letter_titlecase, name_letters );
		tokenizer.registerCharSequenceAsToken( charcat::letter_modifier, name_letters );
		tokenizer.registerCharSequenceAsToken( charcat::letter_other, name_letters );
		tokenizer.registerSingleCharAsToken( '(', symbol_open_round );
		tokenizer.registerSingleCharAsToken( ')', symbol_close_round );
		tokenizer.registerSingleCharAsToken( '@', name_at );
		tokenizer.registerSingleCharAsToken( '\\', name_backslash );
		tokenizer.registerSingleCharAsToken( '=', name_assign );
		tokenizer.registerSingleCharAsToken( '-', symbol_minus );
		tokenizer.registerSingleCharSequenceAsToken( '.', symbol_point );
		tokenizer.registerSingleCharSequenceAsToken( ':', symbol_colon );
		tokenizer.registerSingleCharAsToken( ',', symbol_comma );
		tokenizer.registerCharSequenceAsToken( "//", name( "line_comment" ) );
		tokenizer.registerCharSequenceAsToken( "/-", name( "comment_start" ) );
		tokenizer.registerCharSequenceAsToken( "-/", name( "comment_end" ) );
		tokenizer.registerSingleCharAsToken( '/', symbol_divide );
		tokenizer.registerSingleCharAsToken( ';', symbol_semicolon );
		tokenizer.registerAnySingleCharAsToken( "+*~&|^<>[]", name_operator );
		tokenizer.registerCharSequenceAsToken( "==", symbol_eq );
		tokenizer.registerCharSequenceAsToken( "!=", symbol_ne );
		tokenizer.registerCharSequenceAsToken( ">=", symbol_ge );
		tokenizer.registerCharSequenceAsToken( "<=", symbol_le );
		tokenizer.registerCharSequenceAsToken( "+=", symbol_plus_assign );
		tokenizer.registerCharSequenceAsToken( "-=", symbol_minus_assign );
		tokenizer.registerCharSequenceAsToken( "*=", symbol_multiply_assign );
		tokenizer.registerCharSequenceAsToken( "/=", symbol_divide_assign );
		tokenizer.registerCharSequenceAsToken( "'s", symbol_member );
		tokenizer.registerCharSequenceAsToken( "<<", symbol_lshift );
		tokenizer.registerCharSequenceAsToken( ">>", symbol_rshift );
		tokenizer.registerCharSequenceAsToken( "<=>", symbol_cmp );
		tokenizer.registerCharSequenceAsToken( "T(", name_typetuple );
		tokenizer.registerCharSequenceAsToken( "static(", name_static );
		tokenizer.registerCharSequenceAsToken( "optional(", name_optional );
		tokenizer.registerCharSequenceAsToken( "dynamic(", name_dynamic );
		tokenizer.registerCharSequenceAsToken( "data(", name_data );
		tokenizer.registerCharSequenceAsToken( "e(", name_expression );
		tokenizer.registerSingleCharAsToken( '#', name_hash );
	}
	void Expression::_prepReTokenizer( ReTokenizer& retokenizer )
	{
		retokenizer.addRule( new ReTokenizer::EncloseRule( name_string, name_doublequote, name_backslash ) );
		retokenizer.addRule( new ReTokenizer::PrefixEncloseRule( name_bytes, "B", name_doublequote, name_backslash ) );
		retokenizer.addRule( new ReTokenizer::PrefixEncloseRule( name_path, "p", name_doublequote, name_backslash ) );
		retokenizer.addRule( new ReTokenizer::EncloseRule( name_char, name_singlequote, name_backslash ) );
		retokenizer.addRule( new ReTokenizer::PrefixEncloseRule( name_byte, "B", name_singlequote, name_backslash ) );
		retokenizer.addRule( new ReTokenizer::PrefixEncloseRule( name_regex, "r", name_doublequote, name_backslash ) );
		retokenizer.addRule( new ReTokenizer::LinestartRule( name_indentation, name_space ) );
		retokenizer.addRule( new ReTokenizer::LiteralNameRule( name_bool, { "true", "false" } ) );
		retokenizer.addRule( new ReTokenizer::SequenceRule( name_float, { name_digits, symbol_point, name_digits } ) );
		retokenizer.addRule( new ReTokenizer::SequenceRule( name_literal, { name_hash, name_name } ) );
		retokenizer.addRule( new ReTokenizer::PrefixAlternatingRule( name_namepath, "@", name_name, symbol_divide ) );
		retokenizer.addRule( new ReTokenizer::EncloseRule( name_comment, name( "linecomment" ), name_newline ) );
		retokenizer.addRule(
			new ReTokenizer::EncloseRule( name_comment, name( "commentstart" ), name( "commentend" ) ) );
		retokenizer.addRule( new ReTokenizer::RemoveRule( { name_space } ) );
	}

	void Expression::_parseLargeExpression( ParseData& data )
	{
		do
		{
			_parseSmallExpression( data );
		} while( !_endOfLargeExpression( data ) );
		if( data.errors() )
			throw InvalidExpression();
	}

	void Expression::_parseSmallExpression( ParseData& data )
	{
		data.recordOperatorAsLastSeen();
		if( _parseExpressionDetails( data ) )
		{
			Roots.push_back( std::move( data.operands().top() ) );
			data.operands().pop();
		}
	}

	bool Expression::_parseExpressionDetails( ParseData& data )
	{
		while( !_endOfSmallExpression( data ) )
		{
			if( !_parseDetails( data ) )
				break;
		}
		_finishExpressionDetails( data );
		return !data.operands().empty();
	}

	void Expression::_reportUnknownElementError( ParseData& data )
	{
		data.error();
		data.reporter().error(
			"Unknown element \"" + data.parser().viewed().str() + "\"", data.parser().viewed().source() );
		data.parser().forward();
	}


	bool Expression::_parseDetails( ParseData& data )
	{
		if( _parseLiteral( data ) )
		{
			data.recordOperandAsLastSeen();
			data.parser().forward();
		}
		else if( _parseOperator( data ) )
			data.parser().forward();
		else if( _parseTuple( data ) )
			data.parser().forward();
		else if( _parseName( data ) )
			data.parser().forward();
		else if( _parseExpression( data ) )
			data.parser().forward();
		else
		{
			_reportUnknownElementError( data );
			return false;
		}
		return true;
	}

	bool Expression::_parseLiteral( ParseData& data )
	{
		auto type = data.parser().viewed().type();
		if( type == name_bool )
			return _parseBoolLiteral( data );
		else if( type == name_byte )
			return _parseByteLiteral( data );
		else if( type == name_char )
			return _parseCharLiteral( data );
		else if( type == name_int )
			return _parseIntLiteral( data );
		else if( type == name_float )
			return _parseFloatLiteral( data );
		else if( type == name_digits )
			return _parseIntLiteral( data );
		else if( type == name_bytes )
			return _parseBytesLiteral( data );
		else if( type == name_string )
			return _parseStringLiteral( data );
		else if( type == name_regex )
			return _parseRegexLiteral( data );
		else if( type == name_namepath )
			return _parseNamepathLiteral( data );
		else if( type == name_path )
			return _parsePathLiteral( data );
		return false;
	}

	bool Expression::_parseBoolLiteral( ParseData& data )
	{
		data.operands().push(
			expression::Node::newValue(
				Attribute::newExplicit(
					data.parser().viewed().str() == "true",
					name_bool,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					data.parser().viewed().source() ) ) );
		return true;
	}

	bool Expression::_parseByteLiteral( ParseData& data )
	{
		data.operands().push(
			expression::Node::newValue(
				Attribute::newExplicit(
					static_cast<byte_t>( *data.parser().viewed().str().begin() ),
					name_byte,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					data.parser().viewed().source() ) ) );
		return true;
	}

	bool Expression::_parseCharLiteral( ParseData& data )
	{
		data.operands().push(
			expression::Node::newValue(
				Attribute::newExplicit(
					*data.parser().viewed().str().begin(),
					name_char,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					data.parser().viewed().source() ) ) );
		return true;
	}

	bool Expression::_parseIntLiteral( ParseData& data )
	{
		// If the size of the literal value exceeds that which int can hold, we make it a long!
		const long_t value = data.parser().viewed().str().toLongT();
		if( value < EON_INT_MIN || value > EON_INT_MAX )
		{
			data.operands().push(
				expression::Node::newValue(
					Attribute::newExplicit(
						value,
						name_long,
						type::Qualifier::_literal | type::Qualifier::_rvalue,
						data.parser().viewed().source() ) ) );
		}
		else
		{
			data.operands().push(
				expression::Node::newValue(
					Attribute::newExplicit(
						static_cast<int_t>( value ),
						name_int,
						type::Qualifier::_literal | type::Qualifier::_rvalue,
						data.parser().viewed().source() ) ) );
		}
		return true;
	}

	bool Expression::_parseFloatLiteral( ParseData& data )
	{
		// If the size of the literal value exceeds that which float can hold, we make it a high!
		const high_t value = data.parser().viewed().str().toHighT();
		if( value < EON_FLOAT_MIN || value > EON_FLOAT_MAX )
		{
			data.operands().push(
				expression::Node::newValue(
					Attribute::newExplicit(
						value,
						name_high,
						type::Qualifier::_literal | type::Qualifier::_rvalue,
						data.parser().viewed().source() ) ) );
		}
		else
		{
			data.operands().push(
				expression::Node::newValue(
					Attribute::newExplicit(
						static_cast<flt_t>( value ),
						name_float,
						type::Qualifier::_literal | type::Qualifier::_rvalue,
						data.parser().viewed().source() ) ) );
		}
		return true;
	}

	bool Expression::_parseBytesLiteral( ParseData& data )
	{
		data.operands().push(
			expression::Node::newValue(
				Attribute::newExplicit(
					data.parser().viewed().bytes(),
					name_bytes,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					data.parser().viewed().source() ) ) );
		return true;
	}

	bool Expression::_parseStringLiteral( ParseData& data )
	{
		data.operands().push(
			expression::Node::newValue(
				Attribute::newExplicit(
					data.parser().viewed().str(),
					name_string,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					data.parser().viewed().source() ) ) );
		return true;
	}

	bool Expression::_parseRegexLiteral( ParseData& data )
	{
		data.operands().push(
			expression::Node::newValue(
				Attribute::newExplicit(
					regex( data.parser().viewed().str() ),
					name_regex,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					data.parser().viewed().source() ) ) );
		return true;
	}

	bool Expression::_parseNamepathLiteral( ParseData& data )
	{
		data.operands().push(
			expression::Node::newValue(
				Attribute::newExplicit(
					namepath( data.parser().viewed().str().slice( 1, -1 ) ),
					name_namepath,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					data.parser().viewed().source() ) ) );
		return true;
	}

	bool Expression::_parsePathLiteral( ParseData& data )
	{
		data.operands().push(
			expression::Node::newValue(
				Attribute::newExplicit(
					path( data.parser().viewed().str() ),
					name_path,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					data.parser().viewed().source() ) ) );
		return true;
	}


	bool Expression::_parseOperator( ParseData& data )
	{
		auto type = data.parser().viewed().type();
		if( type == name_operator )
			return _processOperator( data );
		else if( type::Operators::isOperator( type ) )
			return _processOperatorName( data, type );
		else if( type == symbol_semicolon )
			return _processOperator( data, type, data.parser().viewed().source() );
		else if( type == name_assign )
			return _processOperator( data, symbol_assign, data.parser().viewed().source() );

		return false;
	}

	bool Expression::_processOperator( ParseData& data )
	{
		// This is one of: +*=~&|^<>[]
		name_t op_name = compilerName( data.parser().viewed().str() );
		return _processOperator( data, op_name, data.parser().viewed().source() );
	}

	bool Expression::_processOperatorName( ParseData& data, name_t name )
	{
		if( compilerName( data.parser().viewed().str() ) == symbol_minus && data.lastSeen() == Seen::_operator )
			name = name_unary_minus;
		return _processOperator( data, name, data.parser().viewed().source() );
	}

	bool Expression::_processOperator( ParseData& data, name_t op_name, source::Ref op_source )
	{
		if( op_name == symbol_semicolon )
			return _processEndOfSmallExpression( data );
		else if( op_name == symbol_close_round )
			return _processCloseParen( data );
		else
			return _processPlainOperator( data, op_name, op_source );
		return false;
	}

	bool Expression::_processEndOfSmallExpression( ParseData& data )
	{
		while( !data.operators().empty() )
			_bindOperator( data );
		data.recordOperatorAsLastSeen();
		return true;
	}

	bool Expression::_processCloseParen( ParseData& data )
	{
		while( !data.operators().empty() && data.operators().top().Op != symbol_open_round )
			_bindOperator( data );
		if( !data.operators().empty() )
			data.operators().pop();
		else
		{
			data.error();
			data.reporter().error( "Unbalanced parenthesis!", data.parser().viewed().source() );
		}
		data.recordOperatorAsLastSeen();
		return true;
	}

	bool Expression::_processPlainOperator( ParseData& data, name_t new_op_name, source::Ref new_op_source )
	{
		while( _canBindOperator( data, new_op_name ) )
		{
			if( new_op_name == symbol_open_round )
			{
				data.error();
				data.reporter().error( "Unbalanced parenthesis!", data.parser().viewed().source() );
			}
			else
				_bindOperator( data );
		}
		if( type::Operators::isFirstInSequence( new_op_name ) )
			_resolveOperatorSequence( data, new_op_name, new_op_source );
		data.recordOperatorAsLastSeen();
		return true;
	}
	bool Expression::_canBindOperator( ParseData& data, name_t new_op_name )
	{
		return !data.operators().empty()
			&& type::Operators::inputPrecedence( new_op_name )
				<= type::Operators::stackPrecedence( data.operators().top().Op );
	}


	void Expression::_finishExpressionDetails( ParseData& data )
	{
		if( data.parser() && data.parser().viewed().is( symbol_semicolon ) )
			data.parser().forward();
		while( !data.operators().empty() )
			_bindOperator( data );
	}

	void Expression::_bindOperator( ParseData& data )
	{
		if( data.operators().top().Op == symbol_assign )
			_bindAssign( data );
		else
			_bindNormal( data );
	}

	void Expression::_bindAssign( ParseData& data )
	{
		expression::Node lhs, rhs;
		if( !_getOperands( data, { &rhs, &lhs } ) )
			return;

		if( !_isLvalue( data, lhs ) )
			return;

		// Option 1: The types are identical - we can do a direct copy or move!
		if( lhs.type() == rhs.type() )
			_assignDirect( data, lhs, rhs );

		// Option 2: Implicit conversion available - we can convert and do a direct copy or move!
		else if( _canConvertDirectly( data, lhs.type(), rhs ) )
			_assignDirectByConversion( data, lhs, rhs );

		// Option 3: Tuple copy (direct or compatible)!
		else if( rhs.type().isTuple() )
			_assignTuple( data, lhs, rhs );

		// Option 4: Conversion to same type by constructor!
		else
			_assignByConstructor( data, lhs, rhs );
	}

	void Expression::_bindNormal( ParseData& data )
	{
		auto op = expression::Node::newOperator( data.operators().top().Op, data.operators().top().Source );
		_bindOperands( data, op );
		_bindOperatorAction( data, op );
		data.operands().push( std::move( op ) );
		data.operators().pop();
	}

	void Expression::_bindOperands( ParseData& data, expression::Node& op )
	{
		for( size_t i = 0; i < type::Operators::numOperands( op.name() ); ++i )
		{
			if( data.operands().empty() )
			{
				data.error();
				data.reporter().error( "Operator is missing operand!", op.source() );
				break;
			}
			op.addOperand( std::move( data.operands().top() ) );
			data.operands().pop();
		}
	}

	void Expression::_bindOperatorAction( ParseData& data, expression::Node& op )
	{
		if( !op.bindOperator( *Scope, data.reporter() ) )
		{
			data.reporter().fatal( "Operator not implemented!", op.source() );
			data.error();
		}
	}

	bool Expression::_getOperands( ParseData& data, std::initializer_list<expression::Node*> operands )
	{
		for( auto op : operands )
		{
			if( data.operands().empty() )
			{
				data.error();
				data.reporter().error( "Operator is missing operand!", data.operators().top().Source );
				data.operators().pop();
				return false;
			}
			*op = std::move( data.operands().top() );
			data.operands().pop();
		}
		return true;
	}

	bool Expression::_isLvalue( ParseData& data, const expression::Node& operand ) const
	{
		if( operand.value().isRvalue() )
		{
			data.error();
			data.reporter().error( "Cannot assign to an rvalue!", operand.source() );
			data.operators().pop();
			return false;
		}
		return true;
	}

	void Expression::_assignDirect( ParseData& data, expression::Node& lhs, expression::Node& rhs ) const
	{
		name_t type = lhs.type().name();
		auto args = typetuple::convert( { lhs.type() } );
		auto action_signature = rhs.value().isRvalue()
			? _findAction( data, name_take, name_operator, type, args )
			: _findAction( data, type::name_copy, name_operator, type, args );
		if( !action_signature )
			return;

		_bindAction( data, lhs, rhs, action_signature );
	}

	TypeTuple Expression::_findAction( ParseData& data, name_t name, name_t classification, name_t type, TypeTuple args ) const
	{
		std::set<TypeTuple> action_signatures = scope::global().signatures( name, classification, type, args );
		if( action_signatures.size() == 1 )
			return *action_signatures.begin();

		if( action_signatures.empty() )
			_directAssignNotSupportedError( data, type );
		else if( action_signatures.size() > 1 )	// This should not be possible ...
			_directAssignMultipleOptionsError( data, name, type, args, action_signatures );

		return TypeTuple();
	}

	void Expression::_directAssignNotSupportedError( ParseData& data, name_t type ) const
	{
		data.error();
		data.reporter().error(
			"Direct assignment of T(" + eon::str( type ) + ") is not supported!",
			data.operators().top().Source );
		data.operators().pop();
	}

	void Expression::_directAssignMultipleOptionsError(
		ParseData& data, name_t name, name_t type, TypeTuple args, const std::set<TypeTuple>& action_types ) const
	{
		data.error();
		data.reporter().error(
			"Multiple " + eon::str( name ) + " options for T(" + eon::str( type ) + ") from " + args.str() + "!",
			data.operators().top().Source );
		bool first = true;
		for( auto& type : action_types )
		{
			string lead = first ? "Could be" : "or";
			first = false;
			auto action = scope::global().action( type );
			data.reporter().note( lead + " from " + type.str() + " ...", action ? action->source() : source::Ref() );
		}
	}

	void Expression::_bindAction(
		ParseData& data, expression::Node& lhs, expression::Node& rhs, TypeTuple action_signature ) const
	{
		auto action = scope::global().action( action_signature );
		auto assign = expression::Node::newCall( *action, data.operators().top().Source );
		assign.addOperand( std::move( rhs ) );
		assign.addOperand( std::move( lhs ) );
		data.operands().push( std::move( assign ) );
		data.operators().pop();
	}

	void Expression::_assignDirectByConversion( ParseData& data, expression::Node& lhs, expression::Node& rhs ) const
	{
		// We need to convert rhs to the type of lhs
		rhs = expression::Node::newValue( data.Converter.convert( rhs, lhs.type().name() ) );

		// Now we can do direct conversion
		_assignDirect( data, lhs, rhs );
	}

	void Expression::_assignTuple( ParseData& data, expression::Node& lhs, expression::Node& rhs ) const
	{
		if( !rhs.type().compatibleWith( lhs.type() ) )
		{
			_incompatibleTupleError( data, lhs, rhs );
			return;
		}

		auto action_signature = _findAction(
			data, name_assign, name_operator, name_tuple, typetuple::convert( { name_tuple } ) );
		if( action_signature )
			_bindAction( data, lhs, rhs, action_signature );
	}

	void Expression::_incompatibleTupleError( ParseData& data, expression::Node& lhs, expression::Node& rhs ) const
	{
		data.error();
		data.reporter().error(
			"Cannot assign incompatible right operand " + rhs.type().str() + " to left operand "
			+ lhs.type().str() + "!",
			lhs.source() );
		data.operators().pop();
	}

	void Expression::_assignByConstructor( ParseData& data, expression::Node& lhs, expression::Node& rhs ) const
	{
		auto constructor_signature = _findAction(
			data, name_constructor, name_constructor, lhs.type().name(), typetuple::convert( { rhs.type() } ) );
		if( !constructor_signature )
			return;
		auto assign_signature = _findAction(
			data, name_take, name_operator, lhs.type().name(), typetuple::convert( { lhs.type().name() } ) );
		if( !assign_signature )
			return;

		auto constructor_action = scope::global().action( constructor_signature );
		auto constructor = expression::Node::newCall( *constructor_action, data.operators().top().Source );
		constructor.addOperand( std::move( rhs ) );
		auto assign_action = scope::global().action( assign_signature );
		auto assign = expression::Node::newCall( *assign_action, data.operators().top().Source );
		assign.addOperand( std::move( lhs ) );
		assign.addOperand( std::move( constructor ) );
		data.operands().push( std::move( assign ) );
		data.operators().pop();
	}

	bool Expression::_canConvertDirectly( ParseData& data, const TypeTuple& lhs, const expression::Node& rhs ) const noexcept
	{
		if( !rhs.value().isLiteral() )		// Rhs must be a literal value!
			return false;
		return data.Converter.legal( lhs.name(), rhs );
	}


	void Expression::_resolveOperatorSequence( ParseData& data, name_t new_op_name, source::Ref new_op_source )
	{
		// If this is a sequence operator, then parse whatever is inbetween as if parenthesis
		name_t op_name = _resolveOperator( data, new_op_name );
		if( op_name != no_name )
			data.operators().push( Operator( op_name, new_op_source ) );
	}

	name_t Expression::_resolveOperator( ParseData& data, name_t op_name )
	{
		std::vector<name_t> all_options = type::Operators::mapOperator( op_name );
		if( all_options.empty() )
		{
			data.reporter().error( "Internal issue with this operator!", data.parser().viewed().source() );
			throw InvalidExpression();	// Impossible!?
		}

		if( _singleOptionNoSequence( all_options ) )
			return all_options[ 0 ];
		else
			return _resolveNonTrivalOperator( all_options, data, op_name );
	}

	name_t Expression::_resolveNonTrivalOperator( std::vector<name_t>& all_options, ParseData& data, name_t op_name )
	{
		// We have a single option with a sequence, or multiple options (which means all have sequences)!
		std::vector<expression::Node> operands;
		for( index_t seq_no = 1; ; ++seq_no )
		{
			auto result = _resolveNonTrivalOperator( all_options, data, op_name, operands, seq_no );
			if( result != name_nn )
				return result;
		}
		return no_name;
	}

	name_t Expression::_resolveNonTrivalOperator(
		std::vector<name_t>& all_options,
		ParseData& data,
		name_t op_name,
		std::vector<expression::Node>& operands,
		index_t seq_no )
	{
		name_t singular_op_name = _identifySingularOperator( data, all_options, op_name, seq_no );
		if( singular_op_name != no_name )
			return _processSequenceOperator( data, singular_op_name, std::move( operands ) );

		auto remaining_options = _discardPrefixOperatorOptions( all_options, seq_no );
		if( remaining_options.empty() )
			return _operatorMissingElementSyntaxError( data, all_options, seq_no );

		// If we have only one option left, we can continue with that.
		if( remaining_options.size() == 1 )
			return _processSequenceOperator( data, remaining_options[ 0 ], std::move( operands ) );

		// We still have multiple options left and must parse the next operand before we can get to the next operator name
		source::Ref orig_source = data.parser().viewed().source();
		_parseNextNonTrivalOperatorOperand( data, remaining_options, operands, seq_no );

		// If at the end of the tokens, we have a syntax error
		if( !data.parser() )
			return _incompleteOperatorError( data, orig_source );

		// Eliminate options for which the next token does not match the next operator in sequence
		all_options = _matchNextSequenceOperator( data, remaining_options, seq_no );

		return name_nn;
	}

	name_t Expression::_identifySingularOperator(
		ParseData& data, const std::vector<name_t>& options, name_t op_name, index_t seq_no )
	{
		for( auto& option : options )
		{
			auto& sequence = type::Operators::sequence( option );
			if( seq_no >= sequence.size() )
				return option;	// All operators in the sequence have been matched!
			if( !sequence[ seq_no ].prefix() )
				continue;

			auto next_type = data.parser().peekAhead().type();
			if( next_type == sequence[ seq_no ].Op
				|| ( next_type == name_name && name( data.parser().peekAhead().str() ) == sequence[ seq_no ].Op )
				|| ( next_type == name_operator && compilerName( data.parser().peekAhead().str() ) == sequence[ 1 ].Op ) )
			{
				data.parser().forward();
				return option;
			}
		}
		return no_name;
	}

	std::vector<name_t> Expression::_discardPrefixOperatorOptions( const std::vector<name_t>& options, index_t seq_no )
	{
		std::vector<name_t> remaining;
		for( auto& option : options )
		{
			auto& sequence = type::Operators::sequence( option );
			if( seq_no < sequence.size() && !sequence[ seq_no ].prefix() )
				remaining.push_back( option );
		}
		return remaining;
	}

	name_t Expression::_operatorMissingElementSyntaxError(
		ParseData& data, std::vector<name_t>& all_options, index_t seq_no )
	{
		data.error();
		data.reporter().error( "This operator requires an additional element!", data.parser().viewed().source() );
		for( auto& option : all_options )
		{
			auto& sequence = type::Operators::sequence( option );
			if( seq_no < sequence.size() )
			{
				string lead = option == all_options[ 0 ] ? "  Perhaps" : "  or perhaps";
				data.reporter().note( lead + " this one? : " + eon::str( sequence[ seq_no ].Op ), source::Ref() );
			}
		}
		return no_name;
	}

	std::set<string> Expression::_findStopOn( const std::vector<name_t>& options, index_t seq_no )
	{
		std::set<string> stop_on;
		for( auto& option : options )
			stop_on.insert( eon::str( type::Operators::sequence( option )[ seq_no ].Op ) );
		return stop_on;
	}

	std::vector<name_t> Expression::_matchNextSequenceOperator(
		ParseData& data, const std::vector<name_t>& options, index_t seq_no )
	{
		std::vector<name_t> remaining;
		for( auto& option : options )
		{
			auto& sequence = type::Operators::sequence( option );
			if( seq_no >= sequence.size() )	// Not possible!?
				continue;
			auto next_type = data.parser().viewed().type();
			if( next_type == sequence[ seq_no ].Op
				|| ( next_type == name_name && name( data.parser().viewed().str() ) == sequence[ seq_no ].Op )
				|| ( next_type == name_operator
					&& compilerName( data.parser().viewed().str() ) == sequence[ seq_no ].Op ) )
				remaining.push_back( option );
		}
		return remaining;
	}

	name_t Expression::_processSequenceOperator( ParseData& data, name_t op_name, std::vector<expression::Node>&& operands )
	{
		// Get remaining operands - if any
		auto& sequence = type::Operators::sequence( op_name );
		for( index_t seq_no = operands.size() + 1; seq_no < sequence.size(); ++seq_no )
		{
			data.parser().forward();	// Must move past the operator!
			ParseData data2( data, { eon::str( sequence[ seq_no ].Op ) } );
			if( _parseExpressionDetails( data2 ) )
				operands.push_back( std::move( data2.operands().top() ) );
			else
			{
				data.error();
				return no_name;
			}
		}

		// Push all that's left and we're done!
		for( auto& operand : operands )
			data.operands().push( std::move( operand ) );
		return op_name;
	}

	void Expression::_parseNextNonTrivalOperatorOperand(
		ParseData& data, const std::vector<name_t>& options, std::vector<expression::Node>& operands, index_t seq_no )
	{
		data.parser().forward();	// Must move past the operator!
		ParseData data2( data, _findStopOn( options, seq_no ) );
		if( _parseExpressionDetails( data2 ) )
			operands.push_back( std::move( data2.operands().top() ) );
	}

	name_t Expression::_incompleteOperatorError( ParseData& data, source::Ref orig_source )
	{
		data.error();
		data.reporter().error( "Incomplete operator!", orig_source );
		return no_name;
	}


	bool Expression::_parseTuple( ParseData& data )
	{
		auto type = data.parser().viewed().type();
		if( type == name_static )
			return _parseTuple( data, type );
		else if( type == name_typetuple )
			return _parseTypeTuple( data );
		else if( type == name_dynamic || type == name_data )
			return _badTupleInExpressionError( data, type );
		return false;
	}

	bool Expression::_parseTuple( ParseData& data, name_t tupletype )
	{
		source::Ref source = data.parser().viewed().source();
		data.parser().forward();	// Skip tuple identifier

		auto attributes = _parseTupleAttributes( data );
		if( !data.parser() )
			return _operandNotProperlyEndedError( data, "Tuple", source, false );

		auto tuple = eon::tuple::newCustom( tupletype, std::move( attributes ) );
		data.operands().push(
			expression::Node::newValue( Attribute::newTuple( std::move( tuple ), type::Qualifier::_rvalue, source ) ) );
		return true;
	}

	bool Expression::_badTupleInExpressionError( ParseData& data, name_t type )
	{
		data.error();
		data.reporter().error( "Cannot declare " + eon::str( type ) + " tuple inside expression! (Only static tuple!)",
			data.parser().viewed().source() );
		data.parser().forward();
		// TODO: Move parser past the entire tuple to avoid additional errors about it!
		return false;
	}

	std::vector<AttributePair> Expression::_parseTupleAttributes( ParseData& data )
	{
		std::vector<AttributePair> attributes;

		// Each attribute is an expression we must evaluate.
		// Read attributes until we see a ')'!
		ParseData data2( data, { ",", ")" } );
		while( data2.parser() && !data2.parser().viewed().is( symbol_close_round ) )
		{
			_parseTupleAttribute( data2, attributes );
			if( data.parser().viewed().is( symbol_comma ) )
				data.parser().forward();
		}

		return attributes;
	}

	void Expression::_parseTupleAttribute( ParseData& data, std::vector<AttributePair>& attributes )
	{
		auto attrib_name = _parseTupleAttributeName( data );
		if( _parseExpressionDetails( data ) )
		{
			attributes.push_back( AttributePair( attrib_name, std::move( data.operands().top().value() ) ) );
			data.operands().pop();
		}
	}

	name_t Expression::_parseTupleAttributeName( ParseData& data )
	{
		name_t attrib_name = no_name;
		if( data.parser().viewed().is( name_name )
			&& data.parser().exists() && data.parser().peekAhead().type() == name_assign )
		{
			attrib_name = eon::name( data.parser().viewed().str() );
			data.parser().forward( 2 );
		}
		return attrib_name;
	}


	bool Expression::_parseTypeTuple( ParseData& data )
	{
		source::Ref source = data.parser().viewed().source();
		auto ttuple = _parseTypeTupleDetails( data );
		if( !ttuple )
			return false;
		ttuple.expandSubTuples();
		data.operands().push(
			expression::Node::newValue( Attribute::newTypeTuple( std::move( ttuple ), type::Qualifier::_rvalue, source ) ) );
		return true;
	}

	TypeTuple Expression::_parseTypeTupleDetails( ParseData& data )
	{
		source::Ref source = data.parser().viewed().source();
		data.parser().forward();	// Skip tuple identifier

		// A type tuple consists of attributes that are either names or parenthesized sub-tuples, and can be either
		// unnamed or named.
		TypeTuple ttuple;
		_parseTypeTupleAttributes( data, ttuple );

		if( !data.parser() )
			return _operandNotProperlyEndedError( data, "Type tuple", source, TypeTuple() );

		return ttuple;
	}

	void Expression::_parseTypeTupleAttributes( ParseData& data, TypeTuple& ttuple )
	{
		while( data.parser() && !data.parser().viewed().is( symbol_close_round ) )
		{
			auto attrib_name = _parseTupleAttributeName( data );
			_parseTypeTupleAttributeValue( data, attrib_name, ttuple );

			if( !data.parser() )
				break;
			if( data.parser().viewed().is( symbol_comma ) )
				data.parser().forward();
			else if( !data.parser().viewed().is( symbol_close_round ) )
			{
				data.error();
				data.reporter().error( "Expected comma or ')' here!", data.parser().viewed().source() );
			}
		}
	}

	void Expression::_parseTypeTupleAttributeValue( ParseData& data, name_t attrib_name, TypeTuple& ttuple )
	{
		// Do we have a sub-tuple value?
		if( data.parser().viewed().is( symbol_open_round ) )
			_parseTypeTupleTupleAttributeValue( data, attrib_name, ttuple );

		// Must be a name
		else if( data.parser().viewed().is( name_name ) )
		{
			if( attrib_name == no_name )
				ttuple.add( eon::name( data.parser().viewed().str() ) );
			else
				ttuple.set( attrib_name, eon::name( data.parser().viewed().str() ) );
			data.parser().forward();
		}

		else
		{
			data.error();
			data.reporter().error( "Invalid type tuple attribute!", data.parser().viewed().source() );
			data.parser().forward();
		}
	}

	void Expression::_parseTypeTupleTupleAttributeValue( ParseData& data, name_t attrib_name, TypeTuple& ttuple )
	{
		auto sub = _parseTypeTupleDetails( data );
		if( sub )
		{
			sub.expandSubTuples();
			if( attrib_name == no_name )
				ttuple.add( std::move( sub ) );
			else
				ttuple.set( attrib_name, std::move( sub ) );
			data.parser().forward();
		}
	}


	bool Expression::_parseName( ParseData& data )
	{
		if( data.parser().viewed().type() == name_literal )
			return _parseNameLiteral( data );
		else if( data.parser().viewed().type() != name_name )
			return false;

		// NOTE: The name can be: a variable, a name value, an operator or part of an operator, or an action call.

		name_t name = eon::name( data.parser().viewed().str() );
		if( type::Operators::isOperator( name ) )
			return _processOperator( data, name, data.parser().viewed().source() );

		// If the name is followed by a '(' it must be an action call!
		if( data.parser().exists() && data.parser().peekAhead().is( symbol_open_round ) )
			return _parseActionCall( data, name );

		// If followed by ' or ", it must be a misspelled prefix
		if( data.parser().exists()
			&& ( data.parser().peekAhead().is( name_string ) || data.parser().peekAhead().is( name_char ) ) )
			return _misspelledPrefixError( data, name );

		// Can be a variable
		auto var = Scope->findAttributeIncludeParent( name );
		if( var )
			return _processVariable( data, *var, name );

		// Assuming name value!
		return _parseNameLiteral( data, name );
	}

	bool Expression::_parseNameLiteral( ParseData& data, name_t name )
	{
		data.operands().push(
			expression::Node::newValue(
				Attribute::newExplicit(
					name != no_name ? name : eon::name( data.parser().viewed().str().slice( 1, -1 ) ),
					name_name,
					type::Qualifier::_literal | type::Qualifier::_rvalue,
					data.parser().viewed().source() ) ) );
		return true;
	}

	bool Expression::_misspelledPrefixError( ParseData& data, name_t prefix )
	{
		static std::map<name_t, string> suggestions{
			{ eon::name( "b" ), "'B' for Byte(s)" },
			{ eon::name( "P" ), "'p' for path" },
			{ eon::name( "R" ), "'r' for regex" } };
		data.error();
		string tip;
		auto suggestion = suggestions.find( prefix );
		if( suggestion != suggestions.end() )
			tip = " Perhaps you meant " + suggestion->second + "?";
		data.reporter().error( "This is not a supported prefix!" + tip, data.parser().viewed().source() );

		// We handled this "name" so move on to the next token!
		return true;
	}

	bool Expression::_processVariable( ParseData& data, Attribute& variable, name_t name )
	{
		data.operands().push( expression::Node::newValue( Attribute( variable ), name ) );
		data.operands().top().source( data.parser().viewed().source() );
		return true;
	}


	bool Expression::_parseActionCall( ParseData& data, name_t action_name )
	{
		source::Ref source = data.parser().viewed().source();
		data.parser().forward( 2 );		// Skip name + '('

		auto args = _parseActionCallArgs( data );
		if( !data.parser() )
			return _operandNotProperlyEndedError( data, "Action call", source, false );

		// If the action name is a type, we have a constructor
		if( Scope->isType( action_name ) )
			return _processConstructorCall( data, action_name, args, source );

		// TODO: Add support for other action types!
		return false;
	}

	Expression::CallArgs Expression::_parseActionCallArgs( ParseData& data )
	{
		CallArgs args;

		// Each argument is an expression we must evaluate.
		// Read arguments until we see a ')'!
		ParseData data2( data, { ",", ")" } );
		while( data2.parser() && !data2.parser().viewed().is( symbol_close_round ) )
		{
			auto arg_name = _parseTupleAttributeName( data2 );
			if( _parseExpressionDetails( data2 ) )
			{
				args.Args.push_back( std::move( data2.operands().top() ) );
				data2.operands().pop();
				if( args.Args[ args.Args.size() - 1 ].isOperator() )
				{
					if( arg_name == no_name )
						args.ArgTypes.add( args.Args[ args.Args.size() - 1 ].type() );
					else
						args.ArgTypes.set( arg_name, args.Args[ args.Args.size() - 1 ].type() );
				}
				else
				{
					if( arg_name == no_name )
						args.ArgTypes.add( args.Args[ args.Args.size() - 1 ].value().type() );
					else
						args.ArgTypes.set( arg_name, args.Args[ args.Args.size() - 1 ].value().type() );
				}
			}

			if( data.parser().viewed().is( symbol_comma ) )
				data.parser().forward();
		}
		return args;
	}

	bool Expression::_processConstructorCall( ParseData& data, name_t action_name, CallArgs& args, source::Ref source )
	{
		auto constructor_signature = _findAction( data, name_constructor, name_constructor, action_name, args.ArgTypes );
		if( !constructor_signature )
			return true;	// Returning true to indicate we have handled this!
		auto call = expression::Node::newCall( *Scope->action( constructor_signature ), source );
		for( auto& arg : args.Args )
			call.addOperand( std::move( arg ) );
		data.operands().push( std::move( call ) );
		return true;
	}


	bool Expression::_parseExpression( ParseData& data )
	{
		if( !data.parser().viewed().is( name_expression ) )
			return false;

		source::Ref source = data.parser().viewed().source();
		data.parser().forward();	// Skip 'e('
		try
		{
			Expression expr( data.parser(), *Scope, data.reporter(), { ")" } );
			data.operands().push(
				expression::Node::newValue(
					Attribute::newExplicit( std::move( expr ), name_expression, type::Qualifier::_rvalue, source ) ) );
			return true;
		}
		catch( ... )
		{
			return false;
		}
	}
}

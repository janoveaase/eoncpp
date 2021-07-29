#include "Operators.h"
#include <unordered_map>


namespace eon
{
	namespace tup
	{
		std::vector<operators::OperatorDetails> operators::Ops{
			OperatorDetails( code::undef, 2, 0, -1 ),
			OperatorDetails( code::value, 2, 0, 0 ),
			OperatorDetails( code::open_brace, 2, 100, 0 ),
			OperatorDetails( code::close_brace, 2, 0, 99 ),
			OperatorDetails( code::_and, 2, 23, 24 ),
			OperatorDetails( code::_or, 2, 21, 22 ),
			OperatorDetails( code::_not, 1, 73, 74 ),
			OperatorDetails( code::if_else, 3, 12, 11 ),
			OperatorDetails( code::eq, 2, 31, 32 ),
			OperatorDetails( code::ne, 2, 31, 32 ),
			OperatorDetails( code::lt, 2, 33, 34 ),
			OperatorDetails( code::le, 2, 33, 34 ),
			OperatorDetails( code::gt, 2, 33, 34 ),
			OperatorDetails( code::ge, 2, 33, 34 ),
			OperatorDetails( code::plus, 2, 41, 42 ),
			OperatorDetails( code::minus, 2, 41, 42 ),
			OperatorDetails( code::unary_minus, 1, 83, 84 ),
			OperatorDetails( code::multiply, 2, 51, 52 ),
			OperatorDetails( code::divide, 2, 51, 52 ),
			OperatorDetails( code::pow, 2, 62, 61 ),
			OperatorDetails( code::mod, 2, 51, 52 ),
			OperatorDetails( code::sqrt, 1, 71, 72 ),
			OperatorDetails( code::match, 2, 33, 34 ),
			OperatorDetails( code::assign, 2, 1, 2 ),
			OperatorDetails( code::exists, 1, 71, 72 ),
			OperatorDetails( code::trim, 1, 73, 74 ),
			OperatorDetails( code::rtrim, 1, 73, 74 ),
			OperatorDetails( code::ltrim, 1, 73, 74 ),
			OperatorDetails( code::len, 1, 73, 74 ),
			OperatorDetails( code::resize, 2, 43, 44 ),
			OperatorDetails( code::typeof, 1, 91, 92 ),
			OperatorDetails( code::isname, 1, 91, 92 ),
			OperatorDetails( code::toname, 1, 73, 74 ),
			OperatorDetails( code::topath, 1, 73, 74 ),
			OperatorDetails( code::basename, 1, 73, 74 )
		};

		operators::code operators::mapCode( const string& str )
		{
			static std::unordered_map<string, code> types{
				{ "(", code::open_brace },
				{ ")", code::close_brace },
				{ "and", code::_and },
				{ "or", code:: _or },
				{ "not", code::_not },
				{ "if", code::if_else },
				{ "else", code::if_else2 },
				{ "==", code::eq },
				{ "!=", code::ne },
				{ "<", code::lt },
				{ "<=", code::le },
				{ ">", code::gt },
				{ ">=", code::ge },
				{ "+", code::plus },
				{ "-", code::minus },
				{ "*", code::multiply },
				{ "/", code::divide },
				{ "pow", code::pow },
				{ "mod", code::mod },
				{ "sqrt", code::sqrt },
				{ "match", code::match },
				{ "=", code::assign },
				{ "exists", code::exists },
				{ "trim", code::trim },
				{ "rtrim", code::rtrim },
				{ "ltrim", code::ltrim },
				{ "len", code::len },
				{ "resize", code::resize },
				{ "typeof", code::typeof },
				{ "isname", code::isname },
				{ "toname", code::toname },
				{ "topath", code::topath },
				{ "basename", code::basename }
			};
			auto found = types.find( str );
			if( found != types.end() )
				return found->second;
			else
				return code::value;
		}
		string operators::mapCode( code op_code )
		{
			static std::unordered_map<code, string> types{
				{ code::open_brace, "(" },
				{ code::close_brace, ")" },
				{ code::_and, "and" },
				{ code::_or, "or" },
				{ code::_not, "not" },
				{ code::if_else, "if_else" },
				{ code::eq, "==" },
				{ code::ne, "!=" },
				{ code::lt, "<" },
				{ code::le, "<=" },
				{ code::gt, ">" },
				{ code::ge, ">=" },
				{ code::plus, "+" },
				{ code::minus, "-" },
				{ code::unary_minus, "-" },
				{ code::multiply, "*" },
				{ code::divide, "/" },
				{ code::pow, "pow" },
				{ code::mod, "mod" },
				{ code::sqrt, "sqrt" },
				{ code::match, "match" },
				{ code::assign, "=" },
				{ code::exists, "exists" },
				{ code::trim, "trim" },
				{ code::rtrim, "rtrim" },
				{ code::ltrim, "ltrim" },
				{ code::len, "len" },
				{ code::resize, "resize" },
				{ code::typeof, "typeof" },
				{ code::isname, "isname" },
				{ code::toname, "toname" },
				{ code::topath, "topath" },
				{ code::basename, "basename" }
			};
			auto found = types.find( op_code );
			if( found != types.end() )
				return found->second;
			else
				return "";
		}
	}
}


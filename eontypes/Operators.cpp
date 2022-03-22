#include "Operators.h"
#include <unordered_map>
#include <unordered_set>


namespace eon
{
	namespace type
	{
		std::vector<operators::OperatorDetails> operators::Ops;

		operators::code operators::mapCode( const string& str, code context )
		{
			static std::unordered_map<string, std::unordered_set<code>> types{
				{ "(", { code::open_brace } },
				{ ")", { code::close_brace } },
				{ ",", { code::comma } },

				{ "+", { code::plus } },
				{ "-", { code::minus, code::unary_minus } },
				{ "*", { code::multiply } },
				{ "/", { code::divide } },
				{ "<=>", { code::cmp } },
				{ "==", { code::eq } },
				{ "!=", { code::ne } },
				{ "<", { code::lt } },
				{ "<=", { code::le } },
				{ ">", { code::gt } },
				{ ">=", { code::ge } },
				{ "+=", { code::plus_assign } },
				{ "-=", { code::minus_assign } },
				{ "*=", { code::multiply_assign } },
				{ "/=", { code::divide_assign } },
				{ "[", { code::element, code::slice } },
				{ ":", { code::slice2 } },
				{ "]", { code::element2, code::slice3 } },
				{ "in", { code::in } },

				{ "'s", { code::member } },
				{ "if", { code::true_cond, code::if_else } },
				{ "else", { code::if_else2 } },
				{ "for", { code::for_each, code::for_each_where } },
				{ "each", { code::for_each2, code::for_each_where2 } },
				{ "where", { code::for_each_where4 } },
				{ "while", { code::prefix_loop } },
				{ "until", { code::postfix_loop} },

				{ "=", { code::assign } },
				{ "type", { code::type } },
				{ "uid", { code::uid } },
				{ "is", { code::is } },
				{ "size", { code::size_of } },
				{ "swap", { code::swap } },
				{ "find", { code::find } },
				{ "in", { code::for_each3, code::for_each_where3, code::find2 } },

				{ "not", { code::log_not, code::isnot2 } },
				{ "and", { code::swap2, code::log_and } },
				{ "or", { code::log_or } },
				{ "xor", { code::log_xor } },
				{ "~", { code::bit_not } },
				{ "&", { code::bit_and } },
				{ "|", { code::bit_or } },
				{ "^", { code::bit_xor } },
				{ "<<", { code::bit_lsh } },
				{ ">>", { code::bit_rsh } },

				{ "%", { code::pct_of, code::as_pct_of2 } },
				{ "as", { code::as_pct_of } },
				{ "of", { code::size_of2, code::pct_of2, code::as_pct_of3 } },

				{ "return", { code::return_nul, code::return_val } },
				{ "continue", { code::next_iteration } },
				{ "break", { code::end_loop } },
				{ "bumpcall", { code::bumpcall } },
				{ "raise", { code::raise } },
				{ "call", { code::call } }
			};
			auto found = types.find( str );
			if( found != types.end() )
			{
				if( found->second.size() == 1 )
					return *found->second.begin();
				else
				{
					switch( context )
					{
						case code::element:
							if( found->second.find( code::element2 ) != found->second.end() )
								return code::element2;
							break;
						case code::slice:
							if( found->second.find( code::slice2 ) != found->second.end() )
								return code::slice2;
							break;
						case code::slice2:
							if( found->second.find( code::slice3 ) != found->second.end() )
								return code::slice3;
							break;
						case code::if_else:
							if( found->second.find( code::if_else2 ) != found->second.end() )
								return code::if_else2;
							break;
						case code::for_each:
							if( found->second.find( code::for_each2 ) != found->second.end() )
								return code::for_each2;
							break;
						case code::for_each2:
							if( found->second.find( code::for_each3 ) != found->second.end() )
								return code::for_each3;
							break;
						case code::for_each_where:
							if( found->second.find( code::for_each_where2 ) != found->second.end() )
								return code::for_each_where2;
							break;
						case code::for_each_where2:
							if( found->second.find( code::for_each_where3 ) != found->second.end() )
								return code::for_each_where3;
							break;
						case code::for_each_where3:
							if( found->second.find( code::for_each_where4 ) != found->second.end() )
								return code::for_each_where4;
							break;
						case code::isnot:
							if( found->second.find( code::isnot2 ) != found->second.end() )
								return code::isnot2;
							break;
						case code::size_of:
							if( found->second.find( code::size_of2 ) != found->second.end() )
								return code::size_of2;
							break;
						case code::swap:
							if( found->second.find( code::swap2 ) != found->second.end() )
								return code::swap2;
							break;
						case code::find:
							if( found->second.find( code::find2 ) != found->second.end() )
								return code::find2;
							break;
						case code::pct_of:
							if( found->second.find( code::pct_of2 ) != found->second.end() )
								return code::pct_of2;
							break;
						case code::as_pct_of:
							if( found->second.find( code::as_pct_of2 ) != found->second.end() )
								return code::as_pct_of2;
							break;
						case code::as_pct_of2:
							if( found->second.find( code::as_pct_of3 ) != found->second.end() )
								return code::as_pct_of3;
							break;
					}
				}
			}
			return code::undef;
		}
		string operators::mapCode( code op_code )
		{
			static std::unordered_map<code, string> types{
				{ code::open_brace, "(" },
				{ code::close_brace, ")" },
				{ code::comma, "," },

				{ code::plus, "+" },
				{ code::minus, "-" },
				{ code::multiply, "*" },
				{ code::divide, "/" },
				{ code::cmp, "<=>" },
				{ code::eq, "==" },
				{ code::ne, "!=" },
				{ code::lt, "<" },
				{ code::le, "<=" },
				{ code::gt, ">" },
				{ code::ge, ">=" },
				{ code::plus_assign, "+=" },
				{ code::minus_assign, "-=" },
				{ code::multiply_assign, "*=" },
				{ code::divide_assign, "/=" },
				{ code::element, "[" },
				{ code::element2, "]" },
				{ code::in, "in" },
				{ code::slice, "[" },
				{ code::slice2, ":" },
				{ code::slice3, "]" },
				
				{ code::member, "'s" },
				{ code::true_cond, "if" },
				{ code::if_else, "if" },
				{ code::if_else2, "else" },
				{ code::for_each, "for" },
				{ code::for_each2, "each" },
				{ code::for_each_where, "for" },
				{ code::for_each_where2, "each" },
				{ code::for_each_where3, "where" },
				{ code::prefix_loop, "while" },
				{ code::postfix_loop, "until" },

				{ code::assign, "=" },
				{ code::type, "type" },
				{ code::uid, "uid" },
				{ code::is, "is" },
				{ code::size_of, "size" },
				{ code::size_of2, "of" },
				{ code::swap, "swap" },
				{ code::swap2, "in" },
				{ code::find, "find" },
				{ code::find2, "in" },

				{ code::unary_minus, "-" },
				{ code::log_not, "not" },
				{ code::log_and, "and" },
				{ code::log_or , "or" },
				{ code::log_xor, "xor" },
				{ code::bit_not, "~" },
				{ code::bit_and, "&" },
				{ code::bit_or, "|" },
				{ code::bit_xor, "^" },
				{ code::bit_lsh, "<<" },
				{ code::bit_rsh, ">>" },

				{ code::pct_of, "pct" },
				{ code::pct_of2, "of" },
				{ code::as_pct_of, "as" },
				{ code::as_pct_of2, "pct" },
				{ code::as_pct_of3, "of" },

				{ code::return_nul, "return" },
				{ code::return_val, "return" },
				{ code::next_iteration, "continue" },
				{ code::end_loop, "break" },
				{ code::bumpcall, "bumpcall" },
				{ code::raise, "raise" },
				{ code::call, "call" }
			};
			auto found = types.find( op_code );
			if( found != types.end() )
				return found->second;
			else
				return "";
		}




		void operators::_prep()
		{
			if( Ops.empty() )
			{
				// NOTE: These are positioned in Ops based on their operator code!
				//       I.e, code::plus = 3 = Ops[3]!
				Ops = {
					OperatorDetails( { code::undef }, 0, Precedence::none, Precedence::none ),
					OperatorDetails( { code::open_brace }, 0, Precedence::max_hi, Precedence::none ),
					OperatorDetails( { code::close_brace }, 0, Precedence::none, Precedence::max_lo ),
					OperatorDetails( { code::comma }, 2, Precedence::min_lo, Precedence::min_hi ),

					OperatorDetails( { code::plus }, 2, Precedence::high_lo, Precedence::high_hi ),
					OperatorDetails( { code::minus }, 2, Precedence::high_lo, Precedence::high_hi ),
					OperatorDetails( { code::multiply }, 2, Precedence::upper_high_lo, Precedence::upper_high_hi ),
					OperatorDetails( { code::divide }, 2, Precedence::upper_high_lo, Precedence::upper_high_hi ),
					OperatorDetails( { code::cmp }, 2, Precedence::upper_mid_lo, Precedence::upper_mid_hi ),
					OperatorDetails( { code::eq }, 2, Precedence::lower_mid_lo, Precedence::lower_mid_hi ),
					OperatorDetails( { code::ne }, 2, Precedence::lower_mid_lo, Precedence::lower_mid_hi ),
					OperatorDetails( { code::lt }, 2, Precedence::mid_lo, Precedence::mid_hi ),
					OperatorDetails( { code::le }, 2, Precedence::mid_lo, Precedence::mid_hi ),
					OperatorDetails( { code::gt }, 2, Precedence::mid_lo, Precedence::mid_hi ),
					OperatorDetails( { code::ge }, 2, Precedence::mid_lo, Precedence::mid_hi ),
					OperatorDetails( { code::plus_assign }, 2, Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { code::minus_assign }, 2, Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { code::multiply_assign }, 2, Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { code::divide_assign }, 2, Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { Seq( code::element ), Seq( code::element2 ) }, 2,
						Precedence::upper_ultra_lo, Precedence::upper_ultra_hi ),
					OperatorDetails( { code::in }, 2, Precedence::lower_ultra_lo, Precedence::lower_ultra_hi ),
					OperatorDetails( { Seq( code::slice ), Seq( code::slice2 ), Seq( code::slice3 ) }, 3,
						Precedence::lower_ultra_lo, Precedence::lower_ultra_hi ),

					OperatorDetails( { code::member }, 2, Precedence::upper_ultra_lo, Precedence::upper_ultra_hi ),
					OperatorDetails( { code::true_cond }, 2, Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { Seq( code::if_else ), Seq( code::if_else2 ) }, 3,
						Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { Seq( code::for_each ), Seq( code::for_each2, true ), Seq( code::for_each3 ) }, 3,
						Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { Seq( code::for_each_where ), Seq( code::for_each_where2, true ),
						Seq( code::for_each_where3 ), Seq( code::for_each_where4 ) }, 4,
						Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { code::prefix_loop }, 2, Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { code::postfix_loop }, 2, Precedence::min_hi, Precedence::min_lo ),

					OperatorDetails( { code::assign }, 2, Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { code::type }, 1, Precedence::upper_ultra_lo, Precedence::upper_ultra_hi ),
					OperatorDetails( { code::uid }, 1, Precedence::upper_ultra_lo, Precedence::upper_ultra_hi ),
					OperatorDetails( { code::is }, 2, Precedence::ultra_lo, Precedence::ultra_hi ),
					OperatorDetails( { Seq( code::isnot ), Seq( code::isnot2, true ) }, 2,
						Precedence::ultra_lo, Precedence::ultra_hi ),
					OperatorDetails( { Seq( code::size_of ), Seq( code::size_of2, true ) }, 1,
						Precedence::ultra_hi, Precedence::ultra_lo ),
					OperatorDetails( { Seq( code::swap ), Seq( code::swap2 ) }, 2, Precedence::lower_ultra_lo, Precedence::lower_ultra_hi ),
					OperatorDetails( { Seq( code::find ), Seq( code::find2 ) }, 2, Precedence::min_lo, Precedence::min_hi ),

					OperatorDetails( { Seq( code::unary_minus, true ) }, 1, Precedence::ultra_hi, Precedence::ultra_lo ),
					OperatorDetails( { Seq( code::log_not, true ) }, 1, Precedence::ultra_hi, Precedence::ultra_lo ),
					OperatorDetails( { code::log_and }, 2, Precedence::upper_micro_lo, Precedence::upper_micro_hi ),
					OperatorDetails( { code::log_or }, 2, Precedence::lower_micro_lo, Precedence::lower_micro_hi ),
					OperatorDetails( { code::log_xor }, 2, Precedence::micro_lo, Precedence::micro_hi ),
					OperatorDetails( { Seq( code::bit_not, true ) }, 1, Precedence::ultra_hi, Precedence::ultra_lo ),
					OperatorDetails( { code::bit_and }, 2, Precedence::upper_low_lo, Precedence::upper_low_hi ),
					OperatorDetails( { code::bit_or }, 2, Precedence::lower_low_lo, Precedence::lower_low_hi ),
					OperatorDetails( { code::bit_xor }, 2, Precedence::low_lo, Precedence::low_hi ),
					OperatorDetails( { code::bit_lsh }, 2, Precedence::lower_high_lo, Precedence::lower_high_hi ),
					OperatorDetails( { code::bit_rsh }, 2, Precedence::lower_high_lo, Precedence::lower_high_hi ),

					OperatorDetails( { Seq( code::pct_of ), Seq( code::pct_of2, true ) }, 2,
						Precedence::lower_high_lo, Precedence::lower_high_hi ),
					OperatorDetails( { Seq( code::as_pct_of ), Seq( code::as_pct_of2, true ),
						Seq( code::as_pct_of3, true ) }, 2, Precedence::lower_high_lo, Precedence::lower_high_hi ),

					OperatorDetails( { code::return_nul }, 0, Precedence::max_hi, Precedence::max_lo ),
					OperatorDetails( { Seq( code::return_val, true ) }, 1, Precedence::max_hi, Precedence::max_lo ),
					OperatorDetails( { code::next_iteration }, 0, Precedence::min_lo, Precedence::min_hi ),
					OperatorDetails( { code::end_loop }, 0, Precedence::min_lo, Precedence::min_hi ),
					OperatorDetails( { Seq( code::bumpcall, true ) }, 1, Precedence::max_hi, Precedence::max_lo ),
					OperatorDetails( { Seq( code::raise, true ) }, 1, Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { Seq( code::call, true ) }, 1, Precedence::upper_ultra_hi, Precedence::upper_ultra_lo ),

					// These are the second, third, etc. elements of sequence operators
					OperatorDetails( { Seq( code::element ), Seq( code::element2 ) }, 2,
						Precedence::upper_ultra_lo, Precedence::upper_ultra_hi ),
					OperatorDetails( { Seq( code::slice ), Seq( code::slice2 ), Seq( code::slice3 ) }, 3,
						Precedence::lower_ultra_lo, Precedence::lower_ultra_hi ),
					OperatorDetails( { Seq( code::slice ), Seq( code::slice2 ), Seq( code::slice3 ) }, 3,
						Precedence::lower_ultra_lo, Precedence::lower_ultra_hi ),
					OperatorDetails( { Seq( code::if_else ), Seq( code::if_else2 ) }, 3,
						Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { Seq( code::for_each ), Seq( code::for_each2, true ), Seq( code::for_each3 ) }, 3,
						Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { Seq( code::for_each ), Seq( code::for_each2, true ),
						Seq( code::for_each3 ) }, 3, Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { Seq( code::for_each_where ), Seq( code::for_each_where2, true ),
						Seq( code::for_each_where3 ), Seq( code::for_each_where4 ) }, 4,
						Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { Seq( code::for_each_where ), Seq( code::for_each_where2, true ),
						Seq( code::for_each_where3 ), Seq( code::for_each_where4 ) }, 4,
						Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { Seq( code::for_each_where ), Seq( code::for_each_where2, true ),
						Seq( code::for_each_where3 ), Seq( code::for_each_where4 ) }, 4,
						Precedence::min_hi, Precedence::min_lo ),
					OperatorDetails( { Seq( code::isnot ), Seq( code::isnot2, true ) }, 2,
						Precedence::ultra_lo, Precedence::ultra_hi ),
					OperatorDetails( { Seq( code::size_of ), Seq( code::size_of2, true ) }, 1,
						Precedence::ultra_hi, Precedence::ultra_lo ),
					OperatorDetails( { Seq( code::swap ), Seq( code::swap2 ) }, 2,
						Precedence::lower_ultra_lo, Precedence::lower_ultra_hi ),
					OperatorDetails( { Seq( code::find ), Seq( code::find2 ) }, 2, Precedence::min_lo, Precedence::min_hi ),
					OperatorDetails( { Seq( code::pct_of ), Seq( code::pct_of2, true ) }, 2,
						Precedence::lower_high_lo, Precedence::lower_high_hi ),
					OperatorDetails( { Seq( code::as_pct_of ), Seq( code::as_pct_of2, true ),
						Seq( code::as_pct_of3, true ) }, 2, Precedence::lower_high_lo, Precedence::lower_high_hi ),
					OperatorDetails( { Seq( code::as_pct_of ), Seq( code::as_pct_of2, true ),
						Seq( code::as_pct_of3, true ) }, 2, Precedence::lower_high_lo, Precedence::lower_high_hi )
				};
			}
		}
	}
}


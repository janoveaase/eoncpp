#include "Operators.h"


namespace eon
{
	namespace type
	{
		std::unordered_map<name_t, Operators::OperatorDetails> Operators::Details;
		std::unordered_map<name_t, std::vector<name_t>> Operators::ElmToMain;
		const std::vector<name_t> Operators::NoMain;
		std::unordered_set<name_t> Operators::TypeOperators;



		bool Operators::isFirstInSequence( name_t name )
		{
			auto& opts = mapOperator( name );
			for( auto& opt : opts )
			{
				auto& seq = sequence( opt );
				if( seq[ 0 ].Op == name )
					return true;
			}
			return false;
		}



		void Operators::_prep()
		{
			if( Details.empty() )
			{
				Details = {
					{ name_undef, OperatorDetails( { name_undef }, 0, Precedence::none, Precedence::none ) },
					{ symbol_open_round, OperatorDetails( { symbol_open_round }, 0, Precedence::max_hi, Precedence::none ) },
					{ symbol_close_round, OperatorDetails( { symbol_close_round }, 0, Precedence::none, Precedence::max_lo ) },
					{ symbol_comma, OperatorDetails( { symbol_comma }, 2, Precedence::min_lo, Precedence::min_hi ) },

					{ symbol_plus, OperatorDetails( { symbol_plus }, 2, Precedence::mega_lo, Precedence::mega_hi ) },
					{ symbol_minus, OperatorDetails( { symbol_minus }, 2, Precedence::mega_lo, Precedence::mega_hi ) },
					{ symbol_multiply, OperatorDetails( { symbol_multiply }, 2, Precedence::giga_lo, Precedence::giga_hi ) },
					{ symbol_divide, OperatorDetails( { symbol_divide }, 2, Precedence::giga_lo, Precedence::giga_hi ) },
					{ name_mod, OperatorDetails( { name_mod }, 2, Precedence::giga_lo, Precedence::giga_hi ) },
					{ name_pow, OperatorDetails( { name_pow}, 2, Precedence::tera_hi, Precedence::tera_lo ) },
					{ symbol_cmp, OperatorDetails( { symbol_cmp }, 2, Precedence::hecto_lo, Precedence::hecto_hi ) },
					{ symbol_eq, OperatorDetails( { symbol_eq }, 2, Precedence::mid_lo, Precedence::mid_hi ) },
					{ symbol_ne, OperatorDetails( { symbol_ne }, 2, Precedence::mid_lo, Precedence::mid_hi ) },
					{ symbol_lt, OperatorDetails( { symbol_lt }, 2, Precedence::deca_lo, Precedence::deca_hi ) },
					{ symbol_le, OperatorDetails( { symbol_le }, 2, Precedence::deca_lo, Precedence::deca_hi ) },
					{ symbol_gt, OperatorDetails( { symbol_gt }, 2, Precedence::deca_lo, Precedence::deca_hi ) },
					{ symbol_ge, OperatorDetails( { symbol_ge }, 2, Precedence::deca_lo, Precedence::deca_hi ) },
					{ symbol_plus_assign, OperatorDetails( { symbol_plus_assign }, 2, Precedence::min_hi, Precedence::min_lo ) },
					{ symbol_minus_assign, OperatorDetails( { symbol_minus_assign }, 2, Precedence::min_hi, Precedence::min_lo ) },
					{ symbol_multiply_assign, OperatorDetails( { symbol_multiply_assign }, 2, Precedence::min_hi, Precedence::min_lo ) },
					{ symbol_divide_assign, OperatorDetails( { symbol_divide_assign }, 2, Precedence::min_hi, Precedence::min_lo ) },
					{ symbol_element, OperatorDetails( { OperatorElement( symbol_open_square ), OperatorElement( symbol_close_square ) }, 2, Precedence::exa_lo, Precedence::exa_hi ) },
					{ name_in, OperatorDetails( { name_in }, 2, Precedence::tera_lo, Precedence::tera_hi ) },
					{ symbol_slice, OperatorDetails( { OperatorElement( symbol_open_square ), OperatorElement( symbol_colon ), OperatorElement( symbol_close_square) }, 3, Precedence::tera_lo, Precedence::tera_hi ) },

					{ symbol_member, OperatorDetails( { symbol_member }, 2, Precedence::exa_lo, Precedence::exa_hi ) },
					{ name_if, OperatorDetails( { name_if }, 2, Precedence::min_hi, Precedence::min_lo ) },
					{ name_if_else, OperatorDetails( { OperatorElement( name_if ), OperatorElement( name_else ) }, 3, Precedence::min_hi, Precedence::min_lo ) },
					{ name_for_each, OperatorDetails( { OperatorElement( name_for ), OperatorElement( name_each, Position::prefix ), OperatorElement( name_in ) }, 3, Precedence::min_hi, Precedence::min_lo ) },
					{ name_for_each_where, OperatorDetails( { OperatorElement( name_for ), OperatorElement( name_each, Position::prefix ), OperatorElement( name_in ), OperatorElement( name_where ) }, 4, Precedence::min_hi, Precedence::min_lo ) },
					{ name_while, OperatorDetails( { name_while }, 2, Precedence::min_hi, Precedence::min_lo ) },
					{ name_until, OperatorDetails( { name_until }, 2, Precedence::min_hi, Precedence::min_lo ) },

					{ symbol_assign, OperatorDetails( { symbol_assign }, 2, Precedence::min_hi, Precedence::min_lo ) },
					{ name_type, OperatorDetails( { name_type }, 1, Precedence::exa_lo, Precedence::exa_hi ) },
					{ name_uid, OperatorDetails( { name_uid }, 1, Precedence::exa_lo, Precedence::exa_hi ) },
					{ name_is, OperatorDetails( { name_is }, 2, Precedence::peta_lo, Precedence::peta_hi ) },
					{ name_is_not, OperatorDetails( { OperatorElement( name_is ), OperatorElement( name_not, Position::prefix ) }, 2, Precedence::peta_lo, Precedence::peta_hi ) },
					{ name_size_of, OperatorDetails( { OperatorElement( name_size ), OperatorElement( name_of, Position::prefix ) }, 1, Precedence::peta_hi, Precedence::peta_lo ) },
					{ name_swap_with, OperatorDetails( { OperatorElement( name_swap ), OperatorElement( name_with ) }, 2, Precedence::tera_lo, Precedence::tera_hi ) },
					{ name_find_in, OperatorDetails( { OperatorElement( name_find ), OperatorElement( name_in ) }, 2, Precedence::min_lo, Precedence::min_hi ) },

					{ name_reset, OperatorDetails( { OperatorElement( name_reset, Position::prefix ) }, 1, Precedence::max_hi, Precedence::max_lo ) },
					{ name_unary_minus, OperatorDetails( { OperatorElement( name_unary_minus, Position::prefix ) }, 1, Precedence::peta_hi, Precedence::peta_lo ) },
					{ name_not, OperatorDetails( { OperatorElement( name_not, Position::prefix ) }, 1, Precedence::peta_hi, Precedence::peta_lo ) },
					{ name_and, OperatorDetails( { name_and }, 2, Precedence::micro_lo, Precedence::micro_hi ) },
					{ name_or, OperatorDetails( { name_or }, 2, Precedence::pico_lo, Precedence::pico_hi ) },
					{ name_xor, OperatorDetails( { name_xor }, 2, Precedence::nano_lo, Precedence::nano_hi ) },
					{ symbol_bitnot, OperatorDetails( { OperatorElement( symbol_bitnot, Position::prefix ) }, 1, Precedence::peta_hi, Precedence::peta_lo ) },
					{ symbol_bitand, OperatorDetails( { symbol_bitand }, 2, Precedence::deci_lo, Precedence::deci_hi ) },
					{ symbol_bitor, OperatorDetails( { symbol_bitor }, 2, Precedence::milli_lo, Precedence::milli_hi ) },
					{ symbol_bitxor, OperatorDetails( { symbol_bitxor }, 2, Precedence::centi_lo, Precedence::centi_hi ) },
					{ symbol_lshift, OperatorDetails( { symbol_lshift }, 2, Precedence::kilo_lo, Precedence::kilo_hi ) },
					{ symbol_rshift, OperatorDetails( { symbol_rshift }, 2, Precedence::kilo_lo, Precedence::kilo_hi ) },

					{ symbol_pct_of, OperatorDetails( { OperatorElement( symbol_pct ), OperatorElement( name_of, Position::prefix ) }, 2, Precedence::kilo_lo, Precedence::kilo_hi ) },
					{ symbol_as_pct_of, OperatorDetails( { OperatorElement( name_as ), OperatorElement( symbol_pct, Position::prefix ), OperatorElement( name_of, Position::prefix ) }, 2, Precedence::kilo_lo, Precedence::kilo_hi ) },

					{ name_retblank, OperatorDetails( { name_return }, 0, Precedence::max_hi, Precedence::max_lo ) },
					{ name_retvalue, OperatorDetails( { OperatorElement( name_return, Position::prefix ) }, 1, Precedence::max_hi, Precedence::max_lo ) },
					{ name_continue, OperatorDetails( { name_continue }, 0, Precedence::min_lo, Precedence::min_hi ) },
					{ name_break, OperatorDetails( { name_break }, 0, Precedence::min_lo, Precedence::min_hi ) },
					{ name_bumpcall, OperatorDetails( { OperatorElement( name_bumpcall, Position::prefix ) }, 1, Precedence::max_hi, Precedence::max_lo ) },
					{ name_raise, OperatorDetails( { OperatorElement( name_raise, Position::prefix ) }, 1, Precedence::min_hi, Precedence::min_lo ) },
					{ name_call, OperatorDetails( { OperatorElement( name_call, Position::prefix ) }, 1, Precedence::exa_hi, Precedence::exa_lo ) },

					// These are the second, third, etc. elements of sequence operators
					{ symbol_open_square, OperatorDetails( {}, 0, Precedence::min_hi, Precedence::min_lo ) },
					{ symbol_colon, OperatorDetails( {}, 0, Precedence::min_hi, Precedence::min_lo ) },
					{ name_else, OperatorDetails( {}, 0, Precedence::min_hi, Precedence::min_lo ) },
					{ name_each, OperatorDetails( {}, 0, Precedence::min_hi, Precedence::min_lo ) },
					{ name_in, OperatorDetails( {}, 0, Precedence::min_hi, Precedence::min_lo ) },
					{ name_where, OperatorDetails( {}, 0, Precedence::min_hi, Precedence::min_lo ) },
					{ name_of, OperatorDetails( {}, 0, Precedence::peta_hi, Precedence::peta_lo ) },
					{ name_with, OperatorDetails( {}, 0, Precedence::peta_lo, Precedence::peta_hi ) },
					{ name_as, OperatorDetails( {}, 0, Precedence::kilo_lo, Precedence::kilo_hi ) },
					{ symbol_pct, OperatorDetails( {}, 0, Precedence::kilo_lo, Precedence::kilo_hi ) },
				};

				for( auto& main : Details )
				{
					for( auto& elm : main.second.Sequence )
					{
						auto found = ElmToMain.find( elm.Op );
						if( found == ElmToMain.end() )
							ElmToMain[ elm.Op ] = { main.first };
						else
							found->second.push_back( main.first );
					}
				}

				TypeOperators = {
					name_reset, name_unary_minus,
					symbol_plus, symbol_minus, symbol_multiply, symbol_divide, name_mod, name_pow,
					symbol_cmp, symbol_lt, symbol_le, symbol_gt, symbol_ge, symbol_eq, symbol_ne,
					symbol_plus_assign, symbol_minus_assign, symbol_multiply_assign, symbol_divide_assign,
					symbol_element, name_in, symbol_slice
				};
			}
		}
	}
}

#pragma once
#include "Node.h"


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::expr' namespace encloses all hidden expression functionality
	**************************************************************************/
	namespace expr
	{
		/**********************************************************************
		  Eon Operator Node Class - eon::expr::operatornode
		**********************************************************************/
		class operatornode : public node
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			operatornode() = default;
			operatornode( const operatornode& ) = delete;
			inline operatornode( operatornode&& other ) noexcept {
				*this = std::move( other ); }
			inline operatornode( tup::operators::code op_type )
				: node( op_type ) {}

			virtual ~operatornode() = default;




			/******************************************************************
			  Modifier Methods
			******************************************************************/
		public:

			operatornode& operator=( const operatornode& ) = delete;
			inline operatornode& operator=( operatornode&& other ) noexcept {
				Type = other.Type; *static_cast<node*>( this ) = std::move(
					other ); return *this; }




			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline int operands() const noexcept { return static_cast<int>(
				Children.size() ); }

			inline string str() const override {
				return tup::operators::mapCode( Type ); }

			string postorderStr() const override;
			string inorderStr() const override;

			//* Evaluate expression
			tup::valueptr evaluate( tup::variables& vars ) const override;

			nodeptr copy() const override;

			bool equal( const nodeptr& other, const tup::variables& vars )
				const noexcept override;
		};
	}
}

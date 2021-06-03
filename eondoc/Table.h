#pragma once

#include "Block.h"
#include <eonexcept/Exception.h>
#include <unordered_map>


/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::doc' namespace is for specific Æon Document Format elements
	**************************************************************************/
	namespace doc
	{
		//* Aligning of table contents
		enum class h_align
		{
			undef,
			left,
			center,
			right
		};
		enum class v_align
		{
			undef,
			top,
			center,
			bottom
		};

		//* Table cell
		struct Cell
		{
			Cell() = default;
			inline Cell( size_t element_pos, size_t col, size_t row )
				noexcept { Element = element_pos; Col = col; Row = row; }
			size_t Element{ 0 };
			size_t Col{ 0 }, Row{ 0 };
			h_align HAlign{ h_align::undef };
			v_align VAlign{ v_align::undef };
		};

		//* Table column
		struct Column
		{
			h_align HAlign{ h_align::left };
			v_align VAlign{ v_align::top };
		};

		//* Table row
		struct Row
		{
			h_align HAlign{ h_align::left };
			v_align VAlign{ v_align::top };
		};

		//* Exception thrown if one tries to use the default
		//* @[eon::doc::block::add] method to add elements to a table.
		EONEXCEPT( AddDisabledForTable );



		/**********************************************************************
		  Eon Document Element Table Class - eon::doc::table

		  This is a specialization of [eon::doc::block].
		**********************************************************************/
		class table : public block
		{
			/******************************************************************
			  Construction
			******************************************************************/
		public:

			inline table() : block( elmtype::table_t ) {}



			/******************************************************************
			  Read-only Methods
			******************************************************************/
		public:

			inline const std::unordered_map<size_t,
				std::unordered_map<size_t, Cell>> cells() const noexcept {
					return Cells; }

			inline const std::vector<Column>& columns() const noexcept {
				return Cols; }
			inline const std::vector<Row>& rows() const noexcept {
				return Rows; }

			string str( size_t max_line_len, size_t indent ) const override;



			/******************************************************************
			  Write Methods
			******************************************************************/
		public:

			void add( const elementptr& element ) override {
				throw AddDisabledForTable(); };

			void add( const elementptr& element, size_t col, size_t row );




			///////////////////////////////////////////////////////////////////
			//
			// Attributes
			//
		private:
			std::unordered_map<size_t, std::unordered_map<size_t,Cell>> Cells;
			std::vector<Column> Cols;
			std::vector<Row> Rows;
		};
	}
}

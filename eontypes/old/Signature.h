#pragma once

#include <unordered_map>
#include <eonname/Name.h>
#include <eonexcept/Exception.h>



/******************************************************************************
  The 'eon' namespace encloses all public functionality
******************************************************************************/
namespace eon
{
	/**************************************************************************
	  The 'eon::tup' namespace encloses all hidden tuple functionality
	**************************************************************************/
	namespace tup
	{
		/**********************************************************************
		  Eon Signature Class - eon::tup::signature

		  A signature identifies a specific type or action object
		**********************************************************************/
		class signature
		{
		public:
			signature() = default;
			virtual ~signature() = default;

			inline bool isType() const noexcept { return IsType; }
			inline bool isAction() const noexcept { return !IsType; }

			virtual string str() const = 0;

			virtual int compare( const typespec& other ) const noexcept = 0;
			inline bool operator<( const typespec& other ) const noexcept { return compare( other ) < 0; }
			inline bool operator<=( const typespec& other ) const noexcept { return compare( other ) <= 0; }
			inline bool operator>( const typespec& other ) const noexcept { return compare( other ) > 0; }
			inline bool operator>=( const typespec& other ) const noexcept { return compare( other ) >= 0; }
			inline bool operator==( const typespec& other ) const noexcept { return compare( other ) == 0; }
			inline bool operator!=( const typespec& other ) const noexcept { return compare( other ) != 0; }

		protected:
			inline int _compare( const typespec& other ) const noexcept {
				return Main < other.Main ? -1 : other.Main < Main ? 1 : 0; }

			bool IsType{ true };	// false = action
			maintype Main{ maintype::undef };
			Qualifier Qual{ Qualifier::none };
		};




		/**********************************************************************
		  Eon Type Type Specification Class - eon::tup::typetypespec
		**********************************************************************/
		class typetypespec : public typespec
		{
		public:
			inline typetypespec() { Main = maintype::type; }
			inline typetypespec( name_t name ) { Main = maintype::type; Name = name; }
			virtual ~typetypespec() = default;

			inline name_t name() const noexcept { return Name; }
			inline bool is( name_t name ) const noexcept override { return Name == name; }

			virtual string str() const override { return *Name; }

			virtual int compare( const typespec& other ) const noexcept override {
				auto cmp = _compare( other ); return cmp != 0 ? cmp : name::compare(
					Name, dynamic_cast<const typetypespec*>( &other )->Name ); }

		private:
			name_t Name{ no_name };
		};




		/**********************************************************************
		  Eon Instance Type Specification Class - eon::tup::instancespec
		**********************************************************************/
		class typetypespec : public typespec
		{
		public:
			inline typetypespec() { Main = maintype::type; }
			inline typetypespec( name_t name ) { Main = maintype::type; Name = name; }
			virtual ~typetypespec() = default;

			inline name_t name() const noexcept { return Name; }
			inline bool is( name_t name ) const noexcept override { return Name == name; }

			virtual string str() const override { return *Name; }

			virtual int compare( const typespec& other ) const noexcept override {
				auto cmp = _compare( other ); return cmp != 0 ? cmp : name::compare(
					Name, dynamic_cast<const typetypespec*>( &other )->Name ); }

		private:
			name_t Name{ no_name };
		};




		/**********************************************************************
		  Eon Primitive Type Specification Class - eon::tup::primitivespec
		**********************************************************************/
		class primitivespec : public typespec
		{
		public:
			primitivespec() { Main = maintype::primitive; }
			primitivespec( name_t name ) { Main = maintype::primitive; Name = name; }
			virtual ~primitivespec() = default;

			inline name_t name() const noexcept { return Name; }
			inline bool is( name_t name ) const noexcept override { return Name == name; }

			inline string str() const override { return *Name; }

			inline int compare( const typespec& other ) const noexcept override {
				auto cmp = _compare( other ); if( cmp != 0 ) return cmp;
				return name::compare( Name, dynamic_cast<const primitivespec*>( &other )->Name ); }

		private:
			name_t Name{ no_name };
		};




		/**********************************************************************
		  Eon Tuple Type Specification Class - eon::tup::tuplespec
		**********************************************************************/
		class tuplespec : public typespec
		{
		public:
			tuplespec() { Main = maintype::tuple; }
			inline tuplespec( std::vector<const typespec*>&& attributes,
				std::unordered_map<name_t, size_t>&& attribute_names = std::unordered_map<name_t, size_t>() ) {
				Attributes = std::move( attributes ); AttributeNames = std::move( attribute_names ); }
			virtual ~tuplespec() = default;

			inline const std::vector<const typespec*>& attributes() const noexcept { return Attributes; }
			inline const std::unordered_map<name_t, size_t> attributeNames() const noexcept { return AttributeNames; }

			inline size_t numAttributes() const noexcept { return Attributes.size(); }

			EONEXCEPT( NotFound );
			inline const typespec* attribute( size_t pos ) const {
				if( pos < Attributes.size() ) return Attributes[ pos ]; else throw NotFound(); }

			//* Get position of attribute by name
			//* Returns SIZE_MAX if not found
			inline size_t pos( name_t name ) const noexcept {
				auto found = AttributeNames.find( name ); return found != AttributeNames.end() ? found->second : SIZE_MAX; }

			string str() const override;

			int compare( const typespec& other ) const noexcept override;

			//* Check for compatibility
			//* NOTE: This will not check for equality, only attribute names!
			bool compatible( const tuplespec& other ) const noexcept;

		private:
			std::vector<const typespec*> Attributes;
			std::unordered_map<name_t, size_t> AttributeNames;

			friend class eon::tuple;
		};




		/**********************************************************************
		  Eon Template Types Specification Class - eon::tup::templatetypes
		**********************************************************************/
		class templatetypes
		{
		public:
			templatetypes() = default;
			templatetypes( std::initializer_list<name_t> types ) { Types = types; }
			~templatetypes() = default;

			int compare( const templatetypes& other ) const noexcept;
			string str() const;

		private:
			std::vector<name_t> Types;
		};




		/**********************************************************************
		  Eon Standard Type Specification Class - eon::tup::standardspec
		**********************************************************************/
		class standardspec : public typespec
		{
		public:
			inline standardspec() { Main = maintype::standard; }
			inline standardspec( name_t name ) { Main = maintype::standard; Name = name; }
			virtual ~standardspec() = default;

			inline name_t name() const noexcept { return Name; }
			inline bool is( name_t name ) const noexcept override { return Name == name; }

			inline string str() const override { return *Name + Templates.str(); }

			int compare( const typespec& other ) const noexcept override;

		private:
			name_t Name{ no_name };
			templatetypes Templates;
		};




		/**********************************************************************
		  Eon Custom Type Specification Class - eon::tup::customspec
		**********************************************************************/
		class customspec : public tuplespec
		{
		public:
			inline customspec() { Main = maintype::custom; }
			virtual ~customspec() = default;

			inline name_t name() const noexcept { return Name; }
			inline bool is( name_t name ) const noexcept override { return Name == name; }

			inline string str() const override { return *Name; }

			int compare( const typespec& other ) const noexcept override;

		private:
			name_t Name{ no_name };
			templatetypes Templates;
		};




		/**********************************************************************
		  Eon Action Type Specification Class - eon::tup::actionspec
		**********************************************************************/
		class actionspec : public typespec
		{
		public:
			inline actionspec() { Main = maintype::action; }
			virtual ~actionspec() = default;

			inline void returnType( const tuplespec* type ) noexcept { Returns = type; }
			inline void arguments( const tuplespec* arguments ) noexcept { Arguments = arguments; }

			inline name_t name() const noexcept { return Name; }
			inline bool is( name_t name ) const noexcept override { return Name == name; }

			inline const tuplespec* arguments() const noexcept { return Arguments; }
			inline const tuplespec* returns() const noexcept { return Returns; }

			string str() const override;

			int compare( const typespec& other ) const noexcept override;

		private:
			name_t Name{ nullptr };
			const tuplespec* Arguments{ nullptr };
			const tuplespec* Returns{ nullptr };
		};




		/**********************************************************************
		  Eon Reference Type Specification Class - eon::tup::referencespec
		**********************************************************************/
		class referencespec : public typespec
		{
		public:
			referencespec() { Main = maintype::reference; }
			referencespec( const typespec* target ) { Main = maintype::reference; Target = target; }
			virtual ~referencespec() = default;

			inline const typespec* target() const noexcept { return Target; }

			inline string str() const override { return Target ? Target->str() : "N/A"; }

			inline int compare( const typespec& other ) const noexcept override {
				auto cmp = _compare( other ); if( cmp != 0 ) return cmp;
				auto o = dynamic_cast<const referencespec*>( &other );
				if( Target == nullptr ) return o->Target == nullptr ? 0 : -1;
				else return o->Target == nullptr ? 1 : Target->compare( *o->Target ); }

		private:
			const typespec* Target{ nullptr };
		};
	}
}

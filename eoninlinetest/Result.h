#pragma once

#include <eonstring/String.h>
#include "Location.h"



///////////////////////////////////////////////////////////////////////////////
//
// The 'eonitest' namespace encloses all public inline test functionality
//
namespace eonitest
{
	// The result of a test.
	class __Result
	{
	public:

		// Construct a success-result.
		static inline __Result success() { return __Result(); }

		// Construct a comparison failure result.
		static __Result failCompare(
			eon::string operator_name,
			eon::string expected_expression,
			eon::string actual_expression,
			eon::string expected_value,
			eon::string actual_value,
			eon::string details = "" )
		{
			__Result res;
			res.Issue = "normal";
			res.OperatorName = std::move( operator_name );
			res.ExpectedExpr = std::move( expected_expression );
			res.ActualExpr = std::move( actual_expression );
			res.ExpectedVal = std::move( expected_value );
			res.ActualVal = std::move( actual_value );
			res.Details = std::move( details );
			return res;
		}

		// Construct a failed-to-construct-test result.
		static inline __Result failedToConstruct( eon::string additional_details = "" ) {
			__Result res; res.Issue = "construction"; res.Details = std::move( additional_details ); return res; }

		// Construct a failure-by-exception result.
		static __Result failureByException(
			eon::string exception_type, eon::string exception_details, eon::string additional_details = "" )
		{
			__Result res;
			res.Issue = "exception";
			res.ExceptionType = std::move( exception_type );
			res.ExceptionDetails = std::move( exception_details );
			res.Details = std::move( additional_details );
			return res;
		}

		// Construct a failed-to-throw result.
		static inline __Result failedToThrow( eon::string expected_exception ) {
			__Result res; res.Issue = "nothrow"; res.ExceptionDetails = std::move( expected_exception ); return res; }

		// Construct a threw-exception-failure result.
		static inline __Result threwExceptionFailure( eon::string unexpected_exception ) {
			__Result res; res.Issue = "threw"; res.ExceptionDetails = std::move( unexpected_exception ); return res; }

		// Construct a test-operation-missing-failure report.
		static inline __Result testOpMissing() { __Result res; res.Issue = "missing"; return res; }

		inline operator bool() const noexcept { return Issue.empty(); }

		inline bool constructError() const noexcept { return Issue == "construction"; }
		inline bool exceptionError() const noexcept { return Issue == "exception"; }
		inline bool noThrowError() const noexcept { return Issue == "nothrow"; }
		inline bool trewError() const noexcept { return Issue == "threw"; }
		inline bool missingOpError() const noexcept { return Issue == "missing"; }


	public:
		eon::string Issue;
		eon::string ExceptionType; 
		eon::string ExceptionDetails;
		eon::string OperatorName;
		eon::string ExpectedExpr;
		eon::string ActualExpr;
		eon::string ExpectedVal;
		eon::string ActualVal;
		eon::string Details;
	};
}

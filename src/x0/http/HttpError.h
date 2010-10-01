/* <x0/HttpError.h>
 *
 * This file is part of the x0 web server project and is released under LGPL-3.
 * http://www.xzero.ws/
 *
 * (c) 2009-2010 Christian Parpart <trapni@gentoo.org>
 */

#ifndef sw_x0_http_error_hpp
#define sw_x0_http_error_hpp (1)

#include <system_error>

namespace x0 {

enum class HttpError // {{{
{
	Undefined = 0,

	ContinueRequest = 100,
	SwitchingProtocols = 101,
	Processing = 102,

	Ok = 200,
	Created = 201,
	Accepted = 202,
	NonAuthoriativeInformation = 203,
	NoContent = 204,
	ResetContent = 205,
	PartialContent = 206,

	MultipleChoices = 300,
	MovedPermanently = 301,
	MovedTemporarily = 302,
	NotModified = 304,

	BadRequest = 400,
	Unauthorized = 401,
	Forbidden = 403,
	NotFound = 404,
	MethodNotAllowed = 405,
	NotAcceptable = 406,
	ProxyAuthenticationRequired = 407,
	RequestTimeout = 408,
	Conflict = 409,
	Gone = 410,
	LengthRequired = 411,
	PreconditionFailed = 412,
	RequestEntityTooLarge = 413,
	RequestUriTooLong = 414,
	UnsupportedMediaType = 415,
	RequestedRangeNotSatisfiable = 416,
	ExpectationFailed = 417,
	ThereAreTooManyConnectionsFromYourIP = 421,
	UnprocessableEntity = 422,
	Locked = 423,
	FailedDependency = 424,
	UnorderedCollection = 425,
	UpgradeRequired = 426,

	InternalServerError = 500,
	NotImplemented = 501,
	BadGateway = 502,
	ServiceUnavailable = 503,
	GatewayTimedout = 504,
	HttpVersionNotSupported = 505,
	InsufficientStorage = 507
};
// }}}

const std::error_category& http_category() throw();

std::error_code make_error_code(HttpError ec);
std::error_condition make_error_condition(HttpError ec);

bool content_forbidden(HttpError code);

} // namespace x0

namespace std {
	// implicit conversion from HttpError to error_code
	template<> struct is_error_code_enum<x0::HttpError> : public true_type {};
}

// {{{ inlines
namespace x0 {

inline std::error_code make_error_code(HttpError ec)
{
	return std::error_code(static_cast<int>(ec), http_category());
}

inline std::error_condition make_error_condition(HttpError ec)
{
	return std::error_condition(static_cast<int>(ec), http_category());
}

inline bool content_forbidden(HttpError code)
{
	switch (code)
	{
		case /*100*/ HttpError::ContinueRequest:
		case /*101*/ HttpError::SwitchingProtocols:
		case /*204*/ HttpError::NoContent:
		case /*205*/ HttpError::ResetContent:
		case /*304*/ HttpError::NotModified:
			return true;
		default:
			return false;
	}
}

} // namespace x0
// }}}

#endif

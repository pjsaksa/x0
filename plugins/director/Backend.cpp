#include "Backend.h"
#include "Director.h"
#include "HealthMonitor.h"

#if !defined(NDEBUG)
#	define TRACE(msg...) (this->Logging::debug(msg))
#else
#	define TRACE(msg...) do {} while (0)
#endif

using namespace x0;

Backend::Backend(Director* director, const std::string& name, size_t capacity) :
#ifndef NDEBUG
	Logging("Backend/%s", name.c_str()),
#endif
	director_(director),
	name_(name),
	capacity_(capacity),
	load_(),
	role_(Role::Active),
	enabled_(true),
	healthMonitor_(director_->worker_)
{
	healthMonitor_.onStateChange([&](HealthMonitor*) {
		director_->worker_->log(Severity::info, "Director '%s': backend '%s' is now %s.",
			director_->name().c_str(), name_.c_str(), healthMonitor_.state_str().c_str());

		if (healthMonitor_.isOnline()) {
			// try delivering a queued request
			director_->dequeueTo(this);
		}
	});
}

Backend::~Backend()
{
}

size_t Backend::capacity() const
{
	return capacity_;
}

void Backend::setCapacity(size_t value)
{
	capacity_ = value;
}

//virtual
std::string Backend::str() const
{
	return "TODO";
}

size_t Backend::writeJSON(Buffer& output) const
{
	static const std::string roleStr[] = { "Active", "Standby" };
	static const std::string boolStr[] = { "false", "true" };
	size_t offset = output.size();

	output
		<< "\"name\": \"" << name_ << "\", "
		<< "\"capacity\": " << capacity_ << ", "
		<< "\"enabled\": " << boolStr[enabled_] << ", "
		<< "\"role\": \"" << roleStr[static_cast<int>(role_)] << "\",\n     "
		<< "\"load\": " << load_ << ",\n     "
		<< "\"health\": " << healthMonitor_
		;

	return output.size() - offset;
}

void Backend::setState(HealthMonitor::State value)
{
	healthMonitor_.setState(value);
}

/**
 * Invoked internally a request has been fully processed.
 *
 * This decrements the load-statistics, and potentially
 * dequeues possibly enqueued requests to take over.
 */
void Backend::release()
{
	--load_;
	director_->release(this);
}

// {{{ NullProxy
NullProxy::NullProxy(Director* director, const std::string& name, size_t capacity) :
	Backend(director, name, capacity)
{
}

bool NullProxy::process(HttpRequest* r)
{
	r->status = HttpError::ServiceUnavailable;
	r->finish();
	return true;
}
// }}}

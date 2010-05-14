#include <x0/web_client.hpp>
#include <x0/buffer.hpp>
#include <x0/http_error.hpp>
#include <x0/url.hpp>

#include <functional> // bind(), placeholders
#include <iostream>   // clog
#include <cctype>     // atoi()

#include <ev++.h>

int request_count = 2;

void on_response(int vmajor, int vminor, int code, x0::buffer_ref&& text)
{
	std::clog << "S< HTTP/" << vmajor << "." << vminor << " " << code << ' ' << text.str() << std::endl;
}

void on_header(x0::buffer_ref&& name, x0::buffer_ref&& value)
{
	std::clog << "S< " << name.str() << ": " << value.str() << std::endl;
}

bool on_content(x0::buffer_ref&& chunk)
{
#if 0
	std::clog << "S< content of length: " << chunk.size() << std::endl
			  << chunk.str() << std::endl
			  << "S< content end." << std::endl;
#else
	std::clog << chunk.str();
#endif
	return true;
}

bool on_complete(x0::web_client *client)
{
	std::clog << "S< complete." << std::endl;

	static int i = 0;

	++i;

	if (i == request_count)
	{
		std::clog << "S< this was the last response." << std::endl;
		//client->close();
		ev_unloop(ev_default_loop(0), EVUNLOOP_ALL);
		return false;
	}
	return true;
}

int main(int argc, const char *argv[])
{
	struct ev_loop *loop = ev_default_loop(0);

	x0::web_client client(loop);

	using namespace std::placeholders;
	client.on_response = std::bind(&on_response, _1, _2, _3, _4);
	client.on_header = std::bind(&on_header, _1, _2);
	client.on_content = std::bind(&on_content, _1);
	client.on_complete = std::bind(&on_complete, &client);
	client.keepalive_timeout = 5;

	std::string url("http://xzero.ws/cgi-bin/cgi-test.cgi");
	if (argc > 1)
		url = argv[1];

	std::string protocol;
	std::string hostname;
	int port = 80;
	std::string path;

	if (!x0::parse_url(url, protocol, hostname, port, path))
	{
		std::cerr << "URL syntax error" << std::endl;
		return 1;
	}

	client.open(hostname, port);

	if (client.state() == x0::web_client::DISCONNECTED)
	{
		std::clog << "Could not connect to server: " << client.last_error().message() << std::endl;
		return -1;
	}

	for (int i = 0; i < request_count; ++i)
	{
		client.write_request("GET", path);

		client.write_header("Host", hostname);
		client.write_header("User-Agent", "x0");
		client.write_header("X-Foo", "bar");

		client.commit(i == (request_count - 1)); // pass true on last iteration, false otherwise.
	}

	ev_loop(loop, 0);

	if (client.last_error())
	{
		std::clog << "connection error: " << client.last_error() << std::endl;
	}

	return 0;
}

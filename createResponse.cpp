#include "HTTPResponse.hpp"

std::vector<unsigned char>	createResponse(HTTPRequest request, struct server s)
{
	HTTPResponse response(request, s);
	response.defineResponse();
	std::vector<unsigned char> resp(response.resp.begin(), response.resp.end());
	return (resp);
}

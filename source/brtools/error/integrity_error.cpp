#include <brtools/error/integrity_error.h>
#include <brtools/error/errors.h>

using namespace brtools::error;
using namespace std;

bool integrity_error::suppressed = false;

integrity_error::integrity_error(const string& what)
: system_error(codes::integrity_error, what)
{}

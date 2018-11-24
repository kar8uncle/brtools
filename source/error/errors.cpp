#include <error/errors.h>

using namespace std;

namespace 
{
    struct : error_category 
    {
        const char* name() const noexcept override
        {   return "brtools";  }

        string message(const int condition) const override
        {
            switch (static_cast<brtools::error::codes>(condition))
            {
                case brtools::error::codes::integrity_error: return "integrity error";
                default: return "unknown error";
            }
        }
    } const _g_category;
}

namespace brtools
{
namespace error
{
    const error_category& brtools_category()
    {   return _g_category; }
    
    error_code make_error_code(const codes e) noexcept
    {   return {static_cast<int>(e), brtools_category()}; }
    
    error_condition make_error_condition(const codes e) noexcept
    {   return {static_cast<int>(e), brtools_category()}; }

}
}

#pragma once 

#include <version>

#if defined(__cpp_impl_coroutine) && defined(__cpp_lib_coroutine)
#include <coroutine>
#define SUPPORTS_COROUTINES
#endif 


#if !defined(SUPPORT_COROUTINES) && defined(__has_include)

#if __has_include(<coroutine>)

#include <coroutine>
#define SUPPORTS_COROUTINES

#elif __has_include(<experimental/coroutine>) // Check for an experimental version

#include <experimental/coroutine>
namespace std {
    using std::experimental::coroutine_handle;
    using std::experimental::suspend_always;
    using std::experimental::suspend_never;
}
#define SUPPORTS_COROUTINES

#endif

#endif // __has_include
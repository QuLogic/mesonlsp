#include "shared.hpp"

#include <dlfcn.h>

CxaThrowType origCxaThrow = nullptr;

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif
extern "C" EXPORT void __cxa_throw /*NOLINT*/ (void *thrown_exception,
                                               void *pvtinfo,
                                               void (*dest)(void *)) {
  if (origCxaThrow == nullptr) {
    origCxaThrow = (CxaThrowType)dlsym(RTLD_NEXT, "__cxa_throw");
  }
  const auto *typeinfo = (const std::type_info *)pvtinfo;
  logExceptionType(thrown_exception, typeinfo);
  doBacktrace();
  origCxaThrow(thrown_exception, pvtinfo, dest);
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#pragma GCC diagnostic pop
#else
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#endif

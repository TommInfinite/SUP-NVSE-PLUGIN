/*
 * Copyright 2009-2015 Bjorn Fahller <bjorn@fahller.se>
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef CRPCUT_HPP
#define CRPCUT_HPP

#if defined(GMOCK_INCLUDE_GMOCK_GMOCK_H_)  || defined(__CDT_PARSER__)
#undef ADD_FAILURE
#undef ASSERT_ANY_THROW
#undef ASSERT_DEATH
#undef ASSERT_DEBUG_DEATH
#undef ASSERT_DOUBLE_EQ
#undef ASSERT_EQ
#undef ASSERT_EXIT
#undef ASSERT_FALSE
#undef ASSERT_FLOAT_EQ
#undef ASSERT_GE
#undef ASSERT_GT
#undef ASSERT_HRESULT_FAILED
#undef ASSERT_HRESULT_SUCCEEDED
#undef ASSERT_LE
#undef ASSERT_LT
#undef ASSERT_NE
#undef ASSERT_NEAR
#undef ASSERT_NO_FATAL_FAILURE
#undef ASSERT_NO_THROW
#undef ASSERT_PRED
#undef ASSERT_PRED_FORMAT
#undef ASSERT_STRCASEEQ
#undef ASSERT_STRCASENE
#undef ASSERT_STREQ
#undef ASSERT_STRNE
#undef ASSERT_THROW
#undef ASSERT_TRUE
#undef EXPECT_ANY_THROW
#undef EXPECT_DEATH
#undef EXPECT_DEBUG_DEATH
#undef EXPECT_DOUBLE_EQ
#undef EXPECT_EQ
#undef EXPECT_EXIT
#undef EXPECT_FALSE
#undef EXPECT_FATAL_FAILURE
#undef EXPECT_FATAL_FAILURE_ON_ALL_THREADS
#undef EXPECT_FLOAT_EQ
#undef EXPECT_GE
#undef EXPECT_GT
#undef EXPECT_HRESULT_FAILED
#undef EXPECT_HRESULT_SUCCEEDED
#undef EXPECT_LE
#undef EXPECT_LT
#undef EXPECT_NE
#undef EXPECT_NEAR
#undef EXPECT_NONFATAL_FAILURE
#undef EXPECT_NONFATAL_FAILURE_ON_ALL_THREADS
#undef EXPECT_NO_FATAL_FAILURE
#undef EXPECT_NO_THROW
#undef EXPECT_PRED
#undef EXPECT_PRED_FORMAT
#undef EXPECT_STRCASEEQ
#undef EXPECT_STRCASENE
#undef EXPECT_STREQ
#undef EXPECT_STRNE
#undef EXPECT_THROW
#undef EXPECT_TRUE
#undef FAIL
#undef FRIEND_TEST
#undef GTEST_ASSERT_
#undef GTEST_CASE_NAMESPACE_
#undef GTEST_IMPL_CMP_HELPER_
#undef GTEST_INCLUDE_GTEST_GTEST_DEATH_TEST_H_
#undef GTEST_INCLUDE_GTEST_GTEST_H_
#undef GTEST_INCLUDE_GTEST_GTEST_MESSAGE_H_
#undef GTEST_INCLUDE_GTEST_GTEST_PARAM_TEST_H_
#undef GTEST_INCLUDE_GTEST_GTEST_PRED_IMPL_H_
#undef GTEST_INCLUDE_GTEST_GTEST_PROD_H_
#undef GTEST_INCLUDE_GTEST_GTEST_SPI_H_
#undef GTEST_INCLUDE_GTEST_GTEST_TEST_PART_H_
#undef GTEST_INCLUDE_GTEST_GTEST_TYPED_TEST_H_
#undef GTEST_PRED
#undef GTEST_PRED_FORMAT
#undef GTEST_REGISTERED_TEST_NAMES_
#undef GTEST_TYPED_TEST_CASE_P_STATE_
#undef GTEST_TYPE_PARAMS_
#undef INSTANTIATE_TEST_CASE_P
#undef INSTANTIATE_TYPED_TEST_CASE_P
#undef REGISTER_TYPED_TEST_CASE_P
#undef RUN_ALL_TESTS
#undef SCOPED_TRACE
#undef SUCCEED
#undef TEST
#undef TEST_F
#undef TEST_P
#undef TYPED_TEST
#undef TYPED_TEST_CASE
#undef TYPED_TEST_CASE_P
#undef TYPED_TEST_P

#define CRPCUT_DEFINE_REPORTER                                          \
  class crpcut_reporter : public ::testing::EmptyTestEventListener      \
  {                                                                     \
  public:                                                               \
    virtual void OnTestPartResult(const testing::TestPartResult& result) \
    {                                                                   \
      if (result.failed())                                              \
        {                                                               \
          using crpcut::datatypes::fixed_string;                        \
          crpcut::heap::set_limit(crpcut::heap::system);                \
          std::ostringstream loc_stream;                                \
          std::string lstr;                                             \
          fixed_string loc;                                             \
          if (result.file_name() && result.line_number() > 0)           \
            {                                                           \
              loc_stream << result.file_name()                          \
                         << ":"                                         \
                         << result.line_number();                       \
              loc_stream.str().swap(lstr);                              \
              loc = fixed_string::make(lstr.c_str(), lstr.length());    \
            }                                                           \
          else                                                          \
            {                                                           \
              loc = fixed_string::make(CRPCUT_HERE);                    \
            }                                                           \
          std::ostringstream os;                                        \
          os << result.summary() << result.message();                   \
          crpcut::comm::report(crpcut::comm::exit_fail, os, loc);       \
        }                                                               \
    }                                                                   \
  };                                                                    \
  ::testing::TestEventListeners& listeners =                            \
                ::testing::UnitTest::GetInstance()->listeners();        \
  delete listeners.Release(listeners.default_result_printer());         \
  listeners.Append(new crpcut_reporter())

#else

#define CRPCUT_DEFINE_REPORTER do {} while (0)

// In a way this isn't nice, but the resulting compiler error gives
// the user a very obvious hint about what's wrong and what to do instead

#define ERRMSG "You must include <gmock/gmock.h> before <crpcut_hpp>"
#define EXPECT_CALL ERRMSG
#define ON_CALL ERRMSG
#define MOCK_METHOD0 ERRMSG
#define MOCK_METHOD0_T ERRMSG
#define MOCK_METHOD1 ERRMSG
#define MOCK_METHOD1_T ERRMSG
#define MOCK_METHOD2 ERRMSG
#define MOCK_METHOD2_T ERRMSG
#define MOCK_METHOD3 ERRMSG
#define MOCK_METHOD3_T ERRMSG
#define MOCK_METHOD4 ERRMSG
#define MOCK_METHOD4_T ERRMSG
#define MOCK_METHOD5 ERRMSG
#define MOCK_METHOD5_T ERRMSG
#define MOCK_METHOD6 ERRMSG
#define MOCK_METHOD6_T ERRMSG
#define MOCK_METHOD7 ERRMSG
#define MOCK_METHOD7_T ERRMSG
#define MOCK_METHOD8 ERRMSG
#define MOCK_METHOD8_T ERRMSG
#define MOCK_METHOD9 ERRMSG
#define MOCK_METHOD9_T ERRMSG
#define MOCK_METHOD10 ERRMSG
#define MOCK_METHOD10_T ERRMSG

#define MOCK_CONST_METHOD0 ERRMSG
#define MOCK_CONST_METHOD0_T ERRMSG
#define MOCK_CONST_METHOD1 ERRMSG
#define MOCK_CONST_METHOD1_T ERRMSG
#define MOCK_CONST_METHOD2 ERRMSG
#define MOCK_CONST_METHOD2_T ERRMSG
#define MOCK_CONST_METHOD3 ERRMSG
#define MOCK_CONST_METHOD3_T ERRMSG
#define MOCK_CONST_METHOD4 ERRMSG
#define MOCK_CONST_METHOD4_T ERRMSG
#define MOCK_CONST_METHOD5 ERRMSG
#define MOCK_CONST_METHOD5_T ERRMSG
#define MOCK_CONST_METHOD6 ERRMSG
#define MOCK_CONST_METHOD6_T ERRMSG
#define MOCK_CONST_METHOD7 ERRMSG
#define MOCK_CONST_METHOD7_T ERRMSG
#define MOCK_CONST_METHOD8 ERRMSG
#define MOCK_CONST_METHOD8_T ERRMSG
#define MOCK_CONST_METHOD9 ERRMSG
#define MOCK_CONST_METHOD9_T ERRMSG
#define MOCK_CONST_METHOD10 ERRMSG
#define MOCK_CONST_METHOD10_T ERRMSG

#define MOCK_METHOD0_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD0_T_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD1_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD1_T_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD2_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD2_T_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD3_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD3_T_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD4_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD4_T_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD5_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD5_T_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD6_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD6_T_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD7_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD7_T_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD8_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD8_T_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD9_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD9_T_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD10_WITH_CALLTYPE ERRMSG
#define MOCK_METHOD10_T_WITH_CALLTYPE ERRMSG

#endif

#ifdef __clang__
#  if __has_feature(cxx_constexpr)
#    define CRPCUT_SUPPORTS_CONSTEXPR
#  endif
#  if __has_feature(cxx_decltype)
#    define CRPCUT_DECLTYPE decltype
#  else
#    define CRPCUT_DECLTYPE typeof
#  endif
#  if __has_feature(cxx_variadic_templates)
#    define CRPCUT_SUPPORTS_VTEMPLATES
#  endif
#  ifdef __GXX_EXPERIMENTAL_CXX0X__
#    define CRPCUT_EXPERIMENTAL_CXX0X
#  endif
#  define CRPCUT_NORETURN __attribute__((noreturn))
#  if !__has_feature(cxx_exceptions)
#    define CRPCUT_NO_EXCEPTION_SUPPORT
#  endif
#else
#  ifdef __GNUG__
#    ifdef __GXX_EXPERIMENTAL_CXX0X__
#      define CRPCUT_DECLTYPE decltype
#      define CRPCUT_EXPERIMENTAL_CXX0X
#      if (__GNUC__ == 4)
#        if (__GNUC_MINOR__ >= 3)
#          define CRPCUT_SUPPORTS_VTEMPLATES
#        endif
#        if (__GNUC_MINOR__ >= 6)
#          define CRPCUT_SUPPORTS_CONSTEXPR
#        endif
#      endif
#    else
#      define CRPCUT_DECLTYPE typeof
#    endif
#    define CRPCUT_NORETURN __attribute__((noreturn))
#    ifndef __EXCEPTIONS
#      define CRPCUT_NO_EXCEPTION_SUPPORT
#    endif
#  else
#    if defined(__CDT_PARSER__)
#      define CRPCUT_DECLTYPE decltype
#      define CRPCUT_EXPERIMENTAL_CXX0X
#      define CRPCUT_SUPPORTS_VTEMPLATES
#      define __GXX_EXPERIMENTAL_CXX0X__
#      define CRPCUT_SUPPORTS_CONSTEXPR
#    endif
#    define CRPCUT_NORETURN
#  endif
#endif
#include <stdexcept>
#include <sstream>
#include <string>
#include <iostream>
#include <ostream>
#include <iomanip>
#include <cerrno>
#include <cassert>
#ifdef CRPCUT_EXPERIMENTAL_CXX0X
#  include <type_traits>
#  include <array>
#else
#  ifdef BOOST_TR1
#    include <boost/tr1/type_traits.hpp>
#    include <boost/tr1/array.hpp>
#  else
#    include <tr1/type_traits>
#    include <tr1/array>
#  endif
#endif
#include <cstring>
#include <cstdlib>
#include <limits>
#include <memory>
extern "C"
{
#  include <dlfcn.h>
#  include <sys/wait.h>
#  include <regex.h>
#  include <stdint.h>
}

namespace std {
#if (!defined(CRPCUT_EXPERIMENTAL_CXX0X) || defined (BOOST_TR1))
  using std::tr1::array;
  using std::tr1::remove_cv;
  using std::tr1::remove_reference;
#endif
}



#ifdef CRPCUT_NO_EXCEPTION_SUPPORT
#ifndef try
#define try if (true)
#endif
#define CATCH_BLOCK(specifier, code)
#else
#define CATCH_BLOCK(specifier, code) catch (specifier) code
#endif

#define ANY_CODE -1
#define CRPCUT_VERBATIM(x) x
#define CRPCUT_STRINGIZE(...) #__VA_ARGS__
#define CRPCUT_STRINGIZE_(...) CRPCUT_STRINGIZE(__VA_ARGS__)
#define CRPCUT_HERE __FILE__ ":" CRPCUT_STRINGIZE_(__LINE__)

namespace crpcut {

#ifdef CRPCUT_SUPPORTS_CONSTEXPR
template <size_t N>
constexpr char array_index(size_t n, const char (&array)[N])
{
  return n < N ? array[n] : '\0';
}
#endif

  template <bool b, typename T>
  struct enable_if; // I know, it exists in <type_traits>, but not in g++-4.2.4

  template <typename T>
  struct enable_if<true, T>
  {
    typedef T type;
  };

  namespace datatypes {
    template <typename T>
    struct undecorated
    {
      typedef typename std::remove_reference<T>::type noref_type;
      typedef typename std::remove_cv<noref_type>::type type;
    };
  }
  template <typename T>
  struct eval_t
  {
    typedef const typename datatypes::undecorated<T>::type &type;
    static type func(type t) { return t; }
  };

  template <typename T>
  typename eval_t<T>::type eval(const T& t);

  namespace expr {
    template <typename T>
    const T& gen();
  }

  class test_runner;

  typedef enum { verbatim, uppercase, lowercase } case_convert_type;
  template <case_convert_type>
  class collate_t;

  namespace wrapped { // stdc and posix functions
    CRPCUT_NORETURN void abort();
    void                 free(const void* p);
    void                *malloc(size_t n);
    ssize_t              read(int fd, void* p, size_t s);
    int                  regcomp(regex_t*, const char*, int);
    size_t               regerror(int, const regex_t*, char*, size_t);
    int                  regexec(const regex_t*, const char*,
                                 size_t, regmatch_t[], int);
    void                 regfree(regex_t*);
    int                  strcmp(const char *l, const char *r);
    char *               strerror(int n);
    size_t               strlen(const char *r);
    ssize_t              write(int fd, const void* p, size_t s);
    int                  close(int fd);
  }

  namespace lib {
    // works like std::strcpy, except the return value is the pointer to
    // the nul terminator in the destination, making concatenations easy
    // and cheap
    template <typename T, typename U>
    T strcpy(T d, U s)
    {
      while ((*d = *s))
        {
          ++d;
          ++s;
        }
      return d;
    }
  }


  namespace libs
  {
    const char * const * libc();
    const char * const * librt();
    const char * const * rtld_next();
  }

  namespace libwrapper {

    class dlloader // thin wrapper around dlopen(), dlclose(), dlsym()
    {
    protected:
      static void *load(const char *const *lib);
      static void *symbol(void *, const char *name);
      static void unload(void*);
      static void assert_lib_is_loaded(void*);
      dlloader() {}
    private:
    };

    template <const char * const * (&lib)()>
    class loader : dlloader
    {
      static void *& libptr()
      {
        static void *p = 0;
        return p;
      }
      loader()  {
        libptr() = dlloader::load(lib());
        assert_lib_is_loaded(libptr());
      }
      // Defaulted trivial destructor leaks memory from dlload on exit. So what?
    public:
      static bool is_loaded() { return libptr(); }
      template <typename T>
      T sym(const char *name)
      {
        assert_lib_is_loaded(libptr());
        union {       // I don't like silencing the warning this way,
          T func;     // but it should be safe. *IF* the function pointer
          void *addr; // can't be represented by void*, dlsym() can't
        } dlr;        // exist either.
        dlr.addr = symbol(libptr(), name);
        return dlr.func;
      }
      static bool has_symbol(const char *name)
      {
        return is_loaded() && symbol(libptr(), name);
      }
      static loader& obj()
      {
        static loader o;
        return o;
      }
    };
    template <>
    class loader<libs::rtld_next> : dlloader
    {
    public:
      static bool is_loaded() { return true; }
      template <typename T>
      T sym(const char *name)
      {
        union {
          T func;
          void *addr;
        } dlr;
        dlr.addr = symbol(RTLD_NEXT, name);
        return dlr.func;
      }
      static bool has_symbol(const char *name)
      {
        return symbol(RTLD_NEXT, name);
      }
      static loader& obj()
      {
        static loader o;
        return o;
      }
    };

  }


  namespace stream_checker
  {
    template <typename V, typename U>
    char operator<<(V&, const U&);

    template <typename T>
    struct is_output_streamable
    {
    private:
      static std::ostream &os;
      static T& t;

      static char check(char);
      static std::pair<char, char> check(std::ostream&);
    public:
      static const bool value = sizeof(check(os << t)) != sizeof(char);
    };


    template <typename T>
    struct is_output_streamable<const T>
    {
      static const bool value = is_output_streamable<T>::value;
    };

    template <typename T>
    struct is_output_streamable<volatile T>
    {
      static const bool value = is_output_streamable<T>::value;
    };

    template <typename T>
    struct is_output_streamable<T&>
    {
      static const bool value = is_output_streamable<T>::value;
    };

    template <size_t N>
    struct is_output_streamable<char[N]>
    {
      static const bool value = true;
    };

    template <size_t N>
    struct is_output_streamable<const char[N]>
    {
      static const bool value = true;
    };

    template <typename T>
    struct is_output_streamable<T*>
    {
      static const bool value = true;
    };

    template <typename T>
    struct is_output_streamable<T (*)()>
    {
      static const bool value = false;
    };

    template <typename T, typename P1>
    struct is_output_streamable<T (*)(P1)>
    {
      static const bool value = false;
    };

    template <typename T, typename P1, typename P2>
    struct is_output_streamable<T (*)(P1, P2)>
    {
      static const bool value = false;
    };

    template <typename T, typename P1, typename P2, typename P3>
    struct is_output_streamable<T (*)(P1, P2, P3)>
    {
      static const bool value = false;
    };

    template <typename T, typename P1, typename P2, typename P3, typename P4>
    struct is_output_streamable<T (*)(P1, P2, P3, P4)>
    {
      static const bool value = false;
    };

    template <typename T, typename P1, typename P2, typename P3, typename P4,
              typename P5>
    struct is_output_streamable<T (*)(P1, P2, P3, P4, P5)>
    {
      static const bool value = false;
    };

    template <typename T, typename P1, typename P2, typename P3, typename P4,
              typename P5, typename P6>
    struct is_output_streamable<T (*)(P1, P2, P3, P4, P5, P6)>
    {
      static const bool value = false;
    };

    template <typename T, typename P1, typename P2, typename P3, typename P4,
              typename P5, typename P6, typename P7>
    struct is_output_streamable<T (*)(P1, P2, P3, P4, P5, P6, P7)>
    {
      static const bool value = false;
    };

    template <typename T, typename P1, typename P2, typename P3, typename P4,
              typename P5, typename P6, typename P7, typename P8>
    struct is_output_streamable<T (*)(P1, P2, P3, P4, P5, P6, P7, P8)>
    {
      static const bool value = false;
    };

    template <typename T, typename P1, typename P2, typename P3, typename P4,
              typename P5, typename P6, typename P7, typename P8, typename P9>
    struct is_output_streamable<T (*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
    {
      static const bool value = false;
    };

    template <typename T, typename C>
    struct is_output_streamable<T (C::*)()>
    {
      static const bool value = false;
    };

    template <typename T, typename C, typename P1>
    struct is_output_streamable<T (C::*)(P1)>
    {
      static const bool value = false;
    };

    template <typename T, typename C,
              typename P1, typename P2>
    struct is_output_streamable<T (C::*)(P1, P2)>
    {
      static const bool value = false;
    };

    template <typename T, typename C,
              typename P1, typename P2, typename P3>
    struct is_output_streamable<T (C::*)(P1, P2, P3)>
    {
      static const bool value = false;
    };

    template <typename T, typename C,
              typename P1, typename P2, typename P3, typename P4>
    struct is_output_streamable<T (C::*)(P1, P2, P3, P4)>
    {
      static const bool value = false;
    };

    template <typename T, typename C,
              typename P1, typename P2, typename P3, typename P4,
              typename P5>
    struct is_output_streamable<T (C::*)(P1, P2, P3, P4, P5)>
    {
      static const bool value = false;
    };

    template <typename T, typename C,
              typename P1, typename P2, typename P3, typename P4,
              typename P5, typename P6>
    struct is_output_streamable<T (C::*)(P1, P2, P3, P4, P5, P6)>
    {
      static const bool value = false;
    };

    template <typename T, typename C,
              typename P1, typename P2, typename P3, typename P4,
              typename P5, typename P6, typename P7>
    struct is_output_streamable<T (C::*)(P1, P2, P3, P4, P5, P6, P7)>
    {
      static const bool value = false;
    };

    template <typename T, typename C,
              typename P1, typename P2, typename P3, typename P4,
              typename P5, typename P6, typename P7, typename P8>
    struct is_output_streamable<T (C::*)(P1, P2, P3, P4, P5, P6, P7, P8)>
    {
      static const bool value = false;
    };

    template <typename T, typename C,
              typename P1, typename P2, typename P3, typename P4,
              typename P5, typename P6, typename P7, typename P8, typename P9>
    struct is_output_streamable<T (C::*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
    {
      static const bool value = false;
    };
  } // namespace stream_checker


  class crpcut_none {};


  namespace datatypes {

    class crpcut_none {};

    template <typename T1 = crpcut_none, typename T2 = crpcut_none>
    class tlist : public T1,
                  public T2
    {
    public:
      typedef T1 head;
      typedef T2 tail;
    };

    template <typename T>
    class tlist<crpcut_none, T>
    {
      typedef crpcut_none head;
      typedef crpcut_none tail;
    };

#ifdef CRPCUT_SUPPORTS_VTEMPLATES

#  ifdef CRPCUT_SUPPORTS_CONSTEXPR
    template <char ...t>
    struct string_type;

    template <char c, typename T>
    struct append_t;

    template <char ...t>
    struct string_type
    {
      template <char c>
      struct append
      {
        typedef typename append_t<c, string_type>::type type;
      };
      static const size_t size = sizeof...(t);
      static const char c_str[size + 1];
    };

    template <char ...t>
    const char string_type<t...>::c_str[string_type<t...>::size + 1] = { t..., 0 };

    template <char c, char ...t>
    struct append_t<c, string_type<t...> >
    {
      typedef string_type<t..., c> type;
    };

    template <char ...t>
    struct append_t<'\0', string_type<t...> >
    {
      typedef string_type<t...> type;
    };
#  endif

    template <typename... Ts>
    struct tlist_maker;


    template <typename T>
    struct tlist_maker<T>
    {
      typedef tlist<T, tlist<> >    type;
    };

    template <typename T, typename ... Tail>
    struct tlist_maker<T, Tail...>
    {
      typedef tlist<T, typename tlist_maker<Tail...>::type> type;
    };
#else

    template <typename T1 = crpcut_none, typename T2 = crpcut_none,
              typename T3 = crpcut_none, typename T4 = crpcut_none,
              typename T5 = crpcut_none, typename T6 = crpcut_none,
              typename T7 = crpcut_none, typename T8 = crpcut_none,
              typename T9 = crpcut_none, typename T10 = crpcut_none,
              typename T11 = crpcut_none, typename T12 = crpcut_none,
              typename T13 = crpcut_none, typename T14 = crpcut_none,
              typename T15 = crpcut_none, typename T16 = crpcut_none,
              typename T17 = crpcut_none, typename T18 = crpcut_none>
    struct tlist_maker
    {
      typedef tlist<
        T1,
        tlist<
          T2,
          tlist<
            T3,
            tlist<
              T4,
              tlist<
                T5,
                tlist<
                  T6,
                  tlist<
                    T7,
                    tlist<
                      T8,
                      tlist<
                        T9,
                        tlist<
                          T10,
                          tlist<
                            T11,
                            tlist<
                              T12,
                              tlist<
                                T13,
                                tlist<
                                  T14,
                                  tlist<
                                    T15,
                                    tlist<
                                      T16,
                                      tlist<
                                        T17,
                                        tlist<T18>
                                        >
                                      >
                                    >
                                  >
                                >
                              >
                            >
                          >
                        >
                      >
                    >
                  >
                >
              >
            >
          >
        >
      type;
    };
#endif

    struct fixed_string
    {
      const char  *str;
      std::size_t  len;
    private:
      struct secret_bool;
    public:
      static fixed_string make(const char * p, size_t len)
      {
        fixed_string s = { p, len };
        return s;
      }
      template <size_t N>
      static fixed_string make(const char (&f)[N])
      {
        fixed_string s = { f, N - 1 };
        return s;
      }
      operator const secret_bool* () const
      {
        return len ? reinterpret_cast<const secret_bool*>(this) : 0;
      }
      bool operator==(const fixed_string &s) const
      {
        if (len != s.len) return false;
        if (str == s.str) return true;
        for (std::size_t i = 0; i < len; ++i)
          {
            if (str[i] != s.str[i]) return false;
          }
        return true;
      }
      bool operator!=(const fixed_string &s) const
      {
        return !(*this == s);
      }
      friend std::ostream &operator<<(std::ostream &os, const fixed_string &s)
      {
        return os << std::string(s.str, s.len);
      }
    };

    template <template <typename> class envelope, typename T>
    class wrap
    {
    public:
      typedef datatypes::tlist<envelope<typename T::head>,
                               typename wrap<envelope,
                                             typename T::tail>::type> type;
    };

    template <template <typename> class envelope, typename T>
    class wrap<envelope, datatypes::tlist<crpcut_none, T> >
    {
    public:
      typedef datatypes::tlist<> type;
    };



    struct string_traits
    {
      static const char *get_c_str(const std::string &s) { return s.c_str(); }
      static const char *get_c_str(const char *s) { return s; }
      static const char *get_c_str(const std::exception& e) { return e.what(); }
    };

    template <typename T>
    class list_elem
    {
    public:
      list_elem();
      virtual ~list_elem();
      void link_after(list_elem& r);
      void link_before(list_elem &r);
      T *first() { return is_empty() ? 0 : static_cast<T*>(next_); }
      T *last() { return is_empty() ? 0 : static_cast<T*>(prev_); }
      const T *first() const { return is_empty() ? 0 : static_cast<const T*>(next_); }
      const T *last() const { return is_empty() ? 0 : static_cast<const T*>(prev_); }
      T* next_after(T* p) const { return p == last() ? 0 : static_cast<T*>(p->next_); }
      T* prev_before(T* p) const { return p == first() ? 0 : static_cast<T*>(p->prev_); }
      const T* next_after(const T* p) const { return p == last() ? 0 : static_cast<T*>(p->next_); }
      const T* prev_before(const T* p) const { return p == first() ? 0 : static_cast<T*>(p->prev_); }
      bool is_empty() const;
      //      bool is_this(const T *p) const;
    protected:
      void unlink();
    private:
      list_elem(const list_elem&);
      list_elem& operator=(const list_elem&);
      list_elem *next_;
      list_elem *prev_;
    };

    template <typename T>
    inline list_elem<T>::list_elem()
      : next_(this),
        prev_(this)
    {
    }

    template <typename T>
    inline list_elem<T>::~list_elem()
    {
      unlink();
    }

    template <typename T>
    inline void list_elem<T>::link_after(list_elem& r)
    {
      list_elem *p = prev_;
      list_elem *r_n = r.next_;
      p->next_ = r_n;
      r_n->prev_ = p;
      r.next_ = this;
      prev_ = &r;
    }

    template <typename T>
    inline void list_elem<T>::link_before(list_elem &r)
    {
      list_elem *n = next_;
      list_elem *r_p = r.prev_;
      n->prev_ = r_p;
      r_p->next_ = n;
      r.prev_ = this;
      next_ = &r;
    }

    template <typename T>
    inline bool list_elem<T>::is_empty() const
    {
      return next_ == this;
    }
#if 0
    template <typename T>
    inline bool list_elem<T>::is_this(const T* p) const
    {
      return static_cast<const T*>(this) == p;
    }
#endif
    template <typename T>
    inline void list_elem<T>::unlink()
    {
      list_elem *n = next_;
      list_elem *p = prev_;
      n->prev_ = p;
      p->next_ = n;
      prev_ = this;
      next_ = this;
    }

  } // namespace datatypes

  class predicate {};

  template <typename T>
  struct is_predicate
  {
    typedef char no;
    struct yes { char v[2]; };
    static no func(...);
    static yes func(predicate*);
    static const bool value = sizeof(func((T*)0)) == sizeof(yes);
  };


  class regex : public predicate
  {
    class type : public predicate
    {
    public:
      template <typename T>
      type(T t, int flags);
      template <typename T>
      bool operator()(const T &t);
      friend std::ostream& operator<<(std::ostream &os, const type &obj);
      ~type();
    private:
      void init(const char*, int flags);
      bool match(const char*);
      regex_t r_;
      char *errmsg_;
      const char *re_str_;
    };
  public:
    friend std::ostream& operator<<(std::ostream &os, const type &obj);
    typedef enum {
      e = REG_EXTENDED,
      i = REG_ICASE,
      m = REG_NEWLINE
    } regflag;
    template <typename T>
    regex(T t,
          regflag f1 = regflag(),
          regflag f2 = regflag(),
          regflag f3 = regflag())
      : p_(new type(t, f1 | f2 | f3))
    {
    }
#if __cplusplus < 201103L
    regex(const regex& r);
#endif
    template <typename T>
    bool operator()(const T &t)
    {
      return (*p_)(t);
    }
    friend std::ostream& operator<<(std::ostream &os, const regex &r);
  private:
#if __cplusplus >= 201103L
    std::unique_ptr<type> p_;
#else
    mutable std::auto_ptr<type> p_; // Yeach! Ugly
#endif
  };


#ifdef CRPCUT_SUPPORTS_VTEMPLATES
  template <typename D, typename ...T>
  struct match_traits
  {
    typedef D type;
  };
#else
  template <typename D,
            typename T1,                typename T2 = crpcut_none,
            typename T3 = crpcut_none,  typename T4 = crpcut_none,
            typename T5 = crpcut_none,  typename T6 = crpcut_none,
            typename T7 = crpcut_none,  typename T8 = crpcut_none,
            typename T9 = crpcut_none>
  struct match_traits
  {
    typedef D type;
  };
#endif

#define CRPCUT_TEST_PHASES(translator)     \
              translator(creating),        \
              translator(running),         \
              translator(destroying),      \
              translator(post_mortem),     \
              translator(child)

  typedef enum { CRPCUT_TEST_PHASES(CRPCUT_VERBATIM) } test_phase;

#define CRPCUT_TEST_IMPORTANCE(translator) \
  translator(ignored, ' '),                \
  translator(disabled, '-'),               \
  translator(critical, '!'),               \
  translator(non_critical, '?')

#define CRPCUT_VERBATIM_FIRST(p1, p2) p1

  class tag_list_root;
  class tag : public datatypes::list_elem<tag>
  {
    friend class tag_list_root;
    tag();
  protected:
    tag(size_t len, tag_list_root *list);
    virtual ~tag(); // make eclipse happy
  public:
    typedef enum { CRPCUT_TEST_IMPORTANCE(CRPCUT_VERBATIM_FIRST) } importance;
    virtual void fail();
    virtual void pass();
    virtual size_t num_failed() const;
    virtual size_t num_passed() const;
    virtual datatypes::fixed_string get_name() const = 0;
    virtual void set_importance(importance i);
    virtual importance get_importance() const;
  private:
    size_t failed_;
    size_t passed_;
    importance importance_;
  };

  std::ostream &operator<<(std::ostream &os, crpcut::tag::importance i);
  class tag_list_root : public tag
  {
    typedef datatypes::list_elem<tag> tag_list_type;
  public:
    tag_list_root() : longest_tag_name_(0), list_() { link_after(list_);}
    virtual size_t longest_tag_name() const { return longest_tag_name_; }
    void store_name_length(size_t n) { longest_tag_name_ = n; }
    template <typename T>
    class iterator_t
    {
    public:
      iterator_t(T *p, const tag_list_type& list) : p_(p), list_(list) {};
      iterator_t& operator++() { p_ = list_.next_after(p_); return *this; }
      iterator_t operator++(int) { iterator_t rv(*this); ++(*this); return rv;}
      T *operator->() { return p_; }
      T& operator*() { return *p_; }
      bool operator==(const iterator_t &i) const { return p_ == i.p_; }
      bool operator!=(const iterator_t &i) const { return !(operator==(i)); }
    private:
      T                   *p_;
      const tag_list_type &list_;
    };
    typedef iterator_t<tag> iterator;
    typedef iterator_t<const tag> const_iterator;
    const_iterator begin() const { return const_iterator(list_.first(), list_); }
    const_iterator end() const { return const_iterator(0, list_); }
    iterator begin() { return iterator(list_.first(), list_); }
    iterator end() { return iterator(0, list_); }
    void print_to(std::ostream &) const;
    void configure_importance(const char *specification);
  private:
    size_t        longest_tag_name_;
    tag_list_type list_;
  };

  template <typename T>
  class crpcut_tag_info;

  template <>
  class crpcut_tag_info<crpcut_none> : public tag_list_root
  {
    crpcut_tag_info();
    virtual datatypes::fixed_string get_name() const;
  public:
    static tag_list_root& obj();
  };

  typedef crpcut_tag_info<crpcut_none> tag_list;
  template <typename T>
  class crpcut_tag_info : public tag
  {
  public:
    static crpcut_tag_info& obj()
    {
      static crpcut_tag_info t;
      return t;
    }
  private:
    crpcut_tag_info()
      : tag(get_name_len(),
            &crpcut_tag_info<crpcut_none>::obj())
    {
    }
    size_t get_name_len() const;
    virtual datatypes::fixed_string get_name() const;
  };

#if defined(CRPCUT_SUPPORTS_VTEMPLATES) && defined(CRPCUT_SUPPORTS_CONSTEXPR)
  template <char ...t>
  class crpcut_tag_info<datatypes::string_type<t...> > : public tag
  {
    typedef datatypes::string_type<t...> rep;
  public:
    static crpcut_tag_info &obj()
    {
      static crpcut_tag_info instance;
      return instance;
    }
  private:
    crpcut_tag_info()
      : tag(get_name_len(),
            &crpcut_tag_info<crpcut_none>::obj())
    {
    }
    size_t get_name_len() const { return rep::size; }
    virtual datatypes::fixed_string get_name() const
    {
      return datatypes::fixed_string::make(rep::c_str);
    }
  };

#define CRPCUT_APPEND_INDEX(i, str) ::append<crpcut::array_index(i, str)>::type

#endif

  namespace stream {
    template <typename charT, typename traits = std::char_traits<charT> >
    class oabuf : public std::basic_streambuf<charT, traits>
    {
      typedef std::basic_streambuf<charT, traits> parent;
    public:
      oabuf(charT *begin_, charT *end_);
      const charT *begin() const { return parent::pbase(); }
      const charT *end() const { return parent::pptr(); }
    };

    template <typename charT, typename traits = std::char_traits<charT> >
    class basic_oastream : private oabuf<charT, traits>,
                           public  std::basic_ostream<charT, traits>
    {
    public:
      basic_oastream(charT *begin_, charT *end_);
      basic_oastream(charT *begin_, size_t size_);
      template <size_t N>
      basic_oastream(charT (&buff)[N]);
      using oabuf<charT, traits>::begin;
      using oabuf<charT, traits>::end;
      std::size_t size() const { return size_t(end() - begin()); }
      operator datatypes::fixed_string() const;
    };

    template <typename charT, class traits = std::char_traits<charT> >
    class iabuf : public std::basic_streambuf<charT, traits>
    {
    public:
      iabuf(const charT *begin, const charT *end);
      iabuf(const iabuf& b);
    };


    template <typename charT, typename traits = std::char_traits<charT> >
    class basic_iastream : private iabuf<charT, traits>,
                           public std::basic_istream<charT, traits>
    {
    public:
      basic_iastream(const charT *begin, const charT *end);
      basic_iastream(const charT *begin);
      basic_iastream(const basic_iastream& i);
    };

    template <size_t N,
              typename charT = char,
              typename traits = std::char_traits<charT> >
    class toastream : public basic_oastream<charT, traits>
    {
    public:
      toastream() : basic_oastream<charT, traits>(buffer, N) {}
    private:
      charT buffer[N];
    };

    typedef basic_oastream<char> oastream;
    typedef basic_iastream<char> iastream;

  } // stream

  template <typename T>
  class poll;

  namespace comm {

#define CRPCUT_COMM_MSGS(translator)             \
    translator(stdout),           /*  0 */       \
      translator(stderr),         /*  1 */       \
      translator(info),           /*  2 */       \
      translator(exit_ok),        /*  3 */       \
      translator(exit_fail),      /*  4 */       \
      translator(fail),           /*  5 */       \
      translator(dir),            /*  6 */       \
      translator(set_timeout),    /*  7 */       \
      translator(cancel_timeout), /*  8 */       \
      translator(begin_test),     /*  9 */       \
      translator(end_test)        /* 10 */

    typedef enum {
      CRPCUT_COMM_MSGS(CRPCUT_VERBATIM),
      kill_me = 0x100
    } type;
  }

  class crpcut_test_monitor
  {
  public:
    virtual ~crpcut_test_monitor() {}
    virtual void set_timeout(unsigned long ts_us) = 0;
    virtual unsigned long duration_us() const = 0;
    virtual bool deadline_is_set() const = 0;
    virtual void clear_deadline() = 0;
    virtual void crpcut_register_success(bool value = true) = 0;
    virtual void set_phase(test_phase) = 0;
    virtual void kill() = 0;
    virtual bool crpcut_failed() const = 0;
    virtual void set_cputime_at_start(const struct timeval&) = 0;
    virtual void send_to_presentation(comm::type, size_t len, const char*) const = 0;
    virtual void set_death_note() = 0;
    virtual void activate_reader() = 0;
    virtual void deactivate_reader() = 0;
    virtual bool has_active_readers() const = 0;
    virtual void manage_death() = 0;
    virtual bool is_naughty_child() const = 0;
    virtual void freeze() const = 0;
    virtual datatypes::fixed_string get_location() const = 0;
    static crpcut_test_monitor *current_test();
  private:
    friend class test_runner;
    static void make_current(crpcut_test_monitor *p);
  };


  namespace comm {

    class file_descriptor
    {
      template <typename T>
      friend class crpcut::poll;
      virtual void close();
    public:
      void swap(file_descriptor &f)
      {
        std::swap(fd_, f.fd_);
      }
    protected:
      int get_fd() const { return fd_; }
      file_descriptor();
      file_descriptor(int fd);
      virtual ~file_descriptor();
      int fd_;
    };

    class data_reader
    {
    public:
      virtual ~data_reader();
      virtual ssize_t read(void *buff, size_t len) const = 0;
      virtual void read_loop(void *buff, size_t len,
                             const char *context = "read_loop") const;
    };

    class rfile_descriptor : public file_descriptor,
                             public data_reader
    {
    public:
      rfile_descriptor();
      rfile_descriptor(int fd);
      virtual ssize_t read(void *buff, size_t len) const;
    };

    class data_writer
    {
    public:
      virtual ~data_writer();
      virtual ssize_t write(const void *buff, size_t len) const = 0;
      template <typename T>
      const data_writer& write_loop(const T*, size_t len = sizeof(T),
                                    const char *context = "write_loop") const;
      virtual const data_writer &
      write_loop(const void *buff, size_t len,
                 const char *context = "write_loop") const;
    };


    class wfile_descriptor : public file_descriptor,
                             public data_writer
    {
    public:
      wfile_descriptor();
      wfile_descriptor(int fd);
      virtual ssize_t write(const void *buff, size_t len) const;
    };


    // protocol is type -> size_t(length) -> char[length]. length may be 0.

    class reporter
    {
      data_writer  *writer_;
      data_reader  *reader_;
      std::ostream &default_out_;

      typedef crpcut_test_monitor tm;
    public:
      static datatypes::fixed_string no_location()
      {
        datatypes::fixed_string n = { 0, 0 };
        return n;
      }
      virtual ~reporter();
      reporter(std::ostream &default_out = std::cout);
      void set_writer(data_writer *w);
      void operator()(type                       t,
                      const std::ostringstream  &os,
                      datatypes::fixed_string    location = no_location(),
                      const crpcut_test_monitor *mon = tm::current_test()) const;
      template <size_t N>
      void operator()(type                       t,
                      const stream::toastream<N>&os,
                      datatypes::fixed_string    location = no_location(),
                      const crpcut_test_monitor *mon = tm::current_test()) const;
      void operator()(type                       t,
                      const stream::oastream    &os,
                      datatypes::fixed_string    location = no_location(),
                      const crpcut_test_monitor *mon = tm::current_test()) const;
      void operator()(type                       t,
                      const char                *msg,
                      datatypes::fixed_string    location = no_location(),
                      const crpcut_test_monitor *mon = tm::current_test()) const;
      void operator()(type t,
                      const char                *msg,
                      size_t                     len,
                      datatypes::fixed_string    location = no_location(),
                      const crpcut_test_monitor *mon = tm::current_test()) const;
      template <size_t N>
      void operator()(type                       t,
                      const char               (&msg)[N],
                      datatypes::fixed_string    location = no_location(),
                      const crpcut_test_monitor *mon = tm::current_test()) const;
      template <typename T>
      void operator()(type                       t,
                      const T                   &data,
                      datatypes::fixed_string    location = no_location(),
                      const crpcut_test_monitor *mon = tm::current_test()) const;
    private:
      virtual void report(type                       t,
                          const char                *msg,
                          size_t                     len,
                          datatypes::fixed_string    location,
                          const crpcut_test_monitor *mon) const;
      void send_message(type                    t,
                        const char             *msg,
                        size_t                  len,
                        datatypes::fixed_string location) const;

      template <typename T>
      void read(T& t) const;
    };

    extern reporter report;

    template <type t>
    class direct_reporter
    {
    public:
      template <size_t N>
      direct_reporter(const char               (&location)[N],
                      reporter                  &r = report,
                      const crpcut_test_monitor *mon = crpcut_test_monitor::current_test());
      direct_reporter(datatypes::fixed_string  location,
                      reporter                &r = report,
                      const crpcut_test_monitor *mon = crpcut_test_monitor::current_test());
      template <typename V>
      direct_reporter& operator<<(V& v);
      template <typename V>
      direct_reporter& operator<<(const V& v);
      template <typename V>
      direct_reporter& operator<<(V (&p)(V)){ os << p; return *this; }
      template <typename V>
      direct_reporter& operator<<(V& (&p)(V&)){ os << p; return *this; }
      direct_reporter& operator<<(std::ostream& (&p)(std::ostream&))
      {
        os << p; return *this;
      }
      direct_reporter& operator<<(std::ios& (&p)(std::ios&))
      {
        os << p; return *this;
      }
      direct_reporter& operator<<(std::ios_base& (&p)(std::ios_base&))
      {
        os << p; return *this;
      }
      ~direct_reporter();
    private:
      direct_reporter(const direct_reporter &);
      direct_reporter& operator=(const direct_reporter&);

      size_t                         heap_limit;
      const datatypes::fixed_string  location_;
      std::ostringstream             os;
      reporter                      &report_;
      const crpcut_test_monitor     *mon_;
    };

  } // namespace comm

  namespace policies {

    class crpcut_exception_translator
      : public datatypes::list_elem<crpcut_exception_translator>
    {
    public:
      static std::string try_all();
    protected:
      crpcut_exception_translator(crpcut_exception_translator &
                                  r  = root_object());
      ~crpcut_exception_translator();
      crpcut_exception_translator(int);
      std::string do_try_all();
      static crpcut_exception_translator& root_object();
    private:
      crpcut_exception_translator(const crpcut_exception_translator&);
      crpcut_exception_translator& operator=(const crpcut_exception_translator&);
      virtual std::string crpcut_translate() const;
    };

    void
    report_unexpected_exception(comm::type               action,
                                datatypes::fixed_string location,
                                const char              *check_name,
                                const char              *check_type,
                                const char              *params);
    template <size_t N>
    void
    report_unexpected_exception(comm::type   action,
                                const char (&location)[N],
                                const char  *check_name,
                                const char  *check_type,
                                const char  *params)
    {
      report_unexpected_exception(action,
                                  datatypes::fixed_string::make(location),
                                  check_name,
                                  check_type,
                                  params);
    }

    namespace deaths {
      class crpcut_none;
    }

    namespace dependencies {
      class crpcut_none {};
    }

    namespace core_dumps {
      class crpcut_default_handler;
    }

    namespace timeout {
      typedef enum { realtime, cputime } type;

      template <type t, unsigned long us>
      class enforcer;

      template <unsigned long us>
      struct constructor_enforcer
      {
        static const unsigned long crpcut_constructor_timeout_us = us;
      };

      template <unsigned long us>
      struct destructor_enforcer
      {
        static const unsigned long crpcut_destructor_timeout_us = us;
      };

    }
    class crpcut_default_policy
    {
    protected:
      typedef crpcut_none crpcut_test_tag;
      typedef void crpcut_run_wrapper;

      typedef core_dumps::crpcut_default_handler crpcut_core_dump_handler;
      typedef deaths::crpcut_none crpcut_expected_death_cause;

      typedef dependencies::crpcut_none crpcut_dependency;

      typedef timeout::enforcer<timeout::realtime,2000000U> crpcut_realtime_enforcer;
      typedef timeout::enforcer<timeout::cputime, 0> crpcut_cputime_enforcer;

      typedef timeout::constructor_enforcer<1000000U>
      crpcut_constructor_timeout_enforcer;
      typedef timeout::destructor_enforcer<1000000U>
      crpcut_destructor_timeout_enforcer;
    };

    namespace core_dumps {
      class crpcut_default_handler
      {
      public:
        virtual ~crpcut_default_handler(); // make eclipse happy
        virtual bool crpcut_core_dumps_allowed() const;
      };

      class crpcut_ignore : public virtual crpcut_default_handler
      {
      public:
        virtual bool crpcut_core_dumps_allowed() const;
      };
    }
    namespace deaths {

      class crpcut_none
      {
      public:
        crpcut_none() {}
        virtual ~crpcut_none() {}
        virtual bool crpcut_is_expected_exit(int) const;
        virtual bool crpcut_is_expected_signal(int) const;
        virtual void crpcut_expected_death(std::ostream &os) const;
        virtual void crpcut_on_ok_action(const char *wd_name) const;
        virtual unsigned long crpcut_calc_deadline(unsigned long ts) const;
      };

      template <int N, typename action>
      class signal : public virtual crpcut_none,
                     private action
      {
      public:
        virtual void crpcut_on_ok_action(const char *wd_name) const
        {
          action::crpcut_on_ok_action(wd_name);
        }
        virtual bool crpcut_is_expected_signal(int code) const;
        virtual void crpcut_expected_death(std::ostream &os) const;
      };

      template <int N, typename action>
      class exit : public virtual crpcut_none,
                   private action
      {
      public:
        virtual void crpcut_on_ok_action(const char *wd_name) const
        {
          action::crpcut_on_ok_action(wd_name);
        }
        virtual bool crpcut_is_expected_exit(int code) const;
        virtual void crpcut_expected_death(std::ostream &os) const;
      };

      template <unsigned long US>
      class timeout : public virtual crpcut_none
      {
      public:
        virtual bool          crpcut_is_expected_signal(int code) const;
        virtual void          crpcut_expected_death(std::ostream &os) const;
        virtual unsigned long crpcut_calc_deadline(unsigned long ts) const;
      };

      class wrapper;
      class timeout_wrapper;

      class no_action
      {
      public:
        virtual ~no_action() {}
        virtual void crpcut_on_ok_action(const char *) const;
      };

      class wipe_working_dir
      {
      public:
        virtual ~wipe_working_dir() {}
        virtual void crpcut_on_ok_action(const char *wd_name) const;
      private:
      };
    } // namespace deaths

    template <int N, typename action = deaths::no_action>
    class signal_death : protected virtual crpcut_default_policy
    {
    public:
      typedef deaths::wrapper crpcut_run_wrapper;
      typedef deaths::signal<N, action> crpcut_expected_death_cause;
    };

    template <int N, typename action = deaths::no_action>
    class exit_death : protected virtual crpcut_default_policy
    {
    public:
      typedef deaths::wrapper  crpcut_run_wrapper;
      typedef deaths::exit<N, action>  crpcut_expected_death_cause;
    };

    template <unsigned long US>
    class realtime_timeout_death : protected virtual crpcut_default_policy
    {
    public:
      typedef deaths::timeout_wrapper crpcut_run_wrapper;
      typedef deaths::timeout<US>      crpcut_expected_death_cause;
      typedef timeout::enforcer<timeout::realtime, US> crpcut_realtime_enforcer;
    };

    class any_exception_wrapper;

    template <typename exc>
    class exception_wrapper;

    template <typename T>
    class exception_specifier;

    template <typename T>
    class exception_specifier<void (T)> : protected virtual crpcut_default_policy
    {
    public:
      typedef exception_wrapper<T> crpcut_run_wrapper;
      template <comm::type action, size_t N>
      static void check_match(const char (&)[N], const char *,crpcut_none) { }

      template <comm::type action, size_t N>
      static void check_match(const char (&location)[N],
                              const char *param,
                              const char *pattern,
                              crpcut_none);

      template <comm::type action, size_t N, typename M>
      static void check_match(const char (&location)[N],
                              const char *param_string,
                              M           m,
                              crpcut_none);
    };

    template <>
    class exception_specifier<void (...)> : protected virtual crpcut_default_policy
    {
    public:
      typedef any_exception_wrapper crpcut_run_wrapper;
      template <comm::type>
      static void check_match(...) {}
    };

    class no_core_file : protected virtual crpcut_default_policy
    {
    protected:
      typedef core_dumps::crpcut_ignore crpcut_core_dump_handler;
      no_core_file();
    };

    namespace dependencies {

      class basic_enforcer;
      class crpcut_base
      {
      protected:
      public:
        void crpcut_inc();
        virtual ~crpcut_base();
        crpcut_base();
        void crpcut_add(basic_enforcer * other);
        bool crpcut_can_run() const;
        bool crpcut_failed() const;
        bool crpcut_succeeded() const;
        void crpcut_uninhibit_dependants();
        void crpcut_register_success(bool value = true);
      private:
        virtual void crpcut_add_action(basic_enforcer *other);
        virtual void crpcut_dec_action() {}
        enum { crpcut_success, crpcut_fail, crpcut_not_run } crpcut_state;
        int crpcut_num;
        basic_enforcer *crpcut_dependants;
      };

      class basic_enforcer : public virtual crpcut_base
      {
        friend class crpcut_base;
        basic_enforcer *next;
      protected:
        basic_enforcer();
      };


      template <typename T>
      class enforcer : public basic_enforcer
      {
      public:
        enforcer();
      };

      template <typename T, typename U = crpcut::crpcut_none>
      struct nested
      {
        typedef typename T::crpcut_dependency type;
      };

      template <>
      struct nested<crpcut::crpcut_none, crpcut::crpcut_none>
      {
        typedef crpcut::crpcut_none type;
      };

    } // namespace dependencies

    template <typename T>
    class dependency_policy : protected virtual crpcut_default_policy
    {
    public:
      typedef typename datatypes::wrap<dependencies::enforcer,
                                       T>::type  crpcut_dependency;
    };

    namespace timeout {

      template <type t, unsigned long timeout_us>
      class enforcer;

      class cputime_enforcer
      {
      public:
        cputime_enforcer(unsigned long              timeout_us,
                         const crpcut_test_monitor *current_test);
        ~cputime_enforcer();
      private:

        unsigned long              duration_us_;
        unsigned long              start_timestamp_us_;
        const crpcut_test_monitor *current_test_;
      };

      class monotonic_enforcer
      {
      protected:
        monotonic_enforcer(unsigned long              timeout_us,
                           const crpcut_test_monitor *current_test);
        ~monotonic_enforcer();
      private:

        unsigned long              duration_us_;
        unsigned long              start_timestamp_us_;
        const crpcut_test_monitor *current_test_;
      };

      template <unsigned long timeout_us>
      class enforcer<cputime, timeout_us>
      {
      public:
        static const unsigned long crpcut_cputime_timeout_us = timeout_us;
      };


      template <unsigned long timeout_us>
      class enforcer<realtime, timeout_us> : public monotonic_enforcer
      {
      public:
        enforcer(const crpcut_test_monitor*);
      };

    } // namespace timeout


    template <timeout::type t, unsigned long timeout_us>
    class timeout_policy;

    template <unsigned long timeout_us>
    class timeout_policy<timeout::realtime, timeout_us>
      : protected virtual crpcut_default_policy
    {
    public:
      typedef timeout::enforcer<timeout::realtime, timeout_us>
      crpcut_realtime_enforcer;
    };

    template <unsigned long timeout_us>
    class timeout_policy<timeout::cputime, timeout_us>
      : protected virtual crpcut_default_policy
    {
    public:
      typedef timeout::enforcer<timeout::cputime, timeout_us>
      crpcut_cputime_enforcer;
    };


    template <unsigned long us>
    struct constructor_timeout_policy : public virtual crpcut_default_policy
    {
      typedef timeout::constructor_enforcer<us>
      crpcut_constructor_timeout_enforcer;
    };

    template <unsigned long us>
    struct destructor_timeout_policy : public virtual crpcut_default_policy
    {
      typedef timeout::destructor_enforcer<us>
      crpcut_destructor_timeout_enforcer;
    };

    template <typename T>
    struct tag_policy : public virtual policies::crpcut_default_policy
    {
      typedef T crpcut_test_tag;
    };
  } // namespace policies

  class crpcut_test_case_registrator;

  class crpcut_test_case_base
  : protected virtual policies::crpcut_default_policy
  {
    virtual void crpcut_run_test() = 0;
  protected:
    crpcut_test_case_base();
    virtual ~crpcut_test_case_base();
  public:
    virtual crpcut_test_case_registrator& crpcut_get_reg() const = 0;
    virtual void test() = 0;
    void crpcut_run();
    void crpcut_test_finished();
  private:

    bool crpcut_finished;
  };


  namespace heap {
    const size_t system = ~size_t();
    size_t allocated_bytes();
    size_t allocated_objects();
    size_t set_limit(size_t n); // must be higher than allocated_bytes()

    struct mem_list_element
    {
      mem_list_element        *next;
      mem_list_element        *prev;
      mem_list_element        *stack;
      size_t                   stack_size;
      size_t                   mem;
      size_t                   type;
      void insert_last(mem_list_element *p);
      void unlink();
    };

    class local_root : public mem_list_element
    {
    public:
      typedef const local_root *bool_test;
      template <size_t N>
      local_root(comm::type type, const char (&location)[N]);
      local_root(const local_root&);
      ~local_root();
      operator bool_test() const { return 0; }
      void nonsense_func() const {}
      static mem_list_element* current();
    private:
      local_root();
      void assert_empty() const;
      local_root& operator=(const local_root&);

      const datatypes::fixed_string  location_;
      mutable mem_list_element      *old_root_;
      comm::type        const        check_type_;

      static mem_list_element *current_root;
    };

    template <size_t N>
    local_root::local_root(comm::type t, const char (&location)[N])
      : location_(datatypes::fixed_string::make(location)),
        old_root_(current()),
        check_type_(t)
      {
        current_root = this;
        next = this;
        prev = this;
        stack = 0;
        type = 0;
      }

  }

  template <comm::type>
  struct crpcut_check_name
  {
    static const char *string();
  };



  struct namespace_info
  {
  public:
    namespace_info(const char *n, namespace_info *p);
    const char* match_name(const char *n) const;
    // returns 0 on mismatch, otherwise a pointer into n where namespace name
    // ended.

    std::size_t full_name_len() const;
    friend std::ostream &operator<<(std::ostream &, const namespace_info &);
  private:
    const char *name;
    namespace_info *parent;
  };

  class fdreader : public comm::rfile_descriptor
  {
  public:
    bool read_data();
    crpcut_test_monitor *get_monitor() const;
    virtual void close();
    void unregister();
    virtual ~fdreader() { }
  protected:
    fdreader(crpcut_test_monitor *r, int fd = -1);
    void set_fd(int fd, poll<fdreader> *poller);
    crpcut_test_monitor *const mon_;
  private:
    virtual bool do_read_data() = 0;
    poll<fdreader> *poller_;
  };

  template <comm::type t>
  class reader : public fdreader
  {
  public:
    reader(crpcut_test_monitor *r, int fd = -1);
    using fdreader::set_fd;
  private:
    virtual bool do_read_data();
  };

  class report_reader : public fdreader
  {
  public:
    report_reader(crpcut_test_monitor *r);
    using fdreader::set_fd;
  private:
    void set_timeout(void *buff, size_t len);
    void cancel_timeout();
    virtual bool do_read_data();
  };

  class timeboxed
  {
  public:
    virtual ~timeboxed();
    void set_deadline(unsigned long absolute_us);
    void clear_deadline();
    bool deadline_is_set() const;
    virtual void kill() = 0;
    unsigned long absolute_deadline() const;
    static bool compare(const timeboxed *lh, const timeboxed *rh);
  protected:
    timeboxed();
  private:
    unsigned long crpcut_absolute_deadline_us_;
    bool          crpcut_deadline_set_;
  };

  class process_control;
  process_control *process_control_root();
  class filesystem_operations;
  filesystem_operations *filesystem_operations_root();

  class test_environment;
  test_runner *default_test_runner();
  class test_suite_base;
  class crpcut_test_case_registrator
    : public virtual policies::deaths::crpcut_none,
      public virtual policies::dependencies::crpcut_base,
      public virtual policies::core_dumps::crpcut_default_handler,
      public timeboxed,
      public datatypes::list_elem<crpcut_test_case_registrator>,
      public crpcut_test_monitor
  {
    friend class test_suite_base;
  public:
    crpcut_test_case_registrator(const char *name,
                                 datatypes::fixed_string location,
                                 const namespace_info &ns,
                                 unsigned long cputime_limit_us,
                                 comm::reporter *reporter = &comm::report,
                                 process_control *process = process_control_root(),
                                 filesystem_operations *fsops = filesystem_operations_root(),
                                 test_runner *runner = default_test_runner());
    friend std::ostream &operator<<(std::ostream &os,
                                    const crpcut_test_case_registrator &t)
    {
      return t.print_name(os);
    }
    bool deadline_is_set() const { return timeboxed::deadline_is_set(); }
    void crpcut_register_success(bool v) { crpcut_base::crpcut_register_success(v); }
    bool crpcut_failed() const { return crpcut_base::crpcut_failed(); }
    std::size_t full_name_len() const;
    bool match_name(const char *name) const;
    virtual void setup(poll<fdreader> &poller,
                       int             in_fd,
                       int             stdout_fd,
                       int             stderr_fd) = 0;
    void set_test_environment(test_environment *env);
    datatypes::fixed_string get_location() const;
    void manage_death();
    using datatypes::list_elem<crpcut_test_case_registrator>::unlink;
    void kill();
    void clear_deadline();
    void unregister_fds();
    void set_wd(unsigned n);
    void goto_wd() const;
    pid_t get_pid() const;
    bool is_naughty_child() const;
    void freeze() const;
    test_phase get_phase() const;
    void set_phase(test_phase p);
    bool has_active_readers() const;
    void deactivate_reader();
    void activate_reader();
    void set_timeout(unsigned long);
    unsigned long duration_us() const;
    virtual void run_test_case() = 0;
    virtual tag& crpcut_tag() const = 0;
    virtual tag::importance get_importance() const = 0;
    void set_cputime_at_start(const struct timeval &t);
    bool has_death_note() const;
    void set_death_note();
    void send_to_presentation(comm::type t, size_t len, const char *buff) const;
    void set_pid(pid_t pid);
  protected:
    crpcut_test_case_registrator(const char *name = 0, namespace_info *ns = 0);
    void manage_test_case_execution(crpcut_test_case_base*);
    void prepare_destruction(unsigned long us);
    void prepare_construction(unsigned long us);
  private:
     bool check_signal_status(int            signo,
                              unsigned long  cputime_us,
                              std::ostream  &out);
    bool check_exit_status(int status, std::ostream &out);
    bool report_nonempty_working_dir(const char* dirname);
    bool cputime_timeout(unsigned long us) const;
    std::ostream &print_name(std::ostream &) const ;

    const char                   *name_;
    const datatypes::fixed_string location_;
    const namespace_info         *ns_info_;
    crpcut_test_case_registrator *suite_list_;
    unsigned                      active_readers_;
    bool                          killed_;
    bool                          death_note_;
    pid_t                         pid_;
    unsigned long                 real_time_at_start_;
    struct timeval                cpu_time_at_start_;
    unsigned                      dirnum_;
    test_phase                    phase_;
    const unsigned long           cputime_limit_us_;
    test_runner                  *runner_;
    test_environment             *env_;
    comm::reporter               *reporter_;
    process_control              *process_;
    filesystem_operations        *filesystem_;
  };

  namespace cli {
    class interpreter;
  }
  template <typename T>
  class buffer_vector;
  template <typename C>
  struct test_wrapper
  {
    static void run(crpcut_test_case_base *t, comm::reporter &report);
  };

  template <typename exc>
  struct test_wrapper<policies::exception_wrapper<exc> >
  {
    static void run(crpcut_test_case_base *t, comm::reporter &report_obj)
    {
      crpcut_test_monitor &current_test = t->crpcut_get_reg();
      try {
        t->test();
      }
      catch (exc&) {
        return;
      }
      catch (...)
        {
          std::ostringstream os;
          os << "Unexpectedly caught "
             << policies::crpcut_exception_translator::try_all();
          report_obj(comm::exit_fail, os, current_test.get_location(), &current_test);
        }
      report_obj(comm::exit_fail,
                 "Unexpectedly did not throw",
                 current_test.get_location(),
                 &current_test);
    }
  };
  template <typename T,
            size_t size_limit = sizeof(T),
            bool b = stream_checker::is_output_streamable<T>::value>
  struct conditional_streamer
  {
    static void stream(std::ostream &os, const T& t)
    {
      os << t;
    }
  };

  void hexdump(std::ostream &os, std::size_t bytes, const void *addr);

  template <typename T, size_t size_limit>
  struct conditional_streamer<T, size_limit, false>
  {
    static void stream(std::ostream &os, const T& t)
    {
      if (sizeof(T) > size_limit)
        {
          os << '?';
          return;
        }
      hexdump(os, sizeof(T), &t);
    }
  };

  inline void show_value(std::ostream &os, const std::exception &e)
  {
    os << "what() == \"" << e.what() << "\"";
  }

  namespace stream {
    template <typename T>
    class is_std_exception
    {
      typedef char no;
      struct yes { char v[2]; };
      static no func(...);
      static yes func(std::exception *);
    public:
      static const bool value = sizeof(func(static_cast<T*>(0))) == sizeof(yes);
    };

    template <typename T, bool b = is_std_exception<T>::value>
    struct value_streamer
    {
      static void show_value(std::ostream &os, const T& t)
      {
        conditional_streamer<T>::stream(os, t);
      }
    };

    template <typename T>
    struct value_streamer<T, true>
    {
      static void show_value(std::ostream &os, const T& t)
      {
        const std::exception &e = t;
        crpcut::show_value(os, e);
      }
    };

  }

  template <typename T>
  void show_value(std::ostream &os, const T& t)
  {
    stream::value_streamer<T>::show_value(os, t);
  }


  template <size_t N, typename T>
  void show_value(std::ostream &os, const T& t)
  {
    conditional_streamer<T, N>::stream(os, t);
  }



  class null_cmp
  {
    class secret;
  public:
    static char func(secret*);
    static char (&func(...))[2];
    template <typename T>
    static char (&func(T*))[2];
  };

  template <typename T>
  class is_struct // or class or union
  {
    template <typename U>
    static char check_member(double U::*);
    template <typename U>
    static char (&check_member(...))[2];
  public:
    static const bool value = (sizeof(check_member<T>(0)) == 1);
  };

  template <bool b, typename T1, typename T2>
  struct if_else
  {
    typedef T1 type;
  };

  template <typename T1, typename T2>
  struct if_else<false, T1, T2>
  {
    typedef T2 type;
  };

  template <typename T>
  struct param_traits
  {
    typedef typename if_else<is_struct<T>::value, const  T&, T>::type type;
  };

  template <typename T>
  struct param_traits<const T>
  {
    typedef typename param_traits<T>::type type;
  };

  template <typename T>
  struct param_traits<volatile T>
  {
    typedef typename param_traits<T>::type type;
  };

  template <typename T>
  struct param_traits<const volatile T>
  {
    typedef typename param_traits<T>::type type;
  };

  template <typename T, size_t N>
  struct param_traits<T[N]>
  {
    typedef T *type;
  };

  template <typename T, size_t N>
  struct param_traits<const T[N]>
  {
    typedef const T *type;
  };

  template <typename T, size_t N>
  struct param_traits<volatile T[N]>
  {
    typedef volatile T *type;
  };

  template <typename T, size_t N>
  struct param_traits<const volatile T[N]>
  {
    typedef volatile const T *type;
  };

  template <typename T>
  struct param_traits<T&>
  {
    typedef typename param_traits<T>::type type;
  };

  template <typename T>
  bool stream_param(std::ostream &os,
                    const char *prefix,
                    const char *name, const T& t)
  {
    std::ostringstream tmp;
    crpcut::show_value(tmp, t);
    std::string str = tmp.str();
    if (str != name)
      {
        os << prefix << name << " = " << str;
        return true;
      }
    return false;
  }

  class tester_base
  {
  protected:
    template <size_t N>
    tester_base(const char (&loc)[N],
                const char *ops,
                comm::reporter &report,
                const crpcut_test_monitor *mon)
      : location_(datatypes::fixed_string::make(loc)),
        op_(ops),
        report_(report),
        mon_(mon)
    {
    }
    template <comm::type action, typename T1, typename T2>
    void verify(bool b, T1 v1, const char *n1, T2 v2, const char *n2) const
    {
      if (!b)
        {
          heap::set_limit(heap::system);
          using std::ostringstream;
          ostringstream os;
          os << crpcut_check_name<action>::string()
             << "_" << op_ << "(" << n1 << ", " << n2 << ")";

          static const char *prefix[] = { "\n  where ", "\n        " };
          bool prev = stream_param(os, prefix[0], n1, v1);
          stream_param(os, prefix[prev], n2, v2);
          std::string s(os.str());
          os.str(std::string());
          size_t len = s.length();
          char *p = static_cast<char *>(alloca(len));
          s.copy(p, len);
          std::string().swap(s);
          os.~ostringstream();
          new (&os) ostringstream();
          this->report_(action, p, len, location_, mon_);
        }
      }
  private:
    const datatypes::fixed_string  location_;
    const char                    *op_;
    comm::reporter                &report_;
    const crpcut_test_monitor     *mon_;
  };

  template <comm::type action, typename T1, typename T2>
  class tester_t : tester_base
  {
    typedef typename param_traits<T1>::type type1;
    typedef typename param_traits<T2>::type type2;
  public:
    template <size_t N>
    tester_t(const char               (&loc)[N],
             const char                *ops,
             comm::reporter            &report,
             const crpcut_test_monitor *mon)
      : tester_base(loc, ops, report, mon)
    {
    }
    void EQ(type1 v1, const char *n1, type2 v2, const char *n2) const
    {
      verify<action, type1, type2>(v1 == v2, v1, n1, v2, n2);
    }
    void NE(type1 v1, const char *n1, type2 v2, const char *n2) const
    {
      verify<action, type1, type2>(v1 != v2, v1, n1, v2, n2);
    }
    void GT(type1 v1, const char *n1, type2 v2, const char *n2) const
    {
      verify<action, type1, type2>(v1 > v2, v1, n1, v2, n2);
    }
    void GE(type1 v1, const char *n1, type2 v2, const char *n2) const
    {
      verify<action, type1, type2>(v1 >= v2, v1, n1, v2, n2);
    }
    void LT(type1 v1, const char *n1, type2 v2, const char *n2) const
    {
      verify<action, type1, type2>(v1 < v2, v1, n1, v2, n2);
    }
    void LE(type1 v1, const char *n1, type2 v2, const char *n2) const
    {
      verify<action, type1, type2>(v1 <= v2, v1, n1, v2, n2);
    }
  };

  template <comm::type action, typename T1>
  class tester_t<action, T1, void> : tester_base
  {
    typedef typename param_traits<T1>::type type1;
  public:
    template <size_t N>
    tester_t(const char               (&loc)[N],
             const char                *ops,
             comm::reporter            &report,
             const crpcut_test_monitor *mon)
      : tester_base(loc, ops, report, mon)
    {
    }
    template <typename T2>
    void EQ(type1 v1, const char *n1, T2 v2, const char *n2) const
    {
      verify<action, type1, T2>(v1 == 0, v1, n1, v2, n2);
    }
    template <typename T2>
    void NE(type1 v1, const char *n1, T2 v2, const char *n2) const
    {
      verify<action, type1, T2>(v1 != 0, v1, n1, v2, n2);
    }
    template <typename T2>
    void GT(type1 v1, const char *n1, T2 v2, const char *n2) const
    {
      verify<action, type1, T2>(v1 > 0, v1, n1, v2, n2);
    }
    template <typename T2>
    void GE(type1 v1, const char *n1, T2 v2, const char *n2) const
    {
      verify<action, type1, T2>(v1 >= 0, v1, n1, v2, n2);
    }
    template <typename T2>
    void LT(type1 v1, const char *n1, T2 v2, const char *n2) const
    {
      verify<action, type1, T2>(v1 < 0, v1, n1, v2, n2);
    }
    template <typename T2>
    void LE(type1 v1, const char *n1, T2 v2, const char *n2) const
    {
      verify<action, type1, T2>(v1 <= 0, v1, n1, v2, n2);
    }
  };

  template <comm::type action, typename T2>
  class tester_t<action, void, T2> : tester_base
  {
    typedef typename param_traits<T2>::type type2;
  public:
    template <size_t N>
    tester_t(const char               (&loc)[N],
             const char                *ops,
             comm::reporter            &report,
             const crpcut_test_monitor *mon)
      : tester_base(loc, ops, report, mon)
    {
    }
    template <typename T1>
    void EQ(T1 v1, const char *n1, type2 v2, const char *n2) const
    {
      verify<action, T1, type2>(0 == v2, v1, n1, v2, n2);
    }
    template <typename T1>
    void NE(T1 v1, const char *n1, type2 v2, const char *n2) const
    {
      verify<action, T1, type2>(0 != v2, v1, n1, v2, n2);
    }
    template <typename T1>
    void GT(T1 v1, const char *n1, type2 v2, const char *n2) const
    {
      verify<action, T1, type2>(0 > v2, v1, n1, v2, n2);
    }
    template <typename T1>
    void GE(T1 v1, const char *n1, type2 v2, const char *n2) const
    {
      verify<action, T1, type2>(0 >= v2, v1, n1, v2, n2);
    }
    template <typename T1>
    void LT(T1 v1, const char *n1, type2 v2, const char *n2) const
    {
      verify<action, T1, type2>(0 < v2, v1, n1, v2, n2);
    }
    template <typename T1>
    void LE(T1 v1, const char *n1, type2 v2, const char *n2) const
    {
      verify<action, T1, type2>(0 <= v2, v1, n1, v2, n2);
    }
  };

  template <comm::type action, bool null1, typename T1, bool null2, typename T2, size_t N>
  tester_t<action,
           typename if_else<null1, void, T1>::type,
           typename if_else<null2, void, T2>::type>
  tester(const char (&loc)[N],
         const char *op,
         comm::reporter & report = comm::report,
         const crpcut_test_monitor *mon = crpcut_test_monitor::current_test())
  {
    tester_t<action,
             typename if_else<null1, void, T1>::type,
             typename if_else<null2, void, T2>::type> v(loc, op, report, mon);
    return v;
  }

  class failed_check_reporter
  {
  public:
    static std::ostringstream& prepare(std::ostringstream &os,
                                       const char *check_type,
                                       const char *check_name,
                                       const char *expr_string);
  };

  template <comm::type action>
  class bool_tester : failed_check_reporter
  {
  public:
    template <size_t N>
    bool_tester(const char               (&loc)[N],
                comm::reporter            &reporter = comm::report,
                const crpcut_test_monitor *mon = crpcut_test_monitor::current_test())
      : loc_(datatypes::fixed_string::make(loc)),
        report_(reporter),
        mon_(mon)
    {
    }
    template <typename T>
    void check_true(const T& v, const char *vn) const
    {
      if (crpcut::eval(v)) {} else { report("TRUE", v, vn); }
    }
    template <typename T>
    void check_false(const T& v, const char *vn) const
    {
      if (crpcut::eval(v)) { report("FALSE", v, vn); }
    }
  private:
    template <typename T>
    void report(const char *name, const T& v, const char *vn) const
    {
      heap::set_limit(heap::system);
      std::ostringstream os;

      crpcut::show_value<8>(prepare(os,
                                    crpcut_check_name<action>::string(),
                                    name,
                                    vn),
                             v);
      this->report_(action, os, loc_, mon_);
    }
    const datatypes::fixed_string loc_;
    comm::reporter            &report_;
    const crpcut_test_monitor *mon_;
  };

  template <case_convert_type converter>
  struct convert_traits
  {
    static const char *do_convert(char *lo, const char *, const std::locale &)
    {
      return lo;
    }
  };

  template <>
  const char *
  convert_traits<uppercase>
  ::do_convert(char *lo, const char *, const std::locale &);

  template <>
  const char *
  convert_traits<lowercase>
  ::do_convert(char *lo, const char *, const std::locale &);


  class collate_result
  {
    class comparator;
    const char *r;
    const std::string intl;
    std::locale locale;
    enum { left, right } side;
    collate_result(const char *refstr, std::string comp, const std::locale& l);
  public:
    collate_result(const collate_result& o);
    operator const comparator*() const;
    const comparator* operator()() const;
    collate_result& set_lh();
    friend
    std::ostream &operator<<(std::ostream& os, const collate_result &obj);

    template <case_convert_type>
    friend class crpcut::collate_t;
  };

#ifdef CRPCUT_SUPPORTS_VTEMPLATES
  template <int US, typename ...T>
  class param_holder;

  template <int US>
  class param_holder<US>
  {
  public:
    param_holder() {}
    template <typename P, typename ...V>
    bool apply(P& func, const V &...v) const {
      return func(v...);
    }
    void print_to(std::ostream &) const { }
  };

  template <int US, typename T, typename ...Tail>
  class param_holder<US, T, Tail...> : public param_holder<US+1, Tail...>
  {
  public:
    param_holder(const T& v, const Tail&...tail)
      : param_holder<US+1,Tail...>(tail...),
        val(v)
    {
    }
    template <typename P, typename ...V>
    bool apply(P& func, const V&...v) const {
      return param_holder<US+1,Tail...>::apply(func, v..., val);
    }
    void print_to(std::ostream &os) const {
      os << "  param" << US << " = " << val << '\n';
      param_holder<US+1, Tail...>::print_to(os);
    }
  private:
    const T& val;
  };

  template <typename ...T>
  param_holder<1, T...> params(const T&... v)
  {
    return param_holder<1, T...>(v...);
  }

#else

  template <int N, typename T>
  class holder
  {
  protected:
    holder(const T& v) : val(v) {}
    const T& getval() const { return val; }
    void print_to(std::ostream &os) const
    {
      os << "  param" << N << " = " << val << "\n";
    }
  private:
    const T &val;
  };

  template <int N>
  class holder<N, crpcut_none> : private crpcut_none
  {
  protected:
    holder(const crpcut_none&) {}
    void print_to(std::ostream&) const {};
    const crpcut_none& getval() const { return *this; }
  };

  template <typename T1,               typename T2 = crpcut_none,
            typename T3 = crpcut_none, typename T4 = crpcut_none,
            typename T5 = crpcut_none, typename T6 = crpcut_none,
            typename T7 = crpcut_none, typename T8 = crpcut_none,
            typename T9 = crpcut_none>
  class param_holder
    : holder<1, T1>, holder<2, T2>, holder<3, T3>,
      holder<4, T4>, holder<5, T5>, holder<6, T6>,
      holder<7, T7>, holder<8, T8>, holder<9, T9>
  {
  public:
    param_holder(const T1 &v1, const T2 &v2 = T2(), const T3 &v3 = T3(),
                 const T4 &v4 = T4(), const T5 &v5 = T5(),
                 const T6 &v6 = T6(), const T7 &v7 = T7(),
                 const T8 &v8 = T8(), const T9 &v9 = T9())
      : holder<1, T1>(v1),
        holder<2, T2>(v2),
        holder<3, T3>(v3),
        holder<4, T4>(v4),
        holder<5, T5>(v5),
        holder<6, T6>(v6),
        holder<7, T7>(v7),
        holder<8, T8>(v8),
        holder<9, T9>(v9)
    {}
    template <typename P>
    bool apply(P &pred) const;
    void print_to(std::ostream &os) const
    {
      holder<1, T1>::print_to(os);
      holder<2, T2>::print_to(os);
      holder<3, T3>::print_to(os);
      holder<4, T4>::print_to(os);
      holder<5, T5>::print_to(os);
      holder<6, T6>::print_to(os);
      holder<7, T7>::print_to(os);
      holder<8, T8>::print_to(os);
      holder<9, T9>::print_to(os);
    }
  };

  template <typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8, typename T9>
  struct call_traits
  {
    template <typename P>
    static bool call(P &p,
                     const T1 &t1, const T2 &t2, const T3 &t3,
                     const T4 &t4, const T5 &t5, const T6 &t6,
                     const T7 &t7, const T8 &t8, const T9 &t9)
    {
      return p(t1, t2, t3, t4, t5, t6, t7, t8, t9);
    }
  };

  template <typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8>
  struct call_traits<T1, T2, T3, T4, T5, T6, T7, T8, crpcut_none>
  {
    template <typename P>
    static bool call(P &p,
                     const T1 &t1, const T2 &t2, const T3 &t3,
                     const T4 &t4, const T5 &t5, const T6 &t6,
                     const T7 &t7, const T8 &t8, const crpcut_none&)
    {
      return p(t1, t2, t3, t4, t5, t6, t7, t8);
    }
  };

  template <typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7>
  struct call_traits<T1, T2, T3, T4, T5, T6, T7, crpcut_none, crpcut_none>
  {
    template <typename P>
    static bool call(P &p,
                     const T1 &t1, const T2 &t2, const T3 &t3,
                     const T4 &t4, const T5 &t5, const T6 &t6,
                     const T7 &t7, const crpcut_none&, const crpcut_none&)
    {
      return p(t1, t2, t3, t4, t5, t6, t7);
    }
  };

  template <typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6>
  struct call_traits<T1, T2, T3, T4, T5, T6,
                     crpcut_none, crpcut_none, crpcut_none>
  {
    template <typename P>
    static bool call(P &p,
                     const T1 &t1, const T2 &t2, const T3 &t3,
                     const T4 &t4, const T5 &t5, const T6 &t6,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&)
    {
      return p(t1, t2, t3, t4, t5, t6);
    }
  };

  template <typename T1, typename T2, typename T3,
            typename T4, typename T5>
  struct call_traits<T1, T2, T3, T4, T5,
                     crpcut_none, crpcut_none, crpcut_none, crpcut_none>
  {
    template <typename P>
    static bool call(P &p, const T1& t1, const T2 &t2, const T3 &t3,
                     const T4 &t4, const T5 &t5, const crpcut_none&,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&)
    {
      return p(t1, t2, t3, t4, t5);
    }
  };

  template <typename T1, typename T2, typename T3,
            typename T4>
  struct call_traits<T1, T2, T3, T4,
                     crpcut_none, crpcut_none, crpcut_none,
                     crpcut_none, crpcut_none>
  {
    template <typename P>
    static bool call(P &p, const T1& t1, const T2 &t2, const T3 &t3,
                     const T4 &t4, const crpcut_none&, const crpcut_none&,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&)
    {
      return p(t1, t2, t3, t4);
    }
  };

  template <typename T1, typename T2, typename T3>
  struct call_traits<T1, T2, T3, crpcut_none,
                     crpcut_none, crpcut_none, crpcut_none, crpcut_none,
                     crpcut_none>
  {
    template <typename P>
    static bool call(P &p, const T1& t1, const T2 &t2, const T3 &t3,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&, const crpcut_none&)
    {
      return p(t1, t2, t3);
    }
  };

  template <typename T1, typename T2>
  struct call_traits<T1, T2, crpcut_none, crpcut_none, crpcut_none, crpcut_none, crpcut_none, crpcut_none, crpcut_none>
  {
    template <typename P>
    static bool call(P &p, const T1& t1, const T2 &t2, const crpcut_none&,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&, const crpcut_none&)
    {
      return p(t1, t2);
    }
  };

  template <typename T1>
  struct call_traits<T1, crpcut_none, crpcut_none,
                     crpcut_none, crpcut_none, crpcut_none, crpcut_none,
                     crpcut_none, crpcut_none>
  {
    template <typename P>
    static bool call(P &p, const T1& t1, const crpcut_none&,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&)
    {
      return p(t1);
    }
  };

  template <>
  struct call_traits<crpcut_none, crpcut_none, crpcut_none, crpcut_none,
                     crpcut_none, crpcut_none, crpcut_none, crpcut_none,
                     crpcut_none>
  {
    template <typename P>
    static bool call(P &p,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&, const crpcut_none&,
                     const crpcut_none&)
    {
      return p();
    }
  };


  template <typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8, typename T9>
  template <typename P>
  bool
  param_holder<T1, T2, T3, T4, T5, T6, T7, T8, T9>::apply(P &pred) const
  {
    typedef call_traits<T1, T2, T3, T4, T5, T6, T7, T8, T9> traits;
    return traits::call(pred,
                        holder<1, T1>::getval(),
                        holder<2, T2>::getval(),
                        holder<3, T3>::getval(),
                        holder<4, T4>::getval(),
                        holder<5, T5>::getval(),
                        holder<6, T6>::getval(),
                        holder<7, T7>::getval(),
                        holder<8, T8>::getval(),
                        holder<9, T9>::getval());
  }

  template <typename T1>
  param_holder<T1>
  params(const T1& t1)
  {
    typedef param_holder<T1> R;
    return R(t1);
  }

  template <typename T1, typename T2>
  param_holder<T1, T2>
  params(const T1& t1, const T2 &t2)
  {
    typedef param_holder<T1, T2> R;
    return R(t1, t2);
  }

  template <typename T1, typename T2, typename T3>
  param_holder<T1, T2, T3>
  params(const T1& t1, const T2 &t2, const T3 &t3)
  {
    typedef param_holder<T1, T2, T3> R;
    return R(t1, t2, t3);
  }

  template <typename T1, typename T2, typename T3, typename T4>
  param_holder<T1, T2, T3, T4>
  params(const T1& t1, const T2 &t2, const T3 &t3, const T4 &t4)
  {
    typedef param_holder<T1, T2, T3, T4> R;
    return R(t1, t2, t3, t4);
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  param_holder<T1, T2, T3, T4, T5>
  params(const T1& t1, const T2 &t2, const T3 &t3, const T4 &t4, const T5 &t5)
  {
    typedef param_holder<T1, T2, T3, T4, T5> R;
    return R(t1, t2, t3, t4, t5);
  }

  template <typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6>
  param_holder<T1, T2, T3, T4, T5, T6>
  params(const T1& t1, const T2 &t2, const T3 &t3,
         const T4 &t4, const T5 &t5, const T6 &t6)
  {
    typedef param_holder<T1, T2, T3, T4, T5, T6> R;
    return R(t1, t2, t3, t4, t5, t6);
  }

  template <typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7>
  param_holder<T1, T2, T3, T4, T5, T6, T7>
  params(const T1& t1, const T2 &t2, const T3 &t3,
         const T4 &t4, const T5 &t5, const T6 &t6,
         const T7 &t7)
  {
    typedef param_holder<T1, T2, T3, T4, T5, T6, T7> R;
    return R(t1, t2, t3, t4, t5, t6, t7);
  }

  template <typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8>
  param_holder<T1, T2, T3, T4, T5, T6, T7, T8>
  params(const T1& t1, const T2 &t2, const T3 &t3,
         const T4 &t4, const T5 &t5, const T6 &t6,
         const T7 &t7, const T8 &t8)
  {
    typedef param_holder<T1, T2, T3, T4, T5, T6, T7, T8> R;
    return R(t1, t2, t3, t4, t5, t6, t7, t8);
  }

  template <typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8, typename T9>
  param_holder<T1, T2, T3, T4, T5, T6, T7, T8, T9>
  params(const T1& t1, const T2 &t2, const T3 &t3,
         const T4 &t4, const T5 &t5, const T6 &t6,
         const T7 &t7, const T8 &t8, const T9 &t9)
  {
    typedef param_holder<T1, T2, T3, T4, T5, T6, T7, T8, T9> R;
    return R(t1, t2, t3, t4, t5, t6, t7, t8, t9);
  }

  inline
  param_holder<crpcut_none>
  params()
  {
    return param_holder<crpcut_none>(crpcut_none());
  }
#endif
  template <typename P,
            bool streamable = stream_checker::is_output_streamable<P>::value>
  struct predicate_streamer
  {
    predicate_streamer(const char *name, const P& pred) : n(name), p(pred) {}
    std::ostream &stream_to(std::ostream & os) const
    {
      return os << "for " << n << ": " << p << '\n';
    }
  private:
    const char *n;
    const P& p;
  };

  template <typename P>
  struct predicate_streamer<P, false>
  {
    predicate_streamer(const char *,const P&) {}
    std::ostream &stream_to(std::ostream &os) const { return os; }
  private:
  };

  template <typename P, bool unstreamable>
  std::ostream &operator<<(std::ostream &os,
                           const predicate_streamer<P, unstreamable>& s)
  {
    return s.stream_to(os);
  }

  template <typename P>
  predicate_streamer<P> stream_predicate(const char *n, const P& p)
  {
    return predicate_streamer<P>(n, p);
  }

  template <typename Pred, typename Params>
  bool
  match_pred(std::string &msg, const char *sp, Pred p, const Params &params)
  {
    bool b = params.apply(p);
    if (!b)
      {
        heap::set_limit(heap::system);
        std::ostringstream out;
        params.print_to(out);
        out << stream_predicate(sp, p);
        msg = out.str();
      }
    return b;
  }

  template <case_convert_type converter>
  class collate_t
  {
  public:
    collate_t(const std::string &reference,
              const std::locale &loc = std::locale())
      : ref(reference),
        locale(loc)
    {
      char *p = &*ref.begin();
      convert_traits<converter>::do_convert(p,
                                            p + ref.length(),
                                            loc);
    }
    collate_t(const char *reference, const std::locale& loc = std::locale())
      : ref(reference),
        locale(loc)
    {
      char *p = &*ref.begin();
      convert_traits<converter>::do_convert(p,
                                            p + ref.length(),
                                            loc);
    }

    collate_result operator<(const std::string &s) const
    {
      return operator<(s.c_str());
    }
    collate_result operator<(const char *r) const
    {
      collate_result rv(compare(r) < 0
                        ? 0
                        : r,
                        ref,
                        locale);
      return rv;
    }

    collate_result operator<=(const std::string &s) const
    {
      return operator<=(s.c_str());
    }
    collate_result operator<=(const char *r) const
    {
      collate_result rv(compare(r) <= 0
                        ? 0
                        : r,
                        ref,
                        locale);
      return rv;
    }

    collate_result operator>(const std::string &s) const
    {
      return operator>(s.c_str());
    }
    collate_result operator>(const char *r) const
    {
      collate_result rv(compare(r) > 0
                        ? 0
                        : r,
                        ref,
                        locale);
      return rv;
    }

    collate_result operator>=(const std::string &s) const
    {
      return operator>=(s.c_str());
    }

    collate_result operator>=(const char *r) const
    {
      collate_result rv(compare(r) >= 0
                        ? 0
                        : r,
                        ref,
                        locale);
      return rv;
    }

    collate_result operator==(const std::string &s) const
    {
      return operator==(s.c_str());
    }

    collate_result operator==(const char *r) const
    {
      collate_result rv(compare(r) == 0
                        ? 0
                        : r,
                        ref,
                        locale);
      return rv;
    }

    collate_result operator!=(const std::string &s) const
    {
      return operator!=(s.c_str());
    }

    collate_result operator!=(const char *r) const
    {
      collate_result rv(compare(r) != 0
                        ? 0
                        : r,
                        ref,
                        locale);
      return rv;
    }
  private:
    int compare(std::string s) const
    {
      char *begin = &*s.begin();
      char *end = begin + s.length();
      convert_traits<converter>::do_convert(begin,
                                            end,
                                            locale);
      typedef std::collate<char> coll;
      const coll &fac = std::use_facet<coll>(locale);
      return fac.compare(ref.c_str(), ref.c_str() + ref.length(),
                         begin, end);
    }
    int compare(const char *p) const
    {
      return compare(std::string(p));
    }
    int compare(const char *p, size_t len) const
    {
      return compare(std::string(p, len));
    }
    std::string locale_name() const { return locale.name(); }
    const char *reference_string() const { return ref.c_str(); }
  private:
    std::string ref;
    std::locale locale;
  };



  class istream_wrapper
  {
  public:
    istream_wrapper(const char *p) : is_(p) {}
    template <typename T>
    istream_wrapper& operator>>(T& t)
    {
      return assert_stream_extraction(is_ >> t,
                                      "Extract value from stream failed");
    }
    template <typename T>
    istream_wrapper& operator>>(const T &t)
    {
      return assert_stream_extraction(is_ >> t,
                                      "Stream manipulator failed");
    }
    template <typename T>
    istream_wrapper&operator>>(T (&t)(T))
    {
      return assert_stream_extraction(is_ >> t,
                                      "Stream manipulator failed");
    }
  private:
    istream_wrapper
    &assert_stream_extraction(std::istream &is, const char *msg);
    stream::iastream is_;
  };

  template <typename T>
  struct parameter_stream_traits
  {
    typedef T type;
    static T make_value(const char *n, crpcut_test_monitor *current_test);
  };

  template <>
  struct parameter_stream_traits<std::istream>
  {
    typedef istream_wrapper type;
    static type make_value(const char *n, crpcut_test_monitor *current_test);
  };

  template <typename T>
  class relaxed;

  template <>
  struct parameter_stream_traits<relaxed<std::istream> >
  {
    typedef stream::iastream type;
    static type make_value(const char *n, crpcut_test_monitor *current_test);
  };

  bool timeouts_are_enabled();
  unsigned timeout_multiplier();
  bool tests_as_child_processes();
  const char *get_output_charset();
  const char *get_program_charset();
  const char *get_illegal_char_representation();

  void set_charset(const char *set_name);
  const char *get_start_dir();
  const char *get_parameter(const char *name);

  int  run(int argc, char *argv[], std::ostream &os = std::cerr);
  int  run(int argc, const char *argv[], std::ostream &os = std::cerr);

  //// template and inline func implementations

  template <typename T>
  regex::type::type(T t, int flags)
    : errmsg_(0),
      re_str_(datatypes::string_traits::get_c_str(t))
  {
    init(re_str_, flags);
  }

  template <typename T>
  bool regex::type::operator()(const T& t)
  {
    return match(datatypes::string_traits::get_c_str(t));
  }

#if __cplusplus < 201103L
  inline regex::regex(const regex& r)
    : p_(r.p_)
  {
  }
#endif
  inline std::ostream& operator<<(std::ostream &os, const regex &r)
  {
    return os << *r.p_;
  }


  namespace datatypes {
    template <std::size_t N>
    struct fp_rep;

#ifdef __CDT_PARSER__
    template <std::size_t N>
    struct fp_rep
    {
      typedef void type;
    };
#endif // __CDT_PARSER__

#define CRPCUT_MAKE_FP_REP(x)                   \
    template <>                                 \
    struct fp_rep<sizeof(x)>                    \
    {                                           \
      typedef x type;                           \
    }

    CRPCUT_MAKE_FP_REP(uint32_t);
    CRPCUT_MAKE_FP_REP(uint64_t);

#undef CRPCUT_MAKE_FP_REP

    template <typename T>
    union fp
    {
      T data;
      typename fp_rep<sizeof(T)>::type rep;
    };

  } // namespace datatypes

  namespace stream {

    template <typename charT, typename traits>
    oabuf<charT, traits>
    ::oabuf(charT *begin_, charT *end_)
    {
      parent::setp(begin_, end_);
    }

    template <typename charT, typename traits>
    basic_oastream<charT, traits>
    ::basic_oastream(charT *begin_, charT *end_)
      : oabuf<charT, traits>(begin_, end_),
        std::basic_ostream<charT, traits>(this)
    {
    }

    template <typename charT, typename traits>
    basic_oastream<charT, traits>
    ::basic_oastream(charT *begin_, size_t size_)
      : oabuf<charT, traits>(begin_, begin_ + size_),
        std::basic_ostream<charT, traits>(this)
    {
    }

    template <typename charT, typename traits>  template <size_t N>
    basic_oastream<charT, traits>
    ::basic_oastream(charT (&buff)[N])
      : oabuf<charT, traits>(buff, buff + N),
        std::basic_ostream<charT, traits>(this)
    {
    }

    template <typename charT, typename traits>
    basic_oastream<charT, traits>
    ::operator datatypes::fixed_string() const
    {
      datatypes::fixed_string rv = { begin(), size() };
      return rv;
    }

    template <typename charT, class traits>
    iabuf<charT, traits>::iabuf(const charT *begin, const charT *end)
    {
      std::basic_streambuf<charT, traits>::setbuf(const_cast<charT *>(begin),
                                                  end - begin);
      std::basic_streambuf<charT, traits>::setg(const_cast<charT *>(begin),
                                                const_cast<charT *>(begin),
                                                const_cast<charT *>(end));
    }

    template <typename charT, typename traits>
    iabuf<charT, traits>::iabuf(const iabuf& b)
      : std::basic_streambuf<charT, traits>()
    {
      const charT *begin = b.eback();
      const charT *end = b.egptr();
      this->setbuf(const_cast<charT*>(begin), end - begin);
      this->setg(const_cast<charT*>(begin),
                 const_cast<charT*>(begin),
                 const_cast<charT*>(end));
    }

    template <typename charT, typename traits>
    basic_iastream<charT, traits>
    ::basic_iastream(const charT *begin, const charT *end)
      : iabuf<charT, traits>(begin, end),
        std::basic_istream<charT, traits>(this)
    {
    }

    template <typename charT, typename traits>
    basic_iastream<charT, traits>
    ::basic_iastream(const charT *begin)
      : iabuf<charT, traits>(begin, begin + wrapped::strlen(begin)),
        std::basic_istream<charT, traits>(this)
    {
    }

    template <typename charT, typename traits>
    basic_iastream<charT, traits>
    ::basic_iastream(const basic_iastream& i)
      : std::basic_ios<charT>(this),
        iabuf<charT, traits>(i),
        std::basic_istream<charT, traits>(this)
    {
    }

  }


  namespace policies {

    template <typename T> template <comm::type action, size_t N>
    void
    exception_specifier<void (T)>
    ::check_match(const char (&location)[N],
                  const char  *param_string,
                  const char  *pattern,
                  crpcut_none)
    {
      try {
        throw;
      }
      catch (T& t)
        {
          const char *what = t.what();
          if (wrapped::strcmp(what, pattern) != 0)
            {
              std::size_t old_size = heap::set_limit(heap::system);
              {
                comm::direct_reporter<action>(location)
                  << crpcut_check_name<action>::string()
                  << "_THROW("
                  << param_string
                  << ")\nwhat() == \""
                  << what
                  << "\" does not match string \""
                  << pattern
                  << "\"";
              }
              heap::set_limit(old_size);
            }
        }
    }

    template <typename T> template <comm::type action, size_t N, typename M>
    void
    exception_specifier<void (T)>
    ::check_match(const char (&location)[N],
                  const char  *param_string,
                  M            m,
                  crpcut_none)
    {
      try {
        throw;
      }
      catch (T& t)
        {
          if (!m(t))
            {
              std::size_t old_size = heap::set_limit(heap::system);
              {
                std::ostringstream os;
                show_value(os, t);
                comm::direct_reporter<action>(location)
                  << crpcut_check_name<action>::string()
                  << "_THROW("
                  << param_string
                  << ")\n"
                  << m << " does not match " << os.str();
              }
              heap::set_limit(old_size);
            }
        }
    }

    namespace deaths{


      template <int N, typename action>
      bool
      signal<N, action>::crpcut_is_expected_signal(int code) const
      {
        return N == ANY_CODE || code == N;
      }

      template <int N, typename action>
      void
      signal<N, action>::crpcut_expected_death(std::ostream &os) const
      {
        if (N == ANY_CODE)
          {
            os << "any signal";
          }
        else
          {
            os << "signal " << N;
          }
      }

      template <int N, typename action>
      bool
      exit<N, action>::crpcut_is_expected_exit(int code) const
      {
        return N == ANY_CODE || code == N;
      }

      template <int N, typename action>
      void
      exit<N, action>::crpcut_expected_death(std::ostream &os) const
      {
        if (N == ANY_CODE)
          {
            os << "exit with any code";
          }
        else
          {
            os << "exit with code " << N;
          }
      }


      template <unsigned long US>
      bool
      timeout<US>::crpcut_is_expected_signal(int code) const
      {
        return !timeouts_are_enabled() || code == SIGKILL || code == SIGXCPU;
      }

      template <unsigned long US>
      void
      timeout<US>::crpcut_expected_death(std::ostream &os) const
      {
        os << US / 1000 << "ms realtime timeout";
      }

      template <unsigned long US>
      unsigned long
      timeout<US>::crpcut_calc_deadline(unsigned long ts_us) const
      {
        return ts_us;
      }

    } // namespace deaths

    namespace dependencies {


      template <typename T>
      enforcer<T>::enforcer()
      {
        T::crpcut_reg().crpcut_add(this);
      }
    } // namespace dependencies

    namespace timeout {
      template <unsigned long timeout_us>
      enforcer<realtime, timeout_us>
      ::enforcer(const crpcut_test_monitor *current_test)
        : monotonic_enforcer(timeout_us, current_test)
      {
      }
    } // namespace timeout

  } // namespace policies


  namespace comm {

    template <size_t N>
    void
    reporter::operator()(type                        t,
                         const stream::toastream<N> &os,
                         datatypes::fixed_string     location,
                         const crpcut_test_monitor  *mon) const
    {
      const stream::oastream &os_(os);
      operator()(t, os_, location, mon);
    }

    template <size_t N>
    void
    reporter::operator()(type                       t,
                         const char               (&array)[N],
                         datatypes::fixed_string    location,
                         const crpcut_test_monitor *mon) const
    {
      operator()(t, array, N - 1, location, mon);
    }

    template <typename T>
    void
    reporter::operator()(comm::type                 t,
                         const T                   &data,
                         datatypes::fixed_string    location,
                         const crpcut_test_monitor *mon) const
    {
      const void *addr = &data;
      report(t, static_cast<const char*>(addr), sizeof(data), location, mon);
    }

    template <typename T>
    void
    reporter::read(T& t) const
    {
      assert(reader_);
      void *p = static_cast<void*>(&t);
      reader_->read_loop(p, sizeof(T));
    }

    template <comm::type type> template <size_t N>
    direct_reporter<type>
    ::direct_reporter(const char              (&location)[N],
                      reporter                  &r,
                      const crpcut_test_monitor *mon)
      : heap_limit(heap::set_limit(heap::system)),
        location_(datatypes::fixed_string::make(location)),
        report_(r),
        mon_(mon)
    {
    }

    template <comm::type type>
    direct_reporter<type>
    ::direct_reporter(datatypes::fixed_string    location,
                      reporter                  &r,
                      const crpcut_test_monitor *mon)
      : heap_limit(heap::set_limit(heap::system)),
        location_(location),
        report_(r),
        mon_(mon)
    {
    }

    template <comm::type t> template <typename V>
    direct_reporter<t>& direct_reporter<t>::operator<<(const V& v)
    {
      crpcut::show_value(os, v);
      return *this;
    }

    template <comm::type t> template <typename V>
    direct_reporter<t>& direct_reporter<t>::operator<<(V& v)
    {
      crpcut::show_value(os, v);
      return *this;
    }

    template <comm::type type>
    direct_reporter<type>::~direct_reporter()
    {
      using std::ostringstream;
      std::string s;
      os.str().swap(s);
      os.~ostringstream();
      new (&os) ostringstream(); // Just how ugly is this?
      size_t len = s.length();
      char *p = static_cast<char*>(alloca(len));
      s.copy(p, len);
      std::string().swap(s);
      report_(type, p, len, location_, mon_);
      heap::set_limit(heap_limit);
    }

    template <typename T>
    inline
    const data_writer &
    data_writer
    ::write_loop(const T* tp, size_t len, const char *context) const
    {
      const void *address = tp;
      return write_loop(address, len, context);
    }

  } // namespace comm

  template <comm::type t>
  inline
  reader<t>::reader(crpcut_test_monitor *r, int fd)
    : fdreader(r, fd)
  {
  }
  template <comm::type t>
  bool reader<t>::do_read_data()
  {
    static char buff[1024];
    for (;;)
      {
        ssize_t rv = rfile_descriptor::read(buff, sizeof(buff));
        if (rv == 0) return false;
        if(rv == -1)
          {
            assert(errno == EINTR);
            continue;
          }
        mon_->send_to_presentation(t, size_t(rv), buff);
        break;
      }
    return true;
  }

  template <typename T, case_convert_type type>
  collate_result operator==(T r, const collate_t<type> &c)
  {
    return (c == r).set_lh();
  }

  template <typename T, case_convert_type type>
  collate_result operator!=(T r, const collate_t<type> &c)
  {
    return (c != r).set_lh();
  }

  template <typename T, case_convert_type type>
  collate_result operator<(T r, const collate_t<type> &c)
  {
    return (c > r).set_lh();
  }

  template <typename T, case_convert_type type>
  collate_result operator<=(T r, const collate_t<type> &c)
  {
    return (c >= r).set_lh();
  }

  template <typename T, case_convert_type type>
  collate_result operator>(T r, const collate_t<type> &c)
  {
    return (c < r).set_lh();
  }

  template <typename T, case_convert_type type>
  collate_result operator>=(T r, const collate_t<type> &c)
  {
    return (c <= r).set_lh();
  }


#ifdef CRPCUT_SUPPORTS_VTEMPLATES
  template <typename D, typename ...T>
  typename match_traits<D, T...>::type
  match(T... t)
  {
    return typename match_traits<D, T...>::type(t...);
  }
#else
  template <typename D, typename T>
  typename match_traits<D, T>::type
  match(T t)
  {
    typedef match_traits<D, T> traits;
    typename traits::type rv(t);
    return rv;
  }

  template <typename D, typename T1, typename T2>
  typename match_traits<D, T1, T2>::type
  match(T1 t1, T2 t2)
  {
    typedef match_traits<D, T1, T2> traits;
    typename traits::type rv(t1, t2);
    return rv;
  }

  template <typename D,
            typename T1, typename T2, typename T3>
  typename match_traits<D, T1, T2, T3>::type
  match(T1 t1, T2 t2, T3 t3)
  {
    typedef match_traits<D, T1, T2, T3> traits;
    typename traits::type rv(t1, t2, t3);
    return rv;
  }

  template <typename D,
            typename T1, typename T2, typename T3,
            typename T4>
  typename match_traits<D, T1, T2, T3, T4>::type
  match(T1 t1, T2 t2, T3 t3, T4 t4)
  {
    typedef match_traits<D, T1, T2, T3, T4> traits;
    typename traits::type rv(t1, t2, t3, t4);
    return rv;
  }

  template <typename D,
            typename T1, typename T2, typename T3,
            typename T4, typename T5>
  typename match_traits<D, T1, T2, T3, T4, T5>::type
  match(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
  {
    typedef match_traits<D, T1, T2, T3, T4, T5> traits;
    typename traits::type rv(t1, t2, t3, t4, t5);
    return rv;
  }

  template <typename D,
            typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6>
  typename match_traits<D, T1, T2, T3, T4, T5, T6>::type
  match(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
  {
    typedef match_traits<D, T1, T2, T3, T4, T5, T6> traits;
    typename traits::type rv(t1, t2, t3, t4, t5, t6);
    return rv;
  }

  template <typename D,
            typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7>
  typename match_traits<D, T1, T2, T3, T4, T5, T6, T7>::type
  match(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7)
  {
    typedef match_traits<D, T1, T2, T3, T4, T5, T6, T7> traits;
    typename traits::type rv(t1, t2, t3, t4, t5, t6, t7);
    return rv;
  }

  template <typename D,
            typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8>
  typename match_traits<D, T1, T2, T3, T4, T5, T6, T7, T8>::type
  match(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8)
  {
    typedef match_traits<D, T1, T2, T3, T4, T5, T6, T7, T8> traits;
    typename traits::type rv(t1, t2, t3, t4, t5, t6, t7, t8);
    return rv;
  }

  template <typename D,
            typename T1, typename T2, typename T3,
            typename T4, typename T5, typename T6,
            typename T7, typename T8, typename T9>
  typename match_traits<D, T1, T2, T3, T4, T5, T6, T7, T8, T9>::type
  match(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9)
  {
    typedef match_traits<D, T1, T2, T3, T4, T5, T6, T7, T8, T9> traits;
    typename traits::type rv(t1, t2, t3, t4, t5, t6, t7, t8, t9);
    return rv;
  }
#endif
  class abs_diff
  {
  public:
    template <typename T>
    class type
    {
    public:
      type(T v) : t(v) {}
      template <typename U>
      bool operator()(U lh, U rh, T* = static_cast<U*>(0))
      {
        diff = lh-rh;
        if (diff < U()) diff = -diff;
        return diff <= t;
      }
      friend std::ostream& operator<<(std::ostream &os, const type<T>& obj)
      {
        os << "\n    Max allowed difference is "
           << std::setprecision(std::numeric_limits<T>::digits10)
           << obj.t
           << "\n    Actual difference is "
           << std::setprecision(std::numeric_limits<T>::digits10)
           << obj.diff;
        return os;
      }
    private:
      T t;
      T diff;
    };
  };

  template <typename T>
  struct match_traits<abs_diff, T>
  {
    typedef typename abs_diff::template type<T> type;
  };

  class relative_diff
  {
  public:
    template <typename T>
    class type
    {
    public:
      type(T v) : t(v) {}
      template <typename U>
      bool operator()(U lh, U rh, T* = static_cast<U*>(0))
      {
        U ldiff = lh - rh;
        if (ldiff < U()) ldiff = -ldiff;
        U lsum = lh + rh;
        if (lsum < U()) lsum = -lsum;
        diff = 2 * ldiff / lsum;
        return diff <= t;
      }
      friend std::ostream& operator<<(std::ostream &os, const type<T>& obj)
      {
        os << "\n    Max allowed relative difference is "
           << std::setprecision(std::numeric_limits<T>::digits10)
           << obj.t
           << "\n    Actual relative difference is "
           << obj.diff;
        return os;
      }
    private:
      T t;
      T diff;
    };
  };

  template <typename T>
  class long_double_not_supported {};

  template <>
  class long_double_not_supported<long double>;

  template <typename T, bool b = std::numeric_limits<T>::is_iec559>
  struct must_be_ieee754_fp_type : long_double_not_supported<T>
  {
    must_be_ieee754_fp_type(int) {}
  };

  template <typename T>
  struct must_be_ieee754_fp_type<T, false>;



  typedef enum { exclude_inf, include_inf } inf_in_ulps_diff;

  class ulps_diff
  {
  public:
    ulps_diff(unsigned max, inf_in_ulps_diff inf_val= exclude_inf)
      : max_diff(max),
        inf(inf_val),
        diff()
    {
    }
    template <typename T>
    bool operator()(T lh,
                    T rh,
                    const must_be_ieee754_fp_type<T> & = 0)
    {
      if (lh != lh) return false; // NaN
      if (rh != rh) return false; // NaN
      if (!inf && std::numeric_limits<T>::max() / lh == 0.0) return false;
      if (!inf && std::numeric_limits<T>::max() / rh == 0.0) return false;
      datatypes::fp<T> fl;
      datatypes::fp<T> fr;
      fl.data = lh;
      fr.data = rh;
      fl.rep = signbit2bias(fl.rep);
      fr.rep = signbit2bias(fr.rep);
      diff = (fl.rep > fr.rep ? fl.rep - fr.rep : fr.rep - fl.rep);
      return diff <= max_diff;
    }
    friend std::ostream& operator<<(std::ostream &os, const ulps_diff& d)
    {
      return os << "    Max allowed diff = " << d.max_diff
                << " ULPS\n    Actual diff = " << d.diff << " ULPS";
    }
  private:
    template <typename T>
    static T signbit2bias(T t)
    {
      static const T one = T() + 1;
      static const T neg_bit = one << (std::numeric_limits<T>::digits - 1);
      if (t & neg_bit)
        {
            return ~t + 1;
        }
      return t | neg_bit;
    }

    uint64_t         max_diff;
    inf_in_ulps_diff inf;
    uint64_t         diff;
  };
  template <typename T>
  struct match_traits<relative_diff, T>
  {
    typedef typename relative_diff::template type<T> type;
  };



  inline
  collate_t<verbatim>
  collate(const std::string &s, const std::locale& l = std::locale())
  {
    return collate_t<verbatim>(s, l);
  }

  template <case_convert_type type>
  collate_t<type>
  collate(const std::string &s, const std::locale &l = std::locale())
  {
    return collate_t<type>(s, l);
  }

  class test_suite_base : public policies::dependencies::basic_enforcer
  {
  protected:
    test_suite_base();
  public:
    void add_case(crpcut_test_case_registrator* r);
  private:
    crpcut_test_case_registrator *list;
  };

  template <typename T>
  class test_suite : public test_suite_base
  {
  public:
    test_suite() {}
    virtual ~test_suite() {}
    static test_suite& crpcut_reg()
    {
      static test_suite object;
      return object;
    }
    virtual void crpcut_add_action(policies::dependencies::basic_enforcer* e)
    {
      e->crpcut_inc(); // how to handle the case where this is empty?
    }
  private:
    virtual void crpcut_dec_action()
    {
      crpcut_register_success();
    }
  };


  template <typename T>
  static void get_parameter(const char *name, T& t)
  {
    const char *v = get_parameter(name);
    if (v)
      {
        stream::iastream is(v);
        if (is >> t)
          {
            return;
          }
      }
    std::ostringstream msg;
    msg << "\nParameter " << name << " with ";
    if (v)
      {
        msg << "value \"" << v << "\"";
      }
    else
      {
        msg << "no value";
      }
    msg << " cannot be interpreted as desired type";
    comm::report(comm::exit_fail, msg, crpcut_test_monitor::current_test()->get_location());
  }


  template <typename T>
  inline
  typename parameter_stream_traits<T>::type
  get_parameter(const char *name)
  {
    typedef parameter_stream_traits<T> rt;
    return rt::make_value(name, crpcut_test_monitor::current_test());
  }

  template <typename T>
  T parameter_stream_traits<T>::make_value(const char *n, crpcut_test_monitor*)
  {
    T rv;
    get_parameter(n, rv);
    return rv;
  }

  namespace expr {

    template <typename T>
    class predicate_proxy
    {
    public:
      typedef T &type;
      predicate_proxy(type p) : p_(p) {}
      type get_predicate() const { return const_cast<type>(p_); }
    private:
      type p_;
    };

    template <typename T>
    struct is_predicate_proxy
    {
      typedef char no;
      struct yes { char v[2]; };
      static no func(...);
      template <typename U>
      static yes func(predicate_proxy<U>*);
      static const bool value = sizeof(func((T*)0)) == sizeof(yes);
    };

    template <typename L, typename R>
    class predicate_match
    {
    public:
      predicate_match(const typename datatypes::undecorated<L>::type &l,
                      typename datatypes::undecorated<R>::type & r)
        : l_(l),
          r_(r) {}
      friend struct eval_t<predicate_match>;
      friend
      std::ostream &operator<<(std::ostream &os, const predicate_match &pm)
      {
        crpcut::show_value(os, pm.l_);
        os << " =~ ";
        crpcut::show_value(os, pm.r_);
        return os;
      }
    private:
      const typename datatypes::undecorated<L>::type &l_;
      typename datatypes::undecorated<R>::type &r_;
    };
  }

#define CRPCUT_BINOP(name, opexpr)                                      \
  namespace expr {                                                      \
    template <typename T, typename U>                                   \
    class name                                                          \
    {                                                                   \
    public:                                                             \
      name(T t, U u) : t_(t), u_(u) {}                                  \
      friend                                                            \
        std::ostream &operator<<(std::ostream &os, const name &a)       \
      {                                                                 \
        crpcut::show_value<8>(os, a.t_);                                \
        os << " " << #opexpr << " ";                                    \
        crpcut::show_value<8>(os, a.u_);                                \
        return os;                                                      \
      }                                                                 \
      friend struct eval_t<name>;                                       \
      template <typename V>                                             \
      typename enable_if<is_predicate_proxy<V>::value,                  \
                         predicate_match<name, typename V::type> >::type \
      operator=(const V& v)                                             \
      {                                                                 \
        typedef typename V::type predicate;                             \
        return predicate_match<name, predicate>(*this, v.get_predicate());  \
      }                                                                 \
    private:                                                            \
      T t_;                                                             \
      U u_;                                                             \
    };                                                                  \
  }

#define CRPCUT_OPFUNC(name, opexpr)                                     \
  namespace expr {                                                      \
    template <typename T, typename U>                                   \
    typename enable_if<is_struct<U>::value, name<const T&, const U&> >::type   \
    operator opexpr(const T &t, const U &u)                             \
    {                                                                   \
      return name<const T&, const U&>(t, u);                            \
    }                                                                   \
    template <typename T, typename U>                                   \
    typename enable_if<!is_struct<U>::value, name<const T&, U> >::type  \
    operator opexpr(const T &t, U u)                                    \
    {                                                                   \
      return name<const T&, U>(t, u);                                   \
    }                                                                   \
  }                                                                     \
  template <typename T, typename U>                                     \
  struct eval_t<expr::name<T, U> >                                      \
  {                                                                     \
    typedef typename eval_t<T>::type ttype;                             \
    typedef typename eval_t<U>::type utype;                             \
    typedef typename datatypes::undecorated<ttype>::type trtype;       \
    typedef typename datatypes::undecorated<utype>::type urtype;       \
    typedef CRPCUT_DECLTYPE(::crpcut::expr::gen<trtype>() opexpr ::crpcut::expr::gen<urtype>()) type; \
    static type func(const expr::name<T, U>& n)                         \
    {                                                                   \
      return ::crpcut::eval(n.t_) opexpr ::crpcut::eval(n.u_);          \
    }                                                                   \
  };                                                                    \
  template <typename T, typename U>                                     \
  struct eval_t<const expr::name<T, U>&> : public eval_t<expr::name<T, U> > {};

#define CRPCUT_OPS(x)                           \
  x(eq_op, ==)                                  \
  x(ne_op, !=)                                  \
  x(ls_op, <<)                                  \
  x(rs_op, >>)                                  \
  x(lt_op, <)                                   \
  x(le_op, <=)                                  \
  x(gt_op, >)                                   \
  x(ge_op, >=)                                  \
  x(add_op, +)                                  \
  x(sub_op, -)                                  \
  x(mul_op, *)                                  \
  x(div_op, /)                                  \
  x(mod_op, %)                                  \
  x(and_op, &)                                  \
  x(or_op, |)                                   \
  x(xor_op, ^)

  CRPCUT_OPS(CRPCUT_BINOP)
  CRPCUT_OPS(CRPCUT_OPFUNC)
#undef CRPCUT_OFPUNC
#undef CRPCUT_BINOP
#undef CRPCUT_OPS


    template <typename T>
    typename enable_if<is_predicate<T>::value, expr::predicate_proxy<T> >::type
    operator~(const T& t)
    {
      return expr::predicate_proxy<T>(const_cast<T&>(t));
    }
  namespace expr {

    template <typename T, typename U>
    struct use_two_assertions_instead  operator&&(const T&, const U&);

    template <typename T, typename U>
    struct refactor_this_expression  operator||(const T&, const U&);

    template <typename T>
    class atom
    {
    public:
      atom(T t) : t_(t) {}
      friend struct eval_t<atom>;
      friend
      std::ostream &operator<<(std::ostream &os, const atom& a)
      {
        crpcut::show_value<8>(os, a.t_);
        return os;
      }
      template <typename U>
      typename enable_if<is_predicate_proxy<U>::value,
                         predicate_match<T, typename U::type> >::type
      operator=(const U& u)
      {
        return predicate_match<T, typename U::type>(t_, u.get_predicate());
      }
    private:
      const T t_;
    };
  }

  template <typename T>
  struct eval_t<expr::atom<T> >
  {
    typedef typename datatypes::undecorated<T>::type naked_type;
    typedef typename if_else<is_struct<naked_type>::value, const naked_type&, naked_type>::type type;
    static type func(const expr::atom<T> &n) { return n.t_; }
  };

  template <typename T>
  struct eval_t<const expr::atom<T>&> : public eval_t<expr::atom<T> > {};

  template <typename T, typename U>
  struct eval_t<expr::predicate_match<T, U> >
  {
    static typename datatypes::undecorated<U>::type & u_;
    static typename datatypes::undecorated<T>::type & t_;
    typedef CRPCUT_DECLTYPE(u_(crpcut::eval(t_))) type;
    static type func(const expr::predicate_match<T, U> &n)
    {
      return n.r_(crpcut::eval(n.l_));
    }
  };
  namespace expr
  {
    class hook
    {
    public:
      template <case_convert_type type>
      const crpcut::collate_t<type>& operator->*(const crpcut::collate_t<type>& r) const
      {
        return r;
      }
      template <typename T>
      typename enable_if<is_struct<T>::value, atom<const T&> >::type
      operator->*(const T& t) const
      {
        return atom<const T&>(t);
      }
      template <typename T>
      typename enable_if<!is_struct<T>::value, atom<T> >::type
      operator->*(T t) const
      {
        return atom<T>(t);
      }
    };
  }

  template <typename T>
  typename eval_t<T>::type eval(const T& t) { return eval_t<T>::func(t); }

  namespace scope {
    class time_base
    {
    public:
      struct min
      {
        static const char *name();
        static bool busted(unsigned long now, unsigned long deadline);
      };
      struct max
      {
        static const char *name();
        static bool busted(unsigned long now, unsigned long deadline);
      };
      struct realtime
      {
        static const char *name();
        static unsigned long now();
      };
      struct cputime
      {
        static const char *name();
        static unsigned long now();
      };
      ~time_base() {};
      operator bool() const { return false; }
      void silence_warning() const {}
    protected:
      time_base(unsigned long deadline, datatypes::fixed_string location);
      unsigned long const deadline_;
      const datatypes::fixed_string location_;
    };
    template <comm::type action, typename cond, typename clock>
    class time : public time_base
    {
    public:
      template <size_t N>
      time(unsigned long us, const char (&location)[N],
           comm::reporter &reporter = comm::report,
           const crpcut_test_monitor *mon = crpcut_test_monitor::current_test())
        : time_base(clock::now() + us * crpcut::timeout_multiplier(),
                    datatypes::fixed_string::make(location)),
          limit_us_(us),
          reporter_(reporter),
          mon_(mon)
      {
      }
      ~time()
      {
        if (limit_us_ + 1 > 0) // wrap around to 0 signals disabled
          {
            unsigned long t = clock::now();
            if (timeouts_are_enabled() && cond::busted(t, deadline_))
              {
                unsigned long duration_ms = (t - deadline_ + limit_us_) / 1000;
                comm::direct_reporter<action>(location_, reporter_, mon_)
                  << crpcut_check_name<action>::string()
                  << "_SCOPE_" << cond::name()
                  << "_" << clock::name() << "_MS(" << limit_us_ / 1000 << ")"
                  "\nActual time used was " << duration_ms << "ms";
              }
          }
      }
      time(const time& r)
        : time_base(r),
          limit_us_(r.limit_us_),
          reporter_(r.reporter_),
          mon_(r.mon_)
      {
        static const unsigned long zero(0UL);
        r.limit_us_ = ~zero;
      }
    private:
      time& operator=(const time&);

      unsigned long mutable      limit_us_;
      comm::reporter            &reporter_;
      const crpcut_test_monitor *mon_;
    };
  }

} // namespace crpcut

extern crpcut::namespace_info crpcut_current_namespace;

// Note, the order of inheritance below is important. crpcut_test_case_base
// destructor signals ending of test case, so it must be listed as the
// first base class so that its instance is destroyed last

#define CRPCUT_TEST_CASE_DEF(test_case_name, ...)                       \
  class test_case_name                                                  \
    : crpcut::crpcut_test_case_base, __VA_ARGS__                        \
  {                                                                     \
    friend struct crpcut::test_wrapper<crpcut_run_wrapper>;              \
    friend class crpcut::policies::dependencies::enforcer<test_case_name>; \
    friend class crpcut::crpcut_test_case_registrator;                  \
    typedef test_case_name crpcut_test_class;                           \
    test_case_name() {}                                                 \
    virtual void crpcut_run_test()                                      \
    {                                                                   \
      crpcut_realtime_enforcer rt(&crpcut_get_reg());                   \
      using crpcut::policies::timeout::cputime_enforcer;                \
      cputime_enforcer ct(crpcut_cputime_enforcer::crpcut_cputime_timeout_us,\
                          &crpcut_get_reg());                            \
      (void)rt; /* silence warning */                                   \
      (void)ct; /* silence warning */                                   \
      crpcut::test_wrapper<crpcut_run_wrapper>::run(this, crpcut::comm::report); \
      if (crpcut::tests_as_child_processes())                           \
        {                                                               \
          crpcut_test_finished(); /* tell destructor to report success */ \
        }                                                               \
    }                                                                   \
    void test();                                                        \
    class crpcut_registrator                                            \
      : public crpcut::crpcut_test_case_registrator,                    \
        private virtual crpcut::policies::dependencies::crpcut_base,    \
        public virtual test_case_name::crpcut_expected_death_cause,     \
        private virtual test_case_name::crpcut_dependency,              \
        public virtual crpcut_testsuite_dep,                            \
        public test_case_name::crpcut_constructor_timeout_enforcer,     \
        public test_case_name::crpcut_destructor_timeout_enforcer,      \
        public virtual test_case_name::crpcut_core_dump_handler         \
    {                                                                   \
      using crpcut_core_dump_handler::crpcut_core_dumps_allowed;        \
      crpcut::report_reader                report_reader_;              \
      crpcut::reader<crpcut::comm::stdout> stdout_reader_;              \
      crpcut::reader<crpcut::comm::stderr> stderr_reader_;              \
                                                                        \
      typedef crpcut::crpcut_test_case_registrator                      \
        crpcut_registrator_base;                                        \
      static const unsigned long crpcut_cputime_timeout_us              \
        =test_case_name::crpcut_cputime_enforcer::crpcut_cputime_timeout_us; \
      void setup(crpcut::poll<crpcut::fdreader> &poller,                \
                 int                             in_fd,                 \
                 int                             stdout_fd,             \
                 int                             stderr_fd)             \
      {                                                                 \
        stdout_reader_.set_fd(stdout_fd, &poller);                      \
        stderr_reader_.set_fd(stderr_fd, &poller);                      \
        report_reader_.set_fd(in_fd, &poller);                          \
      }                                                                 \
    public:                                                             \
       crpcut_registrator()                                             \
         : crpcut_registrator_base(#test_case_name,                     \
                                   crpcut::datatypes::fixed_string::make(CRPCUT_HERE), \
                                   crpcut_current_namespace,            \
                                   crpcut_cputime_timeout_us),          \
           report_reader_(this),                                        \
           stdout_reader_(this),                                        \
           stderr_reader_(this)                                         \
         {                                                              \
           crpcut::test_suite<crpcut_testsuite_id>::crpcut_reg().add_case(this); \
           crpcut::crpcut_tag_info<crpcut_test_tag>::obj();             \
         }                                                              \
       virtual void run_test_case()                                     \
       {                                                                \
         CRPCUT_DEFINE_REPORTER;                                        \
         prepare_construction(crpcut_constructor_timeout_us);           \
         crpcut_test_class obj;                                         \
         manage_test_case_execution(&obj);                              \
         prepare_destruction(crpcut_destructor_timeout_us);             \
       }                                                                \
       virtual crpcut::tag& crpcut_tag() const                          \
       {                                                                \
         return crpcut::crpcut_tag_info<crpcut_test_tag>::obj();        \
       }                                                                \
       virtual crpcut::tag::importance get_importance() const;          \
    };                                                                  \
    static crpcut_registrator &crpcut_reg()                             \
    {                                                                   \
      static crpcut_registrator obj;                                    \
      return obj;                                                       \
    }                                                                   \
    virtual crpcut::crpcut_test_case_registrator&                       \
      crpcut_get_reg() const                                            \
    {                                                                   \
      return crpcut_reg();                                              \
    }                                                                   \
    class crpcut_trigger                                                \
    {                                                                   \
    public:                                                             \
      crpcut_trigger() { crpcut_reg(); }                                \
    };                                                                  \
    static crpcut_trigger crpcut_trigger_obj;                           \
  };                                                                    \
  test_case_name::crpcut_trigger test_case_name::crpcut_trigger_obj


#define TEST_DEF(test_case_name, ...)                                   \
  CRPCUT_TEST_CASE_DEF(test_case_name, __VA_ARGS__);                    \
  crpcut::tag::importance                                               \
  test_case_name::crpcut_registrator::get_importance() const            \
  {                                                                     \
    return crpcut_tag().get_importance();                               \
  }                                                                     \
  void test_case_name::test()

#define DISABLED_TEST_DEF(test_case_name, ...)                          \
  CRPCUT_TEST_CASE_DEF(test_case_name, __VA_ARGS__);                    \
  crpcut::tag::importance                                               \
  test_case_name::crpcut_registrator::get_importance() const            \
  {                                                                     \
    return crpcut::tag::disabled;                                       \
  }                                                                     \
  void test_case_name::test()

#define CRPCUT_ARG_COUNT_( _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
                          _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
                          N, ...) N
#define CRPCUT_ARG_COUNT(...) CRPCUT_ARG_COUNT_(__VA_ARGS__,    \
                                                20,19,18,17,16, \
                                                15,14,13,12,11, \
                                                10, 9, 8, 7, 6, \
                                                 5, 4, 3, 2, 1, 0)

#define CRPCUT_APPLY(pred, ...) pred(__VA_ARGS__)

#define CRPCUT_JOIN(a, b) a ## b
#define CRPCUT_CONCAT1(a) a
#define CRPCUT_CONCAT2(a, b) a ## b
#define CRPCUT_CONCAT3(a, b, ...) \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT4(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT5(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT6(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT7(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT8(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT9(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT10(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT11(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT12(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT13(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT14(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT15(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT16(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT17(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT18(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT19(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)
#define CRPCUT_CONCAT20(a, b, ...)                                        \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(a ## b, __VA_ARGS__))(a ## b, __VA_ARGS__)


#define CRPCUT_CONCAT(...) \
  CRPCUT_APPLY(CRPCUT_JOIN, CRPCUT_CONCAT, CRPCUT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

#define CRPCUT_LOCAL_NAME(prefix) \
  CRPCUT_CONCAT(crpcut_local_, prefix, __LINE__)


#ifndef CRPCUT_EXPERIMENTAL_CXX0X
#define CRPCUT_REFTYPE(expr) \
  const CRPCUT_DECLTYPE(expr) &
#else
namespace crpcut {
  namespace datatypes {
    template <typename T>
    const volatile typename undecorated<T>::type &gettype();

  }
}
#define CRPCUT_REFTYPE(expr) \
  CRPCUT_DECLTYPE(crpcut::datatypes::gettype<CRPCUT_DECLTYPE(expr)>())
#endif

#define NO_CORE_FILE \
  protected virtual crpcut::policies::no_core_file

#define WIPE_WORKING_DIR \
  crpcut::policies::deaths::wipe_working_dir

#define EXPECT_EXIT(...) \
  protected virtual crpcut::policies::exit_death<__VA_ARGS__>

#define EXPECT_REALTIME_TIMEOUT_MS(time) \
  protected virtual crpcut::policies::realtime_timeout_death<(time)*1000UL>

#define EXPECT_SIGNAL_DEATH(...) \
  protected virtual crpcut::policies::signal_death<__VA_ARGS__>


#ifndef CRPCUT_NO_EXCEPTION_SUPPORT
#define EXPECT_EXCEPTION(type) \
  protected virtual crpcut::policies::exception_specifier<void (type)>
#define CRPCUT_DECLARE_EXCEPTION_TRANSLATOR(class_name, t)              \
  class class_name                                                      \
    : public crpcut::policies::crpcut_exception_translator              \
  {                                                                     \
  public:                                                               \
    class_name(crpcut::policies::crpcut_exception_translator &r =       \
               crpcut::policies::crpcut_exception_translator::root_object()) \
      : crpcut::policies::crpcut_exception_translator(r) {}             \
  private:                                                              \
    std::string crpcut_translate() const                                \
    {                                                                   \
      return crpcut_catcher(crpcut_do_translate);                       \
    }                                                                   \
    static std::string crpcut_do_translate(t);                          \
    template <typename U>                                               \
      static std::string crpcut_catcher(std::string (U))                \
    {                                                                   \
      try {                                                             \
        throw;                                                          \
      }                                                                 \
      catch(typename std::remove_reference<U>::type &u)                 \
        {                                                               \
          return crpcut_do_translate(u);                                \
        }                                                               \
    }                                                                   \
  }

#define CRPCUT_DESCRIBE_EXCEPTION(t)                                    \
  CRPCUT_DECLARE_EXCEPTION_TRANSLATOR(CRPCUT_LOCAL_NAME(specific_exception_translator), t); \
  namespace {                                                           \
    CRPCUT_LOCAL_NAME(specific_exception_translator) CRPCUT_LOCAL_NAME(exception_translator_obj); \
  }                                                                     \
  std::string CRPCUT_LOCAL_NAME(specific_exception_translator)::crpcut_do_translate(t)

#define CRPCUT_DEFINE_EXCEPTION_TRANSLATOR_CLASS(class_name, t) \
  CRPCUT_DECLARE_EXCEPTION_TRANSLATOR(class_name, t);           \
  std::string class_name::crpcut_do_translate(t)

#endif

#define DEPENDS_ON(...) \
  crpcut::policies::dependency_policy<crpcut::datatypes::tlist_maker<__VA_ARGS__>::type >



#define DEADLINE_CPU_MS(time) \
  crpcut::policies::timeout_policy<crpcut::policies::timeout::cputime, (time)*1000UL>

#define FIXTURE_CONSTRUCTION_DEADLINE_REALTIME_MS(time) \
  public crpcut::policies::constructor_timeout_policy<(time)*1000UL>
#define FIXTURE_DESTRUCTION_DEADLINE_REALTIME_MS(time) \
  public crpcut::policies::destructor_timeout_policy<(time)*1000UL>

#define DEADLINE_REALTIME_MS(time) \
  crpcut::policies::timeout_policy<crpcut::policies::timeout::realtime, time*1000UL>

#define CRPCUT_WRAP_FUNC(lib, name, rv, param_list, param)              \
  extern "C" typedef rv (*f_ ## name ## _t) param_list;                 \
  rv name param_list                                                    \
  {                                                                     \
    static f_ ## name ## _t f_ ## name                                  \
      = ::crpcut::libwrapper::loader< ::crpcut::libs::lib>::obj().sym<f_ ## name ## _t>(#name); \
    return f_ ## name param;                                            \
  }


#define CRPCUT_WRAP_V_FUNC(lib, name, rv, param_list, param)            \
  extern "C" typedef rv (*f_ ## name ## _t) param_list;                 \
  rv name param_list                                                    \
  {                                                                     \
    static f_ ## name ## _t f_ ## name                                  \
      = ::crpcut::libwrapper::loader< ::crpcut::libs::lib>::obj().sym<f_ ## name ## _t>(#name); \
    f_ ## name param;                                                   \
  }


#define CRPCUT_IS_ZERO_LIT(x) (sizeof(crpcut::null_cmp::func(x)) == 1)

#define CRPCUT_BINARY_CHECK(action, name, lh, rh)                       \
  do {                                                                  \
    try {                                                               \
      crpcut::tester                                                    \
        <crpcut::comm::action,                                          \
         CRPCUT_IS_ZERO_LIT(lh), CRPCUT_DECLTYPE(lh),                   \
         CRPCUT_IS_ZERO_LIT(rh), CRPCUT_DECLTYPE(rh)>                   \
        (CRPCUT_HERE, #name)                                            \
        .name(lh, #lh, rh, #rh);                                        \
    }                                                                   \
    CATCH_BLOCK(..., {                                                  \
      using crpcut::policies::report_unexpected_exception;              \
      report_unexpected_exception(crpcut::comm::action,                 \
                                  CRPCUT_HERE,                          \
                                  crpcut::crpcut_check_name<crpcut::comm::action>::string(), \
                                  #name,                                \
                                  #lh ", " #rh);                        \
    })                                                                  \
  } while(0)





#ifdef __CDT_PARSER__
#define CRPCUT_CHECK_FALSE(action, a)                   \
  do {                                                  \
    crpcut::bool_tester<crpcut::comm::action>           \
      (CRPCUT_HERE)                                     \
      .check_false((a), #a);                            \
  } while (0)

#define CRPCUT_CHECK_TRUE(action, a)                    \
  do {                                                  \
    crpcut::bool_tester<crpcut::comm::action>           \
      (CRPCUT_HERE)                                     \
      .check_true((a), #a);                             \
  } while (0)


#else
#define CRPCUT_CHECK_TRUE(action, a)                                    \
  do {                                                                  \
    try {                                                               \
      crpcut::bool_tester<crpcut::comm::action>                         \
        (CRPCUT_HERE)                                                   \
        .check_true((crpcut::expr::hook()->*a), #a);                    \
    }                                                                   \
    CATCH_BLOCK(..., {                                                  \
        using crpcut::policies::report_unexpected_exception;            \
        report_unexpected_exception(crpcut::comm::action,               \
                                     CRPCUT_HERE,                       \
                                     crpcut::crpcut_check_name<crpcut::comm::action>::string(), \
                                     "TRUE",                            \
                                     #a);                               \
      })                                                                \
  } while(0)

#define CRPCUT_CHECK_FALSE(action, a)                                   \
  do {                                                                  \
    try {                                                               \
      crpcut::bool_tester<crpcut::comm::action>                         \
        (CRPCUT_HERE)                                                   \
        .check_false((crpcut::expr::hook()->*a), #a);                   \
    }                                                                   \
    CATCH_BLOCK(..., {                                                  \
        using crpcut::policies::report_unexpected_exception;            \
        report_unexpected_exception(crpcut::comm::action,               \
                                    CRPCUT_HERE,                        \
                                    crpcut::crpcut_check_name<crpcut::comm::action>::string(), \
                                    "FALSE",                    \
                                    #a);                        \
      })                                                                \
  } while(0)
#endif // __CDT_PARSER__
#define ASSERT_TRUE(a) CRPCUT_CHECK_TRUE(exit_fail, a)
#define VERIFY_TRUE(a) CRPCUT_CHECK_TRUE(fail, a)

#define ASSERT_FALSE(a) CRPCUT_CHECK_FALSE(exit_fail, a)
#define VERIFY_FALSE(a) CRPCUT_CHECK_FALSE(fail, a)

#define ASSERT_EQ(lh, rh)  CRPCUT_BINARY_CHECK(exit_fail, EQ, lh, rh)

#define ASSERT_NE(lh, rh)  CRPCUT_BINARY_CHECK(exit_fail, NE, lh, rh)

#define ASSERT_GE(lh, rh)  CRPCUT_BINARY_CHECK(exit_fail, GE, lh, rh)

#define ASSERT_GT(lh, rh)  CRPCUT_BINARY_CHECK(exit_fail, GT, lh, rh)

#define ASSERT_LT(lh, rh)  CRPCUT_BINARY_CHECK(exit_fail, LT, lh, rh)

#define ASSERT_LE(lh, rh)  CRPCUT_BINARY_CHECK(exit_fail, LE, lh, rh)

#define VERIFY_EQ(lh, rh)  CRPCUT_BINARY_CHECK(fail, EQ, lh, rh)

#define VERIFY_NE(lh, rh)  CRPCUT_BINARY_CHECK(fail, NE, lh, rh)

#define VERIFY_GE(lh, rh)  CRPCUT_BINARY_CHECK(fail, GE, lh, rh)

#define VERIFY_GT(lh, rh)  CRPCUT_BINARY_CHECK(fail, GT, lh, rh)

#define VERIFY_LT(lh, rh)  CRPCUT_BINARY_CHECK(fail, LT, lh, rh)

#define VERIFY_LE(lh, rh)  CRPCUT_BINARY_CHECK(fail, LE, lh, rh)

namespace crpcut
{
  template <typename Exception>
  void assert_exception_match(Exception, crpcut_none)
  {
  }
}

#ifndef CRPCUT_NO_EXCEPTION_SUPPORT
#define CRPCUT_CHECK_THROW(action, str, expr, exc, ...)                 \
  do {                                                                  \
    try {                                                               \
      try {                                                             \
        expr;                                                           \
        CRPCUT_CHECK_REPORT_HEAD(action) <<                             \
          "_THROW(" #expr ", " #exc ")\n"                               \
          "  Did not throw";                                            \
      }                                                                 \
      catch (exc) {                                                     \
        crpcut::policies::exception_specifier<void(exc)>                \
          ::check_match<crpcut::comm::action>(CRPCUT_HERE,              \
                                              str,                      \
                                              __VA_ARGS__);             \
        break;                                                          \
      }                                                                 \
    }                                                                   \
    CATCH_BLOCK(..., {                                                  \
        using crpcut::policies::report_unexpected_exception;            \
        report_unexpected_exception(crpcut::comm::action,               \
                                    CRPCUT_HERE,                        \
                                    crpcut::crpcut_check_name<crpcut::comm::action>::string(), \
                                    "THROW",                            \
                                    str);                               \
      })                                                                \
  } while (0)

#define ASSERT_THROW(expr, ...)                 \
  CRPCUT_CHECK_THROW(exit_fail, CRPCUT_STRINGIZE(expr, __VA_ARGS__), expr, __VA_ARGS__, crpcut::crpcut_none())

#define VERIFY_THROW(expr, ...)                                         \
  CRPCUT_CHECK_THROW(fail,  CRPCUT_STRINGIZE(expr, __VA_ARGS__), expr, __VA_ARGS__, crpcut::crpcut_none())
#endif

#ifndef CRPCUT_NO_EXCEPTION_SUPPORT
#define CRPCUT_CHECK_NO_THROW(action, expr)                             \
  do {                                                                  \
    try {                                                               \
      expr;                                                             \
    }                                                                   \
    CATCH_BLOCK(..., {                                                  \
        using crpcut::policies::report_unexpected_exception;            \
        report_unexpected_exception(crpcut::comm::action,               \
                                    CRPCUT_HERE,                        \
                                    crpcut::crpcut_check_name<crpcut::comm::action>::string(), \
                                    "NO_THROW",                         \
                                    #expr);                             \
      })                                                                \
  } while (0)

#define ASSERT_NO_THROW(expr)                                           \
  CRPCUT_CHECK_NO_THROW(exit_fail, expr)

#define VERIFY_NO_THROW(expr)                                           \
  CRPCUT_CHECK_NO_THROW(fail, expr)
#endif


#define CRPCUT_CHECK_REPORT_HEAD(action)                                \
  crpcut::comm::direct_reporter<crpcut::comm::action>(CRPCUT_HERE)      \
    << crpcut::crpcut_check_name<crpcut::comm::action>::string()        \

#define CRPCUT_CHECK_PRED(action, pred, ...)                            \
  do {                                                                  \
    static const char CRPCUT_LOCAL_NAME(sep)[][3] = { ", ", "" };       \
    try {                                                               \
      std::string CRPCUT_LOCAL_NAME(m);                                 \
      if (!crpcut::match_pred(CRPCUT_LOCAL_NAME(m),                     \
                              #pred,                                    \
                              pred,                                     \
                              crpcut::params(__VA_ARGS__)))             \
        {                                                               \
          size_t CRPCUT_LOCAL_NAME(len) = CRPCUT_LOCAL_NAME(m).length(); \
          char *CRPCUT_LOCAL_NAME(p)                                    \
            = static_cast<char*>(alloca(CRPCUT_LOCAL_NAME(len)+1));     \
          CRPCUT_LOCAL_NAME(m).copy(CRPCUT_LOCAL_NAME(p),               \
                                    CRPCUT_LOCAL_NAME(len));            \
          CRPCUT_LOCAL_NAME(p)[CRPCUT_LOCAL_NAME(len)]=0;               \
          std::string().swap(CRPCUT_LOCAL_NAME(m));                     \
          CRPCUT_CHECK_REPORT_HEAD(action)                              \
            << "_PRED(" #pred                                           \
            << CRPCUT_LOCAL_NAME(sep)[!*#__VA_ARGS__]                   \
            << #__VA_ARGS__ ")\n"                                       \
            << CRPCUT_LOCAL_NAME(p);                                    \
        }                                                               \
    }                                                                   \
    CATCH_BLOCK(..., {                                                  \
        using crpcut::policies::report_unexpected_exception;            \
        report_unexpected_exception(crpcut::comm::action,               \
                                    CRPCUT_HERE,                        \
                                    crpcut::crpcut_check_name<crpcut::comm::action>::string(), \
                                    "PRED",                             \
                                    CRPCUT_STRINGIZE(pred, __VA_ARGS__)); \
      })                                                                \
  } while (0)

#define ASSERT_PRED(pred, ...)                                          \
  CRPCUT_CHECK_PRED(exit_fail, pred, __VA_ARGS__)

#define VERIFY_PRED(pred, ...)                                          \
  CRPCUT_CHECK_PRED(fail, pred, __VA_ARGS__)

#define CRPCUT_CHECK_SCOPE_TYPE_TIME_MS(action, type, clock, ms)        \
  if (const crpcut::scope::time_base& CRPCUT_LOCAL_NAME(time_scope)     \
      = crpcut::scope::time<crpcut::comm::action,                       \
                            crpcut::scope::time_base::type,             \
                            crpcut::scope::time_base::clock>((ms)*1000UL*crpcut::timeout_multiplier(), \
                                                             CRPCUT_HERE)) \
    { CRPCUT_LOCAL_NAME(time_scope).silence_warning(); } else           \

#define ASSERT_SCOPE_MAX_REALTIME_MS(ms)        \
  CRPCUT_CHECK_SCOPE_TYPE_TIME_MS(exit_fail, max, realtime, ms)

#define ASSERT_SCOPE_MIN_REALTIME_MS(ms)        \
  CRPCUT_CHECK_SCOPE_TYPE_TIME_MS(exit_fail, min, realtime, ms)

#define ASSERT_SCOPE_MAX_CPUTIME_MS(ms)         \
  CRPCUT_CHECK_SCOPE_TYPE_TIME_MS(exit_fail, max, cputime, ms)

#define VERIFY_SCOPE_MAX_REALTIME_MS(ms)        \
  CRPCUT_CHECK_SCOPE_TYPE_TIME_MS(fail, max, realtime, ms)

#define VERIFY_SCOPE_MIN_REALTIME_MS(ms)        \
  CRPCUT_CHECK_SCOPE_TYPE_TIME_MS(fail, min, realtime, ms)

#define VERIFY_SCOPE_MAX_CPUTIME_MS(ms)         \
  CRPCUT_CHECK_SCOPE_TYPE_TIME_MS(fail, max, cputime, ms)

#define CRPCUT_CHECK_SCOPE_HEAP_LEAK_FREE(type)                         \
  if (const crpcut::heap::local_root & CRPCUT_LOCAL_NAME(leak_free_scope) \
      = crpcut::heap::local_root(crpcut::comm::type, CRPCUT_HERE))      \
    {                                                                   \
      CRPCUT_LOCAL_NAME(leak_free_scope).nonsense_func();               \
    }                                                                   \
  else

#define ASSERT_SCOPE_HEAP_LEAK_FREE \
  CRPCUT_CHECK_SCOPE_HEAP_LEAK_FREE(exit_fail)

#define VERIFY_SCOPE_HEAP_LEAK_FREE \
  CRPCUT_CHECK_SCOPE_HEAP_LEAK_FREE(fail)

class crpcut_testsuite_id;
class crpcut_testsuite_dep
  :
  public virtual crpcut::policies::dependencies::crpcut_base
{
};

#define TEST(...) TEST_DEF(__VA_ARGS__, crpcut::crpcut_none)
#define DISABLED_TEST(...) DISABLED_TEST_DEF(__VA_ARGS__, crpcut::crpcut_none)

#define TESTSUITE_DEF(name, ...)                                        \
  namespace name {                                                      \
    typedef crpcut_testsuite_dep crpcut_parent_testsuite_dep;           \
    namespace {                                                         \
      class crpcut_testsuite_dep                                        \
        : public virtual crpcut_parent_testsuite_dep,                   \
          public virtual crpcut::policies::dependencies::nested<__VA_ARGS__>::type \
      {                                                                 \
      };                                                                \
      static crpcut::namespace_info *parent_namespace                   \
      = &crpcut_current_namespace;                                      \
    }                                                                   \
    class crpcut_testsuite_id;                                          \
    static crpcut::namespace_info                                       \
    crpcut_current_namespace(#name, parent_namespace);                  \
  }                                                                     \
  namespace name

#define ALL_TESTS(suite_name)                                           \
  crpcut::test_suite<suite_name :: crpcut_testsuite_id >

#define TESTSUITE(...) TESTSUITE_DEF(__VA_ARGS__, crpcut::crpcut_none)

#define INFO crpcut::comm::direct_reporter<crpcut::comm::info>(CRPCUT_HERE)
#define FAIL crpcut::comm::direct_reporter<crpcut::comm::exit_fail>(CRPCUT_HERE)


#define CRPCUT_REPEAT_1(M, ...)                                   M( 0, __VA_ARGS__)
#define CRPCUT_REPEAT_2(M, ...)  CRPCUT_REPEAT_1(M, __VA_ARGS__)  M( 1, __VA_ARGS__)
#define CRPCUT_REPEAT_3(M, ...)  CRPCUT_REPEAT_2(M, __VA_ARGS__)  M( 2, __VA_ARGS__)
#define CRPCUT_REPEAT_4(M, ...)  CRPCUT_REPEAT_3(M, __VA_ARGS__)  M( 3, __VA_ARGS__)
#define CRPCUT_REPEAT_5(M, ...)  CRPCUT_REPEAT_4(M, __VA_ARGS__)  M( 4, __VA_ARGS__)
#define CRPCUT_REPEAT_6(M, ...)  CRPCUT_REPEAT_5(M, __VA_ARGS__)  M( 5, __VA_ARGS__)
#define CRPCUT_REPEAT_7(M, ...)  CRPCUT_REPEAT_6(M, __VA_ARGS__)  M( 6, __VA_ARGS__)
#define CRPCUT_REPEAT_8(M, ...)  CRPCUT_REPEAT_7(M, __VA_ARGS__)  M( 7, __VA_ARGS__)
#define CRPCUT_REPEAT_9(M, ...)  CRPCUT_REPEAT_8(M, __VA_ARGS__)  M( 8, __VA_ARGS__)
#define CRPCUT_REPEAT_10(M, ...) CRPCUT_REPEAT_9(M, __VA_ARGS__)  M( 9, __VA_ARGS__)
#define CRPCUT_REPEAT_11(M, ...) CRPCUT_REPEAT_10(M, __VA_ARGS__) M(10, __VA_ARGS__)
#define CRPCUT_REPEAT_12(M, ...) CRPCUT_REPEAT_11(M, __VA_ARGS__) M(11, __VA_ARGS__)
#define CRPCUT_REPEAT_13(M, ...) CRPCUT_REPEAT_12(M, __VA_ARGS__) M(12, __VA_ARGS__)
#define CRPCUT_REPEAT_14(M, ...) CRPCUT_REPEAT_13(M, __VA_ARGS__) M(13, __VA_ARGS__)
#define CRPCUT_REPEAT_15(M, ...) CRPCUT_REPEAT_14(M, __VA_ARGS__) M(14, __VA_ARGS__)
#define CRPCUT_REPEAT_16(M, ...) CRPCUT_REPEAT_15(M, __VA_ARGS__) M(15, __VA_ARGS__)
#define CRPCUT_REPEAT_17(M, ...) CRPCUT_REPEAT_16(M, __VA_ARGS__) M(16, __VA_ARGS__)
#define CRPCUT_REPEAT_18(M, ...) CRPCUT_REPEAT_17(M, __VA_ARGS__) M(17, __VA_ARGS__)
#define CRPCUT_REPEAT_19(M, ...) CRPCUT_REPEAT_18(M, __VA_ARGS__) M(18, __VA_ARGS__)
#define CRPCUT_REPEAT_20(M, ...) CRPCUT_REPEAT_19(M, __VA_ARGS__) M(19, __VA_ARGS__)
#define CRPCUT_REPEAT_21(M, ...) CRPCUT_REPEAT_20(M, __VA_ARGS__) M(20, __VA_ARGS__)
#define CRPCUT_REPEAT_22(M, ...) CRPCUT_REPEAT_21(M, __VA_ARGS__) M(21, __VA_ARGS__)
#define CRPCUT_REPEAT_23(M, ...) CRPCUT_REPEAT_22(M, __VA_ARGS__) M(22, __VA_ARGS__)
#define CRPCUT_REPEAT_24(M, ...) CRPCUT_REPEAT_23(M, __VA_ARGS__) M(23, __VA_ARGS__)
#define CRPCUT_REPEAT_25(M, ...) CRPCUT_REPEAT_24(M, __VA_ARGS__) M(24, __VA_ARGS__)
#define CRPCUT_REPEAT_26(M, ...) CRPCUT_REPEAT_25(M, __VA_ARGS__) M(25, __VA_ARGS__)
#define CRPCUT_REPEAT_27(M, ...) CRPCUT_REPEAT_26(M, __VA_ARGS__) M(26, __VA_ARGS__)
#define CRPCUT_REPEAT_28(M, ...) CRPCUT_REPEAT_27(M, __VA_ARGS__) M(27, __VA_ARGS__)
#define CRPCUT_REPEAT_29(M, ...) CRPCUT_REPEAT_28(M, __VA_ARGS__) M(28, __VA_ARGS__)
#define CRPCUT_REPEAT_30(M, ...) CRPCUT_REPEAT_29(M, __VA_ARGS__) M(29, __VA_ARGS__)
#define CRPCUT_REPEAT_31(M, ...) CRPCUT_REPEAT_30(M, __VA_ARGS__) M(30, __VA_ARGS__)
#define CRPCUT_REPEAT_32(M, ...) CRPCUT_REPEAT_31(M, __VA_ARGS__) M(31, __VA_ARGS__)
#define CRPCUT_REPEAT_33(M, ...) CRPCUT_REPEAT_32(M, __VA_ARGS__) M(32, __VA_ARGS__)
#define CRPCUT_REPEAT_34(M, ...) CRPCUT_REPEAT_33(M, __VA_ARGS__) M(33, __VA_ARGS__)
#define CRPCUT_REPEAT_35(M, ...) CRPCUT_REPEAT_34(M, __VA_ARGS__) M(34, __VA_ARGS__)
#define CRPCUT_REPEAT_36(M, ...) CRPCUT_REPEAT_35(M, __VA_ARGS__) M(35, __VA_ARGS__)
#define CRPCUT_REPEAT_37(M, ...) CRPCUT_REPEAT_36(M, __VA_ARGS__) M(36, __VA_ARGS__)
#define CRPCUT_REPEAT_38(M, ...) CRPCUT_REPEAT_37(M, __VA_ARGS__) M(37, __VA_ARGS__)
#define CRPCUT_REPEAT_39(M, ...) CRPCUT_REPEAT_38(M, __VA_ARGS__) M(38, __VA_ARGS__)
#define CRPCUT_REPEAT_40(M, ...) CRPCUT_REPEAT_39(M, __VA_ARGS__) M(39, __VA_ARGS__)
#define CRPCUT_REPEAT_41(M, ...) CRPCUT_REPEAT_40(M, __VA_ARGS__) M(40, __VA_ARGS__)
#define CRPCUT_REPEAT_42(M, ...) CRPCUT_REPEAT_41(M, __VA_ARGS__) M(41, __VA_ARGS__)
#define CRPCUT_REPEAT_43(M, ...) CRPCUT_REPEAT_42(M, __VA_ARGS__) M(42, __VA_ARGS__)
#define CRPCUT_REPEAT_44(M, ...) CRPCUT_REPEAT_43(M, __VA_ARGS__) M(43, __VA_ARGS__)
#define CRPCUT_REPEAT_45(M, ...) CRPCUT_REPEAT_44(M, __VA_ARGS__) M(44, __VA_ARGS__)
#define CRPCUT_REPEAT_46(M, ...) CRPCUT_REPEAT_45(M, __VA_ARGS__) M(45, __VA_ARGS__)
#define CRPCUT_REPEAT_47(M, ...) CRPCUT_REPEAT_46(M, __VA_ARGS__) M(46, __VA_ARGS__)
#define CRPCUT_REPEAT_48(M, ...) CRPCUT_REPEAT_47(M, __VA_ARGS__) M(47, __VA_ARGS__)
#define CRPCUT_REPEAT_49(M, ...) CRPCUT_REPEAT_48(M, __VA_ARGS__) M(48, __VA_ARGS__)
#define CRPCUT_REPEAT_50(M, ...) CRPCUT_REPEAT_49(M, __VA_ARGS__) M(49, __VA_ARGS__)
#define CRPCUT_REPEAT_51(M, ...) CRPCUT_REPEAT_50(M, __VA_ARGS__) M(50, __VA_ARGS__)
#define CRPCUT_REPEAT_52(M, ...) CRPCUT_REPEAT_51(M, __VA_ARGS__) M(51, __VA_ARGS__)
#define CRPCUT_REPEAT_53(M, ...) CRPCUT_REPEAT_52(M, __VA_ARGS__) M(52, __VA_ARGS__)
#define CRPCUT_REPEAT_54(M, ...) CRPCUT_REPEAT_53(M, __VA_ARGS__) M(53, __VA_ARGS__)
#define CRPCUT_REPEAT_55(M, ...) CRPCUT_REPEAT_54(M, __VA_ARGS__) M(54, __VA_ARGS__)
#define CRPCUT_REPEAT_56(M, ...) CRPCUT_REPEAT_55(M, __VA_ARGS__) M(55, __VA_ARGS__)
#define CRPCUT_REPEAT_57(M, ...) CRPCUT_REPEAT_56(M, __VA_ARGS__) M(56, __VA_ARGS__)
#define CRPCUT_REPEAT_58(M, ...) CRPCUT_REPEAT_57(M, __VA_ARGS__) M(57, __VA_ARGS__)
#define CRPCUT_REPEAT_59(M, ...) CRPCUT_REPEAT_58(M, __VA_ARGS__) M(58, __VA_ARGS__)
#define CRPCUT_REPEAT_60(M, ...) CRPCUT_REPEAT_59(M, __VA_ARGS__) M(59, __VA_ARGS__)
#define CRPCUT_REPEAT_61(M, ...) CRPCUT_REPEAT_60(M, __VA_ARGS__) M(60, __VA_ARGS__)
#define CRPCUT_REPEAT_62(M, ...) CRPCUT_REPEAT_61(M, __VA_ARGS__) M(61, __VA_ARGS__)
#define CRPCUT_REPEAT_63(M, ...) CRPCUT_REPEAT_62(M, __VA_ARGS__) M(62, __VA_ARGS__)
#define CRPCUT_REPEAT_64(M, ...) CRPCUT_REPEAT_63(M, __VA_ARGS__) M(63, __VA_ARGS__)
#define CRPCUT_REPEAT_65(M, ...) CRPCUT_REPEAT_64(M, __VA_ARGS__) M(64, __VA_ARGS__)
#define CRPCUT_REPEAT_66(M, ...) CRPCUT_REPEAT_65(M, __VA_ARGS__) M(65, __VA_ARGS__)
#define CRPCUT_REPEAT_67(M, ...) CRPCUT_REPEAT_66(M, __VA_ARGS__) M(66, __VA_ARGS__)
#define CRPCUT_REPEAT_68(M, ...) CRPCUT_REPEAT_67(M, __VA_ARGS__) M(67, __VA_ARGS__)
#define CRPCUT_REPEAT_69(M, ...) CRPCUT_REPEAT_68(M, __VA_ARGS__) M(68, __VA_ARGS__)
#define CRPCUT_REPEAT_70(M, ...) CRPCUT_REPEAT_69(M, __VA_ARGS__) M(69, __VA_ARGS__)
#define CRPCUT_REPEAT_71(M, ...) CRPCUT_REPEAT_70(M, __VA_ARGS__) M(70, __VA_ARGS__)
#define CRPCUT_REPEAT_72(M, ...) CRPCUT_REPEAT_71(M, __VA_ARGS__) M(71, __VA_ARGS__)
#define CRPCUT_REPEAT_73(M, ...) CRPCUT_REPEAT_72(M, __VA_ARGS__) M(72, __VA_ARGS__)
#define CRPCUT_REPEAT_74(M, ...) CRPCUT_REPEAT_73(M, __VA_ARGS__) M(73, __VA_ARGS__)
#define CRPCUT_REPEAT_75(M, ...) CRPCUT_REPEAT_74(M, __VA_ARGS__) M(74, __VA_ARGS__)
#define CRPCUT_REPEAT_76(M, ...) CRPCUT_REPEAT_75(M, __VA_ARGS__) M(75, __VA_ARGS__)
#define CRPCUT_REPEAT_77(M, ...) CRPCUT_REPEAT_76(M, __VA_ARGS__) M(76, __VA_ARGS__)
#define CRPCUT_REPEAT_78(M, ...) CRPCUT_REPEAT_77(M, __VA_ARGS__) M(77, __VA_ARGS__)
#define CRPCUT_REPEAT_79(M, ...) CRPCUT_REPEAT_78(M, __VA_ARGS__) M(78, __VA_ARGS__)
#define CRPCUT_REPEAT_80(M, ...) CRPCUT_REPEAT_79(M, __VA_ARGS__) M(79, __VA_ARGS__)

#if defined(CRPCUT_SUPPORTS_VTEMPLATES) && defined(CRPCUT_SUPPORTS_CONSTEXPR)

#  define WITH_TEST_TAG(...)                                         \
  crpcut::policies::tag_policy<crpcut::datatypes::string_type<>      \
                               CRPCUT_REPEAT_80(CRPCUT_APPEND_INDEX, \
                                                CRPCUT_STRINGIZE(__VA_ARGS__)) >

#  define DEFINE_TEST_TAG(...) class crpcut_DEFINE_TEST_TAG_is_deprecated

#else

#  define WITH_TEST_TAG(tag_name)                         \
    crpcut::policies::tag_policy<crpcut::crpcut_tags::tag_name>

#  define DEFINE_TEST_TAG(tag_name)                               \
    namespace crpcut {                                            \
      namespace crpcut_tags {                                     \
        struct tag_name;                                          \
      }                                                           \
      template <>                                                 \
      inline                                                      \
      size_t                                                      \
      crpcut_tag_info<crpcut::crpcut_tags::tag_name>              \
      ::get_name_len() const                                      \
      {                                                           \
        return sizeof(#tag_name) - 1U;                            \
      }                                                           \
      template <>                                                 \
      inline                                                      \
      crpcut::datatypes::fixed_string                             \
      crpcut_tag_info<crpcut::crpcut_tags::tag_name>              \
      ::get_name() const                                          \
      {                                                           \
        using crpcut::datatypes::fixed_string;                    \
        fixed_string s = { #tag_name, get_name_len() };           \
        return s;                                                 \
      }                                                           \
    }                                                             \
    using crpcut::crpcut_tags::tag_name

#endif
#ifdef GMOCK_INCLUDE_GMOCK_GMOCK_H_



#endif
#endif // CRPCUT_HPP

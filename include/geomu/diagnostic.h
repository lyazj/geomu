/*
 * A simple and flexible logger for diagnostic.
 *
 * Written by <lyazj@github.com>.
 *
 * Usage:
 *
 *   ddbg << ... << dend  // output to clog
 *   difo << ... << dend  // output to clog
 *   dwrn << ... << dend  // output to cerr
 *   derr << ... << dend  // output to cerr
 *   dftl << ... << dend  // output to cerr and exit
 *
 * Logging level adjustment should precede including this header, like:
 *
 *   #define DIAG_LVL  DIAG_DBG
 *   #include "diagnostic.h"
 *
 * To avoid interleaving log lines from different threads, build with macro
 * DIAG_SERIALIZE defined in commane line (used in diagnostic.cpp).
 */
#pragma once

#include "ECMA48_SGR.h"
#include <utility>

#define DIAG_MIN  0
#define DIAG_DBG  0
#define DIAG_IFO  1
#define DIAG_WRN  2
#define DIAG_ERR  3
#define DIAG_FTL  4
#define DIAG_MAX  4

#ifndef DIAG_LVL
#define DIAG_LVL  DIAG_IFO
#endif  /* DIAG_LVL */

#if DIAG_LVL < DIAG_MIN || DIAG_LVL > DIAG_MAX
#error "DIAG_LVL out of range"
#endif  /* DIAG_LVL */

struct LoggingTerminator { };

/*
 * Here we implement a two state automata.
 *
 *   Status 0: idle
 *   Status 1: activated
 *   An output operation (<<) triggers state transition 0 -> 1 or 1 -> 1.
 *   A logging terminator (dend) triggers state transition (0 ->) 1 -> 0.
 *
 * A preceding message containing diagnostic information and logging color
 * modulation are implemented by state transition side effects.
 */
class DiagnosticLogger {

public:
  DiagnosticLogger(int level, ECMA48_SGR color, std::ostream &os, const char *file, size_t line, const char *func)
    : level_(level), color_(color), os_(os), file_(file), line_(line), func_(func) { }

protected:
  /*
   * Two states are independent to each other if they are owned by different
   * threads, or if they are for different logging levels. Otherwise, the value
   * is unique for all instances, which makes sence especially for:
   *
   *   difo << "numbers:";  // Preceding message only produced here.
   *   for(int i = 0; i < nnumber; ++i) difo << " " << numbers[i];
   *   difo << dend;  // No linebreak until here.
   */
  static thread_local int state_[DIAG_MAX + 1];

  int level_;         // Level of this logging request.
  ECMA48_SGR color_;  // Color of preceding messages.
  std::ostream &os_;  // Underlying output stream.
  const char *file_;
  size_t line_;
  const char *func_;

  // Automata implementation.
  void on_output();
  void on_terminate();

};

namespace {

/*
 * Here we define a unique logger for each compilation unit, so that each source
 * file can define its own logging level.
 */
class UniqueDiagnosticLogger : private DiagnosticLogger {

public:
  using DiagnosticLogger::DiagnosticLogger;

  bool enabled() const { return level_ >= DIAG_LVL; }

  /*
   * Delegate all output requests to the underlying output stream.
   */
  template<class T>
  UniqueDiagnosticLogger &operator<<(T &&t) {
    if(enabled()) {
      on_output();
      os_ << std::forward<T>(t);
    }
    return *this;
  }

  /*
   * This wraps std::endl to make necessary state transitions.
   */
  UniqueDiagnosticLogger &operator<<(LoggingTerminator) {
    if(enabled()) {
      on_terminate();
    }
    return *this;
  }

};

}  // namespace

#define ddbg  (UniqueDiagnosticLogger(DIAG_DBG, ECMA48_SGR::FG_BCYN, std::clog, __FILE__, __LINE__, __PRETTY_FUNCTION__))
#define difo  (UniqueDiagnosticLogger(DIAG_IFO, ECMA48_SGR::FG_BGRN, std::clog, __FILE__, __LINE__, __PRETTY_FUNCTION__))
#define dwrn  (UniqueDiagnosticLogger(DIAG_WRN, ECMA48_SGR::FG_BBRN, std::cerr, __FILE__, __LINE__, __PRETTY_FUNCTION__))
#define derr  (UniqueDiagnosticLogger(DIAG_ERR, ECMA48_SGR::FG_BRED, std::cerr, __FILE__, __LINE__, __PRETTY_FUNCTION__))
#define dftl  (UniqueDiagnosticLogger(DIAG_FTL, ECMA48_SGR::FG_BMGT, std::cerr, __FILE__, __LINE__, __PRETTY_FUNCTION__))
#define dend  (LoggingTerminator())  // Additional () also guarantees disambiguating as an expression.

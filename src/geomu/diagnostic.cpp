#include "geomu/diagnostic.h"
#include <stdlib.h>
#include <unistd.h>

thread_local int DiagnosticLogger::state_[DIAG_MAX + 1];
static bool stderr_to_tty = isatty(STDERR_FILENO);

#ifdef DIAG_SERIALIZE
#include <mutex>
static std::mutex mutex;
#endif  /* DIAG_SERIALIZE */

void DiagnosticLogger::on_output()
{
  if(state_[level_] == 0) {
#ifdef DIAG_SERIALIZE
    mutex.lock();
#endif  /* DIAG_SERIALIZE */
    if(stderr_to_tty) os_ << color_;
    os_ << file_ << ":" << line_ << ": " << func_ << " [" << level_["DIWEF"] << "]: ";
    if(stderr_to_tty) os_ << ECMA48_SGR::FG_BWHT;
    state_[level_] = 1;
  }
}

void DiagnosticLogger::on_terminate()
{
  if(state_[level_] == 0) on_output();  // Avoid empty line.
  if(stderr_to_tty) os_ << ECMA48_SGR::FG_DFT;
  os_ << std::endl;
  state_[level_] = 0;
  if(level_ == DIAG_FTL) exit(EXIT_FAILURE);
#ifdef DIAG_SERIALIZE
  mutex.unlock();
#endif  /* DIAG_SERIALIZE */
}

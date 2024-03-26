#pragma once
#include <iostream>

enum class ECMA48_SGR {  // Set Graphics Rendition.
  // General control sequences.
  RST = 0,  // reset
  BLD = 1,  // bold
  DIM = 2,  // dim
  ITL = 3,  // italic
  UDS = 4,  // underscore
  BLK = 5,  // blink
  RVS = 7,  // reverse
  HID = 8,  // hidden
  SRK = 9,  // strike
  DUDS = 21,  // double underscore
  NDIM = 22,  // not dim
  NITL = 23,  // not italic
  NUDS = 24,  // no underscore
  NBLK = 25,  // no blink
  NRVS = 27,  // no reverse
  NHID = 28,  // not hidden
  NSRK = 29,  // no strike

  // Frontground colors.
  FG_BLK = 30,
  FG_RED = 31,
  FG_GRN = 32,
  FG_BRN = 33,
  FG_BLU = 34,
  FG_MGT = 35,
  FG_CYN = 36,
  FG_WHT = 37,
  FG_DFT = 39,

  // Background colors.
  BG_BLK = 40,
  BG_RED = 41,
  BG_GRN = 42,
  BG_BRN = 43,
  BG_BLU = 44,
  BG_MGT = 45,
  BG_CYN = 46,
  BG_WHT = 47,
  BG_DFT = 49,

  // Bright frontground colors.
  FG_BBLK = 90,
  FG_BRED = 91,
  FG_BGRN = 92,
  FG_BBRN = 93,
  FG_BBLU = 94,
  FG_BMGT = 95,
  FG_BCYN = 96,
  FG_BWHT = 97,

  // Bright background colors.
  BG_BBLK = 100,
  BG_BRED = 101,
  BG_BGRN = 102,
  BG_BBRN = 103,
  BG_BBLU = 104,
  BG_BMGT = 105,
  BG_BCYN = 106,
  BG_BWHT = 107,
};

inline std::ostream &operator<<(std::ostream &os, ECMA48_SGR sgr)
{
  return os << "\033[" << (unsigned)sgr << "m";
}

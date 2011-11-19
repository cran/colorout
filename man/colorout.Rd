\name{ColorOut}
\alias{ColorOut}
\title{Colorize R output in terminal emulator}
\description{
  Colorize output of R running in a terminal emulator.
}
\usage{
ColorOut()
}

\value{
  NULL.
}

\details{
  There is no support for Graphical User Interfaces, such as Windows RGui,
  Emacs+ESS, RStudio etc. The functions of this package only work if R is
  compiled for Unix systems and it is running interactively in an terminal
  emulator. The terminal must support ANSI escape codes. The package contains
  a C library with a function that replaces the default Rstd_WriteConsoleEx
  which, when enabled, is responsible for printing messages in the Console
  when R is running in interactive mode.
}

\author{Jakson A. Aquino \email{jalvesaq@gmail.com}}


\name{setOutputColors}
\alias{setOutputColors}
\title{Set the colors to be used on R output}
\description{
  Set the output colors to be used R is running in a terminal emulator.
}
\usage{
setOutputColors(normal = c(0, 32), number = c(0, 33), string = c(0, 36),
                const = c(0, 35), stderror = c(0, 34), warn = c(1, 31),
                error = c(41, 37))
}
\arguments{
  \item{normal}{Formating and color of normal text.}
  \item{number}{Formating and color of numbers.}
  \item{string}{Formating and color of quoted text.}
  \item{const}{Formating and color of \code{TRUE}, \code{FALSE}, \code{NULL},
    \code{NA}, \code{NaN} and \code{Inf}.}
  \item{stderror}{Formating and color of text sent to stderr.}
  \item{warn}{Formating and color of warnings.}
  \item{error}{Formating and color of errors.}
}
\details{
  The function takes four numeric vectors with two elements each. The first one
  indicates the formating and the second one the color. Strings are colored as
  errors and warnings if they start with \code{"Error"} or \code{"Warning"}
  (or their translations, if not running in an English locale).
 
  Valid values for either formating or background color are (some codes do not
  work properly in some terminal emulators):
 
  \tabular{lrclr}{
    Formating         \tab Value \tab Background \tab Value \cr
    No formating      \tab 0     \tab Black      \tab 40 \cr
    Bold or bright    \tab 1     \tab Red        \tab 41 \cr
    Faint             \tab 2     \tab Green      \tab 42 \cr
    Italic or inverse \tab 3     \tab Yellow     \tab 43 \cr
    Underline         \tab 4     \tab Blue       \tab 44 \cr
    Blink slowly      \tab 5     \tab Magenta    \tab 45 \cr
    Blink quickly     \tab 6     \tab Cyan       \tab 46 \cr
    Invert            \tab 7     \tab White      \tab 47 \cr
  }

  Valid color values are: 

  \tabular{lr}{
    Color   \tab Value \cr
    Black   \tab 30 \cr
    Red     \tab 31 \cr
    Green   \tab 32 \cr
    Yellow  \tab 33 \cr
    Blue    \tab 34 \cr
    Magenta \tab 35 \cr
    Cyan    \tab 36 \cr
    White   \tab 37 \cr
  }
}
\value{
  NULL.
}

\author{Jakson A. Aquino \email{jalvesaq@gmail.com}}


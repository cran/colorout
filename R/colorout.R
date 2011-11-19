
.onLoad <- function(libname, pkgname) {
    library.dynam("colorout", pkgname, libname, local = FALSE);
    termenv <- Sys.getenv("TERM")
    if(termenv == "dumb" || termenv == "")
        return(invisible(NULL))
    else
        ColorOut()
}

.onUnload <- function(libpath) {
    noColorOut()
    library.dynam.unload("colorout", libpath)
}


ColorOut <- function()
{
    termenv <- Sys.getenv("TERM")
    if(termenv == "dumb" || termenv == ""){
        msg <- sprintf(gettext("It seems that your terminal does not support ANSI escape codes.\nSys.getenv('TERM') returned '%s'.",
                               domain = "R-colorout"), termenv)
        stop(msg, call. = FALSE)
    }
    .C("colorout_ColorOutput", PACKAGE="colorout")
    return (invisible(NULL))
}

noColorOut <- function()
{
    termenv <- Sys.getenv("TERM")
    if(termenv == "dumb" || termenv == ""){
        msg <- sprintf(gettext("It seems that your terminal does not support ANSI escape codes.\nSys.getenv('TERM') returned '%s'.",
                               domain = "R-colorout"), termenv)
        stop(msg, call. = FALSE)
    }
    .C("colorout_noColorOutput", PACKAGE="colorout")
    return (invisible(NULL))
}

setOutputColors <- function(normal = c(0, 32),
                            number = c(0, 33),
                            string = c(0, 36),
                            const = c(0, 35),
                            stderror = c(0, 34),
                            warn = c(1, 31),
                            error = c(41, 37))
{
    if(!(is.numeric(normal) && is.numeric(number) &&
         is.numeric(string) && is.numeric(const) &&
         is.numeric(stderror) && is.numeric(warn) && is.numeric(error)))
        stop(gettext("All values must be numbers corresponding to ANSI escape code.",
                     domain = "R-colorout"))

    normal[normal > 99] <- 99
    normal[normal < 0] <- 0
    number[number > 99] <- 99
    number[number < 0] <- 0
    string[string > 99] <- 99
    string[string < 0] <- 0
    const[const > 99] <- 99
    const[const < 0] <- 0
    stderror[stderror > 99] <- 99
    stderror[stderror < 0] <- 0
    warn[warn > 99] <- 99
    warn[warn < 0] <- 0
    error[error > 99] <- 99
    error[error < 0] <- 0

    .C("colorout_SetColors",
       as.integer(normal), as.integer(number), as.integer(string),
       as.integer(const), as.integer(stderror), as.integer(warn),
       as.integer(error), PACKAGE="colorout")
    return (invisible(NULL))
}

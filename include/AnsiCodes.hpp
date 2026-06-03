#pragma once 
#if !defined(DISABLE_STYLE)
namespace fmt{
const static char* bold                             = "\e[1m";
const static char* underline                        = "\e[4m";
const static char* underbold                        = "\e[1;4m";

const static char* clear                            = "\e[0m";
const static char* italic                           = "\e[4m";
const static char* rev                              = "\e[7m";
const static char* bold_red                         = "\e[1;31m";
const static char* bold_green                       = "\e[1;32m";
const static char* bold_blue                        = "\e[1;33m";
const static char* bold_yellow                      = "\e[1;34m";
const static char* bg_white_fg_black                = "\e[47;30m";

const static char* ligrey                           = "\e[90m";
const static char* cyan                             = "\e[34m";
const static char* green                            = "\e[32m";
const static char* blue                             = "\e[36m";
const static char* orange                           = "\e[38;5;208m";
const static char* grey                             = "\e[90";
const static char* yellow                           = "\e[33m";
const static char* purple                           = "\e[35m";
const static char* iyellow                          = "\e[7;33m";
const static char* pink                             = "\e[35m";
const static char* lired                            = "\e[91m";
const static char* red                              = "\e[31m";
const static char* dwhite                           = "\e[96m";
const static char* bold_dwhite                      = "\e[1;96m";
}
#else 
namespace fmt{
const static char* bold                             = "";
const static char* underline                        = "";
const static char* underbold                        = "";

const static char* clear                            = "";
const static char* italic                           = "";
const static char* rev                              = "";
const static char* bold_red                         = "";
const static char* bold_green                       = "";
const static char* bold_blue                        = "";
const static char* bold_yellow                      = "";
const static char* bg_white_fg_black                = "";

const static char* ligrey                           = "";
const static char* cyan                             = "";
const static char* green                            = "";
const static char* blue                             = "";
const static char* orange                           = "";
const static char* grey                             = "";
const static char* yellow                           = "";
const static char* purple                           = "";
const static char* iyellow                          = "";
const static char* pink                             = "";
const static char* lired                            = "";
const static char* red                              = "";
const static char* dwhite                           = "";
const static char* bold_dwhite                      = "";
}
#endif

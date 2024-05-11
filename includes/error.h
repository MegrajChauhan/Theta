#ifndef _ERROR_
#define _ERROR_

typedef enum STATE STATE;

enum STATE
{
    _NORMAL_,
    _INTERNAL_ERROR_, // generally allocation error 

    // preprocessing error,
    IS_A_DIRECTORY,
    FILE_DOESNT_EXIST,
    EMPTY_FILE, // not really an error[The compiler will just terminate any furthur steps]
};

#endif
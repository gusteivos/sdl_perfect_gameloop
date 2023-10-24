
#ifndef CNANOSLEEP_H_

    #include <stdio.h>
    #include <math.h>

    #ifdef _WIN32
    
        #include <windows.h>
        #include <libloaderapi.h>

    #endif

    #include <time.h>


    #ifdef _WIN32
    
        extern HMODULE Kernel32_dll;

        extern HANDLE (*CreateWaitableTimerExW_dll)(LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD);


        extern HANDLE (*create_cnanosleep_timer   )(LPSECURITY_ATTRIBUTES, LPCWSTR);
    
    #endif


    extern int init_cnanosleep(void);


    extern int cnanosleep(const struct timespec *request, struct timespec *remaining);


    extern int quit_cnanosleep(void);

#endif

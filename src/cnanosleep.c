#include <cnanosleep.h>


#ifdef _WIN32


    HMODULE Kernel32_dll = INVALID_HANDLE_VALUE;

    HANDLE (*CreateWaitableTimerExW_dll)(LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD) = NULL;


    HANDLE (*create_cnanosleep_timer   )(LPSECURITY_ATTRIBUTES, LPCWSTR)               = NULL;

#endif


static HANDLE create_cnanosleep_hight_timer(LPSECURITY_ATTRIBUTES lpTimerAttributes, LPCWSTR lpTimerName)
{

    /*

        https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-createwaitabletimerexw


        CREATE_WAITABLE_TIMER_MANUAL_RESET    0x00000001

        CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002


        https://learn.microsoft.com/en-us/windows/win32/sync/synchronization-object-security-and-access-rights


        TIMER_ALL_ACCESS 0x1F0003

    */


    return CreateWaitableTimerExW_dll(lpTimerAttributes, lpTimerName, 0x00000001 | 0x00000002, 0x1F0003);

}

static HANDLE create_cnanosleep_norma_timer(LPSECURITY_ATTRIBUTES lpTimerAttributes, LPCWSTR lpTimerName)
{

    /*

        https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-createwaitabletimerw


        This timer uses common precision and TRUE to not reset itself.

    */


    return CreateWaitableTimerW(lpTimerAttributes, TRUE, lpTimerName);

}


int init_cnanosleep()
{

#ifdef _WIN32

    Kernel32_dll = LoadLibraryA((LPCSTR)"Kernel32.dll");


    if (!Kernel32_dll || Kernel32_dll == INVALID_HANDLE_VALUE)
    {

        fprintf(stderr, "Error loading Kernel32.dll\n");

        return 1;

    }


    CreateWaitableTimerExW_dll = (HANDLE (*)(LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD))GetProcAddress(Kernel32_dll, "CreateWaitableTimerExW");

    if (!CreateWaitableTimerExW_dll)
    {

        create_cnanosleep_timer = create_cnanosleep_norma_timer;

    }
    else
    {

        create_cnanosleep_timer = create_cnanosleep_hight_timer;

    }


#endif


    return 0;

}

/*Temp: */

    static double map_value(double value, double fromMin, double fromMax, double toMin, double toMax)
    {
    
        value = fmax(fmin(value, fromMax), fromMin);


        return toMin + (value - fromMin) * (toMax - toMin) / (fromMax - fromMin);
    
    }

int cnanosleep(const struct timespec *request, struct timespec *remaining)
{

#ifdef _WIN32

    HANDLE cnanosleep_timer = create_cnanosleep_timer(NULL, NULL);

    if (!cnanosleep_timer || cnanosleep_timer == INVALID_HANDLE_VALUE)
    {

        return 1;

    }


    LARGE_INTEGER time_required;

    time_required.QuadPart  = -(request->tv_sec * 10000000);

    time_required.QuadPart += -(long)map_value(request->tv_nsec, 0, 1000000000, 0, 10000000);


    if (!SetWaitableTimer(cnanosleep_timer, &time_required, 0, NULL, NULL, FALSE))
    {

        CloseHandle(cnanosleep_timer);

        return 1;

    }


    DWORD wait_retrun = WaitForSingleObject(cnanosleep_timer, INFINITE);


	CloseHandle(cnanosleep_timer);

#else

    switch (nanosleep(request, remaining))
    {

    case 0: return 0;


    case EINTR:
        break;

    case EINVAL:

        fprintf(stderr, "tv_nsec - not in range or tv_sec is negative\n");

        break;


    default:

        perror("nanosleep in cnanosleep");

        break;

    }

#endif


    return 0;

}


int quit_cnanosleep()
{

#ifdef _WIN32

    if (Kernel32_dll && Kernel32_dll != INVALID_HANDLE_VALUE)
    {

        if(FreeLibrary(Kernel32_dll))
        {

            return 1;

        }

        CreateWaitableTimerExW_dll = NULL;

    }

#endif


    return 0;

}

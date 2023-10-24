#include <main.h>


SDL_Window *main_window = NULL;


int main(int argc, char *argv[])
{

    init_cnanosleep();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {

        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());


        return 1;

    }


    if ((main_window = SDL_CreateWindow(MAIN_WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 854, 480, SDL_WINDOW_SHOWN)) == NULL)
    {

        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());

        SDL_Quit();


        return 1;

    }


    bool is_running = true;


    Uint64 freq = SDL_GetPerformanceFrequency();

    Uint64 lastTime = SDL_GetPerformanceCounter();

    float deltaTime = 0.0f;

    


    SDL_Event main_window_event;

    while (is_running)
    {

        struct timespec remaining, request = { 0, 1000000 };

                                                //1458136

        while (SDL_PollEvent(&main_window_event))
        {

            if (main_window_event.type == SDL_QUIT)
            {

                is_running = false;

            }

        }



        UINT64 currentTime = SDL_GetPerformanceCounter();
        deltaTime = (float)(currentTime - lastTime) / freq;
        lastTime = currentTime;
        
        printf("delta time: %.9f\n", deltaTime);

        float teste = (1.0f / 60.0f);

        teste -= deltaTime;
        
        if (teste > 0)
        {

            request.tv_nsec = (long)(((float)request.tv_nsec) * (999 * teste));

            // printf("delay time: %.9f\n", teste);
            // printf("delay value %ld\n", request.tv_nsec);

            // SDL_Delay(14);

            cnanosleep(&request, NULL);

        }

    }


exit:

    quit_cnanosleep();

    SDL_DestroyWindow(main_window);

    SDL_Quit();


    return 0;

}


#if defined(_WIN32) && !defined(NDEBUG)

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    UNREFERENCED_PARAMETER(hPrevInstance);

    return main(__argc, __argv);

}

#endif

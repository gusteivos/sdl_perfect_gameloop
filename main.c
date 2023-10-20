#include <main.h>


SDL_Window *main_window = NULL;


int main(int argc, char *argv[])
{

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


    /*Temp: */    

        bool is_running = true;


        SDL_Event main_window_event;

        while (is_running)
        {

            while (SDL_PollEvent(&main_window_event))
            {

                if (main_window_event.type == SDL_QUIT)
                {

                    is_running = false;

                }

            }
            

            SDL_Delay(1);

        }
        
        SDL_DestroyWindow(main_window);


exit:

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

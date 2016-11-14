#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

bool init();

bool loadMedia();

void close();

SDL_Surface *loadSurface(std::string path);

SDL_Texture *loadTexture(std::string path);

SDL_Window *gWindow = NULL;

SDL_Renderer *gRenderer = NULL;

SDL_Texture *gTexture = NULL;

SDL_Surface *gScreenSurface = NULL;

SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

SDL_Surface *gCurrentSurface = NULL;

SDL_Surface *gStretchedSurface = NULL;

SDL_Surface *gPNGSurface = NULL;

int main(int argc, char *args[])
{
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            bool quit = false;

            SDL_Event e;

            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    // else if (e.type == SDL_KEYDOWN)
                    // {
                    //     switch (e.key.keysym.sym)
                    //     {
                    //     case SDLK_UP:
                    //         gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                    //         break;

                    //     case SDLK_DOWN:
                    //         gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                    //         break;

                    //     case SDLK_LEFT:
                    //         gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                    //         break;

                    //     case SDLK_RIGHT:
                    //         gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                    //         break;

                    //     default:
                    //         gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                    //         break;
                    //     }
                    // }
                }

                // SDL_Rect stretchRect;
                // stretchRect.x = 0;
                // stretchRect.y = 0;
                // stretchRect.w = SCREEN_WIDTH;
                // stretchRect.h = SCREEN_HEIGHT;
                // SDL_BlitScaled(gStretchedSurface, NULL, gScreenSurface, &stretchRect);

                // SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

                // SDL_BlitSurface(gPNGSurface, NULL, gScreenSurface, NULL);

                // SDL_UpdateWindowSurface(gWindow);

                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // SDL_Rect fillRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
                // SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
                // SDL_RenderFillRect(gRenderer, &fillRect);

                // SDL_Rect outlineRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
                // SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
                // SDL_RenderDrawRect(gRenderer, &outlineRect);

                // SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
                // SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

                // //Draw vertical line of yellow dots
                // SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                // for (int i = 0; i < SCREEN_HEIGHT; i += 4)
                // {
                //     SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i);
                // }

                SDL_Rect topLeftViewport;
                topLeftViewport.x = 0;
                topLeftViewport.y = 0;
                topLeftViewport.w = SCREEN_WIDTH / 2;
                topLeftViewport.h = SCREEN_HEIGHT / 2;
                SDL_RenderSetViewport(gRenderer, &topLeftViewport);
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
                SDL_Rect topRightViewport;
                topRightViewport.x = SCREEN_WIDTH / 2;
                topRightViewport.y = 0;
                topRightViewport.w = SCREEN_WIDTH / 2;
                topRightViewport.h = SCREEN_HEIGHT / 2;
                SDL_RenderSetViewport(gRenderer, &topRightViewport);
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
                SDL_Rect bottomViewport;
                bottomViewport.x = 0;
                bottomViewport.y = SCREEN_HEIGHT / 2;
                bottomViewport.w = SCREEN_WIDTH;
                bottomViewport.h = SCREEN_HEIGHT / 2;
                SDL_RenderSetViewport(gRenderer, &bottomViewport);
                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

                SDL_RenderPresent(gRenderer);
            }
        }
    }

    close();

    return 0;
}

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
            // gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

bool loadMedia()
{
    bool success = true;

    // gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("04_key_presses/press.bmp");
    // if (gKeyPressSurfaces == NULL)
    // {
    //     printf("Failed to load default image!\n");
    //     success = false;
    // }

    // gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("04_key_presses/up.bmp");
    // if (gKeyPressSurfaces == NULL)
    // {
    //     printf("Failed to load up image!\n");
    //     success = false;
    // }

    // gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("04_key_presses/down.bmp");
    // if (gKeyPressSurfaces == NULL)
    // {
    //     printf("Failed to load down image!\n");
    //     success = false;
    // }

    // gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("04_key_presses/left.bmp");
    // if (gKeyPressSurfaces == NULL)
    // {
    //     printf("Failed to load left image!\n");
    //     success = false;
    // }

    // gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("04_key_presses/right.bmp");
    // if (gKeyPressSurfaces == NULL)
    // {
    //     printf("Failed to load right image!\n");
    //     success = false;
    // }

    // gStretchedSurface = loadSurface("05_optimized_surface_loading_and_soft_stretching/stretch.bmp");
    // if (gStretchedSurface == NULL)
    // {
    //     printf("Failed to load stretching image!\n");
    //     success = false;
    // }

    // gPNGSurface = loadSurface("06_extension_libraries_and_loading_other_image_formats/loaded.png");
    // if (gPNGSurface == NULL)
    // {
    //     printf("Failed to load PNG image!\n");
    //     success = false;
    // }

    // gTexture = loadTexture("07_texture_loading_and_rendering/texture.png");
    // if (gTexture == NULL)
    // {
    //     printf("Failed to load texture image!\n");
    //     success = false;
    // }

    gTexture = loadTexture( "09_the_viewport/viewport.png" );
	if( gTexture == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

    return success;
}

void close()
{
    // SDL_FreeSurface(gCurrentSurface);
    // gCurrentSurface = NULL;

    // SDL_FreeSurface(gStretchedSurface);
    // gStretchedSurface = NULL;

    // SDL_FreeSurface(gPNGSurface);
    // gPNGSurface = NULL;

    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

SDL_Surface *loadSurface(std::string path)
{
    SDL_Surface *optimizedSurface = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
        if (optimizedSurface == NULL)
        {
            printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

SDL_Texture *loadTexture(std::string path)
{
    SDL_Texture *newTexture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}
#include <iostream>
#include <SDL2/SDL.h>
#include <complex> 
#include <windows.h>

using namespace std;

int create_fractal(int max){
    SetProcessDPIAware();
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;

        return 1;
    }
    //My screen resolution
    int width = 1920, height = 1200;
    int x = 0 , y = 0;

    SDL_Window *window = SDL_CreateWindow("template", x, y, width, height, SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI );
    if(window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;

        SDL_Quit();

        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == nullptr) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;

        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    SDL_Event event;
    SDL_RenderClear(renderer);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    
    bool quit = false;
    while(!quit) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        complex<double> initial_c(static_cast<float>(mouseX)/static_cast<float>(width)*4.8-2.4,static_cast<float>(mouseY)/static_cast<float>(height)*3-1.5);

        void* pixels;
        int pitch;
        SDL_LockTexture(texture, NULL, &pixels, &pitch);
        for (int i = 0; i < width; i++){
            double x = (i - width/2.0) / height * 3.0;
            for (int j = 0; j < height; j++){
                double y = (j - height/2.0) / height * 3.0;

                complex<double> z(x,y);

                double iterations = 0;
                while (iterations <= max) {
                    double z_real_squared = real(z) * real(z);
                    double z_imag_squared = imag(z) * imag(z);
                    if (z_real_squared + z_imag_squared >= 4.0) {
                        break;
                    }
                    z = complex<double>(z_real_squared - z_imag_squared, 2.0 * real(z) * imag(z)) + initial_c;
                    iterations++;
                }

                if(iterations > max){
                    Uint32* pixel = (Uint32*)((Uint8*)pixels + j * pitch + i * sizeof(Uint32));
                    *pixel = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 0 , 0, 0, 255);
                } else {
                    if(iterations <= max/4){
                        Uint32* pixel = (Uint32*)((Uint8*)pixels + j * pitch + i * sizeof(Uint32));
                        *pixel = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 0 , 0, 128+iterations*127/max*4, 255);
                    } else if(iterations <= max/2){
                        Uint32* pixel = (Uint32*)((Uint8*)pixels + j * pitch + i * sizeof(Uint32));
                        *pixel = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 0 +(4*iterations-max)*255/max ,  0 + (4*iterations-max)*255/max, 255, 255);
                    } else if(iterations <= max*3/4){
                        Uint32* pixel = (Uint32*)((Uint8*)pixels + j * pitch + i * sizeof(Uint32));
                        *pixel = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888),255, 255 -(2*iterations-max)*55*2/max, 255-(2*iterations-max)*255*2/max, 255); 
                    } else {
                        Uint32* pixel = (Uint32*)((Uint8*)pixels + j * pitch + i * sizeof(Uint32));
                        *pixel = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888),255-(4*iterations-max*3)*255/max, 200-(4*iterations-max*3)*200/max, 0, 255);
                    }
                }
            }
        }
        
        SDL_UnlockTexture(texture);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    return 0;
}

int get_max(){
    int max;
    cout << "What is the max amount of iterations before something is considered infinite" << endl;
    cout << "Lower numbers give clearer but les detailed fractals. recommended input 10 - 300" << endl;
    cin >> max;
    return max;
}

int main(int argc, char*argv[]) {
    // complex<double> z = get_z();
    // complex<double> c = get_c();
    int max = get_max();
    return create_fractal(max);
}
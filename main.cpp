#include <iostream>
#include <SDL2/SDL.h>
#include <complex> 
#include <windows.h>

using namespace std;

int create_fractal(complex<double> initial_z, complex<double> initial_c, int max){
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
    for (int i = 0; i < width; i++){
        double x = i-width/2;
        x = x/height;
        x = x*3;
        for (int j = 0; j < height; j++){
            double y = j-height/2;
            y = y/height;
            y = y*3;
            double zx,zy,cx,cy;
            if (real(initial_z)==10.0){zx = x;}
            else if (real(initial_z)==-10.0){zx=y;}
            else{zx=real(initial_z);}
            if (imag(initial_z)==10.0){zy = x;}
            else if (imag(initial_z)==-10.0){zy=y;}
            else{zy=imag(initial_z);}

            if (real(initial_c)==10.0){cx = x;}
            else if (real(initial_c)==-10.0){cx=y;}
            else{cx=real(initial_c);}
            if (imag(initial_c)==10.0){cy = x;}
            else if (imag(initial_c)==-10.0){cy=y;}
            else{cy=imag(initial_c);}

            complex<double> z(zx,zy);
            complex<double> initial_c(cx,cy);
            double iterations = 0;
            while (abs(z)<2&&iterations <= max){
                z = z*z+initial_c;
                iterations++;
            }
            if(iterations > max){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else {
                if(iterations <= max/4){
                    SDL_SetRenderDrawColor(renderer,0 , 0, 128+iterations*127/max*4, 255);
                } else if(iterations <= max/2){
                    SDL_SetRenderDrawColor(renderer,0 +(4*iterations-max)*255/max ,  0 + (4*iterations-max)*255/max, 255, 255);
                } else if(iterations <= max*3/4){
                    SDL_SetRenderDrawColor(renderer,255 , 255 -(2*iterations-max)*55*2/max, 255-(2*iterations-max)*255*2/max, 255);
                } else {
                    SDL_SetRenderDrawColor(renderer,255-(4*iterations-max*3)*255/max, 200-(4*iterations-max*3)*200/max, 0, 255);
                }
                
            }
            SDL_RenderDrawPoint(renderer, i, j);
            
        }
    }

    // Used for testing colors
    // for (int i = 0; i < width; i++)
    // {
    //     if(i < width/4){
    //         SDL_SetRenderDrawColor(renderer,0 , 0, 128+i*127/width*4, 255);
    //     } else if(i < width/2){
    //         SDL_SetRenderDrawColor(renderer,0 +(i-width/4)*255/width*4 ,  0 + (i-width/4)*255/width*4, 255, 255);
    //     } else if(i < width/4*3){
    //         SDL_SetRenderDrawColor(renderer,255 , 255 -(i-width/2)*55/width*4, 255-(i-width/2)*255/width*4, 255);
    //     } else {
    //         SDL_SetRenderDrawColor(renderer,255-(i-width/4*3)*255/width*4, 200-(i-width/4*3)*200/width*4, 0, 255);
    //     }

    //     SDL_RenderDrawPoint(renderer, i, 50);
    //     SDL_RenderDrawPoint(renderer, i, 51);
    //     SDL_RenderDrawPoint(renderer, i, 52);
    //     SDL_RenderDrawPoint(renderer, i, 53);
    // }
    SDL_RenderPresent(renderer);
    cout << "done" << endl;
    bool quit = false;
    while(!quit) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    return 0;
}
complex<double> get_z (){
    string real_z_string;
    string imag_z_string;
    double real_z;
    double imag_z;
    while(true){
        cout << "What do you want the real part of z to be pick x, y or any double in range -2 to 2" << endl;
        getline(cin,real_z_string,'\n');
        if(real_z_string == "x" ||real_z_string == "X"){
            real_z = 10.0;
            break;
        } else if(real_z_string == "y"||real_z_string == "Y"){
            real_z = -10.0;
            break;
        } else{
            real_z = stod(real_z_string);
            if(real_z > -2.0 && real_z < 2.0){
                break;
            } else {
                cout << "Your input should be x, y or a double in between -2 and 2 ie 1.45" << endl;
            }
        }
    }
    while(true){
        cout << "What do you want the imaginary part of z to be pick x, y or any double in range -2 to 2" << endl;
        getline(cin,imag_z_string,'\n');
        if(imag_z_string == "x" ||imag_z_string == "X"){
            imag_z = 10.0;
            break;
        } else if(imag_z_string == "y"||imag_z_string == "Y"){
            imag_z = -10.0;
            break;
        } else{
            imag_z = stod(imag_z_string);
            if(imag_z > -2.0 && imag_z < 2.0){
                break;
            } else {
                cout << "Your input should be x, y or a double in between -2 and 2 ie 1.45" << endl;
            }
        }
    }
    complex<double> z(real_z,imag_z);
    return z;
}

complex<double> get_c (){
    string real_c_string;
    string imag_c_string;
    double real_c;
    double imag_c;
    while(true){
        cout << "What do you want the real part of c to be pick x, y or any double in range -2 to 2" << endl;
        getline(cin,real_c_string,'\n');
        if(real_c_string == "x" ||real_c_string == "X"){
            real_c = 10.0;
            break;
        } else if(real_c_string == "y"||real_c_string == "Y"){
            real_c = -10.0;
            break;
        } else{
            real_c = stod(real_c_string);
            if(real_c > -2.0 && real_c < 2.0){
                break;
            } else {
                cout << "Your input should be x, y or a double in between -2 and 2 ie 1.45" << endl;
            }
        }
    }
    while(true){
        cout << "What do you want the imaginary part of c to be pick x, y or any double in range -2 to 2" << endl;
        getline(cin,imag_c_string,'\n');
        if(imag_c_string == "x" ||imag_c_string == "X"){
            imag_c = 10.0;
            break;
        } else if(imag_c_string == "y"||imag_c_string == "Y"){
            imag_c = -10.0;
            break;
        } else{
            imag_c = stod(imag_c_string);
            if(imag_c > -2.0 && imag_c < 2.0){
                break;
            } else {
                cout << "Your input should be x, y or a double in between -2 and 2 ie 1.45" << endl;
            }
        }
    }
    complex<double> c(real_c,imag_c);
    return c;
}

int get_max(){
    int max;
    cout << "What is the max amount of iterations before something is considered infinite" << endl;
    cout << "Lower numbers give clearer but les detailed fractals. recommended input 10 - 300" << endl;
    cin >> max;
    return max;
}
int main(int argc, char*argv[]) {
    complex<double> z = get_z();
    complex<double> c = get_c();
    int max = get_max();
    return create_fractal(z,c,max);
}
#include <string>
#include <fstream>
#include <stdint.h>

#define MAX_ITTERATIONS 1000

unsigned char r_pallete[MAX_ITTERATIONS];
unsigned char g_pallete[MAX_ITTERATIONS];
unsigned char b_pallete[MAX_ITTERATIONS];

//Creates a pallete for the fractal
void create_pallete(){
    for (size_t i = 0; i < MAX_ITTERATIONS; i++)
    {
        r_pallete[i] = 0;
        g_pallete[i] = double(i)/MAX_ITTERATIONS * 255;
        b_pallete[i] = double(i*i)/double(MAX_ITTERATIONS) > 1 ? 0 : double(i*i)/double(MAX_ITTERATIONS) * 255.0;
    }
}

//Uses the ppm format for image storage http://netpbm.sourceforge.net/doc/ppm.html
int write_to_ppm(unsigned char* const red, unsigned char* const green, unsigned char* const blue, 
        unsigned w, unsigned h, const std::string& fileName){

    std::ofstream out;
    out.open(fileName, std::ofstream::trunc);   //trunc for clear

    out << "P3\n";
    out << w <<" "<<h<<"\n";
    out << 255 << "\n";

    for (size_t i = 0; i < w * h; i++)
    {
        out << std::to_string(red[i]) <<" "<<std::to_string(green[i])<<" "<<std::to_string(blue[i])<<"\n";
    }
    out.close();

    return 0;
}

//Taken from https://en.wikipedia.org/wiki/Mandelbrot_set
//Generates a color for every pixel coresponding with its number of itterations in the mandelbrot set
int fractal(unsigned char* red, unsigned char* green, unsigned char* blue, 
        unsigned w, unsigned h) {

    for (size_t iy = 0; iy < h; iy++)
    {
        for (size_t ix = 0; ix < w; ix++)
        {
            double x0 = double(ix)/double(w) * 3.5 - 2.5, y0 = double(iy)/double(h) * 2.0 - 1.0,x = 0.0,y=0.0;
            int iteration = 0;
            int max_iteration = MAX_ITTERATIONS;
            
            while (x*x + y*y <= 2*2  &&  iteration < max_iteration) {
                double xtemp = x*x - y*y + x0;
                y = 2*x*y + y0;
                x = xtemp;
                iteration++;
            }
            red[iy*w + ix] = r_pallete[iteration];
            green[iy*w + ix] = g_pallete[iteration];
            blue[iy*w + ix] = b_pallete[iteration];
        }
        
    }
    
    return 0;
}

int main(){
    unsigned char* red = new unsigned char[1000000];
    unsigned char* green = new unsigned char[1000000];
    unsigned char* blue = new unsigned char[1000000];

    create_pallete();

    fractal(red,green,blue,1000,1000);
    write_to_ppm(red,green,blue,1000,1000,"mandelbrot.ppm");

    delete[] red;
    delete[] green;
    delete[] blue;

    return 0;
}
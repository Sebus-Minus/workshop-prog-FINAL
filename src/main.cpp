#include <sil/sil.hpp>
#include <iostream>
#include "random.hpp"
#include <cmath>
#include <numbers>

sil::Image vert(sil::Image img){
    for (glm::vec3& color : img.pixels()){
        color.r = 0.f;
        color.b = 0.f;
    }
    return img;
}

sil::Image swap(sil::Image img){
    for (glm::vec3& color : img.pixels()){
        std::swap(color.b, color.g);
    }
    return img;
}

sil::Image grey(sil::Image img){
    for (glm::vec3& color : img.pixels()){
        float greylvl = (color.r + color.g + color.b )/3;
        color.r = greylvl;
        color.g = greylvl;
        color.b = greylvl;
    }
    return img;
}

sil::Image neg(sil::Image img){
    for (glm::vec3& color : img.pixels()){
        color.r = 1 - color.r;
        color.g = 1 - color.g;
        color.b = 1 - color.b;
    }
    return img;
}

sil::Image grad(){
    sil::Image image{300/*width*/, 200/*height*/};
    for (float i{0}; image.width() > i; i++){
        for (float j{0}; image.height() > j; j++){
            image.pixel(i,j).r = i/image.width();
            image.pixel(i,j).g = i/image.width();
            image.pixel(i,j).b = i/image.width();
        }
    }
    return image;
}

sil::Image mirror(sil::Image img){
    int w = img.width();
    int h = img.height();
    int mirX = 0;
    sil::Image cpy{w, h};
    for (float i{0};w > i; i++){
        for (float j{0}; h > j; j++){
            mirX = (w-1) - i;
            cpy.pixel(i,j) = img.pixel(mirX,j);
        }
    }
    return cpy;
}

sil::Image noise(sil::Image img, int noiseLvl = 25){
    /* noiseLvl permet de définir à quel point l'image sera bruitée, 
    0 étant le minimum et 100 étant le maximum*/
    for (glm::vec3& color : img.pixels()){
        int rdChance = random_int(0, 100);
        if (rdChance < noiseLvl){
            color.r = random_float(0,1);
            color.g = random_float(0,1);
            color.b = random_float(0,1);
        }
    }
    return img;
}

sil::Image rot(sil::Image img){
    int w = img.width();
    int h = img.height();
    int mirX = 0;
    sil::Image cpy{h, w};
    for (float i{0};w > i; i++){
        for (float j{0}; h > j; j++){
            cpy.pixel(h-j-1,i) = img.pixel(i,j);
        }
    }
    return cpy;
}

sil::Image split(sil::Image img, int offset = 10){
    /* offset permet de plus ou moins décaler les couleurs les unes des autres
    il est représenté en un pourcentage (0 à 100) par rapport à la largeur de l'image*/
    int w = img.width();
    int h = img.height();
    int pixOff = w * offset/100;
    sil::Image cpy{w, h};
    for (float i{0};w > i; i++){
        for (float j{0}; h > j; j++){
            cpy.pixel(i,j).g = img.pixel(i,j).g;
            if (i - pixOff >= 0){
                cpy.pixel(i - pixOff,j).b = img.pixel(i,j).b;
            }
            if (i + pixOff < w){
                cpy.pixel(i + pixOff,j).r = img.pixel(i,j).r;
            }
        }
    }
    return cpy;
}

sil::Image lumin(sil::Image img, float power = 2){
    /* pow est la puissance à laquelle la couleur de chaque pixel sera soumise
    se rapprocher de 0 éclaircit et s'en éloigner assombrit*/
    int w = img.width();
    int h = img.height();
    sil::Image cpy{w, h};
    for (float i{0};w > i; i++){
        for (float j{0}; h > j; j++){
            cpy.pixel(i,j).r = pow(img.pixel(i,j).r, power);
            cpy.pixel(i,j).g = pow(img.pixel(i,j).g, power);
            cpy.pixel(i,j).b = pow(img.pixel(i,j).b, power);
        }
    }
    return cpy;
}

sil::Image disc(int rad = 30){
    /* rad correspond à la proportion que prendra le disque par rapport à l'image*/
    sil::Image img{500, 500};
    int w = img.width();
    int h = img.height();
    int centerX = w/2;
    int centerY = h/2;
    float rayon = (w+h/2.0) * (rad/200.0);
    for (float i{0};w > i; i++){
        for (float j{0}; h > j; j++){
            if (pow(i-centerX,2) + pow(j-centerY,2) < rayon*rayon ){
                img.pixel(i,j) = glm::vec3{1};
            }
        }
    }
    return img;
}

sil::Image circle(int thickness, int rad = 30){
    /* rad correspond à la proportion que prendra le disque par rapport à l'image*/
    sil::Image img{500, 500};
    int w = img.width();
    int h = img.height();
    int centerX = w/2;
    int centerY = h/2;
    float rayon = ((w+h/2.0)) * (rad/200.0);
    int oDist;
    for (float i{0};w > i; i++){
        for (float j{0}; h > j; j++){
            oDist = pow(i-centerX,2) + pow(j-centerY,2);
            if (oDist < pow(rayon+thickness, 2) && oDist > pow(rayon-thickness,2)){
                img.pixel(i,j) = glm::vec3{1};
            }
        }
    }
    return img;
}

void anim(int frames = 50, int rad = 30){
    /* rad correspond à la proportion que prendra le disque par rapport à l'image*/
    int w = 500;
    int h = 500;
    int centerY = h/2;
    float rayon = (w+h/2.0) * (rad/200.0);
    int startX = 0-ceil(rayon);
    int step = (w + ceil(rayon)*2) / frames;
    for(int f = 0; f <= frames; f++){
        sil::Image img{500, 500};
        int centerX = startX + step*f;
        for (float i{0};w > i; i++){
            for (float j{0}; h > j; j++){
                if (pow(i-centerX,2) + pow(j-centerY,2) < rayon*rayon ){
                    img.pixel(i,j) = glm::vec3{1};
                }
            }
        }
    
        img.save(std::string("output/anim/") + std::to_string(f) + ".png");
    }
}

sil::Image rose(int thickness, int rad = 30, int petals = 5){
    /* rad correspond à la proportion que prendra le disque par rapport à l'image*/
    sil::Image img{500, 500};
    float step = (M_PI*2) / petals;
    int w = img.width();
    int h = img.height();
    int centerX = w/2;
    int centerY = h/2;
    float rayon = ((w+h/2.0)) * (rad/200.0);
    int oDist;
    for (int p = 0; petals >= p; p++){
        for (float i{0};w > i; i++){
            for (float j{0}; h > j; j++){
                oDist = pow(i-centerX,2) + pow(j-centerY,2);
                if (oDist < pow(rayon+thickness, 2) && oDist > pow(rayon-thickness,2)){
                    img.pixel(i,j) = glm::vec3{1};
                }
            }
        }
        centerX = w/2 + ceil(cos(step*p)*rayon);
        centerY = h/2 + ceil(sin(step*p)*rayon);
    }
    return img;
}

sil::Image mosa(sil::Image img, int scale = 5){
    /* scale détermine la taille de la mosaïque (nombre d'images par ligne)*/
    int w = img.width();
    int h = img.height();
    sil::Image cpy{w, h};
    int wMos = w/scale;
    int hMos = h/scale;
    int wStep = w/wMos;
    int hStep = h/hMos;
    for(int i{0}; w > i; i++){
        for(int j{0}; h > j; j++){
            cpy.pixel(i,j) = img.pixel((i*wStep)%w, (j*hStep)%h);
        }
    }
    return cpy;
}

sil::Image mirosaic(sil::Image img, int scale = 3){
    /* scale détermine la taille de la mosaïque (nombre de couples d'images par ligne)*/
    int w = img.width();
    int h = img.height();
    sil::Image cpy{w, h};
    int wMos = w/scale;
    int hMos = h/scale;
    int step = scale*2;

    for(int k = 0; k < scale; k++){
        for(int l = 0; l < scale; l++){
            for(int i = 0; i < wMos / 2; i++){
                for(int j = 0; j < hMos / 2; j++){
                    // Pixel source échantillonné
                    int startX = wMos * k;
                    int startY = hMos * l;
                    cpy.pixel(startX + i, startY + j) = img.pixel((i * step) % w, (j * step) % h);
                    cpy.pixel(startX + wMos - 1 - i, startY + j) = img.pixel((i * step) % w, (j * step) % h);
                    cpy.pixel(startX + i, startY + hMos - 1 - j) = img.pixel((i * step) % w, (j * step) % h);
                    cpy.pixel(startX + wMos - 1 - i, startY + hMos - 1 - j) = img.pixel((i * step) % w, (j * step) % h);
                }
            }
        }
    }
    return cpy;
}

sil::Image glitch(sil::Image img, int glitchLvl = 25){
    /* glitchLvl indique le nombre de rectangles qui seront intervertis*/
    int w = img.width();
    int h = img.height();
    int maxW = ceil(w/20.0);
    int maxH = ceil(h/20.0);
    
    for(int k{0}; k < glitchLvl; k++){
        int glitchW = random_int(10,maxW);
        int glitchH = random_int(4,maxH);
        int x1 = random_int(0,w);
        int y1 = random_int(0,h);
        int x2 = random_int(0,w);
        int y2 = random_int(0,h);
        for(int i{0}; glitchW > i; i++){
            for(int j{0}; glitchH > j; j++){
                std::swap(img.pixel((x1 + i) % w, (y1 + j) % h),img.pixel((x2 + i) % w, (y2 + j) % h));
            }
        }
    }
    return img;
}


int main(){
    sil::Image image{"images/huh.jpeg"};
    image = rose(3,30, 10);
    image.save("output/rosas.png");
    return 0;
}
#define _CRT_SECURE_NO_WARNINGS

#include "Util.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <iostream>

// dodaj ovo:
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Autor: Nedeljko Tesanovic
// Opis: pomocne funkcije za ucitavanje sejdera
unsigned int compileShader(GLenum type, const char* source)
{
    // Uzima kod u fajlu na putanji "source", kompajlira ga i vraca sejder tipa "type"
    // Citanje izvornog koda iz fajla
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
    std::string temp = ss.str();
    const char* sourceCode = temp.c_str(); // Izvorni kod sejdera koji citamo iz fajla na putanji "source"

    int shader = glCreateShader(type); // Napravimo prazan sejder odredjenog tipa (vertex ili fragment)

    int success; // Da li je kompajliranje bilo uspjesno (1 - da)
    char infoLog[512]; // Poruka o gresci (Objasnjava sta je puklo unutar sejdera)
    glShaderSource(shader, 1, &sourceCode, NULL); // Postavi izvorni kod sejdera
    glCompileShader(shader); // Kompajliraj sejder

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // Provjeri da li je sejder uspjesno kompajliran
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog); // Pribavi poruku o gresci
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf("%s\n", infoLog);
    }
    return shader;
}

unsigned int createShader(const char* vsSource, const char* fsSource)
{
    // Pravi objedinjeni sejder program koji se sastoji od Vertex sejdera ciji je kod na putanji vsSource

    unsigned int program; // Objedinjeni sejder
    unsigned int vertexShader; // Verteks sejder (za prostorne podatke)
    unsigned int fragmentShader; // Fragment sejder (za boje, teksture itd)

    program = glCreateProgram(); // Napravi prazan objedinjeni sejder program

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);   // Napravi i kompajliraj vertex sejder
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource); // Napravi i kompajliraj fragment sejder

    // Zakaci verteks i fragment sejdere za objedinjeni program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);    // Povezi ih u jedan objedinjeni sejder program
    glValidateProgram(program); // Izvrsi provjeru novopecenog programa

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success); // Slicno kao za sejdere
    if (success == GL_FALSE)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    // Posto su kodovi sejdera u objedinjenom sejderu, oni pojedinacni programi nam ne trebaju, pa ih brisemo
    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}

// ---------------------- NOVO: ucitavanje teksture ----------------------
// Ovo je genericki loader kao sa vezbi (twolegs, grass, itd.)
unsigned int loadTexture(const char* path)
{
    int width = 0, height = 0, nrChannels = 0;

    // opciono: ako želiš flip po Y osi (da slika ne bude naopako)
    // stbi_set_flip_vertically_on_load(true);

    // UVEK tražimo 4 kanala (RGBA)
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);

    if (!data)
    {
        std::cout << "Greska pri ucitavanju teksture: " << path << std::endl;
        return 0;
    }

    std::cout << "Ucitana tekstura: " << path
        << " (" << width << "x" << height
        << ", kanala: " << nrChannels << ", force RGBA)"
        << std::endl;

    unsigned int textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // SVE je GL_RGBA (i internal i external format)
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,             // internal format
        width,
        height,
        0,
        GL_RGBA,             // external format
        GL_UNSIGNED_BYTE,
        data
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    // Wrap i filter parametri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return textureID;
}


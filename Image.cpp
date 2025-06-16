#include "Image.h"
#include <iostream>
#include <fstream>

Color::Color()
    :r(0), g(0), b(0)
{
}

Color::Color(float r, float g, float b)
    :r(r), g(g), b(b)
{
}

Color::~Color()
{
}

Image::Image()
    :m_width(640), m_height(640), m_colors(std::vector<Color>(640*640))
{
}

Image::Image(int width, int height)
    :m_width(width), m_height(height), m_colors(std::vector<Color>(height*width))
{
}

Image::~Image()
{
}

Color Image::GetColor(int x, int y) const
{
    return m_colors[y * m_width + x];
}

void Image::setColor(const Color& color, int x, int y)
{
    m_colors[y * m_width + x].r = color.r;
    m_colors[y * m_width + x].g = color.g;
    m_colors[y * m_width + x].b = color.b;
}

void Image::Read(const char *path)
{
    std::ifstream f ;
    f.open(path, std::ios::in | std::ios::binary);

    if (!f.is_open())
    {
        std::cout<<"File \""<<path<<"\" could not be opened" << std::endl;
        return;
    }

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;

    unsigned char fileHeader[fileHeaderSize];
    f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

    if (fileHeader[0] != 'B' || fileHeader[1] != 'M')
    {
        std::cout<<"Specified file is not a bitmap image file"<<std::endl;
        f.close();
        return;
    }

    unsigned char informationHeader[informationHeaderSize];
    f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    int fileSize = fileHeader[2] + (fileHeader[3]<<8) + (fileHeader[4]<<16) +(fileHeader[5]<<24);

    m_width = informationHeader[4] + (informationHeader[5]<<8) + (informationHeader[6]<<16) + (informationHeader[7]<<24);
    m_height = informationHeader[8]+ (informationHeader[9]<<8) + (informationHeader[10]<<16) + (informationHeader[11]<<24);

    m_colors.resize(m_width * m_height);

    const int paddingAmount = ((4 - (m_width*3)%4)%4);

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            unsigned char color[3];
            f.read(reinterpret_cast<char*>(color), 3);

            m_colors[y * m_width + x].r = static_cast<float>(color[2]) / 255.0f;
            m_colors[y * m_width + x].g = static_cast<float>(color[1]) / 255.0f;
            m_colors[y * m_width + x].b = static_cast<float>(color[0]) / 255.0f;
        }
        f.ignore(paddingAmount);
    }

    f.close();

    // std::cout<<"File succesfully read"<<std::endl;
}

void Image::Export(const char *path) const
{
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);

    if (!f.is_open()) {
        std::cout<<"File could not be opened"<<std::endl;
        return;
    }

    unsigned char bmpPad[3] = {0,0,0};
    const int paddingAmount = ((4 - (m_width*3)%4)%4);

    const int fileHeaderSize = 14;
    const int infoHeaderSize = 40;
    const int fileSize = fileHeaderSize + infoHeaderSize + m_width + m_height + 3 + paddingAmount + m_width;

    unsigned char fileHeader[fileHeaderSize];

    //File Type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    //Image width
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    //Reserved 1 (not used)
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    //reserved 2 (not used)
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    //Pixel dat offset
    fileHeader[10] = fileHeaderSize + infoHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char infoHeader[infoHeaderSize];

    //header size
    infoHeader[0] = infoHeaderSize;
    infoHeader[1] = 0;
    infoHeader[2] = 0;
    infoHeader[3] = 0;
    //Image width
    infoHeader[4] = m_width;
    infoHeader[5] = m_width >> 8;
    infoHeader[6] = m_width >> 16;
    infoHeader[7] = m_width >> 24;
    //Image height
    infoHeader[8] = m_height;
    infoHeader[9] = m_height >> 8;
    infoHeader[10] = m_height >> 16;
    infoHeader[11] = m_height >>24;
    //Planes
    infoHeader[12] = 0;
    infoHeader[13] = 0;
    //Bits per pixel (rgb)
    infoHeader[14] = 24;
    infoHeader[15] = 0;
    //Compression (None)
    infoHeader[16] = 0;
    infoHeader[17] = 0;
    infoHeader[18] = 0;
    infoHeader[19] = 0;
    //Image size (No compression)
    infoHeader[20] = 0;
    infoHeader[21] = 0;
    infoHeader[22] = 0;
    infoHeader[23] = 0;
    //X pixels per meter (not specified)
    infoHeader[24] = 0;
    infoHeader[25] = 0;
    infoHeader[26] = 0;
    infoHeader[27] = 0;
    //Y pixels per meter (not specified)
    infoHeader[28] = 0;
    infoHeader[29] = 0;
    infoHeader[30] = 0;
    infoHeader[31] = 0;
    //Total color (color pallet)
    infoHeader[32] = 0;
    infoHeader[33] = 0;
    infoHeader[34] = 0;
    infoHeader[35] = 0;
    //Important Colors (generally ignored)
    infoHeader[36] = 0;
    infoHeader[37] = 0;
    infoHeader[38] = 0;
    infoHeader[39] = 0;

    f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    f.write(reinterpret_cast<char*>(infoHeader), infoHeaderSize);

    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            unsigned char r = static_cast<unsigned char>(GetColor(x,y).r *255.0f);
            unsigned char g = static_cast<unsigned char>(GetColor(x,y).g *255.0f);
            unsigned char b = static_cast<unsigned char>(GetColor(x,y).b *255.0f);

            unsigned char color[] = {b,g,r};

            f.write(reinterpret_cast<char*>(color),3);
    }
    }
    f.close();
    std::cout<<"File Successfully Created"<<std::endl;
}

int Image::getHeight()
{
    return m_height;
}

int Image::getWidth()
{
    return m_width;
}


#ifndef LOADER
#define LOADER

#include "util/util.h"

struct imageDB
{
public:
    imageDB() = delete;
    ~imageDB() { }
    imageDB(std::string filePath);
    UINT8 *operator[](int N);
    void print(int N);
    int size();

private:
    UINT8 *m_data;
    int m_size;
};

struct labelDB
{
public:
    labelDB() = delete;
    ~labelDB() {  }
    labelDB(std::string filePath);
    char operator[](int N);
    int size();

private:
    UINT8 *m_data;
    int m_size;
};

imageDB::imageDB(std::string filePath)
{
    std::cout << "detect file \"" << filePath << "\"" << std::endl;
    std::cout << "loading the file.." << std::endl;
    std::fstream fileIn(filePath, std::ios::in | std::ios::binary);
    if(!fileIn.is_open())
    {
        throw file_inexist("filePath");
    }
    std::string fileData = "";
    char tmpChar;
    while(!fileIn.eof())
    {
        fileIn.read(&tmpChar, 1);
        fileData.push_back(tmpChar);
    }
    UINT8 *tmp = (UINT8 *)fileData.c_str();
    m_data = new UINT8[fileData.size() - 1];
    memcpy(m_data, tmp, fileData.size() - 1);
    if(htonl(*(UINT32 *)m_data) != IMAGE_MAGIC_NUMBER)
    {
        std::cout << "error: file format is wrong" << std::endl;
        exit(0x00000003);
    }
    int num_rows = ntohl(*(UINT32 *)(m_data + 8));
    int num_cols = ntohl(*(UINT32 *)(m_data + 12));
    if(num_rows != 28 || num_cols != 28)
    {
        std::cerr << "error: unrecognized image size" << std::endl;
        exit(0x00000005);
    }
    std::cout << "file size: " << fileData.size() - 1 << " bytes" << std::endl;
    m_size = fileData.size() / 784;
    fileIn.close();
}

void imageDB::print(int N)
{
    std::fstream headFile("head", std::ios::binary | std::ios::in);
    std::stringstream ss;
    ss << N;
    std::string outFileName;
    ss >> outFileName;
    ss.clear();
    outFileName = "./pic/" + outFileName;
    outFileName += '_';
    outFileName += "digit.bmp";
    std::fstream fileOut(outFileName, std::ios::binary | std::ios::out);

    char *head_info = new char[0x435];
    headFile.read(head_info, 0x435);
    fileOut.write(head_info, 0x435);
    fileOut.write((char *)m_data + 16 + LENGTH * 
            LENGTH * N, LENGTH * LENGTH);
    fileOut.close();
    headFile.close();
    delete []head_info;
}

UINT8 *imageDB::operator[](int N)
{
    return m_data + 16 + LENGTH * LENGTH * N;
}

int imageDB::size()
{
    return m_size;
}

labelDB::labelDB(std::string filePath)
{
    std::cout << "detect file \"" << filePath << "\"" << std::endl;
    std::cout << "loading the file.." << std::endl;
    std::fstream fileIn(filePath, std::ios::in | std::ios::binary);
    if(!fileIn.is_open())
    {
        throw file_inexist("filePath");
    }
    std::string fileData = "";
    char tmpChar;
    while(!fileIn.eof())
    {
        fileIn.read(&tmpChar, 1);
        fileData.push_back(tmpChar);
    }
    UINT8 *tmp = (UINT8 *)fileData.c_str();
    m_data = new UINT8[fileData.size() - 1];
    memcpy(m_data, tmp, fileData.size() -1);
    if(htonl(*(UINT32 *)m_data) != LABEL_MAGIC_NUMBER)
    {
        std::cout << "error: file format is wrong" << std::endl;
        exit(0x00000002);
    }
    std::cout << "file size: " << fileData.size() - 1 << " bytes" << std::endl;
    m_size = fileData.size() - 9;
    fileIn.close();
}

char labelDB::operator[](int N)
{
    return *(char *)(m_data + N + 8);
}

int labelDB::size()
{
    return m_size;
}


#endif

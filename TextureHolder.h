#ifndef TextureHolder_H
#define TextureHolder_H
#include <SFML/Graphics.hpp>
#include<map>

using namespace sf;

class TextureHolder {

public:
    TextureHolder();  
    static Texture& GetTexture(std::string const& filename); 

private:
    std::map<std::string, Texture> m_Textures; 
    static TextureHolder* m_s_Instance; 

};

#endif
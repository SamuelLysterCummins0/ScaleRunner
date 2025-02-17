#include "TextureHolder.h"
#include <assert.h>
// TextureHolder class adapted from Zombie Arena
TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder() {
    assert(m_s_Instance == nullptr);
    m_s_Instance = this;
}

Texture& TextureHolder::GetTexture(std::string const& filename) {
    //Get a reference to the m_Textures map using the m_s_Instance pointer
    auto& m = m_s_Instance->m_Textures;

    //Create an iterator and search m for the passed in filename
    auto keyValuePair = m.find(filename);

    if (keyValuePair != m.end()) {
        //if found return the texture
        return keyValuePair->second;
    }//if
    else {
        //Create new key pair using the filename
        auto& texture = m[filename];
        //Load the texture from file into map beside its filename
        texture.loadFromFile(filename);
        return texture;

    }//else

}//GetTexture function
//
//  Texture.cpp
//  jpngame
//
//  Created by Vincent K on 12/25/13.
//  Copyright (c) 2013 Unlocked Doors. All rights reserved.
//

#include "Texture.h"
#include "Common.h"


using namespace ud;

Texture::Texture() : texture_(nullptr) {
}

Texture::Texture(std::shared_ptr<Surface> surface) {
    std::cout << "Texture: creating texture from surface" << std::endl;
    texture_ = SDL_CreateTextureFromSurface(renderer, surface->get());
    if(texture_ == NULL) {
        throw std::runtime_error{std::string{"Texture could not create SDL_Texture from SDL_Surface: "} + SDL_GetError()};
    }
}

Texture::~Texture() {
    std::cout << "Texture: destructor called, destroying underlying SDL_Texture" << std::endl;
    if(texture_ != nullptr) {
        SDL_DestroyTexture(texture_);
    }
}

Texture::Texture(const Texture &that) {
    Uint32 format;
    int access, w, h;
    
    SDL_QueryTexture(that.texture_, &format, &access, &w, &h);
    texture_ = SDL_CreateTexture(renderer, format, access, w, h);
    if(texture_ == NULL) {
        throw std::runtime_error{std::string{"Texture could not create SDL_Texture: "} + SDL_GetError()};
    }
    
    SDL_SetRenderTarget(renderer, texture_);
    SDL_RenderCopy(renderer, that.texture_, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);
}

Texture::Texture(Texture &&that) : Texture() {
    swap(*this, that);
}

Texture& Texture::operator=(Texture that) {
    swap(*this, that);
    return *this;
}

void ud::swap(Texture &first, Texture &second) {
    using std::swap;
    
    swap(first.texture_, second.texture_);
}

SDL_Texture * const Texture::get() const {
    return texture_;
}
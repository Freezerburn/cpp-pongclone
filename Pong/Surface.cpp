//
//  Surface.cpp
//  jpngame
//
//  Created by Vincent K on 12/25/13.
//  Copyright (c) 2013 Unlocked Doors. All rights reserved.
//

#include "Surface.h"
#include "Common.h"


using namespace ud;

Surface::Surface() : surface_(nullptr), file_() {
    std::cout << "Surface: constructing default (protected constructor)" << std::endl;
}

Surface::Surface(std::string file) : file_(file) {
    std::cout << "Surface: constructing with file: " << file << std::endl;
    surface_ = IMG_Load(file.c_str());
    if(surface_ == NULL) {
        throw std::runtime_error{std::string{"Surface could not load image: "} + IMG_GetError()};
    }
}

Surface::Surface(SDL_Surface *surface) : file_(), surface_(surface) {
    std::cout << "Surface: constructed with pre-created SDL_Surface." << std::endl;
    if(surface_ == NULL) {
        throw std::invalid_argument{"Cannot create a Surface with a NULL SDL_Surface*."};
    }
}

Surface::~Surface() {
    std::cout << "Surface: destructor called, freeing underlying SDL_Surface for file: " << file_ << std::endl;
    if(surface_ != nullptr) {
        SDL_FreeSurface(surface_);
    }
}

Surface::Surface(const Surface &that) {
    std::cout << "Surface: copy constructor called. copying file '" << that.file_ << "' into new Surface" << std::endl;
    surface_ = SDL_CreateRGBSurface(that.surface_->flags,
                                    that.surface_->w, that.surface_->h,
                                    that.surface_->format->BitsPerPixel,
                                    that.surface_->format->Rmask, that.surface_->format->Gmask, that.surface_->format->Bmask,
                                    that.surface_->format->Amask);
    if(surface_ == NULL) {
        throw std::runtime_error{std::string{"Surface could not create new RGB surface: "} + SDL_GetError()};
    }
    SDL_BlitSurface(that.surface_, NULL, surface_, NULL);
}

Surface::Surface(Surface &&that) : Surface() {
    std::cout << "Surface: move constructor called for file: " << that.file_ << std::endl;
    swap(*this, that);
}

Surface& Surface::operator=(Surface that) {
    std::cout << "Surface: assignment operator called for file: " << file_ << std::endl;
    swap(*this, that);
    return *this;
}

void ud::swap(Surface &first, Surface &second) {
    std::cout << "Surface: swapping two surfaces" << std::endl;
    using std::swap;
    
    swap(first.surface_, second.surface_);
}

SDL_Surface * const Surface::get() const {
    return surface_;
}
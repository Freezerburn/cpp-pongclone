//
//  Texture.h
//  jpngame
//
//  Created by Vincent K on 12/25/13.
//  Copyright (c) 2013 Unlocked Doors. All rights reserved.
//

#ifndef __jpngame__Texture__
#define __jpngame__Texture__

#include <iostream>
#include "Surface.h"

struct SDL_Texture;

namespace ud {
    class Texture {
    protected:
        SDL_Texture *texture_;
        
        Texture();
        
    public:
        Texture(std::shared_ptr<Surface> surface);
        ~Texture();
        
        Texture(const Texture &that);
        Texture(Texture &&that);
        Texture& operator=(Texture that);
        
        friend void swap(Texture &first, Texture &second);
        
        SDL_Texture * const get() const;
    };
}

#endif /* defined(__jpngame__Texture__) */

//
//  Surface.h
//  jpngame
//
//  Created by Vincent K on 12/25/13.
//  Copyright (c) 2013 Unlocked Doors. All rights reserved.
//

#ifndef __jpngame__Surface__
#define __jpngame__Surface__

#include <iostream>

struct SDL_Surface;

namespace ud {
    class Surface {
    protected:
        SDL_Surface *surface_;
        std::string file_;
        
        Surface();
        
    public:
        Surface(std::string file);
        Surface(SDL_Surface *surface);
        ~Surface();
        
        Surface(const Surface &that);
        Surface(Surface &&that);
        Surface& operator=(Surface that);
        
        friend void swap(Surface &first, Surface &second);
        
        SDL_Surface * const get() const;
    };
}

#endif /* defined(__jpngame__Surface__) */

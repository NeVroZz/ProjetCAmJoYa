#include <SDL/SDL.h>
#include <stdio.h>

// Initialisation simple de la fenêtre
SDL_Surface* initWindow(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur d'initialisation SDL: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Surface* screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
    }
    SDL_WM_SetCaption(title, NULL);
    return screen;
}

// Gestion des événements (clavier + souris)
void handleEvents(int* running, int* mouse_x, int* mouse_y) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            *running = 0;
        } else if (e.type == SDL_MOUSEMOTION) {
            *mouse_x = e.motion.x;
            *mouse_y = e.motion.y;
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
            *running = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    // Initialisation de la fenêtre
    SDL_Surface* screen = initWindow("Prototype SDL", 800, 600);
    if (!screen) return 1;

    // Charger une image simple pour l'entité (doit être dans le même dossier)
    SDL_Surface* image = SDL_LoadBMP("tortue.bmp");
    if (!image) {
        fprintf(stderr, "Erreur de chargement de l'image: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Position initiale de la tortue
    int tortue_x = 400, tortue_y = 300;
    int mouse_x = 0, mouse_y = 0;
    int running = 1;

    // Boucle principale
    while (running) {
        handleEvents(&running, &mouse_x, &mouse_y);

        // Déplacer la tortue vers la souris
        if (tortue_x < mouse_x) tortue_x++;
        if (tortue_x > mouse_x) tortue_x--;
        if (tortue_y < mouse_y) tortue_y++;
        if (tortue_y > mouse_y) tortue_y--;

        // Efface l'écran
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        // Dessine la tortue
        SDL_Rect pos = { tortue_x, tortue_y, 0, 0 };
        SDL_BlitSurface(image, NULL, screen, &pos);
    }

    // Libérer les ressources
    SDL_FreeSurface(image);
    SDL_Quit();
    return 0;
}

#include "lib.h"
#include "SDL.h"


sdlLib::sdlLib()
{
}


void callback(void* userData, Uint8* stream, int len) {
    sdlLib* sdl = (sdlLib*)userData;
    sdl->callbackWrapper(stream, len);
}

int sdlLib::initAudio()
{
	SDL_AudioSpec ideal, actual;

	ideal.freq = 44100;
	ideal.format = AUDIO_S16SYS;
	ideal.channels = 1;
	ideal.samples = 2048;
	ideal.callback = callback;
	ideal.userdata = this;

	if (SDL_OpenAudio(&ideal, &actual) < 0) {
		std::cout << "Fail\n";
		return 1;
	}

	audioFreq = actual.freq;
	audioBufferSize = actual.samples;

	return 0;
}

int sdlLib::initDisplay()
{
	window = SDL_CreateWindow("NES-emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, displayWidth,
		displayHeight, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		renderWidth, renderHeight);
	SDL_RenderSetLogicalSize(renderer, renderWidth, renderHeight);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	image = new uint32_t[renderWidth * renderHeight];

	return 0;
}

void sdlLib::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t pixel = 0;
	pixel |= 255 << 24;
	pixel |= (uint32_t)r << 16;
	pixel |= (uint32_t)g << 8;
	pixel |= (uint32_t)b << 0;
	image[x + y * renderWidth] = pixel;
}

void sdlLib::renderSync()
{
	SDL_UpdateTexture(texture, NULL, image, renderWidth * sizeof(uint32_t));
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void sdlLib::parseInput()
{
    SDL_Event event;
    int button;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_w:
                button = BUTTON_UP;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                button = BUTTON_DOWN;
                break;
            case SDLK_LEFT:
            case SDLK_a:
                button = BUTTON_LEFT;
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                button = BUTTON_RIGHT;
                break;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                button = BUTTON_START;
                break;
            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                button = BUTTON_SELECT;
                break;
            case SDLK_z:
            case SDLK_n:
                button = BUTTON_A;
                break;
            case SDLK_x:
            case SDLK_m:
                button = BUTTON_B;
                break;
            case SDLK_k:
                button = BUTTON_SAVE;
                break;
            case SDLK_l:
                button = BUTTON_RESTORE;
                break;
            default:
                continue;
            }
            if (event.type == SDL_KEYDOWN) {
                buttonState[button] = true;
            }
            else {
                buttonState[button] = false;
            }
            break;
        case SDL_QUIT:
            exit(1);
            break;
        default:
            break;
        }
    }
}

bool sdlLib::getButtonState(int button) {
    return buttonState[button]; 
}



void sdlLib::callbackWrapper(uint8_t* stream, int len)
{
    if (audioCallback) {
        audioCallback(callbackData, stream, len);
    }
}

void sdlLib::registerAudioCallback(Callback* audioCallback, void* data)
{
    callbackData = data;
    audioCallback = audioCallback;
    SDL_PauseAudio(0);
}

void sdlLib::unregisterAudioCallback()
{
    SDL_PauseAudio(1);
    audioCallback = nullptr;
    callbackData = nullptr;
}

uint32_t sdlLib::getSampleRate()
{
    return audioFreq;
}

uint32_t sdlLib::getChunkSize()
{
    return audioBufferSize;
}

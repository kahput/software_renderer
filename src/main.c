#include "core/arena.h"
#include "platform.h"

typedef struct {
	Arena *permanent, *frame;
} State;

int main(void) {
	State state = {
		.permanent = arena_alloc(),
		.frame = arena_alloc(),
	};
	Platform* platform = platform_startup(state.permanent, 1280, 720, "Software Renderer");

	while (platform_should_close(platform) == false) {
		platform_poll_events(platform);
	}

	platform_shutdown(platform);
	return 0;
}

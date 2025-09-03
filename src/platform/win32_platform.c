#include "win32_platform.h"
#include "platform.h"

#ifdef PLATFORM_WINDOWS
bool platform_init_win32(struct platform *platform) {
	return false;
}


bool win32_platform_startup(struct platform *platform) {
	return false;
}
void win32_platform_shutdown(struct platform *platform) {
	return false;
}

void win32_platform_poll_events(struct platform *platform);
bool win32_platform_should_close(struct platform *platform);

void win32_platform_get_window_size(struct platform *platform, uint32_t *width, uint32_t *height);
void win32_platform_get_framebuffer_size(struct platform *platform, uint32_t *width, uint32_t *height);

struct VkSurfaceKHR_T;
struct VkInstance_T;

bool win32_platform_create_vulkan_surface(struct platform *platform, struct VkInstance_T *instance, struct VkSurfaceKHR_T **surface);
#endif

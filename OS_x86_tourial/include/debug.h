#include "console.h"
#include "vargs.h"

void printk(const char *format, ...);
void printk_color(real_color_t back, real_color_t fore, const char *format , ...);
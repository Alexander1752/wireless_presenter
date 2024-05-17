#ifdef __cplusplus
extern "C" {
#endif

// #define DEBUG 1

#ifdef DEBUG
#define dprintf(format, ...) {if(DEBUG)printf(format, ##__VA_ARGS__);}
#else
#define dprintf(format, ...)
#endif

#ifdef __cplusplus
}
#endif

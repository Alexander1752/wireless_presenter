#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG 1

#define dprintf(format, ...) {if(DEBUG)printf(format, ##__VA_ARGS__);}

#ifdef __cplusplus
}
#endif

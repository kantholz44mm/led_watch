extern void _estack(void);
extern void _reset(void);

__attribute__((section(".vectors"))) void (*const tab[16 + 32])(void) = {
    _estack,
    _reset
};
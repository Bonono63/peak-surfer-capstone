#include <stdio.h>
#include <stdint.h>
#include <stdbit.h>

struct CajuState {
    uint32_t k0;
    uint32_t k1;
    uint32_t k2;
    uint32_t k3;
};

uint8_t toffoli(uint8_t x, uint8_t y, uint8_t z) {
    return x ^ (y & z);
}

void s_box(uint32_t *S) {
    uint8_t z = *S & 0xFF;
    uint8_t y = (*S >> 8) & 0xFF;
    uint8_t x = (*S >> 16) & 0xFF;
    uint8_t w = (*S >> 24) & 0xFF;
    x = toffoli(x, w, y);
    z = toffoli(z, x, y);
    y = toffoli(y, w, z);
    w = toffoli(w, x, z);
    *S = z | (y << 8) | (x << 16) | (w << 24);
}

uint8_t a_lcs_sigma_xor_b(uint8_t A, uint8_t B, uint8_t sigma) {
    return __builtin_stdc_rotate_left(A, sigma) ^ B;
}

void linear_map(uint32_t *S) {
    uint8_t z = *S & 0xFF;
    uint8_t y = (*S >> 8) & 0xFF;
    uint8_t x = (*S >> 16) & 0xFF;
    uint8_t w = (*S >> 24) & 0xFF;
    uint8_t sigma_1 = 2, sigma_2 = 6, sigma_3 = 1, sigma_4 = 7;
    w = a_lcs_sigma_xor_b(w, x, sigma_1);
    x = a_lcs_sigma_xor_b(x, w, sigma_2);
    w = a_lcs_sigma_xor_b(w, x, sigma_3);
    x = a_lcs_sigma_xor_b(x, w, sigma_4);
    uint8_t wt = w, xt = x;
    w = y;
    x = z;
    y = wt;
    z = xt;
    *S = z | (y << 8) | (x << 16) | (w << 24);
}



void caju_update_key(struct CajuState *state) {
    uint32_t k2 = state->k2;
    state->k2 = __builtin_stdc_rotate_left(state->k0 ^ state->k1, 19) ^ state->k3;
    state->k3 = state->k0 ^ state->k1;
    state->k0 = state->k1;
    state->k1 = k2;
}

uint32_t caju_round(struct CajuState *state, uint32_t *x, uint32_t round) {
    s_box(x);
    linear_map(x);
    caju_update_key(state);
    uint32_t round_key = state->k0 ^ round;
    // printf("round key: 0x%x\n", round_key);
    *x = *x ^ round_key;
    // printf("x: 0x%08x\n", *x);
}

void __attribute__((noinline)) caju(struct CajuState *state, uint32_t *x) {
    *x = *x ^ state->k0;
    // printf("pre-add key: 0x%08x\n", state->k0);
    // printf("x: 0x%08x\n", *x);
    for(int i = 0; i < 12; i++) {
        caju_round(state, x, i);
    }
}

int main() {

    struct CajuState state = {
        .k0 = 0x01234567,
        .k1 = 0x89abcdef,
        .k2 = 0x12345678,
        .k3 = 0x9abcdef0,
    };

    uint32_t x = 0x78563412;

    caju(&state, &x);
    printf("x: 0x%08x\n", x);

    for(int i = 0; i < 10000000; i++) {
        uint32_t x = i;
        caju(&state, &x);
    }
}
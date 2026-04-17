/*
 * Simon128 Block Cipher Implementation
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#define u8  uint8_t
#define u64 uint64_t



#define ROTL64(x,r) (((x)<<(r)) | ((x)>>(64-(r))))
#define ROTR64(x,r) (((x)>>(r)) | ((x)<<(64-(r))))


void BytesToWords64(u8 bytes[], u64 words[], int numbytes)
{
    int i, j = 0;
    for (i = 0; i < numbytes / 8; i++) {
        words[i] = (u64)bytes[j]         |
                   ((u64)bytes[j+1] << 8)  |
                   ((u64)bytes[j+2] << 16) |
                   ((u64)bytes[j+3] << 24) |
                   ((u64)bytes[j+4] << 32) |
                   ((u64)bytes[j+5] << 40) |
                   ((u64)bytes[j+6] << 48) |
                   ((u64)bytes[j+7] << 56);
        j += 8;
    }
}

void Words64ToBytes(u64 words[], u8 bytes[], int numwords)
{
    int i, j = 0;
    for (i = 0; i < numwords; i++) {
        bytes[j]   = (u8)words[i];
        bytes[j+1] = (u8)(words[i] >> 8);
        bytes[j+2] = (u8)(words[i] >> 16);
        bytes[j+3] = (u8)(words[i] >> 24);
        bytes[j+4] = (u8)(words[i] >> 32);
        bytes[j+5] = (u8)(words[i] >> 40);
        bytes[j+6] = (u8)(words[i] >> 48);
        bytes[j+7] = (u8)(words[i] >> 56);
        j += 8;
    }
}

/* ============================================================
 * SIMON128 — Feistel cipher, 64-bit words
 *
 * Round function: f(x) = (x<<<1 & x<<<8) ^ x<<<2
 * Two-rounds-at-a-time macro avoids word swaps.
 * ============================================================ */

#define f64(x)            ((ROTL64(x,1) & ROTL64(x,8)) ^ ROTL64(x,2))
#define R64x2(x,y,k1,k2) (y ^= f64(x), y ^= (k1), x ^= f64(y), x ^= (k2))

/* --- Simon128/128  (key: 128 bits, rounds: 68) ------------- */

void Simon128128KeySchedule(u64 K[], u64 rk[])
{
    u64 i, B = K[1], A = K[0];
    u64 c = 0xfffffffffffffffcULL, z = 0x7369f885192c0ef5ULL;

    for (i = 0; i < 64;) {
        rk[i++] = A; A ^= c ^ (z & 1) ^ ROTR64(B,3) ^ ROTR64(B,4); z >>= 1;
        rk[i++] = B; B ^= c ^ (z & 1) ^ ROTR64(A,3) ^ ROTR64(A,4); z >>= 1;
    }
    rk[64] = A; A ^= c ^ 1 ^ ROTR64(B,3) ^ ROTR64(B,4);
    rk[65] = B; B ^= c ^ 0 ^ ROTR64(A,3) ^ ROTR64(A,4);
    rk[66] = A; rk[67] = B;
}

void Simon128128Encrypt(u64 Pt[], u64 Ct[], u64 rk[])
{
    u64 i;
    Ct[0] = Pt[0]; Ct[1] = Pt[1];
    for (i = 0; i < 68; i += 2) R64x2(Ct[1], Ct[0], rk[i], rk[i+1]);
}

void Simon128128Decrypt(u64 Pt[], u64 Ct[], u64 rk[])
{
    int i;
    Pt[0] = Ct[0]; Pt[1] = Ct[1];
    for (i = 67; i >= 0; i -= 2) R64x2(Pt[0], Pt[1], rk[i], rk[i-1]);
}

/* --- Simon128/192  (key: 192 bits, rounds: 69) ------------- */

void Simon128192KeySchedule(u64 K[], u64 rk[])
{
    u64 i, C = K[2], B = K[1], A = K[0];
    u64 c = 0xfffffffffffffffcULL, z = 0xfc2ce51207a635dbULL;

    for (i = 0; i < 63;) {
        rk[i++] = A; A ^= c ^ (z & 1) ^ ROTR64(C,3) ^ ROTR64(C,4); z >>= 1;
        rk[i++] = B; B ^= c ^ (z & 1) ^ ROTR64(A,3) ^ ROTR64(A,4); z >>= 1;
        rk[i++] = C; C ^= c ^ (z & 1) ^ ROTR64(B,3) ^ ROTR64(B,4); z >>= 1;
    }
    rk[63] = A; A ^= c ^ 1 ^ ROTR64(C,3) ^ ROTR64(C,4);
    rk[64] = B; B ^= c ^ 0 ^ ROTR64(A,3) ^ ROTR64(A,4);
    rk[65] = C; C ^= c ^ 1 ^ ROTR64(B,3) ^ ROTR64(B,4);
    rk[66] = A; rk[67] = B; rk[68] = C;
}

void Simon128192Encrypt(u64 Pt[], u64 Ct[], u64 rk[])
{
    u64 i, t;
    Ct[0] = Pt[0]; Ct[1] = Pt[1];
    for (i = 0; i < 68; i += 2) R64x2(Ct[1], Ct[0], rk[i], rk[i+1]);
    /* one final odd round */
    t = Ct[1]; Ct[1] = Ct[0] ^ f64(Ct[1]) ^ rk[68]; Ct[0] = t;
}

void Simon128192Decrypt(u64 Pt[], u64 Ct[], u64 rk[])
{
    int i;
    u64 t;
    Pt[0] = Ct[0]; Pt[1] = Ct[1];
    t = Pt[0]; Pt[0] = Pt[1] ^ f64(Pt[0]) ^ rk[68]; Pt[1] = t;
    for (i = 67; i >= 0; i -= 2) R64x2(Pt[0], Pt[1], rk[i], rk[i-1]);
}

/* --- Simon128/256  (key: 256 bits, rounds: 72) ------------- */

void Simon128256KeySchedule(u64 K[], u64 rk[])
{
    u64 i, D = K[3], C = K[2], B = K[1], A = K[0];
    u64 c = 0xfffffffffffffffcULL, z = 0xfdc94c3a046d678bULL;

    for (i = 0; i < 64;) {
        rk[i++] = A; A ^= c^(z&1)^ROTR64(D,3)^ROTR64(D,4)^B^ROTR64(B,1); z>>=1;
        rk[i++] = B; B ^= c^(z&1)^ROTR64(A,3)^ROTR64(A,4)^C^ROTR64(C,1); z>>=1;
        rk[i++] = C; C ^= c^(z&1)^ROTR64(B,3)^ROTR64(B,4)^D^ROTR64(D,1); z>>=1;
        rk[i++] = D; D ^= c^(z&1)^ROTR64(C,3)^ROTR64(C,4)^A^ROTR64(A,1); z>>=1;
    }
    rk[64]=A; A^=c^0^ROTR64(D,3)^ROTR64(D,4)^B^ROTR64(B,1);
    rk[65]=B; B^=c^1^ROTR64(A,3)^ROTR64(A,4)^C^ROTR64(C,1);
    rk[66]=C; C^=c^0^ROTR64(B,3)^ROTR64(B,4)^D^ROTR64(D,1);
    rk[67]=D; D^=c^0^ROTR64(C,3)^ROTR64(C,4)^A^ROTR64(A,1);
    rk[68]=A; rk[69]=B; rk[70]=C; rk[71]=D;
}

void Simon128256Encrypt(u64 Pt[], u64 Ct[], u64 rk[])
{
    u64 i;
    Ct[0] = Pt[0]; Ct[1] = Pt[1];
    for (i = 0; i < 72; i += 2) R64x2(Ct[1], Ct[0], rk[i], rk[i+1]);
}

void Simon128256Decrypt(u64 Pt[], u64 Ct[], u64 rk[])
{
    int i;
    Pt[0] = Ct[0]; Pt[1] = Ct[1];
    for (i = 71; i >= 0; i -= 2) R64x2(Pt[0], Pt[1], rk[i], rk[i-1]);
}

/* ============================================================
 * Test vector helpers
 * ============================================================ */

static void print_hex(const char *label, u8 *buf, int len)
{
    printf("%s", label);
    for (int i = 0; i < len; i++) printf("%02x ", buf[i]);
    printf("\n");
}

static int bytes_equal(u8 *a, u8 *b, int len)
{
    for (int i = 0; i < len; i++) if (a[i] != b[i]) return 0;
    return 1;
}

static int run_test(const char *name,
                    u8 *pt, u8 *key, int keylen,
                    u8 *expected,
                    void (*ks)(u64*, u64*),
                    void (*enc)(u64*, u64*, u64*),
                    void (*dec)(u64*, u64*, u64*),
                    u64 *rk, int rklen)
{
    u64 Pt[2], K[4], Ct[2], Dt[2];
    u8 ct[16], dt[16];
    (void)rklen;

    BytesToWords64(pt,  Pt, 16);
    BytesToWords64(key, K,  keylen);
    ks(K, rk);
    enc(Pt, Ct, rk);
    Words64ToBytes(Ct, ct, 2);
    dec(Dt, Ct, rk);
    Words64ToBytes(Dt, dt, 2);

    int enc_ok = bytes_equal(ct, expected, 16);
    int dec_ok = bytes_equal(dt, pt, 16);
    printf("%-16s encrypt: %s  decrypt: %s\n",
           name, enc_ok ? "PASS" : "FAIL", dec_ok ? "PASS" : "FAIL");
    if (!enc_ok) {
        print_hex("  got:      ", ct, 16);
        print_hex("  expected: ", expected, 16);
    }
    return enc_ok && dec_ok;
}

/* ============================================================
 * main
 * ============================================================ */

int main(void)
{
    int all = 1;

    /* ---- Simon128 test vectors ---- */

    u8 s128_128_pt[]  = {0x20,0x74,0x72,0x61,0x76,0x65,0x6c,0x6c,
                         0x65,0x72,0x73,0x20,0x64,0x65,0x73,0x63};
    u8 s128_128_key[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                         0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    u8 s128_128_ct[]  = {0xbc,0x0b,0x4e,0xf8,0x2a,0x83,0xaa,0x65,
                         0x3f,0xfe,0x54,0x1e,0x1e,0x1b,0x68,0x49};

    u8 s128_192_pt[]  = {0x72,0x69,0x62,0x65,0x20,0x77,0x68,0x65,
                         0x6e,0x20,0x74,0x68,0x65,0x72,0x65,0x20};
    u8 s128_192_key[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                         0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
                         0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17};
    u8 s128_192_ct[]  = {0x5b,0xb8,0x97,0x25,0x6e,0x8d,0x9c,0x6c,
                         0x4f,0x0d,0xdc,0xfc,0xef,0x61,0xac,0xc4};

    u8 s128_256_pt[]  = {0x69,0x73,0x20,0x61,0x20,0x73,0x69,0x6d,
                         0x6f,0x6f,0x6d,0x20,0x69,0x6e,0x20,0x74};
    u8 s128_256_key[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
                         0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
                         0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
                         0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};
    u8 s128_256_ct[]  = {0x68,0xb8,0xe7,0xef,0x87,0x2a,0xf7,0x3b,
                         0xa0,0xa3,0xc8,0xaf,0x79,0x55,0x2b,0x8d};

    u64 rk[72]; 

    printf("=== Simon128 Test Vectors ===\n\n");
    all &= run_test("Simon128/128", s128_128_pt, s128_128_key, 16, s128_128_ct,
                    Simon128128KeySchedule, Simon128128Encrypt, Simon128128Decrypt, rk, 68);
    all &= run_test("Simon128/192", s128_192_pt, s128_192_key, 24, s128_192_ct,
                    Simon128192KeySchedule, Simon128192Encrypt, Simon128192Decrypt, rk, 69);
    all &= run_test("Simon128/256", s128_256_pt, s128_256_key, 32, s128_256_ct,
                    Simon128256KeySchedule, Simon128256Encrypt, Simon128256Decrypt, rk, 72);

    printf("\n%s\n", all ? "All tests PASSED." : "One or more tests FAILED.");
    return all ? 0 : 1;
}

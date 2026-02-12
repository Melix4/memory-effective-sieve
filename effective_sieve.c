#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <limits.h>

#define MAXN 10000
#define EFFICIENCY_SCALE CHAR_BIT * 3  // во сколько раз решето эффективнее обычного по памяти

struct sieve_t{
    int n;
    unsigned char *mod1;
    unsigned char *mod5;
};


int byte_number(int n);
int bit_number(int n);

int sieve_bound(int n);
void fill_sieve(struct sieve_t *sv);
int is_prime(struct sieve_t *sv, unsigned n);

int is_prime_slow(unsigned n);

int main() {
    struct sieve_t sv;
    sv.n = sieve_bound(MAXN);
    sv.mod1 = calloc(sv.n, sizeof(unsigned char));
    sv.mod5 = calloc(sv.n, sizeof(unsigned char));
    fill_sieve(&sv);

    for (int i = 1; i <= 5000; i++) {
        assert(is_prime(&sv, i) == is_prime_slow(i));
        printf("Accepted\n");
    }
    free(&sv);
    return 0;
}

int sieve_bound(int n) {
    double dn = n * 1.0;
    double dres = dn * (log(dn) + log(log(dn)));
    return (int)(dn + EFFICIENCY_SCALE - 1) / EFFICIENCY_SCALE;
}

void fill_sieve(struct sieve_t *sv) {
    if (sv->n == 0) 
        abort();

    sv->mod1[0] |= 1;
    
    long long max_num = 6 * sv->n * CHAR_BIT, i = 1;
    int prime, cnt = 0;        
    while (i <= max_num) {
        prime = 0;
        if (i % 6 == 1 && !(sv->mod1[byte_number(i)] & (1 << bit_number(i)))) 
            prime = 1;
        if (i % 6 == 5 && !(sv->mod5[byte_number(i)] & (1 << bit_number(i))))
            prime = 1;
        
        if (prime)
            for (long long j = i * i; j <= max_num; j += 2 * i) {
                if (j % 6 == 1)
                    sv->mod1[byte_number(j)] |= (1 << bit_number(j));
                if (j % 6 == 5)
                    sv->mod5[byte_number(j)] |= (1 << bit_number(j));
            }
        if (cnt % 2 == 0)
            i += 4;
        else
            i += 2;
        cnt++;
    }
}

int is_prime(struct sieve_t *sv, unsigned n) {
    if (n <= 0 || n >= 6 * sv->n * CHAR_BIT)
        abort();
    if (n == 2 || n == 3)
        return 1;
    if (n % 6 == 1)
        return (!(sv->mod1[byte_number(n)] & (1 << bit_number(n)))) ? 1 : 0;
    if (n % 6 == 5)
        return (!(sv->mod5[byte_number(n)] & (1 << bit_number(n)))) ? 1 : 0;
    return 0;
}

int is_prime_slow(unsigned n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}

int byte_number(int n) {
    return (n / 6) / CHAR_BIT;
}

int bit_number(int n) {
    return (n / 6) % CHAR_BIT;
}

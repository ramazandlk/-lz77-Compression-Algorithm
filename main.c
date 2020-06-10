#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define DENK_BIT 5
#define KOPYA_BIT (8-DENK_BIT)
#define DENK_MASK ((1 << (DENK_BIT)) - 1)
#define KOPYA_MASK ((1 << (KOPYA_BIT)) - 1)
#define DENK_KOPYA(x,y) (x << KOPYA_BIT | y)


struct token
{
    uint8_t offset_len;
    char c;
};

struct token *encode(char *text, int kac_karakter, int *token_say)
{
    int kapasite = 1 << 3;
    int token_say_temp= 0;
    struct token t;
    char *islenmemis_buf, *arama_buffer;
    struct token *encoded = malloc(kapasite * sizeof(struct token));

    for (islenmemis_buf = text; islenmemis_buf < text + kac_karakter; islenmemis_buf++)
    {
        arama_buffer = islenmemis_buf - DENK_MASK; // gerisine koy

        if (arama_buffer < text)
            arama_buffer = text;

        int max_benzer = 0;
        char *max_poz = islenmemis_buf;

        for (arama_buffer; arama_buffer < islenmemis_buf; arama_buffer++)
        {
            int len = prefix_poz_benzer(arama_buffer, islenmemis_buf, KOPYA_MASK);

            if (len > max_benzer)
            {
                max_benzer = len;
                max_poz = arama_buffer;
            }
        }

        if (islenmemis_buf + max_benzer >= text + kac_karakter)
        {
            max_benzer = text + kac_karakter - islenmemis_buf - 1;
        }

        t.offset_len = DENK_KOPYA(islenmemis_buf - max_poz, max_benzer);
        islenmemis_buf += max_benzer;
        t.c = *islenmemis_buf;


        if (token_say_temp + 1 > kapasite) // hafýzada yer aç
        {
            kapasite = kapasite << 1;
            encoded = realloc(encoded, kapasite * sizeof(struct token));
        }

        encoded[token_say_temp++] = t;
    }

    if (token_say)
        *token_say = token_say_temp;

    return encoded;
}

 int prefix_poz_benzer(char *k1, char *k2, int limit)
{
    int len = 0;

    while (*k1++ == *k2++ && len < limit)
        len++;

    return len;
}

char *file_read(FILE *f, int *boyut)
{
    char *veri;
    fseek(f, 0, SEEK_END);
    *boyut = ftell(f);
    veri = malloc(*boyut);
    fseek(f, 0, SEEK_SET);
    fread(veri, 1, *boyut, f);
    return veri;
}


int main(void) {

FILE *f;
    int metin_boyut , token_say;
    char *kaynak;
    struct token *encoded_text;

    if (f = fopen("metin.txt", "rb"))
    {
        kaynak = file_read(f, &metin_boyut);
        fclose(f);
    }

    encoded_text = encode(kaynak, metin_boyut, &token_say);

    if (f = fopen("encoded.txt", "w"))
    {
        fwrite(encoded_text, sizeof(struct token), token_say, f);
        fclose(f);
    }

    printf("Orjinal Boyut: %d, Encode Boyutu: %d\n", metin_boyut, token_say * sizeof(struct token));




    return 0;
}



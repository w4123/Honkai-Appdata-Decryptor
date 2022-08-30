#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <cstdio>
#include <cstring>

int box[624];
int v3 = 0;
void init(int seed)
{
    box[0] = seed;
    for (long long i = 1; i != 624; ++i)
    {
        box[i] = i + 1812433253 * (box[i - 1] ^ (unsigned int)(box[i - 1] >> 30));;
    }
}
int getInt()
{
    if (v3 == 0)
    {
        int v2 = 0;
        while (1)
        {
            int v5 = (v2 + 1) % 624;
            int v6 = (box[v2]) ^ ((box[v2]) ^ (box[v5])) & 0x7FFFFFFF;
            int v7 = (v2 + 397) % 624;
            box[v2] = (v6 >> 1) ^ box[v7];
            if ((v6 & 1) != 0)
            {
                box[v2] ^= 0x9908B0DF;
            }
            if (++v2 == 624)
                break;
        }
    }
    int v4 = box[v3];
    int v5 = v3 + 1;
    long long v6 = ((((v4 >> 11) ^ v4) & 0xFF3A58AD) << 7) ^ (v4 >> 11) ^ v4;
    long long v7 = (int)((unsigned long long)(3524075731LL * ((int)v3 + 1)) >> 32) >> 9;
    int result1 = ((v6 & 0xFFFFDF8C) << 15) ^ v6 ^ ((int)(((v6 & 0xFFFFDF8C) << 15) ^ v6) >> 18);
    v3 = v5 - 624 * ((v7 >> 31) + v7);
    return result1;
}
char key[0x200];
char data[0x80];
char buffer[4096];
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("Usage: %s [input] [output]", argv[0]);
        return -1;
    }
    
    FILE* f = fopen(argv[1], "rb");
    if (!f)
    {
        printf("Failed to open input file");
        return -1;
    }

    int seed = 0;
    fread(&seed, sizeof(char), 4, f);
    fread(data, sizeof(char), 0x80, f);
    init(seed);

    for (int i = 0; i != 128; i++)
    {
        int number = getInt();
        char* b = (char*)&number;
        b[0] ^= data[(i * 4) % 0x80];
        b[1] ^= data[(i * 4 + 7) % 0x80];
        b[2] ^= data[(i * 4 + 13) % 0x80];
        b[3] ^= data[(i * 4 + 11) % 0x80];
        memcpy(&key[i * 4], b, 4);
    }

    long long off = getInt();

    FILE* f2 = fopen(argv[2], "wb");
    if (!f2)
    {
        printf("Failed to open output file");
        return -1;
    }

    while (true)
    {
        size_t nbytes = fread(buffer, sizeof(char), 4096, f);

        for (int i = 0; i != nbytes; i++)
        {
            buffer[i] ^= key[(off++) % 0x200];
        }

        fwrite(buffer, sizeof(char), nbytes, f2);

        if (feof(f) || ferror(f))
            break;
    }

    fclose(f);
    fclose(f2);
    return 0;
}

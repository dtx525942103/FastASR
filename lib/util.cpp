
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "Tensor.h"

float *loadparams(const char *filename)
{

    FILE *fp;
    fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_END);
    uint32_t nFileLen = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    float *params_addr = (float *)aligned_malloc(32, nFileLen);
    int n = fread(params_addr, 1, nFileLen, fp);
    fclose(fp);

    return params_addr;
}

int val_align(int val, int align)
{
    float tmp = ceil((float)val / (float)align) * (float)align;
    return (int)tmp;
}

void disp_params(float *din, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        printf("%f ", din[i]);
    }
    printf("\n");
}
void SaveDataFile(const char *filename, void *data, uint32_t len)
{
    FILE *fp;
    fp = fopen(filename, "wb+");
    fwrite(data, 1, len, fp);
    fclose(fp);
}

string pathAppend(const string &p1, const string &p2)
{

    char sep = '/';
    string tmp = p1;

#ifdef _WIN32
    sep = '\\';
#endif

    if (p1[p1.length()] != sep) { // Need to add a
        tmp += sep;               // path separator
        return (tmp + p2);
    } else
        return (p1 + p2);
}

void relu(Tensor<float> *din)
{
    int i;
    for (i = 0; i < din->buff_size; i++) {
        float val = din->buff[i];
        din->buff[i] = val < 0 ? 0 : val;
    }
}

void swish(Tensor<float> *din)
{
    int i;
    for (i = 0; i < din->buff_size; i++) {
        float val = din->buff[i];
        din->buff[i] = val / (1 + exp(-val));
    }
}


void softmax(float *din, int mask, int len)
{
    float *tmp = (float *)malloc(mask * sizeof(float));
    int i;
    float sum = 0;
    float max = -INFINITY;

    for (i = 0; i < mask; i++) {
        max = max < din[i] ? din[i] : max;
    }
    max = max * 0.9;

    for (i = 0; i < mask; i++) {
        tmp[i] = exp(din[i] - max);
        sum += tmp[i];
    }
    for (i = 0; i < mask; i++) {
        din[i] = tmp[i] / sum;
    }
    free(tmp);
    for (i = mask; i < len; i++) {
        din[i] = 0;
    }
}

void log_softmax(float *din, int len)
{
    float *tmp = (float *)malloc(len * sizeof(float));
    int i;
    float sum = 0;
    for (i = 0; i < len; i++) {
        tmp[i] = exp(din[i]);
        sum += tmp[i];
    }
    for (i = 0; i < len; i++) {
        din[i] = log(tmp[i] / sum);
    }
    free(tmp);
}

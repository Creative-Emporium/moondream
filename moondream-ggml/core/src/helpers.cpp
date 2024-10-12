#include <cstdio>
#include <limits>

#include "helpers.hpp"

size_t utf8_len(char src) {
    const size_t lookup[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 4 };
    uint8_t highbits = static_cast<uint8_t>(src) >> 4;
    return lookup[highbits];
}

double bytes_to_gib(size_t n_bytes) {
    return static_cast<double>(n_bytes) / (1024.0 * 1024.0 * 1024.0);
}

bool size_to_int32(size_t s, int32_t * i) {
    if (s <= static_cast<size_t>(std::numeric_limits<int32_t>::max())) {
        *i = static_cast<int32_t>(s);
        return true;
    }
    return false;
}

void set_tensor_name(ggml_tensor * cur, const char * name, int il) {
    if (il >= 0) {
        ggml_format_name(cur, "%s-%d", name, il);
    } else {
        ggml_set_name(cur, name);
    }
}

void log_tensor(ggml_tensor * dst, const ggml_tensor * src, int ith, int nth, void * userdata) {
    if (ith != 0) {
        // Only log from the first thread.
        return;
    }

    printf("Shape: %lld %lld %lld %lld\n", dst->ne[3], dst->ne[2], dst->ne[1], dst->ne[0]);
    switch (dst->type) {
        case GGML_TYPE_F16:
            printf("Type: f16\n");
            break;
        case GGML_TYPE_F32:
            printf("Type: f32\n");
            break;
        default:
            printf("Type: unknown\n");
            break;
    }

    //printf("logged tensor first value: %f\n", ((float *)src->data)[128]);
    // Emit last 2 dimension values.
    int n_channels = src->ne[2];
    for (int j = 0; j < 1/*src->ne[0]*/; j++) {
        for (int i = 0; i < src->ne[1]; i++) {
            if (i > 0) {
                printf("\t");
            }
            float f = ((float *)src->data)[(j * src->ne[1] + i) * n_channels];
            //float f = ggml_get_f32_nd(src, i, j, 0, 0);
            printf("%.7f", (double)f);
        }
        printf("\n");
    }
}

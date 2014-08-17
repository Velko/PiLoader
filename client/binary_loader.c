/* Copyright (c) 2013 Jurģis Brigmanis
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "bootpc.h"
#include <sys/stat.h>
#include <stdlib.h>

struct bin_part *allocate_parts(struct bin_part *original, size_t new_count, size_t *p_size)
{
    // new count is small enough, return immediately
    if (new_count < *p_size)
        return original;

    // double the size
    (*p_size) *= 2;
    struct bin_part *parts = realloc(original, sizeof(struct bin_part) * (*p_size));
    if (parts == NULL) {
        vm_fail("Out of memory or something.\n");
    }
    return parts;
}

struct bin_part *split_binary(uint32_t *sdata, size_t data_size)
{
    data_size /= sizeof(uint32_t);
    size_t part_size = 8;
    size_t current_part = 0;
    struct bin_part *parts = malloc(sizeof(struct bin_part) * (part_size));

    size_t data_idx = 0;

    bool zero = false;

    parts[current_part].addr = sdata;
    parts[current_part].size = 0;

    for (data_idx = 0; data_idx < data_size; ++data_idx)
    {
        if (zero && sdata[data_idx] != 0)
        {
            // switch to nonzero
            if (parts[current_part].size < 512)
            {
                // zero part was too small, switch back to previous non-zero
                --current_part;
                parts[current_part].size += parts[current_part+1].size;
                parts[current_part+1].size = 0;
            } else {
                // there was enough zeros, create new non-zero part
                ++current_part;
                parts = allocate_parts(parts, current_part+1, &part_size);
                parts[current_part].addr = sdata+data_idx;
                parts[current_part].size = 0;
            }
            zero = false;
        }
        else if (!zero && sdata[data_idx] == 0)
        {
            // switch to zero part
            ++current_part;
            parts = allocate_parts(parts, current_part+1, &part_size);
            parts[current_part].addr = NULL;
            parts[current_part].size = 0;
            zero = true;
        }
        parts[current_part].size += sizeof(uint32_t);
    }

    // add terminating part
    ++current_part;
    parts = allocate_parts(parts, current_part+1, &part_size);
    parts[current_part].addr = NULL;
    parts[current_part].size = 0;

    return parts;
}

void load_binary()
{
    struct stat st;
    fstat(fileno(ufile), &st);

    void *sdata = malloc(st.st_size);
    if (sdata == NULL) {
        vm_fail("Out of memory or something.\n");
    }
    fseek(ufile, 0, SEEK_SET);
    fread(sdata, st.st_size, 1, ufile);


    struct bin_part *parts = split_binary(sdata, st.st_size);
    int i;

    for (i = 0; parts[i].size > 0; ++i)
    {
        if (parts[i].addr) {
            vm_print_s("LOAD %08x %08x %08x...", e_load, parts[i].addr - sdata, parts[i].size);
            load_buffer(e_load, parts[i].addr, parts[i].size);
            e_load += parts[i].size;
        } else
        {
            zero_section(e_load, parts[i].size);
            e_load += parts[i].size;
        }
    }

    free(parts);
    free(sdata);
}

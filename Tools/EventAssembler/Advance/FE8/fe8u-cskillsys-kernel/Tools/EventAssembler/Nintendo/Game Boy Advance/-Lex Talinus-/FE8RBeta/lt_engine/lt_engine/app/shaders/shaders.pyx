# distutils: language=c
# cython: binding=False, boundscheck=False, wraparound=False, nonecheck=False, cdivision=True
# cython: optimize.use_switch=True
# cython: profile=False
# encoding: utf-8

# import numpy 

cimport cython
from cython.parallel cimport prange, parallel

from libc.stdlib cimport malloc, free
from libc.math cimport sin, cos, pow, atan2, floor, ceil, fabs, sqrt

import pygame
from pygame import BLEND_RGB_ADD
from pygame.surfarray import pixels3d, make_surface
from pygame.transform import scale, smoothscale

cdef int THREADS = 4

# Colorkey definition
cdef unsigned char COLORKEY_R = 128
cdef unsigned char COLORKEY_G = 160
cdef unsigned char COLORKEY_B = 128

# GENERIC HELPER FUNCTIONS
@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
@cython.cdivision(True)
cdef inline float minf(float a, float b)nogil:
    return b if b < a else a

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
@cython.cdivision(True)
cdef inline float maxf(float a, float b)nogil:
    return b if b > a else a

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
@cython.cdivision(True)
cdef inline float clampf(float n, float low, float high)nogil:
    cdef float max_ = maxf(low, high)
    cdef float min_ = minf(low, high)
    return minf(max_, maxf(min_, n))

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
@cython.cdivision(True)
cdef inline float smoothstep(float edge0, float edge1, float x)nogil:
    cdef float t = clampf((x - edge0) / (edge1 - edge0), 0., 1.)
    return t * t * (3. - 2. * t)

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
@cython.cdivision(True)
cdef inline float CL(float x, float a, float b)nogil:
    return smoothstep(0, 1, (2./3.) * (x - a) / (b - a) + (1./6.)) * (b - a) + a

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
@cython.cdivision(True)
cdef inline float mixlerp(float a, float b, float perc)nogil:
    return a * (1 - perc) + b * perc
    
# ACTUAL SHADERS START HERE
@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
@cython.cdivision(True)
cdef inline void shader_invert(unsigned char[:, :, :] rgb_array):
    """
    Takes a surface (rgb_array) and inverts the color of the pixels
    Expects r, g, b array to be size w x h x 3 or w x h x 4
    """
    cdef Py_ssize_t w, h
    w, h = rgb_array.shape[:2]

    cdef:
        int i=0, j=0
        unsigned char *r
        unsigned char *g
        unsigned char *b

    with nogil, parallel():
        for j in prange(h, schedule="static"):
            for i in range(w):
                r = &rgb_array[i, j, 0]
                g = &rgb_array[i, j, 1]
                b = &rgb_array[i, j, 2]
                if r[0] != COLORKEY_R and g[0] != COLORKEY_G and b[0] != COLORKEY_B:
                    r[0] = 255 - r[0]
                    g[0] = 255 - g[0]
                    b[0] = 255 - b[0]
# End shader_invert

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
@cython.cdivision(True)
cdef inline void shader_bright_pass_filter(unsigned char[:, :, :] rgb_array, int threshold=128):
    assert 0 <= threshold <= 255, "Threshold must be in range 0 to 255"

    cdef Py_ssize_t w, h
    w, h = rgb_array.shape[:2]

    cdef:
        int i=0, j=0
        float lum, c
        unsigned char *r
        unsigned char *g
        unsigned char *b

    with nogil, parallel():
        for j in prange(h, schedule="static"):
            for i in range(w):
                r = &rgb_array[i, j, 0]
                g = &rgb_array[i, j, 1]
                b = &rgb_array[i, j, 2]

                if r[0] != COLORKEY_R and g[0] != COLORKEY_G and b[0] != COLORKEY_B:
                    lum = r[0] * <float>0.299 + g[0] * <float>0.587 + b[0] * <float>0.114
                    
                    if lum > threshold:
                        c = (lum - threshold) / lum
                        r[0] = <unsigned char>(r[0] * c)
                        g[0] = <unsigned char>(g[0] * c)
                        b[0] = <unsigned char>(b[0] * c)
                    else:
                        r[0] = 0
                        g[0] = 0
                        b[0] = 0
# End shader_bright_pass_filter

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
@cython.cdivision(True)
cdef inline shader_bpf_return(unsigned char[:, :, :] rgb_array, int threshold=128):
    assert 0 <= threshold <= 255, "Threshold must be in range 0 to 255"

    cdef Py_ssize_t w, h
    w, h = rgb_array.shape[:2]

    cdef:
        int i=0, j=0, idx
        float lum, c
        # unsigned char [:, :, :] output_array = numpy.zeros((h, w, 3), numpy.uint8)
        int arr_size = h * w * 3 * sizeof(unsigned char*)
        unsigned char *output_array_ = <unsigned char *>malloc(arr_size)
        unsigned char [:, :, ::1] output_array = <unsigned char[:h, :w, :3]>output_array_
        unsigned char *r
        unsigned char *g
        unsigned char *b

    # Zero out the output array
    for j in range(h):
        for i in range(w):
            output_array[j, i, 0] = 0
            output_array[j, i, 1] = 0
            output_array[j, i, 2] = 0

    with nogil, parallel():
        for j in prange(h, schedule="static"):
            for i in range(w):
                r = &rgb_array[i, j, 0]
                g = &rgb_array[i, j, 1]
                b = &rgb_array[i, j, 2]

                if r[0] != COLORKEY_R and g[0] != COLORKEY_G and b[0] != COLORKEY_B:
                    lum = r[0] * <float>0.299 + g[0] * <float>0.587 + b[0] * <float>0.114
                    
                    if lum > threshold:
                        c = (lum - threshold) / lum
                        output_array[j, i, 0] = <unsigned char>(r[0] * c)
                        output_array[j, i, 1] = <unsigned char>(g[0] * c)
                        output_array[j, i, 2] = <unsigned char>(b[0] * c)
    return pygame.image.frombuffer(output_array, (w, h), 'RGB')
# End shader_bpf_return

cdef float[5] GAUSS_KERNEL = [1.0/16.0, 4.0/16.0, 6.0/16.0, 4.0/16.0, 1.0/16.0]

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
@cython.cdivision(True)
cdef inline void shader_blur_5x5(unsigned char[:, :, :] rgb_array, int t=1):
    """
    Applies a Gaussian Blur effect to the surface rgb_array
    Expects r, g, b array to be size w x h x 3 or w x h x 4
    t is the number of times to apply
    """

    cdef Py_ssize_t w, h
    w, h = rgb_array.shape[:2]

    cdef:
        int iters = t
        short int kernel_half = 2
        # unsigned char [:, :, :] convolve = numpy.zeros((w, h, 3), numpy.uint8)
        int arr_size = w * h * 3 * sizeof(unsigned char)
        unsigned char *convolve_ = <unsigned char *>malloc(arr_size)
    if not convolve_:
        raise MemoryError()
    cdef:
        unsigned char [:, :, ::1] convolve = <unsigned char[:w, :h, :3]>convolve_
        Py_ssize_t kernel_length = len(GAUSS_KERNEL)
        int x, y, xx, yy, i
        float r, g, b
        char kernel_offset
        unsigned char red, green, blue
        float *k
        unsigned char *c1
        unsigned char *c2
        unsigned char *c3
        unsigned char *c4
        unsigned char *c5
        unsigned char *c6

    # Zero out the convolve
    for y in range(h):
        for x in range(w):
            convolve[x, y, 0] = 0
            convolve[x, y, 1] = 0
            convolve[x, y, 2] = 0

    for r in range(iters):
        with nogil, parallel():
            # Horizontal Convolution
            for y in prange(h, schedule="static"):
                c1 = &rgb_array[0, y, 0]
                c2 = &rgb_array[0, y, 1]
                c3 = &rgb_array[0, y, 2]
                c4 = &rgb_array[w-1, y, 0]
                c5 = &rgb_array[w-1, y, 1]
                c6 = &rgb_array[w-1, y, 2]

                for x in range(w):
                    r, g, b = 0, 0, 0
                    for kernel_offset in range(-kernel_half, kernel_half + 1):
                        k = &GAUSS_KERNEL[kernel_offset + kernel_half]
                        xx = x + kernel_offset

                        # Check boundaries
                        if xx < 0:
                            red, green, blue = c1[0], c2[0], c3[0]
                        elif xx > (w - 1):
                            red, green, blue = c4[0], c5[0], c6[0]
                        else:
                            red = rgb_array[xx, y, 0]
                            green = rgb_array[xx, y, 1]
                            blue = rgb_array[xx, y, 2]
                            # if red + green + blue == 0:
                                # continue

                        r = r + red * k[0]
                        g = g + green * k[0]
                        b = b + blue * k[0]

                    convolve[x, y, 0] = <unsigned char>r
                    convolve[x, y, 1] = <unsigned char>g
                    convolve[x, y, 2] = <unsigned char>b

            # Vertical Convolution
            for x in prange(w, schedule="static"):
                c1 = &rgb_array[x, 0, 0]
                c2 = &rgb_array[x, 0, 1]
                c3 = &rgb_array[x, 0, 2]
                c4 = &rgb_array[x, h-1, 0]
                c5 = &rgb_array[x, h-1, 1]
                c6 = &rgb_array[x, h-1, 2]

                for y in range(h):
                    r, g, b = 0, 0, 0
                    for kernel_offset in range(-kernel_half, kernel_half + 1):
                        k = &GAUSS_KERNEL[kernel_offset + kernel_half]
                        yy = y + kernel_offset

                        # Check boundaries
                        if yy < 0:
                            red, green, blue = c1[0], c2[0], c3[0]
                        elif yy > (h - 1):
                            red, green, blue = c4[0], c5[0], c6[0]
                        else:
                            red = convolve[x, yy, 0]
                            green = convolve[x, yy, 1]
                            blue = convolve[x, yy, 2]
                            # if red + green + blue == 0:
                                # continue

                        r = r + red * k[0]
                        g = g + green * k[0]
                        b = b + blue * k[0]

                    rgb_array[x, y, 0] = <unsigned char>r
                    rgb_array[x, y, 1] = <unsigned char>g
                    rgb_array[x, y, 2] = <unsigned char>b
    free(convolve_)
# End shader_blur_5x5

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
@cython.cdivision(True)
cdef inline void shader_bloom(surface_, int threshold_, bint fast_=False):
    """
    Creates bloom effect
    Can be 24 or 32 bit format surface
    threshold is uint between 0 and 255, used to control bright pass filter
    Higher threshold means less bloom
    fast: if true the bloom effect will be approximated and done on 16x smaller surface
        before scaling back up
    """
    assert 0 <= threshold_ <= 255, "Threshold must be in range 0 to 255"

    cdef:
        Py_ssize_t w, h
        int w2, h2, e4, h4, w8, h8, w16, h16
        bint x2, x4, x8, x16 = False

    w, h = surface_.get_size()

    # with nogil:
    # Right shift essentially integer divides by 2
    w2, h2 = <int>w >> 1, <int>h >> 1
    w4, h4 = w2 >> 1, h2 >> 1
    w8, h8 = w4 >> 1, h4 >> 1
    w16, h16 = w8 >> 1, h8 >> 1

    # with nogil:
    if w2 > 0 and h2 > 0:
        x2 = True
    else:
        x2 = False

    if w4 > 0 and h4 > 0:
        x4 = True
    else:
        x4 = False

    if w8 > 0 and h8 > 0:
        x8 = True
    else:
        x8 = False

    if w16 > 0 and h16 > 0:
        x16 = True
    else:
        x16 = False

    # Too small to perform a subsurface downscale
    if not x2:
        return

    if fast_:
        x2 = False

    surface_cp = shader_bpf_return(pixels3d(surface_), threshold=threshold_)

    # Subsurface downscale x2
    if x2:
        s2 = smoothscale(surface_cp, (w2, h2))
        # s2_array = s2.get_view('3')
        s2_array = pixels3d(s2)
        shader_blur_5x5(s2_array)
        b2_blurred = make_surface(s2_array)
        s2 = smoothscale(b2_blurred, (w, h))
        surface_.blit(s2, (0, 0), special_flags=BLEND_RGB_ADD)

    if x4:
        s4 = smoothscale(surface_cp, (w4, h4))
        # s4_array = s4.get_view('3')
        s4_array = pixels3d(s4)
        shader_blur_5x5(s4_array)
        b4_blurred = make_surface(s4_array)
        s4 = smoothscale(b4_blurred, (w, h))
        surface_.blit(s4, (0, 0), special_flags=BLEND_RGB_ADD)

    if x8:
        s8 = smoothscale(surface_cp, (w8, h8))
        # s8_array = s8.get_view('3')
        s8_array = pixels3d(s8)
        shader_blur_5x5(s8_array)
        b8_blurred = make_surface(s8_array)
        s8 = smoothscale(b8_blurred, (w, h))
        surface_.blit(s8, (0, 0), special_flags=BLEND_RGB_ADD)

    if x16:
        s16 = smoothscale(surface_cp, (w16, h16))
        # s16_array = s16.get_view('3')
        s16_array = pixels3d(s16)
        shader_blur_5x5(s16_array)
        b16_blurred = make_surface(s16_array)
        s16 = smoothscale(b16_blurred, (w, h))
        surface_.blit(s16, (0, 0), special_flags=BLEND_RGB_ADD)
# End shader_bloom

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.nonecheck(False)
@cython.cdivision(True)
cdef inline void shader_vortex(unsigned char[:, :, :] rgb_array, float time):
    cdef Py_ssize_t w, h
    w, h = rgb_array.shape[:2]

    cdef:
        float SCALE = 2.
        float pi = 3.1415
        float tau = pi * 2
        float radians = pi / 180

    cdef:
        int i=0, j=0
        unsigned char *r
        unsigned char *g
        unsigned char *b

    cdef:
        float start_time = -31 * radians
        float t = start_time + (time * tau) / 3600
        float light_speed = (time * tau) / 5
        float n, z, x, y
        float magnitude, screen_arc, increment
        float archimedean, archimedean_c, winding_ratio
        float visual_denominator, visual_denominator2
        float color, gradient, red, green, blue, col1, col2, col3

    n = sin(t) if cos(t) > 0 else 1 / sin(t)
    z = pow(500, n) # autozoom
    z = clampf(z, CL(z, 1e-16, 1e-15), CL(z, 1e17, 1e18))

    with nogil, parallel():
        for j in prange(h, schedule="static"):
            for i in range(w):
                r = &rgb_array[i, j, 0]
                g = &rgb_array[i, j, 1]
                b = &rgb_array[i, j, 2]

                # Offset myself from center
                x = (i - 0.5 * 320) / 180 * SCALE * z
                y = (j - 0.5 * 180) / 180 * SCALE * z
                magnitude = sqrt(pow(x, 2) + pow(y, 2))

                # MATH
                screen_arc = atan2(x, -y)
                increment = screen_arc / tau
                archimedean = pow(magnitude, 0.5/n) - increment  # At 0.5
                archimedean_c = ceil(archimedean)
                winding_ratio = archimedean_c + increment
                visual_denominator = (archimedean_c * tau + screen_arc) / (n * 2)
                visual_denominator2 = 2 * visual_denominator

                color = sin(winding_ratio * visual_denominator + light_speed)
                # smooth edges
                color = color * pow(sin((archimedean - floor(archimedean)) * pi), floor(fabs(n * 2)) + 5)
                color = color * sin(visual_denominator2 * winding_ratio + pi / 2 + light_speed * 2)
                gradient = mixlerp(0, 1, pow(magnitude/z, -1/n))  # dark gradient
                color = min(color, gradient)
                # actually assign color
                red = cos(visual_denominator2) + 1
                green = fabs(sin(time))
                blue = cos(pi + visual_denominator2) + 1
                col1 = color*3 - red*0.5
                col2 = color*3 - green*0.5
                col3 = color*3 - blue*0.5
                r[0] = <unsigned char>floor(255 * clampf(col1, 0, 1))
                g[0] = <unsigned char>floor(255 * clampf(col2, 0, 1))
                b[0] = <unsigned char>floor(255 * clampf(col3, 0, 1))
                # r[0] = <unsigned char>floor(mixlerp(<float>r[0], 255 * clampf(col1, 0, 1), 0.75))
                # g[0] = <unsigned char>floor(mixlerp(<float>g[0], 255 * clampf(col2, 0, 1), 0.75))
                # b[0] = <unsigned char>floor(mixlerp(<float>b[0], 255 * clampf(col3, 0, 1), 0.75))
# End shader_cool_shit

cpdef inline void invert(object surface_):
    shader_invert(pixels3d(surface_)) ##

cpdef inline void bright_pass_filter(object surface_, int threshold_=128):
    shader_bright_pass_filter(pixels3d(surface_), threshold_)

cpdef inline void blur(object surface_, int t_=1):
    shader_blur_5x5(pixels3d(surface_), t_)

cpdef inline void bloom(object surface_, int threshold_, bint fast_=False):
    shader_bloom(surface_, threshold_, fast_)

cpdef inline void vortex(object surface_, float time_):
    shader_vortex(pixels3d(surface_), time_)

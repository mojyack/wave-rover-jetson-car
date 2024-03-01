#pragma once

template <class T, unsigned N>
unsigned array_size(const T (&array)[N]) {
    return N;
}

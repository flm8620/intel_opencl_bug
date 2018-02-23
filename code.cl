struct Obj {
    int a[5];
};

__kernel void test(
    __global double* output,
    __private struct Obj param1,
    __private struct Obj param2,
    __private struct Obj param3,
    __private struct Obj param4,
    __private struct Obj param5
    //__private struct Obj param6
)
{
    int gl = get_global_id(0);
    const int N = 5;
    if (gl == 0) {
        for (int i = 0; i < N; i++)
            output[i] = param1.a[i];
        for (int i = 0; i < N; i++)
            output[i + N * 1] = param2.a[i];
        for (int i = 0; i < N; i++)
            output[i + N * 2] = param3.a[i];
        for (int i = 0; i < N; i++)
            output[i + N * 3] = param4.a[i];
        for (int i = 0; i < N; i++)
            output[i + N * 4] = param5.a[i];
    }
}
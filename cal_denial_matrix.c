//现存的方法感觉效果和cal_local_connection_with_new_powl_gen效果差不多，就算了
//生成矩阵的方式感觉需要np之类的压缩减小储存读取空间，但是纯用for循环写感觉会时间开销会非常离谱，需要思考一下怎样聪明的填满矩阵
//超过double最小精度的zero也可以观察一下
#include <stdio.h>
#include <stdlib.h> //handle system("pause")，结束后不立即退出
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define list 3000000000LL    // 结果列表
//-------------------------------------------------------------------------------------------------//
// 目标
#define Target_number (double)8.080174247945128e+53 // 8.08017424794512875815e+53 //double t;t=powl(2,46)*powl(3,20)*powl(5,9)*powl(7,6)*powl(11,2)*powl(13,3)*17*19*23*29*31*41*47*59*71;printf("%.15Le\n",t);
// 组成的常数
#define self 1 // could be generated by ^0
// Physics
#define Speed_of_light 2.99792458e8
#define Proton_length 1.754e-15 // 0.84e-15--0.87e-15/0.833e-15
#define Planck_constant 6.62607015e-34
#define Planck_constant_reduced 1.054571817646156391262428e-34
#define Planck_time 5.39116e-44
#define Planck_mass 2.176434e-8
#define Planck_length 1.616229e-35
#define Proton_mass 1.6726231e-27
#define Gravitational_constant 6.67430e-11
#define Charge 1.602176e-19
#define Vacuum_Permittivity_E0 8.854187817e-12
#define Vacuum_Permittivity_u0 1.25663706212e-6
#define Coulomb_constant_F 8.9875517923e9
#define Conductance_quantum 7.748091729e-5
#define Wien_constant 2.897771955e-3
// Chemistry
#define Boltzmann_constant 1.380649e-23
#define Stefan_Boltzmann_constant 5.670374419e-8
#define gas_constant_R 8.3144598
//#define Faraday_constant_F 9.648533212e4 //=eNA
#define Avogadro_constant 6.02214076e23
// math
#define Chaos_costant 4.6692016
#ifndef Pi
#define Pi 3.1415926535897932384626433832795
#endif
#define mathematical_constant_e 2.71828182845904523536028747135266249775724709369995
#define golden_ratio 1.6180339887498948482045868343656

//-------------------------------------------------------------------------------------------------//
// divation define 最终运行完后寻找的分子分母
const double EPS = 1e-15; // double 的误差控制,print_find_EPS存在的原因，找这个参数的值找了我一个下午
//const double EPS_debug = 4.5e-16; //4e-17 for 7.662571e-011
// #define uper 7.194727e+077
// #define lower 8.904174e+023 //final line of sorted buffer gen
#define uper 9.639690e+111
#define lower 1.193005e+058 // final line of unsorted buffer gen
//-------------------------------------------------------------------------------------------------//
// Control
#define Target_diva_point 1e-13 // 3.42e-14
#define Target_diva_map_max 1e-8//1e-9//1e-10
#define Target_diva_map_min 1e-17//1e-10//1e-17
#define Target_diva_debug 5.427871e-010//3.834622e-011//7.662571e-011//5.412208e-010 //old 9.861436e-010 9.861434e-010
#define All_pass_start 0                // 8250000//分段时开始计算偏移的数组头，legacy parameter
#define All_pass_end i1_t[power_length - 1] / 3
// 二选一
#define all_pass
//#define debug // final step

// 二选一
// #define my_sort
#define my_qsort

// Choose target diva method三选一
// #define point
#define map
// #define debug_point//倒数第二步

#define print_buffer_zero
//--------------------------------------全局变量----------------------------------------------//
unsigned long long count = 0;       // 删掉后剩下的数据
unsigned long long count_del = 0;   // 删掉的重复数据
unsigned long long local_count = 0; // 在指定diva中的个数
clock_t start_time;                 // 开始的时间
unsigned long long q_left = 0, q_right; // delete
#ifdef all_pass
int *i_t, *j_t;
    double *s1_t, *s2_t;
    double *diva_t;
#endif
#ifndef multi_thread
#define max1 6450000000LL                       // buffer_original
double *buffer_original, *buffer_original_save; // 原始数组
double *buffer_shrink, *buffer_shrink_save;     // 删除后留下的数组
unsigned long long total_pushed = 0;            // 数组排序删前的个数
unsigned long long zero = 0;
#endif
// double stack[] = {
//     Planck_length, Planck_mass, Planck_time, Planck_constant,
//     Speed_of_light, Charge, Avogadro_constant, Gravitational_constant, Boltzmann_constant,
//     Pi, Chaos_costant, mathematical_constant_e, golden_ratio, Vacuum_Permittivity_E0,
//     Proton_length, Proton_mass, Stefan_Boltzmann_constant, gas_constant_R
//     ,Planck_constant_reduced, Vacuum_Permittivity_u0
//     ,Coulomb_constant_F, Conductance_quantum, Wien_constant
// };//the original stack
double stack[] = {
    Planck_length, Planck_mass, Planck_time, Planck_constant,
    Charge, Avogadro_constant, Gravitational_constant, Boltzmann_constant,
    Pi, Chaos_costant, mathematical_constant_e, golden_ratio, Vacuum_Permittivity_E0,
    Proton_length, Proton_mass, Stefan_Boltzmann_constant
    ,Planck_constant_reduced, Vacuum_Permittivity_u0
    ,Coulomb_constant_F, Conductance_quantum, Wien_constant
};//after first round of 2 similar, delete speed of light and gas constant is more practical.
int length_ele; // how to calculate : printf("%d\n",sizeof(stack) / sizeof(stack[0]));
int length_ele_extended;
//--------------------------------------用户函数--------------------------------------------//
//void quicksort(int left, int right, double *buffer);

double calculate_divation(double result)
{
    return result / Target_number - 1;
}
int qsort_cmp(const void *a, const void *b)
{
    return ((*(double *)a - *(double *)b) > 0 ? 1 : -1); // 升序
    //	return ((*(double *)a - *(double *)b)<0?1:-1);//降序
}
void print_time(clock_t func_time)
{
    clock_t end_time;
    end_time = clock();
    printf("%lf\n", (double)(end_time - func_time) / CLOCKS_PER_SEC);
}
//---------------------------------------MAIN---------------------------------------------//
int main(void)
{
    double Exponent_TDD = floor(log10(Target_diva_debug));//Exponent_TDD means number after E; It's alwayas gonna be lower than 0
    diva_t = (double *)malloc(list * sizeof(double));
    bool a1238;
    printf("Started\n");
    printf("%d\n",sizeof(a1238));
    printf("%d\n",sizeof(unsigned char));
    printf("%d\n",sizeof(char));
    printf("%d\n",sizeof(float));
    start_time = clock();
    length_ele = sizeof(stack) / sizeof(stack[0]);
    length_ele_extended = sizeof(stack) / sizeof(stack[0]) * 7; //5 //4种^
    printf("length_ele = %d length_ele_extended = %d\n", length_ele, length_ele_extended);//符合定义的函数组
//--------------------------------single_thread_gen-----------------------------------------//
    buffer_original = (double *)malloc(max1 * sizeof(double));
    buffer_original_save = buffer_original;
    long long i, j;
    int k, l, m, o;
    double s1, s2, s3, s4, s5, s6;
    double stack_extended[length_ele_extended];
    double zero_max=0,zero_min=-300.000000,zero_temp;
    
    for (i = 0; i < length_ele; i++)
    {
        stack_extended[i + 0 * length_ele] = stack[i];
        stack_extended[i + 1 * length_ele] = powl(stack[i], 0.5);
        stack_extended[i + 2 * length_ele] = powl(stack[i], 1./3);
        stack_extended[i + 3 * length_ele] = powl(stack[i], 1./4);
        stack_extended[i + 4 * length_ele] = powl(stack[i], 2);
        stack_extended[i + 5 * length_ele] = powl(stack[i], 3);
        stack_extended[i + 6 * length_ele] = powl(stack[i], 4);
    }
//-------------------------------new_single_thread_gen---------------------------------------//
    for (i = 0; i < length_ele_extended; i++) // 产生6个一组的数据
        for (j = i + 1; j < length_ele_extended; j++)
            if(i % length_ele != j % length_ele)//  防止同一常数在单一表达式内重复出现
            for (k = j + 1; k < length_ele_extended; k++)
                if(i % length_ele != k % length_ele)
                if(j % length_ele != k % length_ele)
                for (l = k + 1; l < length_ele_extended; l++)
                    if(i % length_ele != l % length_ele)
                    if(j % length_ele != l % length_ele)
                    if(k % length_ele != l % length_ele)
                    for (m = l + 1; m < length_ele_extended; m++)
                        if(i % length_ele != m % length_ele)
                        if(j % length_ele != m % length_ele)
                        if(k % length_ele != m % length_ele)
                        if(l % length_ele != m % length_ele)
                        for (o = m + 1; o < length_ele_extended; o++)
                            if(i % length_ele != o % length_ele)
                            if(j % length_ele != o % length_ele)
                            if(k % length_ele != o % length_ele)
                            if(l % length_ele != o % length_ele)
                            if(m % length_ele != o % length_ele)
                            {
                                *buffer_original = stack_extended[i] * stack_extended[j] * stack_extended[k] * stack_extended[l] * stack_extended[m] * stack_extended[o];

                                if (*buffer_original == 0 || *buffer_original < 0 ) // 超出最小精度
                                {
                                    zero_temp = floor(log10(stack_extended[i])+log10(stack_extended[j])+log10(stack_extended[k])+log10(stack_extended[l])+log10(stack_extended[m])+log10(stack_extended[o]));
                                    if(zero_max > zero_temp)
                                        zero_max = zero_temp;
                                    if(zero_min < zero_temp & zero_temp != 0)
                                        zero_min = zero_temp;//-187.000000
                                    if (zero < 500 & zero % 50 == 0)//if (zero < 500 & zero % 5 == 0)
                                    {
                                        printf("i == %d j == %d k == %d l == %d m == %d o == %d\n", i, j, k, l, m, o);
                                        printf("A_buffer == %e B_buffer == %e C_buffer == %e D_buffer == %e E_buffer == %e F_buffer == %e\n",
                                        stack_extended[i],stack_extended[j],stack_extended[k],stack_extended[l],stack_extended[m],stack_extended[o]);
                                        printf("*buffer_original == %e zero_max == %f zero_min == %f\n", *buffer_original, zero_max, zero_min);
                                    }
                                    zero++;
                                    count_del++;
                                }
                                else
                                {
                                    buffer_original++;
                                    total_pushed++;
                                    count++;
                                    if (total_pushed % 50000000 == 0)
                                        printf("push = %lld buffer_current = %e\n", total_pushed, *(buffer_original - 1));
                                }
                            }
    printf("zero_max == %f zero_min == %f\n", zero_max, zero_min);
    printf("count = %lld count_del = %lld\n", count, count_del);
#ifdef print_buffer_zero
    // 最大没有外溢
    printf("total pushed before quick sort =%lld zero = %d\n", total_pushed, zero);
#endif
    buffer_original = buffer_original_save;
    free(buffer_original);

    printf("End time\n");
    print_time(start_time);
    system("pause");
    return 0;
}

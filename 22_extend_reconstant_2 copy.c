#include <stdio.h>
#include <stdlib.h> //handle system("pause")，结束后不立即退出
#include <math.h>
#include <time.h>


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

// #define print_buffer_before_sort
#define print_buffer_before_shrink
// #define print_buffer_after_shrink
// #define print_buffer_zero
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
void quicksort(long int left, long int right, double *buffer)
{
    long int i = left, j = right;
    double mid = buffer[(left + right) / 2]; // 请注意，在处理的过程中a[(left+right)/2]的值是会变化的
    double t;
    while (i <= j)
    { // i,j的值是会跳跃的,循环结束时i>j
        while (buffer[i] < mid)
            i++;
        while (buffer[j] > mid)
            j--;
        if (i <= j)
        {
            t = buffer[i];
            buffer[i] = buffer[j];
            buffer[j] = t;
            i++;
            j--;
        }
    }
    if (i < right)
        quicksort(i, right, buffer);
    if (left < j)
        quicksort(left, j, buffer);
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
    
    // if ((fabs(5.412208e-010 - Target_diva_debug)) < 4.5*1/pow(10,abs(Exponent_TDD)+5) && Exponent_TDD < 0)
    // {
    //     printf("successful_1\n");
    //     printf("%lf %e %e\n",Exponent_TDD,4.5*1/pow(10,abs(Exponent_TDD)+5),fabs(5.412208e-010 - Target_diva_debug));
    // }
    // Exponent_TDD = floor(log10(5.412211e010));
    // if (fabs(5.412208e010 - 5.412211e010) < 4.5*(pow(10,abs(Exponent_TDD)-5)) && Exponent_TDD > 0)
    // {
    //     printf("successful_2\n");
    //     printf("%lf %e %e\n",Exponent_TDD,4.5*(pow(10,abs(Exponent_TDD)-5)),(fabs(5.412208e010 - 5.412211e010)));
    // }

    // i_t = (int *)malloc(list * sizeof(int));
    // j_t = (int *)malloc(list * sizeof(int));
    // s1_t = (double *)malloc(list * sizeof(double));
    // s2_t = (double *)malloc(list * sizeof(double));
    diva_t = (double *)malloc(list * sizeof(double));

    printf("Started\n");
    start_time = clock();
    length_ele = sizeof(stack) / sizeof(stack[0]);
    length_ele_extended = sizeof(stack) / sizeof(stack[0]) * 7; //5 //4种^
    printf("length_ele = %d length_ele_extended = %d\n", length_ele, length_ele_extended);//符合定义的函数组
//--------------------------------single_thread_gen-----------------------------------------//
    buffer_original = (double *)malloc(max1 * sizeof(double));
    buffer_original_save = buffer_original;
    //buffer_shrink = (double *)malloc(max2 * sizeof(double));
    //buffer_shrink_save = buffer_shrink;
    long long i, j;
    int k, l, m, o;
    double s1, s2, s3, s4, s5, s6;
    double stack_extended[length_ele_extended];

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
                            {
                                *buffer_original = stack_extended[i] * stack_extended[j] * stack_extended[k] * stack_extended[l] * stack_extended[m];
    #ifdef debug
                                //if ( fabs(*buffer_original-uper)<EPS_debug || fabs(*buffer_original-lower)<EPS_debug)
                                //0.999999 or 1.0000001
                                if ((*buffer_original >= uper * 0.99999 & *buffer_original <= uper * 1.000001) || (*buffer_original >= lower * 0.999999 & *buffer_original <= lower * 1.00001))
                                {
                                    printf("count = %d\n", total_pushed);
                                    printf("i == %d j == %d k == %d l == %d m == %d o == %d\n", i, j, k, l, m);
                                    // printf("stack[i] = %e stack[j] = %e stack[k] = %e stack[l] = %e stack[m] = %e stack[o] = %e\n",stack[i],stack[j],stack[k],stack[l],stack[m],stack[o]);
                                    // printf("s1 == %.1f s2 == %.1f s3 == %.1f s4 == %.1f s5 == %.1f s6 == %.1f\n",  s1, s2, s3, s4, s5, s6);
                                    printf("A_buffer == %e B_buffer == %e C_buffer == %e D_buffer == %e E_buffer == %e\n",
                                        stack_extended[i], stack_extended[j], stack_extended[k], stack_extended[l], stack_extended[m]);
                                    printf("*buffer_original == %e", *buffer_original);
                                    printf("\n\n");
                                }
    #endif
                                if (*buffer_original == 0 || *buffer_original < 0 ) // 超出最小精度
                                {
                                    if (zero < 100)
                                    {
                                        // printf("i == %d j == %d k == %d l == %d m == %d o == %d\n", i, j, k, l, m);
                                        // printf("A_buffer == %e B_buffer == %e C_buffer == %e D_buffer == %e E_buffer == %e\n",
                                        // stack_extended[i],stack_extended[j],stack_extended[k],stack_extended[l],stack_extended[m]);
                                        // printf("*buffer_original == %e\n", *buffer_original);
                                    }
                                    zero++;
                                }
                                else
                                {
                                    buffer_original++;
                                    total_pushed++;
                                    if (total_pushed % 50000000 == 0)
                                        printf("push = %lld buffer_current = %e\n", total_pushed, *(buffer_original - 1));
                                }
                            }

    printf("count = %ld count_del = %ld i= %lld\n", count, count_del, total_pushed);
#ifdef print_buffer_zero
    // 最大没有外溢
    printf("total pushed before quick sort =%d zero = %d\n", total_pushed, zero);
#endif


//--------------------------------------quick sort--------------------------------------------//
#ifdef print_buffer_before_sort
    printf("before sort\n");
    printf("total_pushed = %ld\n", total_pushed);
    printf("original buffer before quicksort tail=%e tail+1 = %e head=%e\n", buffer_original[total_pushed - 1], buffer_original[total_pushed], buffer_original[0]);
    printf("head10\n");
    for (i = 0; i < 10; i++)
        printf("count = %ld buffer_original[i] = %e\n", i, buffer_original[i]);
    printf("tail10\n");
    for (i = total_pushed - 10; i < total_pushed; i++)
        printf("count = %ld buffer_original[i] = %e\n", i, buffer_original[i]);
#endif

#ifdef my_sort
    printf("Time before my_quicksort\n");
    print_time(start_time);
    buffer_original = (double *)malloc(max1 * sizeof(double));
    buffer_original = buffer_original_save;
    quicksort(0, total_pushed - 1, buffer_original); // 快排
    printf("Time after my_quicksort\n");
    print_time(start_time);
#endif
#ifdef my_qsort
    printf("Time before qsort\n");
    print_time(start_time);
    buffer_original = buffer_original_save;
    qsort(buffer_original, total_pushed, sizeof(double), qsort_cmp);
    printf("Time after qsort\n");
    print_time(start_time);
#endif

#ifdef print_buffer_before_shrink
    printf("after quicksort before shirink\n");
    // for (i = 0; i < 350; i++)
    //     printf("count = %ld buffer_original[i] = %e\n", i, buffer_original[i]);
    printf("original buffer after quicksort tail=%e tail+1 = %e head=%e\n", buffer_original[total_pushed - 1], buffer_original[total_pushed], buffer_original[0]);
    printf("total_pushed = %ld\n", total_pushed);
#endif
    buffer_original = buffer_original_save;
    q_left = 0;
    count_del = 0;
    count = 0;
    for (q_right = 1; q_right < total_pushed; q_right++) // 清除重复项
    {
        if (q_right % 200000000 == 0)
            printf("count = %lld count_del = %lld i = %lld\n", count, count_del, q_right);
        if (buffer_original[q_left] != buffer_original[q_right])
        {
            buffer_original[++q_left] = buffer_original[q_right];
            count++;
        }
        else
        {
            // printf("%e %e\n",buffer_original[i],buffer_original[i+1]); //判断清除条件
            count_del++;
        }
    }
    buffer_original = buffer_original_save;
    printf("Time after deleting duplicate\n");
    print_time(start_time);
    printf("count = %ld count_del = %ld\n", count, count_del);
#ifdef print_buffer_after_shrink
    for (i = count - 100; i < count + 10; i++)
        printf("count = %ld buffer_shrink[i] = %e\n", i, buffer_original[i]);
    printf("count = %ld buffer_shrink's tail count - 1 = %e\n", count - 1, buffer_original[count - 1]);
    printf("count = %ld buffer_shrink's tail count + 1 = %e\n", count + 1, buffer_original[count + 1]);
    printf("count = %ld buffer_shrink's tail count = %e\n", count, buffer_original[count]); // 确认count-1是最后的对尾，count中无数据
#endif
//-------------------------------------all_pass------------------------------------------//
//  幂函数分组归类
#ifdef all_pass
    double ideal_number;          // 理想中对应的目标数值
    double diva_plus, diva_minus; // 插入后左右diva正负不同
    double EPS_debug, EPS_map;
    j = 0;
    local_count = 0;
    printf("all_pass_started\n");
//-------------------------------------remember to check count + 1------------------------------------------//
    for (i = All_pass_start; i < All_pass_start + count + 1; i++) // i1_t[power_length-1] All_pass_end
    {
        ideal_number = Target_number * buffer_original[i];
        if (ideal_number > buffer_original[count - 1])
            break; // 目标值大于数组中存在的最大值直接退出循环
        while (buffer_original[j] < ideal_number)
        {
            if (j < count)
                j++; // 如果j移动到count外会越界,j是连续记录，重复不多
        }
        // printf("ideal_number = %e number_in_que-1 = %e number_in_que+1 = %e\n",ideal_number,buffer_shrink[j-1],buffer_shrink[j]);
        // printf("%d\n",j);
        diva_minus = -calculate_divation(buffer_original[j - 1] / buffer_original[i]); // 左
        diva_plus = calculate_divation(buffer_original[j] / buffer_original[i]);       // 右
#ifdef debug_point
        //double EPS_debug = 4.5e-16; 
        //Exponent_TDD = floor(log10(Target_diva_debug));
        if (1) // i % 100 == 0 太多数据了
        if(Exponent_TDD < 0)
        {
            EPS_debug = 1*1/pow(10,abs(Exponent_TDD)+6);
            if ((fabs(diva_minus - Target_diva_debug)) < EPS_debug)
            {
                local_count++;
                printf("diva_minus ");
                printf("1 %e 2 %e diva= %e ", buffer_original[j - 1], buffer_original[i], diva_minus);
                printf("%e %e\n",EPS_debug,(fabs(diva_minus - Target_diva_debug)));
            }
            if ((fabs(diva_plus - Target_diva_debug)) < EPS_debug)
            {
                printf("diva_plus ");
                local_count++;
                printf("1 %e 2 %e diva= %e ", buffer_original[j - 1], buffer_original[i], diva_plus);
                printf("%e %e\n",EPS_debug,(fabs(diva_plus - Target_diva_debug)));
            }
        }
        // else if(Exponent_TDD > 0)
        // {
        //     if (fabs(diva_minus - Target_diva_debug) < 4.5*pow(10,abs(Exponent_TDD)-6))
        //     {
        //         printf("3 ");
        //         local_count++;
        //         printf("1 %e 2 %e %e ", buffer_original[j - 1], buffer_original[i], diva_minus);
        //         printf("%e %e\n",4.5*pow(10,abs(Exponent_TDD)-6),(fabs(diva_minus - Target_diva_debug)));
        //     }
        //     if (fabs(diva_plus - Target_diva_debug) < 4.5*pow(10,abs(Exponent_TDD)-6))
        //     {
        //         printf("4 ");
        //         local_count++;
        //         printf("1 %e 2 %e %e ", buffer_original[j - 1], buffer_original[i], diva_plus);
        //         printf("%e %e\n",4.5*pow(10,abs(Exponent_TDD)-6),(fabs(diva_plus - Target_diva_debug)));
        //     }
        // }
        j--;
#endif
#ifdef point
        if (diva_minus < diva_plus && diva_minus < Target_diva_point)
        {
            i_t[local_count] = i;
            j_t[local_count] = j - 1;
            diva_t[local_count] = diva_minus;
            local_count++;
        }
        else if (diva_plus < diva_minus && diva_plus < Target_diva_point)
        {
            i_t[local_count] = i;
            j_t[local_count] = j;
            diva_t[local_count] = diva_plus;
            local_count++;
        }
        j--;
#endif
#ifdef map
        if (diva_minus < diva_plus && diva_minus < Target_diva_map_max && diva_minus > Target_diva_map_min)
        {
            // i_t[local_count] = i;
            // j_t[local_count] = j - 1;
            diva_t[local_count] = diva_minus;
            local_count++;
            if (local_count % 2000000 == 0)
            {
                printf("local_count = %d\n ", local_count);
                //printf("1 %e %e \n", diva_minus, diva_plus);
            }
        }
        else if (diva_plus < diva_minus && diva_plus < Target_diva_map_max && diva_plus > Target_diva_map_min)
        {
            // i_t[local_count] = i;
            // j_t[local_count] = j;
            diva_t[local_count] = diva_plus;
            local_count++;
            if (local_count % 2000000 == 0)
            {
                printf("local_count = %d\n ", local_count);
                //printf("2 %e %e \n", diva_minus, diva_plus);
            }
        }
        j--;
#endif
    }
//---------------------------------print_result-----------------------------------------//
//打印结果
#ifdef debug_point
    printf("debug_point\n");
    printf("local_count = %ld\n", local_count);
#endif
#ifdef point
    printf("point\n");
    for (i = 0; i < local_count; i++) // 搜索完成后打印符合的选项
    {
        printf("count = %d i = %d j = %d ", i, i_t[i], j_t[i]);
        printf("buffer_original[i] = %e buffer_original[j] = %e ", buffer_shrink[i_t[i]], buffer_shrink[j_t[i]]);
        printf("diva = %e \n", diva_t[i]);
    }
#endif
#ifdef map
    printf("map\n");
    // 对diva进行排序和删除
    int *numbers;
    numbers = (int *)malloc(list * sizeof(int));
    free(buffer_original);
    buffer_original = (double *)malloc(list * sizeof(double));
    buffer_shrink = (double *)malloc(list * sizeof(double) / 4);//buffer_shrink save the final diva list
    buffer_original = diva_t;
    quicksort(0, local_count - 1, diva_t);
    count = 0;
    count_del = 0;
    for (i = 0; i < local_count; i++) //Count each diva number
    {
        Exponent_TDD = floor(log10(buffer_original[i]));//Exponent_TDD means number after E; It's alwayas gonna be lower than 0
        EPS_map = 1*1/pow(10,abs(Exponent_TDD)+8);
        // if(i % 100 == 0) printf("count = %ld count_del = %ld i = %ld\n",count,count_del,i);
        if (fabs(buffer_original[i] - buffer_original[i + 1]) > EPS_map) 
        {                                                            
            buffer_shrink[count] = buffer_original[i];
            numbers[count]++; // counter
            count++;
            if (count % 1000000000 == 0)
                printf("Buffer_related = %e %e %e  delta_related = %e %e\n",buffer_original[i],buffer_original[i+1],(buffer_original[i+1] - buffer_original[i])
                ,Exponent_TDD,EPS_map); //判断清除条件
        }
        else
        {
            // printf("%e %e\n",buffer_original[i],buffer_original[i+1]); //Judge truely different or not
            numbers[count]++;
            count_del++;
        }
    }
    printf("count = %ld count_del = %ld\n", count, count_del);
    // write into a txt
    FILE *file_d, *file_n;
    errno_t err_d = fopen_s(&file_d, "diva.txt", "w");
    errno_t err_n = fopen_s(&file_n, "number.txt", "w");
    if (err_d != 0)
    {
        printf("diva.txt open failed\n");
        return -1;
    }
    if (err_n != 0)
    {
        printf("number.txt open failed\n");
        return -1;
    }
    for (i = 0; i < count; i++)
    {
        // if(i % 100 == 0)printf("diva = %e number = %d\n", buffer_shrink[i],numbers[i]);
        if(numbers[i] > 1000)//10//1000//50000
        {
            fprintf(file_d, "%e\n", buffer_shrink[i]); // 向fille标识的文件写入
            fprintf(file_n, "%d\n", numbers[i]);       // 向fille标识的文件写入
        }
    }
printf("min_diva i-1= %e number = %d\n", buffer_shrink[i-1], numbers[i-1]);
    // printf("min_diva i= %e number = %d\n", buffer_shrink[i], numbers[i]); //output is zero
    fclose(file_d); // 关闭文件d
    fclose(file_n); // 关闭文件n
    free(buffer_original);
    free(buffer_shrink);
#endif
#endif
#ifndef all_pass
    free(buffer_original);
    free(buffer_shrink);
#endif
    printf("End time\n");
    print_time(start_time);
    system("pause");
    return 0;
}

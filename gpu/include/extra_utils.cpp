#include "extra_utils.hpp"

void print_all(int limit, float ITIME, FILE *out)
{

    if (out == NULL)
    {
        for (int i = 0; i < limit; i++) {
            printf("%.10f %6d %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f\n",
                    ITIME,
                    i,
                    h_r[i].x, h_r[i].y, h_r[i].z,
                    h_v[i].x, h_v[i].y, h_v[i].z,
                    h_f[i].a[0], h_f[i].a[1], h_f[i].a[2],
                    h_f[i].a1[0], h_f[i].a1[1], h_f[i].a1[2],
                    h_dt[i]);
        }
    }
    else if (out != NULL)
    {
        for (int i = 0; i < limit; i++) {
            fprintf(out,"%.10f %6d %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f\n",
                    ITIME,
                    i,
                    h_r[i].x, h_r[i].y, h_r[i].z,
                    h_v[i].x, h_v[i].y, h_v[i].z,
                    h_f[i].a[0], h_f[i].a[1], h_f[i].a[2],
                    h_f[i].a1[0], h_f[i].a1[1], h_f[i].a1[2],
                    h_dt[i]);
        }
    }
}

void print_positions(int limit)
{
    for (int i = 0; i < limit; i++) {
        printf("%6d %.10f %.10f %.10f\n", i, h_r[i].x, h_r[i].y, h_r[i].z );
    }
}

void print_velocities(int limit)
{
    for (int i = 0; i < limit; i++) {
        printf("%6d %.10f %.10f %.10f\n", i, h_v[i].x, h_v[i].y, h_v[i].z );
    }
}
void print_accelerations(int limit)
{
    for (int i = 0; i < limit; i++) {
        printf("%6d %.10f %.10f %.10f\n", i, h_f[i].a[0], h_f[i].a[1], h_f[i].a[2] );
    }

}
void print_jrks(int limit)
{
    for (int i = 0; i < limit; i++) {
        printf("%6d %.10f %.10f %.10f\n", i, h_f[i].a1[0], h_f[i].a1[1], h_f[i].a1[2] );
    }
}

void print_accjrk(int limit)
{
    for (int i = 0; i < limit; i++) {
        printf("%6d %4.10f %4.10f %4.10f %4.10f %4.10f %4.10f\n",
                i,
                h_f[i].a[0], h_f[i].a[1], h_f[i].a[2],
                h_f[i].a1[0], h_f[i].a1[1], h_f[i].a1[2]
                );
    }
}
void print_masses(int limit)
{
    for (int i = 0; i < limit; i++) {
        printf("%6d %.10f\n", i, h_m[i] );
    }
}

void print_times(int limit, float itime)
{
    int exp = 0;
    for (int i = 0; i < limit; i++) {
        exp = (int)std::ceil(log(h_dt[i])/log(2));
        printf("%.10f %6d %.10f %2d %.10f\n", itime, i, h_dt[i], exp, h_t[i]);
    }
}

// Print old
void print_old(int limit)
{
    for (int i = 0; i < limit; i++) {
        printf("%6d %.10f %.10f %.10f %.10f %.10f %.10f\n",
                i,
                h_old_a[i].x,  h_old_a[i].y,  h_old_a[i].z,
                h_old_a1[i].x, h_old_a1[i].y, h_old_a1[i].z);
    }
}

void print_predicted(int limit)
{
    for (int i = 0; i < limit; i++) {
        printf("%6d %.10f %.10f %.10f %.10f %.10f %.10f\n",
                i,
                h_p[i].r[0], h_p[i].r[1], h_p[i].r[2],
                h_p[i].v[0], h_p[i].v[1], h_p[i].v[2]);
    }
}

void print_movement(int limit, int total, float ITIME)
{
    printf("%.6f ", ITIME);
    for (int i = 0; i < limit; i++)
    {
        int value = 0;
        for(int j = 0; j < total ; j++)
        {
            int k = h_move[j];
            if (k == i)
                value = 1;

        }
        printf("%d ", value);
    }
    printf("\n");
}


void print_particle(int i)
{
    printf("%5d %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f %.10f\n",
            i,
            h_r[i].x, h_r[i].y, h_r[i].z, h_v[i].x, h_v[i].y, h_v[i].z,
            h_f[i].a[0], h_f[i].a[1], h_f[i].a[2], h_f[i].a1[0], h_f[i].a1[1], h_f[i].a1[2],
            h_dt[i]);

}



double get_magnitude(double x, double y, double z)
{
    return sqrt(x*x + y*y + z*z);
}

void get_energy_log(double ITIME, int iterations, int nsteps, FILE *out, double energy)
{
    energy_end = energy;
    double relative_error   = abs((energy_end-energy_tmp)/energy_ini);
    double cumulative_error = abs((energy_end-energy_ini)/energy_ini);
    energy_tmp = energy_end;
    float time = (float)clock()/CLOCKS_PER_SEC - ini_time;

    if((int)ITIME == 0)
    {
        fprintf(out, "# %3s\t %10s\t %10s\t %10s\t %10s\t %8s\t %8s\t %8s\t %8s\t %8s\n",
        //printf("# %3s\t %10s\t %10s\t %10s\t %10s\t %8s\t %8s\t %8s\t %8s\t %8s\n",
                "Time",
                "CpuIte",
                "GpuIte",
                "Ite",
                "Nsteps",
                "GpuT",
                "TTime",
                "Energy",
                "RelErr",
                "CumErr");
    }
    fprintf(out, "# % 3d\t % 10d\t % 10d\t % 10d\t % 10d\t % 6.4f\t % 6.4f\t % .6e\t % .6e\t % .6e\n",
    //printf("#% 3d\t % 10d\t % 10d\t % 10d\t % 10d\t % 6.4f\t % 6.4f\t % .6e\t % .6e\t % .6e\n",
            (int)ITIME,
            cpu_iterations,
            gpu_iterations,
            iterations,
            nsteps,
            gpu_time,
            time,
            energy_end,
            relative_error,
            cumulative_error);

    if (print_log)
    {
        print_all(n,ITIME,out);
    }
    //fflush(out);
}


#include "hermite.cuh"

void integrate_gpu()
{
    double ATIME = 1.0e+10; // Actual integration time
    double ITIME = 0.0;     // Integration time
    int nact     = 0;       // Active particles
    int nsteps   = 0;       // Amount of steps per particles on the system
    iterations   = 0;       // Iterations of the integration

    // Tmp setting nblocks and nthreads
    nthreads = BSIZE;
    nblocks = ceil(n/(float)nthreads);

    // Copying the input file information from the CPU to the GPU
    CUDA_SAFE_CALL(cudaMemcpy(d_r,  h_r,  d4_size,cudaMemcpyHostToDevice));
    CUDA_SAFE_CALL(cudaMemcpy(d_v,  h_v,  d4_size,cudaMemcpyHostToDevice));
    CUDA_SAFE_CALL(cudaMemcpy(d_m,  h_m,  f1_size,cudaMemcpyHostToDevice));

    gpu_init_acc_jrk();   // Initial calculation of a and a1

    // Copying a and a1 from the GPU to the CPU
    CUDA_SAFE_CALL(cudaMemcpy(h_f,  d_f,  sizeof(Forces) * n ,cudaMemcpyDeviceToHost));

    init_dt(&ATIME);  // Initial calculation of time-steps using simple equation

    energy_ini = gpu_energy(); // Initial calculation of the energy of the system
    energy_tmp = energy_ini;  // Saving initial energy, to calculate errors

    get_energy_log(ITIME, iterations, nsteps, out, energy_tmp); // First log of the integration

    double tmp_time = 0.0;
    gpu_time = 0.0;


    double kernel_time_tmp = 0.0;
    double kernel_time = 0.0;
    static long long interactions = 0;

    while (ITIME < int_time)
    {
        ITIME = ATIME;                         // New integration time
        nact = find_particles_to_move(ITIME);  // Find particles to move (nact)
        save_old(nact);                        // Save old information

        tmp_time = omp_get_wtime();

        predicted_pos_vel(ITIME);

        kernel_time_tmp = omp_get_wtime();
        gpu_update(nact);     // Update a and a1 of nact particles
        kernel_time += (omp_get_wtime() - kernel_time_tmp);

        correction_pos_vel(ITIME, nact);       // Correct r and v of nact particles

        CUDA_SAFE_CALL(cudaMemcpy(d_r, h_r, d4_size, cudaMemcpyHostToDevice));
        CUDA_SAFE_CALL(cudaMemcpy(d_v, h_v, d4_size, cudaMemcpyHostToDevice));

        gpu_time += omp_get_wtime() - tmp_time;
        gpu_iterations++;
        interactions += nact * n;

        next_itime(&ATIME);                    // Find next integration time

        if(std::ceil(ITIME) == ITIME)          // Print log in every integer ITIME
        //if(nact == n)          // Print log in every integer ITIME
        {
           get_energy_log(ITIME, iterations, nsteps, out, gpu_energy());
        }

        nsteps += nact;                        // Update nsteps with nact
        iterations++;                          // Increase iterations
    }
    printf("Total Average GLOPS/s: %f \n", 60.10e-9 * (interactions / kernel_time));
}

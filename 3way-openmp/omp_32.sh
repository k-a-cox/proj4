#!/usr/bin/env bash

#SBATCH --ntasks=32
#SBATCH --nodes=16
SCRIPTPATH=$(dirname "$SCRIPT")
$SCRIPTPATH/omp_32_thread
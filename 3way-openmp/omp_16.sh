#!/usr/bin/env bash

#SBATCH --ntasks=16
#SBATCH --nodes=16
SCRIPTPATH=$(dirname "$SCRIPT")
$SCRIPTPATH/omp_16_thread
#!/usr/bin/env bash

#SBATCH --ntasks=2
#SBATCH --nodes=2
SCRIPTPATH=$(dirname "$SCRIPT")
$SCRIPTPATH/omp_2_thread
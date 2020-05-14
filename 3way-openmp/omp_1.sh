#!/usr/bin/env bash

#SBATCH --ntasks=1
#SBATCH --nodes=1
SCRIPTPATH=$(dirname "$SCRIPT")
$SCRIPTPATH/omp_1_thread
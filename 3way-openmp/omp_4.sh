#!/usr/bin/env bash

#SBATCH --ntasks=4
#SBATCH --nodes=4
SCRIPTPATH=$(dirname "$SCRIPT")
$SCRIPTPATH/omp_4_thread
#!/bin/bash

#SBATCH --ntasks=8
#SBATCH --nodes=8
SCRIPTPATH=$(dirname "$SCRIPT")
$SCRIPTPATH/omp_8_thread